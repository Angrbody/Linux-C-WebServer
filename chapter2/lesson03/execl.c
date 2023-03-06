/*
     #include <unistd.h>

       extern char **environ;

       int execl(const char *path, const char *arg, ...
                       );
       int execlp(const char *file, const char *arg, ...
                       );
       int execle(const char *path, const char *arg, ...
                       );
       int execv(const char *path, char *const argv[]);
       int execvp(const char *file, char *const argv[]);
       int execvpe(const char *file, char *const argv[],
                       char *const envp[]);
*/
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
int main(){
    
    //创建子进程
    pid_t pid = fork();
    if(pid > 0){
        printf("父进程在操作, pid : %d\n", getpid());
        sleep(1);
    }else if(pid == 0){
        // execl("hello", "hello", NULL);
        printf("子进程在操作：\n");
        // execl("/bin/ps", "ps", "aux", NULL);
        //注意这里，execl只能替换一次，因为是使用目标进程替换当前的进程
        execl("/bin/ls", "ls", NULL);
    }

    printf("这里是主程序\n");
    for(int i = 0; i<3; i++){
        printf(" i = %d, pid: %d\n", i, getpid());
    }
    return 0;
}