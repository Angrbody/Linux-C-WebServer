/*
    #include <pthread.h>

    int pthread_cancel(pthread_t thread);
        - 功能：取消线程（结束线程）
        - 特点：只是发送一个取消的请求，不一定立即终止线程，而是到取消点才进行取消操作
    Compile and link with -pthread.



*/
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
void * callback(void * arg){
    printf("child thread , tid = %ld\n", pthread_self());
    
    for(int i = 0; i < 5; i ++){
        // 这里的printf就是一个取消点
        printf("child\n");
    }
    return NULL;
}

int main(){

    // 创建子线程
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, callback, NULL);
    if(ret != 0){
        char * errstr = strerror(ret);
        printf("error1: %s\n", errstr);
    }

    // 输出主线程和子线程的id
    pthread_t mainid = pthread_self();
    printf("main tid: %ld, child tid: %ld\n", mainid, tid);

    for(int i = 0; i < 5; i ++){
        printf("i = %d\n", i);
    }

    // 取消线程
    pthread_cancel(tid);

    // 退出主线程
    pthread_exit(NULL);   

    return 0;
}