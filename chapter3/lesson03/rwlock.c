/*

    读写锁
        写锁的优先级更高
        可以有多个读锁同时获取共享资源，但是会被写锁阻塞

    案例：创建8个线程，操作同一个全局变量
        其中有3个线程不定时写，其余5个不定时读

*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
// 创建共享数据
int num = 1;
pthread_mutex_t mutex;
pthread_rwlock_t rwlock;

void * writeNum(void * arg){

    while(1){
        pthread_rwlock_wrlock(&rwlock);
        num++;
        printf("++write, tid = %ld, num = %d\n", pthread_self(), num);
        pthread_rwlock_unlock(&rwlock);
        usleep(100);
    }        
    return NULL;
}

void * readNum(void * arg){

    while(1){
        pthread_rwlock_rdlock(&rwlock);
        printf("===read, tid = %ld, num = %d\n", pthread_self(), num);
        pthread_rwlock_unlock(&rwlock);
        usleep(100);
    }

    return NULL;

}
int main(){

    pthread_rwlock_init(&rwlock, NULL);

    // 创建8个子线程,3个写，5个读
    pthread_t wtids[3];
    pthread_t rtids[5];
    for(int i = 0; i < 3; i++){
        pthread_create(&wtids[i], NULL, writeNum, NULL);
    }

    for(int i = 0; i < 5; i++){
        pthread_create(&rtids[i], NULL, readNum, NULL);
    }

    // 设置线程分离
    for(int i = 0; i < 3; i++){
        pthread_detach(wtids[i]);
    }

    for(int i = 0; i < 5; i++){
        pthread_detach(rtids[i]);
    }  

    
    pthread_exit(NULL);

    pthread_rwlock_destroy(&rwlock);

    return 0;
}