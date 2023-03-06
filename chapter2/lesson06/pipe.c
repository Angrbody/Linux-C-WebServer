/*
    创建一个匿名管道用于进程间通信
    匿名管道只能用于具有关系的进程之间的通信（父子进程、兄弟进程）
    管道默认是阻塞的，如果管道中没有数据，read阻塞，如果管道数据满了，write阻塞
    #include <unistd.h>

       int pipe(int pipefd[2]);
        参数：数组int pipefd[2]，
            pipefd[0] 为管道读端
            pipefd[0] 为管道写端
        返回值：
            成功返回 0
            失败返回-1，并设置错误号
       #define _GNU_SOURCE             
       #include <unistd.h>

       int pipe2(int pipefd[2], int flags);

*/
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

// 子进程发送数据给父进程，父进程读取到数据输出
int main(){

    //在fork之前创建管道
    int pipefd[2];

    int ret = pipe(pipefd);

    if(ret == -1){
        perror("pipe");
        exit(0);
    }

    //创建子进程
    pid_t pid = fork();

    if(pid > 0){
        //父进程
        //从管道的输出端读取数据
        //关闭写端
        close(pipefd[1]);
        char buf[1024] = {0};
        printf("i am parent process , pid : %d\n" , getpid());
        while(1){
            int len = read(pipefd[0], buf, sizeof(buf));
            printf("parent recv : %s, pid : %d\n", buf, getpid());
            bzero(buf, 1024);

            //父进程写
            // char * str = "hello , i'm parent";
            // write(pipefd[1], str, strlen(str));
            // sleep(1);
        }
    }else if(pid == 0){
        //子进程
        //向管道中不断写入数据 
        //关闭读端
        close(pipefd[0]);
        char buf[1024] = {0};
        printf("i am child process , pid : %d\n" , getpid());
        while(1){
            char * str = "hello , i'm child";
            write(pipefd[1], str, strlen(str));
            // sleep(1);

            // int len = read(pipefd[0], buf, sizeof(buf));
            // printf("child recv : %s, pid : %d\n", buf, getpid());
            // bzero(buf, 1024);
        }
        
    }

    return 0;
}