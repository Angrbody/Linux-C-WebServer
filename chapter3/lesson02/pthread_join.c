/*
    #include <pthread.h>

    int pthread_join(pthread_t thread, void **retval);
        - 功能：和一个已经终止的线程进行连接 
            -> 释放子进程的资源，否则形成僵尸线程
        - 特点：
            -> 函数是阻塞函数，且调用一次只能回收一个子线程
            -> 一般在主线程中去使用
        - 参数：
            1. thread：需要回收的子线程的ID
            2. retval：接收子线程退出时的返回值，是一个二级指针
        - 返回值：
            成功：0
            失败：错误号，非零值
    Compile and link with -pthread.

*/
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
const int value = 10;
void * callback(void * arg){
    printf("iam child thread\n");
    printf("child thread id : %ld\n", pthread_self());
    // sleep(3);
    pthread_exit((void*)&value); // 返回的时候不要传递局部变量
}
int main(){

    pthread_t tid;
    int ret = pthread_create(&tid, NULL, callback, NULL);
    if(ret != 0){
        char* str = strerror(ret);
        printf("%s\n", str);
    }
    
    // 主线程
    for(int i = 0; i < 5; i++){
        printf("%d\n",i);
    }

    pthread_t maintid = pthread_self();
    printf("tid : %ld, main thread id : %ld\n", tid, maintid);


    // 主线程调用pthread_join()去回收子线程的资源
    int * thread_retval;

    ret = pthread_join(tid, (void**)&thread_retval);
    if(ret != 0){
        char* str = strerror(ret);
        printf("%s\n", str);
    }

    printf("回收子线程资源成功！\n");
    printf("exit data: %d\n", *thread_retval);

    // 退出主线程，其退出时不会影响其他正在运行的线程
    pthread_exit(NULL);

    return 0;
}