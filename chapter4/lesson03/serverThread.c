/*

    使用多线程的方式实现 一个服务器-多个客户端

*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

struct sockInfo{
    int fd;
    pthread_t tid;
    struct sockaddr_in addr;
};

void * callback(void * arg){
    // 子线程 用于与客户端进行通信 need cfd
    // 获取客户端信息
    struct sockInfo * pinfo = (struct sockInfo * )arg;
    int fd = pinfo->fd;
    int tid = pinfo->tid;
    

    char cliIp[16];
    unsigned short cliport = ntohs(pinfo->addr.sin_port);
    inet_ntop(AF_INET, &pinfo->addr.sin_addr.s_addr, cliIp, strlen(cliIp));
    printf("client connect succeed!\n");
    printf("clinet IP : %s, port : %d\n", cliIp, cliport);

    
    char buf[1024];
    while(1){
        // 读取客户端发送过来的数据
        int ret = read(fd, &buf, sizeof(buf));

        if(ret == -1){
            perror("read");
            exit(-1);
        }else if(ret > 0){
            printf("from client : %s\n", buf);
        }else if(ret == 0){
            printf("client closed ... \n");
            printf("子线程 %ld 已被回收\n", pinfo->tid);
            break;
        }
        
        // 回射客户端数据
        ret = write(fd, buf, strlen(buf)+1);
        if(ret == -1){
            perror("write");
            exit(-1);
        }
    }
    close(fd);
    
    return NULL;
}

struct sockInfo sockInfos[128];

int main(){

    // 1. 创建socket
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if(lfd == -1){
        perror("server socket");
        exit(-1);
    }

    // 2. 绑定ip port
    int port = 9999;
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    int ret = inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr.s_addr);
    if(ret == -1){
        perror("server inet_pton");
        exit(-1);
    }

    ret = bind(lfd, (struct sockaddr*)&saddr, sizeof(saddr));

    if(ret == -1){
        perror("server bind");
        exit(-1);
    }

    // 3. 监听
    ret = listen(lfd, 8);
    if(ret == -1){
        perror("server listen");
        exit(-1);
    }

    // 初始化sockinfo数据
    int max = sizeof(sockInfos)/sizeof(sockInfos[0]);
    for(int i = 0; i < max; i++){
        bzero(&sockInfos[i], sizeof(sockInfos[i]));
        sockInfos[i].fd = -1;
        sockInfos[i].tid = -1;

    }

    while(1){

        // 循环等待客户端连接
        struct sockaddr_in cliaddr;
        int len = sizeof(cliaddr);
        // 接受连接
        int cfd = accept(lfd, (struct sockaddr*)&cliaddr, &len);
        if(cfd == -1) {
            perror("accept");
            exit(-1);
        }

        // 创建多线程
        struct sockInfo * pinfo;
        for(int i = 0; i < max; i++){
            if(sockInfos[i].fd == -1){
                pinfo = &sockInfos[i];
                break;
            }
            if(i == max-1){
                sleep(1);
                i--;
            }
        }
        pinfo->fd = cfd;
        memcpy(&pinfo->addr, &cliaddr, len);

        pthread_create(&pinfo->tid, NULL, callback, pinfo);

        // pthread_join(); // 阻塞的,不能用
        // 这里用detach直接设置线程资源回收
        int ret = pthread_detach(pinfo->tid);
        if(ret == 0){
            printf("子线程 %ld 已被回收\n", pinfo->tid);
        }else if(ret == -1){
            perror("pthread_detach");
            exit(-1);
        }
    }
    close(lfd);

    return 0;
}
