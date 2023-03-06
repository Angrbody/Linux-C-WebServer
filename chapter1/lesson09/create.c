#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
int main(){
    int fd = open("create.txt", O_RDWR | O_CREAT, 0777);
    if(fd == -1)    perror("open");
    close(fd);
    return 0;
}