/*
    修改文件权限
    #include <sys/stat.h>

       int chmod(const char *pathname, mode_t mode);
       int fchmod(int fd, mode_t mode);

*/
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
int main(){
    
    int ret = chmod("a.txt", 0775);

    if(ret == -1){
        perror("chmod");
        return -1;
    }
    
    return 0;
}