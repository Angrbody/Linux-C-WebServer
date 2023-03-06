// tcp 通信的服务器端
// 练习1：回射服务器
// 联系2：键盘录入

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

int main(){

    // 1. 创建socket套接字用于监听
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if(lfd == -1){
        perror("socket");
        exit(0);
    }

    // 绑定之前先设置端口复用
    int optval = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

    // 2. 绑定本地 ip和port
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    inet_pton(AF_INET, "10.0.16.5", &saddr.sin_addr.s_addr);
    saddr.sin_port = htons(9999);

    int ret = bind(lfd, (struct sockaddr*)&saddr, sizeof(saddr));
    if(ret == -1){
        perror("bind");
        exit(0);
    }

    // 3. 监听
    ret = listen(lfd, 8);
    if(ret == -1){
        perror("listen");
        exit(0);
    }

    // 4. 接收客户端的连接
    struct sockaddr_in clientaddr;
    socklen_t len = sizeof(clientaddr);
    int cfd = accept(lfd, (struct sockaddr*)&clientaddr, &len);
    if(cfd == -1){
        perror("accept");
        exit(0);
    }

    // 输出客户端信息
    char clientIP[16] = "";
    inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, clientIP, sizeof(clientIP));
    unsigned short clientPort = ntohs(clientaddr.sin_port);
    printf("client ip is %s, port is %d\n", clientIP, clientPort);

    // 5. 通信
    while(1){
        // memset(rcvbuf, 0, sizeof(rcvbuf));
        char rcvbuf[1024] = {0};
        // 获取数据
        int len1 = read(cfd, rcvbuf, sizeof(rcvbuf));
        if(len1 == -1){
            perror("read");
            exit(0);
        }else if(len1 > 0){
            printf("recv client data : %s\n", rcvbuf);
        }else if(len1 == 0){
            printf("clinet closed...\n");
            break;
        }

        // 发送数据
        int len3 = write(cfd, &rcvbuf, sizeof(rcvbuf));
        if(len3 == -1){
            perror("write");
            exit(0);
        }
        // sleep(1);
    }
    

    // 6. 关闭文件描述符
    close(cfd);
    close(lfd);

    return 0;
}