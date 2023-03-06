#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

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

    // 创建 fd_set 集合
    fd_set rdset, tmp;   //可以容纳1024个文件描述符
    FD_ZERO(&rdset);
    FD_SET(lfd, &rdset);
    FD_SET(lfd, &rdset);
    int maxfd = lfd;

    while(1){
        tmp = rdset;
        // 调用select系统函数，让内核检测那些文件描述符有数据
        int ret = select(maxfd+1, &tmp, NULL, NULL, NULL);
        if(ret == -1){
            perror("select");
            exit(-1);
        }else if(ret == 0){
            continue;
        }else if(ret > 0){
            // 检测到有文件描述符对应缓冲区的数据发生改变
            if(FD_ISSET(lfd, &tmp)){
                // 接受新的客户端连接
                struct sockaddr_in cliaddr;
                int len = sizeof(cliaddr);
                int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &len);
                printf("--- client connect succeed! ---\n");
                
                char clientIP[16];
                inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, clientIP, sizeof(clientIP));
                unsigned short clientPort = ntohs(cliaddr.sin_port);
                
                printf("--- client IP: %s, port: %d ---\n", clientIP, clientPort);

                // 将新的文件描述符添加到集合中
                FD_SET(cfd, &rdset);

                // 更新最大描述符
                maxfd = maxfd > cfd ? maxfd : cfd;
            }

            for(int i = lfd + 1; i <= maxfd; i++){
                if(FD_ISSET(i, &tmp)){
                    // 文件描述符 i对应的客户端发来了数据
                    char buf[1024] = {0};
                    int len = read(i, buf, sizeof(buf));
                    if(len == -1){
                        perror("read");
                        exit(-1);
                    }else if(len == 0){
                        printf("client closed...\n");
                        close(i);
                        FD_CLR(i, &rdset);
                    }else if(len > 0){
                        printf("read buf = %s\n", buf);
                        write(i, buf, strlen(buf)+1);
                    }
                }
            }
        }
    }
    close(lfd);
    return 0;
}