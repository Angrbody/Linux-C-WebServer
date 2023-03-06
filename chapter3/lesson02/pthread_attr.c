/*

    线程属性相关
    初始化

    释放资源

    获取线程分离的状态属性

    设置线程分离的状态属性

*/
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
void * callback(void * arg){
    printf("i am child thread\n");
    return NULL;
}
int main(){

    // 创建一个线程属性变量
    pthread_attr_t attr;

    // 初始化线程属性变量
    pthread_attr_init(&attr);

    // 设置属性
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    // 创建子线程
    pthread_t tid;
    int ret = pthread_create(&tid, &attr, callback, NULL);
    if(ret != 0){
        char * errstr = strerror(ret);
        printf("error1: %s\n", errstr);
    }

    // 输出主线程和子线程的id
    pthread_t mainid = pthread_self();
    printf("main tid: %ld, child tid: %ld\n", mainid, tid);

    
    // 获取线程 stack的大小
    size_t stackSize;
    pthread_attr_getstacksize(&attr, &stackSize);

    // 输出stack size
    printf("stack size = %ld\n", stackSize);

    // 释放线程属性资源
    pthread_attr_destroy(&attr);

    // 退出主线程
    pthread_exit(NULL);

    return 0;   
}