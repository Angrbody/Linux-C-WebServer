#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

int main(){

    // 1. 创建socket套接字用于监听
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1){
        perror("socket");
        exit(0);
    }


    // 2.连接服务器
    struct sockaddr_in serveraddr;
    inet_pton(AF_INET, "10.0.16.5", &serveraddr.sin_addr.s_addr);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(9999);
    int ret = connect(fd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if(ret == -1){
        perror("connect");
        exit(0);
    }

    // 3. 通信
    char rcvbuf[1024] = {0};
    while(1){
        // memset(rcvbuf, 0, sizeof(rcvbuf));
        char rcvbuf[1024] = {0};
        // 发送数据
        char s[128];
        printf("请输入...\n");
        scanf("%s", s);

        int len = write(fd, &s, strlen(s));
        if(len == -1){
            perror("write");
            exit(0);
        }

        // 获取数据
        len = read(fd, rcvbuf, sizeof(rcvbuf));
        if(len == -1){
            perror("read");
            exit(0);
        }else if(len > 0){
            printf("recv server data : %s\n", rcvbuf);
        }else if(len == 0){
            printf("server closed...\n");
            break;
        }
    }
    

    

    // 4. 关闭文件描述符
    close(fd);
    
    return 0;
}