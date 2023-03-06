/*
    #include <unistd.h>

    复制一个新的文件描述符（多个fd可以指向同一个文件）
    int dup(int oldfd);
    int dup2(int oldfd, int newfd);

       #define _GNU_SOURCE             /* See feature_test_macros(7) */

#include <unistd.h>
#include <fcntl.h> 
#include <errno.h>
#include <stdio.h>
#include <string.h>
int main(){

    int fd = open("a.txt", O_RDWR | O_CREAT, 0664);

    int fd1 = dup(fd);
    if(fd1 == -1){
        perror("dup");
        return -1;
    }

    printf("fd: %d, fd1: %d\n", fd, fd1);

    close(fd);

    char* str = "write by fd1\n";
    int ret = write(fd1, str, strlen(str));
    if(ret == -1){
        perror("write");
        return -1;
    }

    close(fd1);
    return 0;
}