/*
    创建一个目录
    #include <sys/stat.h>
    #include <sys/types.h>

    int mkdir(const char *pathname, mode_t mode);


*/
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
int main(){
    int ret = mkdir("aaa", 0777);

    if(ret == -1){
        perror("mkdir");
        return -1;
    }

    return 0;
}