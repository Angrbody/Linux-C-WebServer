#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <signal.h>
#include <assert.h>

#include "http_conn.h"
#include "locker.h"
#include "threadpool.h"

#define MAX_FD 65535 //最大的文件描述符个数
#define MAX_EVENT_NUM 10000 //一次监听的最大事件数量

// 添加信号捕捉
void addsig(int sig, void(handler)(int)){
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = handler;
    sigfillset(&sa.sa_mask);
    assert(sigaction(sig, &sa, NULL) != -1);
}

// 添加文件描述符到epoll中
extern void addfd(int epollfd, int fd, bool one_shot);

// 从epoll中删除文件描述符
extern void removefd(int epollfd, int fd);

// 修改文件描述符
extern void modfd(int epollfd, int fd, int ev);

int main(int argc, char* argv[]){

    if(argc <= 1){
        printf("按照如下格式运行： %s port_number\n", basename(argv[0]));
        exit(-1);
    }

    // 获取端口号
    int port = atoi(argv[1]);
    
    // 对SIGPIE信号做处理
    addsig(SIGPIPE, SIG_IGN);

    // 创建并初始化线程池
    threadpool<http_conn> * pool = NULL;
    try{
        pool = new threadpool<http_conn>;

    }catch(...){
        return 1;
    }

    // 创建一个数组用于保存所有的客户端信息(可优化成 vector？)
    http_conn * users = new http_conn[MAX_FD];
    
    printf("threadpool is created!\n\n");

    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    if(listenfd == -1){
        perror("socket");
        exit(-1);
    }

    printf("socket created ok!\n\n");

    // 设置端口复用
    int reuse = 1;
    int ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    if(ret == -1){
        perror("setsockopt");
        exit(-1);
    }

    // 绑定
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;
    // inet_pton(AF_INET, "124.222.101.7", &address.sin_addr.s_addr);
    ret = bind(listenfd, (struct sockaddr*)&address, sizeof(address));
    if(ret == -1){
        perror("bind");
        exit(-1);
    }

    char serverIP[16];
    inet_ntop(AF_INET, &address.sin_addr.s_addr, serverIP, sizeof(serverIP));
    printf("server IP address = %s, port = %d\n\n", serverIP, port);

    // 监听
    ret = listen(listenfd, 5);
    if(ret == -1){
        perror("listen");
        exit(-1);
    }

    // 创建epoll对象,事件数组，添加
    epoll_event events[MAX_EVENT_NUM];
    int epollfd = epoll_create(5);

    // 将监听的文件描述符添加到epoll对象中
    addfd(epollfd, listenfd, false);
    http_conn::m_epollfd = epollfd;

    printf("prepared!\n\n");

    while(true){
        
        printf("epoll is waiting...\n");

        int num = epoll_wait(epollfd, events, MAX_EVENT_NUM, -1);

        if((num < 0) && (errno != EINTR)){
            printf("epoll failure\n");
            break;
        }

        printf("waiting for client...\n");

        // 循环遍历事件数组
        for(int i = 0; i < num; i++){
            int sockfd = events[i].data.fd;
            if(sockfd == listenfd){
                // 有客户端连进来
                struct sockaddr_in cliaddr;
                socklen_t size = sizeof(cliaddr);
                int connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &size);
                
                if(connfd < 0){
                    printf("error is : %d\n", errno);
                    continue;
                }

                if(http_conn::m_user_count >= MAX_FD){
                    // 目前连接数满了
                    printf("server busy....\n");
                    close(connfd);
                    continue;
                }

                // 将新的客户的数据初始化，并放到 http_conn数组中
                users[connfd].init(connfd, cliaddr);

            }else if(events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)){
                
                // 对方异常断开 或错误等事件
                users[sockfd].close_conn(); // 关闭该用户的连接

            }else if(events[i].events & EPOLLIN){
                
                // 有数据写入
                // 一次性把所有数据都读出来，然后交给子线程
                if(users[sockfd].read()){

                    pool->append(users + sockfd);

                }else{

                    users[sockfd].close_conn();

                }

            }else if(events[i].events & EPOLLOUT){
               
                // 客户端需要读数据
                // 需要返回 HTTP响应报文
                if(!users[sockfd].write()){

                    users[sockfd].close_conn();

                }

            }
        }

    }
    close(epollfd);
    close(listenfd);
    delete []users;
    delete pool;

    return 0;
}