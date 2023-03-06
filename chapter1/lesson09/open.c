#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
//1.打开一个已经存在的文件
// int open(const char* pathname, int flags);
// - pathname:文件路径
// - flags:文件的操作权限设置以及其他设置
// O_RDONLY, O_WRONLY, O_RDWR 三者互斥
//返回值：返回一个新的文件描述符，若失败返回-1

//errno：属于linux系统函数库，库里面的一个全局变量，记录的是最近的错误号
//2，创建并打开一个不存在的文件
// int open(const char* pathname, int flags, mode_t mode);

// void perror(const char *s);

int main(){
    //打开一个文件
    int fd = open("abc.txt", O_RDONLY);
    
    if(fd == -1){
      
    }
    
    //读写操作

    //关闭
    close(fd);
    return 0;
}
