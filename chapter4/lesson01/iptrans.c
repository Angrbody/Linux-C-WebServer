/*

    点分十进制ip地址和二进制形式的转换


*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
int main(){

    // 创建一个点分十进制的 IP地址字符串
    char buf[] = "192.168.1.4";

    unsigned int num = 0;

    inet_pton(AF_INET, buf, &num);  // 将点分十进制字符串转化为网络字节序的整数

    unsigned char * p = (unsigned char *)&num;

    printf("%d %d %d %d\n", *p, *(p+1), *(p+2), *(p+3));

    // 将网络字节序的IP整数转化成点分十进制的 IP地址字符串
    char ip[16] = "";

    const char* str = inet_ntop(AF_INET, &num, ip, 16);

    printf("str : %s\n", str);
    printf("ip : %s\n", str);
    printf("%d\n", ip == str);

    return 0;
}