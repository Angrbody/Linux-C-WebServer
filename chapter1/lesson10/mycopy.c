#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
int main(){
    //打开已存在的txt
    int srcfd = open("legendStory.txt", O_RDONLY);
    if(srcfd == -1){
        perror("open");
        return -1;
    }

    //创建并打开新的txt
    int targetfd = open("cpyLegendStory.txt", O_RDWR | O_CREAT);
    if(targetfd == -1){
        perror("open");
        return -1;
    }

    //读取源文件并向目标中写入
    char buffer[1024] = {0};
    int curLen = 0;
    while((curLen = read(srcfd, buffer, sizeof(buffer))) > 0){
        write(targetfd, buffer, curLen);
    }
    //关闭文件
    close(targetfd);
    close(srcfd);

    return 0;
}