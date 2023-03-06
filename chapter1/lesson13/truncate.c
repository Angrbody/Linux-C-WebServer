/*
    缩减或扩展文件至指定大小
    #include <unistd.h>
    #include <sys/types.h>

       int truncate(const char *path, off_t length);
       int ftruncate(int fd, off_t length);

*/
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
int main(){
    int ret = truncate("b.txt", 5);
    if(ret == -1){
        perror("truncate");
        return -1;
    }
    return 0;
}