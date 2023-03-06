#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
void pfileInfo(int fd){
    //1. 打印文件初始位置

    //2. 打印当前偏移量

    //3. 打印文件长度
}
int main(){
    //1. 打开已有的文件
    int fd = open("./Story.txt", O_RDWR);
    if(fd == -1){
        perror("open");
        return -1;
    }

    //2. 进行文件扩展
    int res = lseek(fd, 100, SEEK_END);
    if(res == -1){
        perror("lseek");
        return -1;
    }
    //3. 添加一个新的数据
    write(fd, " ", 1);

    //4. 关闭文件
    close(fd);

    return 0;
}