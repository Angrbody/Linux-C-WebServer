/*
    判断某个文件是否有某个权限，或者判断文件是否存在
    int access(const char *pathname, int mode);
*/
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

int main(){
    int ret = access("a.txt", F_OK);
    if(ret == -1){
        perror("access");
        return -1;
    }
    printf("文件存在 ！ \n");
    return 0;
}
       