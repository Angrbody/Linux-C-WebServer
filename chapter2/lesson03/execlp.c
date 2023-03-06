/*
     #include <unistd.h>

       extern char **environ;

       int execl(const char *path, const char *arg, ...
                       );
       int execlp(const char *file, const char *arg, ... ->会到环境变量中查找文件
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
        printf("父进程, pid : %d\n", getpid());
        sleep(1);
    }else if(pid == 0){
        // execl("hello", "hello", NULL);
        execlp("ps", "ps", "aux", NULL);
        perror("ps");
        printf("子进程\n");
    }

    for(int i = 0; i<3; i++){
        printf(" i = %d, pid: %d\n", i, getpid());
    }
    return 0;
}