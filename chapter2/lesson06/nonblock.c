#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
//设置管道非阻塞 
// int flags = fcntl(fd[0], F_GETFL);   //获取原来的flag
// flags |= 0_NONBLOCK;                 //设置新的flag
// fcntl(fd[0], F_SETFL, flags);        //应用新的flag
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

        int flags = fcntl(pipefd[0], F_GETFL);   //获取原来的flag
        flags |= O_NONBLOCK;                 //设置新的flag
        fcntl(pipefd[0], F_SETFL, flags);        //应用新的flag

        printf("i am parent process , pid : %d\n" , getpid());
        while(1){
            int len = read(pipefd[0], buf, sizeof(buf));
            printf("len: %d\n", len);
            printf("parent recv : %s, pid : %d\n", buf, getpid());
            memset(buf, 0, 1024);
            bzero(buf, 1024);
            sleep(2);
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
            sleep(5);
        }
        
    }

    return 0;
}