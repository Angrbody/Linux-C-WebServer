#include <sys/epoll.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(){

    // socket
    int lfd = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in saddr;
    saddr.sin_port = htons(9999);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;

    // bind
    bind(lfd, (struct sockaddr * )&saddr, sizeof(saddr));

    // listen
    listen(lfd, 8);

    // epoll_create : create a instance of epoll
    int epfd = epoll_create(100);

    // 将监听的文件描述符相关检测信息添加到 epoll实例中
    struct epoll_event epev;
    epev.events = EPOLLIN;
    epev.data.fd = lfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &epev);

    struct epoll_event epevs[1024];
    while(1){

        // epoll_wait
        int ret = epoll_wait(epfd, epevs, 1024, -1);
        if(ret == -1){
            perror("epoll wait");
            exit(-1);
        }

        printf("ret = %d\n", ret);

        for(int i = 0; i < ret; i++){
            
            int curfd = epevs[i].data.fd;
            if(curfd == lfd){
                // 监听的文件描述符有数据到达，即有客户端连接
                struct sockaddr_in cliaddr;
                int len = sizeof(cliaddr);
                int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &len);
                printf("--- client connect succeed! ---\n");
                
                char clientIP[16];
                inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, clientIP, sizeof(clientIP));
                unsigned short clientPort = ntohs(cliaddr.sin_port);
                
                printf("--- client IP: %s, port: %d ---\n", clientIP, clientPort);

                // 添加客户端的文件描述符信息
                epev.events = EPOLLIN;
                epev.data.fd = cfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &epev);
            }else{
                // 有其他文件描述符数据到达，进行通信
                char buf[5] = {0};
                    int len = read(curfd, buf, sizeof(buf));
                    if(len == -1){
                        perror("read");
                        exit(-1);
                    }else if(len == 0){
                        printf("client closed...\n");
                        epoll_ctl(epfd, EPOLL_CTL_DEL, curfd, NULL);
                        close(curfd);
                        
                    }else if(len > 0){
                        printf("read buf = %s\n", buf);
                        write(curfd, buf, strlen(buf)+1);
                    }
            }

        }
    }
    close(lfd);
    close(epfd);

    return 0;
}