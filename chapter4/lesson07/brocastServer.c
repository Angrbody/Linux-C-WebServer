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


    // 设置广播属性
    int op = 1;
    setsockopt(cfd, SOL_SOCKET, SO_BROADCAST, &op, sizeof(op));

    // 创建一个广播地址
    struct sockaddr_in caddr;
    caddr.sin_family = AF_INET;
    caddr.sin_port = htons(9999);
    inet_pton(AF_INET, "127.0.0.255", &caddr.sin_addr.s_addr);

    // 通信
    int num = 0;
    while(1){
       
        char sendbuf[128];

        sprintf(sendbuf, "hello client... %d\n", num++);

        // 发送数据
        sendto(cfd, sendbuf, strlen(sendbuf)+1, 0, (struct sockaddr*)&caddr, sizeof(caddr));

        printf("广播的数据: %s\n", sendbuf);
        sleep(1);

    }
    close(cfd);

    return 0;
}