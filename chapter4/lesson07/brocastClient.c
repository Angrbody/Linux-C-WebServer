#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

int main(){

    // socket
    int cfd = socket(PF_INET, SOCK_DGRAM, 0);

    // 客户端绑定本地 IP和端口
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = INADDR_ANY;

    int ret = bind(cfd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret == -1){
        perror("bind");
        exit(-1);
    }

    int i = 0;
    // 通信
    while(1){
        // recvfrom
        char recvbuf[128];
        // 接收数据
        int num = recvfrom(cfd, recvbuf, sizeof(recvbuf), 0, NULL, NULL);
        if(num == -1){
            perror("recvfrom");
            exit(-1);
        }
        // 输出接收到的数据
        printf("client say: %s\n", recvbuf);   

    }
    close(cfd);

    return 0;
}