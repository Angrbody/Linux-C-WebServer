/*

    字节序转换函数
    网络通信时，需要将主机字节序转化为网络字节序(大端序)
    另外一端获取到数据后根据情况将网络字节序转化为主机字节序

*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(){

    // htons 一般用于转换端口
    unsigned short a = 0x0102;

    unsigned short b = htons(a);

    printf("%x\n", a);

    printf("%x\n", b);

    printf("=====================\n");

    // htonl 一般用于转换 IP
    char buf[4] = {192, 168, 1, 100};
    int num = *(int *)buf;
    unsigned int num2 = htonl(num);

    unsigned char *p = (char *)&num2;
    printf("%d %d %d %d\n", *p ,*(p+1), *(p+2), *(p+3));

    printf("=====================\n");

    // ntohl
    unsigned char buf1[4] = {1, 1, 168, 192};
    int num1 = *(int *)buf1;
    int sum1 = ntohl(num1);
    unsigned char *p1 = (unsigned char *)&sum1;
    printf("%d %d %d %d\n", *p1 ,*(p1+1), *(p1+2), *(p1+3));

    printf("=====================\n");

    // ntohs
    unsigned short rcvd = 0x0201;
    unsigned short trans = ntohs(rcvd);
    printf("%x\n", trans);

    return 0;
}