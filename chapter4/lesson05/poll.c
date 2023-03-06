#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#include <poll.h>

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

    // 初始化 检测的文件描述符数组 poll
    struct pollfd fds[1024];
    for(int i = 0; i < 1024; i++){
        fds[i].fd = -1;
        fds[i].events = POLLIN;
    }
    fds[0].fd = lfd;
    
    int nfds = 0;

    while(1){

        // 调用poll系统函数，让内核检测那些文件描述符有数据
        int ret = poll(fds, nfds+1, -1);

        if(ret == -1){
            perror("poll");
            exit(-1);
        }else if(ret == 0){
            continue;
        }else if(ret > 0){
            // 检测到有文件描述符对应缓冲区的数据发生改变
            if(fds[0].revents & POLLIN){
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
                for(int i = 1; i < 1024; i++){
                    if(fds[i].fd == -1){
                        fds[i].fd = cfd;
                        fds[i].events = POLLIN;
                        break;
                    }
                }

                // 更新最大描述符
                nfds = nfds > cfd ? nfds : cfd;
            }

            for(int i = 1; i <= nfds; i++){
                if(fds[i].revents & POLLIN){
                    // 文件描述符 i对应的客户端发来了数据
                    char buf[1024] = {0};
                    int len = read(fds[i].fd, buf, sizeof(buf));
                    if(len == -1){
                        perror("read");
                        exit(-1);
                    }else if(len == 0){
                        printf("client closed...\n");
                        close(fds[i].fd);
                        fds[i].fd = -1;
                    }else if(len > 0){
                        printf("read buf = %s\n", buf);
                        write(fds[i].fd, buf, strlen(buf)+1);
                    }
                }
            }
        }
    }
    close(lfd);
    return 0;
}