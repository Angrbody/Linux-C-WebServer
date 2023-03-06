
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
int main(){
    
    int fd = open("a.txt", O_RDWR | O_CREAT, 0664);
    if(fd == -1){
        perror("open");
        return -1;
    }
    int fd1 = open("b.txt", O_RDWR | O_CREAT, 0664);
    if(fd1 == -1){
        perror("open");
        return -1;
    }

    printf("fd: %d, fd1: %d \n", fd, fd1);
    int newfd = dup2(fd, fd1);
    if(newfd == -1){
        perror("open");
        return -1;
    }

    char* c = "hello\n";
    int ret = write(newfd, c, strlen(c));
    if(ret == -1){
        perror("write");
        return -1;
    }

    printf("fd: %d, fd1: %d, newfd: %d \n", fd, fd1, newfd);

    close(fd);
    close(fd1);    
    return 0;
}