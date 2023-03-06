#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
int main(){

    //获取当前目录
    char buf[512];
    char *s = getcwd(buf, sizeof(buf));
    if(s == NULL){
        perror("getcwd");
        return -1;
    }
    printf("当前工作目录： %s\n", buf);

    //修改目录
    int ret = chdir("../lesson13");
    if(ret == -1){
        perror("chdir");
        return -1;
    }

    //生成新文件
    ret = open("txtfromlesson14.txt", O_RDWR | O_CREAT, 664);
    if(ret == -1){
        perror("open");
        return -1;
    }

    //获取当前目录
    char buf1[512];
    getcwd(buf1, sizeof(buf));
    printf("当前工作目录： %s\n", buf1);

    return 0;
}