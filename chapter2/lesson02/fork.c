/*
    概要：
     #include <sys/types.h>
       #include <unistd.h>

       pid_t fork(void);

    函数作用：用于创建子进程（复制了地址空间）

    返回值：
        成功：父进程中返回子进程id, 子进程返回0（可以区分父进程和子进程）
        失败：父进程中返回-1，设置errno
        失败的两种主要原因：1.进程数到达上限， 2.内存不足

    fork: 写时拷贝， 只有进行写操作时才复制父进程的空间， 只涉及到读操作的时候父子进程共享空间。
    //父子进程共享文件，包括文件描述符fd， 文件指针， 引用计数器等。

    父子进程之间的区别：
        1. fork()返回值不同

        2. pcb中的一些数据不同
            当前进程的id：pid
            当前进程的父进程id：ppid
*/
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
int main(){
    //创建子进程
    pid_t pid = fork();
    int num = 0;
    //下面这里分别为父进程和子进程的专用代码区
    //判断父进程/子进程
    if(pid > 0){
        //只有父进程能执行
        for(int i = 0; i<5; i++){
            printf("i am parent process, pid: %d, ppid: %d\n", getpid(), getppid());
        }
        num += 10;
        printf("parent num: %d", num);

    }else if(pid == 0){
        //只有子进程能执行
        for(int i = 0; i<5; i++){
            printf("i am child process, pid: %d, ppid: %d\n", getpid(), getppid());
        }

        num += 20;
        printf("child num: %d", num);
    }else{
        perror("fork");
        return -1;
    }

    //下面为父进程和子进程的公共部分
    sleep(1);
    for(int i = 0; i<10; i++){
        printf("i : %d , pid: %d \n", i, getpid());
        sleep(1);
    }

    return 0;
}