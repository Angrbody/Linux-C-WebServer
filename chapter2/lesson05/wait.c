#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main(){

    
    pid_t pid;

    for(int i = 0; i < 5; i++){
        pid = fork();
        if(pid == 0){
            break;
        }
    }

    if(pid > 0){
        while(1){
            printf("parent, pid = %d\n", getpid());

            int ret = wait(NULL);
            if(ret == -1){
                break;
            }

            printf("child die, pid = %d\n", ret);

            sleep(1);
        }
    }else if(pid == 0){
        printf("child pid = %d\n", getpid());
        sleep(3);
        exit(1);
        
    }

    return 0;
}