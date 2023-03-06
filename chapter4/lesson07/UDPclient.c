#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

int main(){

    // socket
    int cfd = socket(PF_INET, SOCK_DGRAM, 0);

    // 获取服务器的地址信息
    struct sockaddr_in  saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);
    inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr.s_addr);
    
    int i = 0;
    // 通信
    while(1){
        // recvfrom
        char sendbuf[1024];
        char recvbuf[1024];
        sprintf(sendbuf, "from client %d\n", i++);

        // 发送数据
        sendto(cfd, sendbuf, strlen(sendbuf)+1, 0, (struct sockaddr*)&saddr, sizeof(saddr));


        // 接受数据
        int num = recvfrom(cfd, recvbuf, sizeof(recvbuf), 0, NULL, NULL);
        if(num == -1){
            perror("recvfrom");
            exit(-1);
        }
        // 输出接收到的数据
        printf("client say: %s\n", recvbuf);   

        sleep(1);
    }
    close(cfd);

    return 0;
}