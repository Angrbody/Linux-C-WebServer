/*

    模拟死锁
    
    - 死锁的几种产生原因
        1. 没有释放锁
        2. 多次加同一个锁
        3. 多线程操作多个锁

*/
/*

    1. 初始化互斥量
    2. 释放互斥量
    3. 上锁
    4. 解锁


*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int tickets = 1000;
// 创建一个互斥量
pthread_mutex_t mutex;

void * sellticket(void * arg){
    // 每个线程代表一个窗口，执行卖票的操作
    // int tickets = 100; //不能用局部变量
    while(1){
        // 加锁
        pthread_mutex_lock(&mutex);

        if(tickets > 0){
            usleep(5000);
            printf("%ld 正在卖第 %d 张门票\n", pthread_self(), tickets);
            tickets--;
        }else{
            break;
        }
        // 解锁
        pthread_mutex_unlock(&mutex);

    }

    // 解锁
    pthread_mutex_unlock(&mutex);

    return NULL;
}
int main(){

    // 初始化互斥量
    pthread_mutex_init(&mutex,NULL);
    
    // 创建三个子线程
    pthread_t tid1, tid2, tid3;

    pthread_create(&tid1, NULL, sellticket, NULL);
    pthread_create(&tid2, NULL, sellticket, NULL);
    pthread_create(&tid3, NULL, sellticket, NULL);


    // 设置线程分离
    pthread_detach(tid1);
    pthread_detach(tid2);
    pthread_detach(tid3);

    // 退出主线程
    pthread_exit(NULL);


    // 释放互斥量资源
    pthread_mutex_destroy(&mutex);

    return 0;
}