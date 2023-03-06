#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

int main(){

    int pipefd[2];
    int ret =  pipe(pipefd);
    long size = fpathconf(pipefd[0], _PC_PIPE_BUF);
    printf("length of pipe is : %ld\n", size);

    return 0;
}