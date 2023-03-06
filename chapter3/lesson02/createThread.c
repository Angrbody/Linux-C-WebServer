/*
    #include <pthread.h>

    int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg);
    - 创建一个新的线程（子线程）
    - 一般情况下main函数所在的线程为主线程，其余创建的线程为子线程
    - 程序中默认只有一个进程，fork()函数调用后变成两个
    - 程序中默认只有一个线程，pthread_create()函数调用后变成两个
    Compile and link with -pthread.

*/
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void* callback(void* arg){
    printf("i am a child thread...\n");
    printf("arg value: %d\n", *(int*)arg);
    return NULL;
}

int main(){

    pthread_t tid;

    int num = 10;

    // 创建一个子线程
    int ret = pthread_create(&tid, NULL, callback, (void*)&num);

    sleep(5);

    if(ret != 0){
        char * str = strerror(ret);
        printf("%s\n", str);
    }

    for(int i = 0; i < 5; i++){
        printf("%d\n",i);
    }



    return 0;
}