/*

    使用udp协议进行通信的 服务器 - 客户端


*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

int main(){

    // socket
    int cfd = socket(PF_INET, SOCK_DGRAM, 0);

    // bind
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);
    saddr.sin_addr.s_addr = INADDR_ANY;
    int ret = bind(cfd, (struct sockaddr*)&saddr, sizeof(saddr));
    if(ret == -1){
        perror("bind");
        exit(-1);
    }

    // 通信
    while(1){
        // recvfrom
        char buf[128];
        char clientIP[16];

        struct sockaddr_in cliaddr;
        int len = sizeof(cliaddr);

        int num = recvfrom(cfd, buf, sizeof(buf), 0, (struct sockaddr*)&cliaddr, &len);
        if(num == -1){
            perror("recvfrom");
            exit(-1);
        }

        // 输出客户端信息
        printf("client connect succeed!\n");
        printf("client IP: %s, port: %d\n",
        inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, clientIP, sizeof(clientIP)), 
        ntohs(cliaddr.sin_port));

        // 输出接收到的数据
        printf("client say: %s\n", buf);

        // 发送数据
        sendto(cfd, buf, strlen(buf)+1, 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr));



    }
    close(cfd);

    return 0;
}