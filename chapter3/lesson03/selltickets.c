/*

    使用多线程实现买票的案例
    有三个窗口，一共100张票


*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
int tickets = 100;

void * sellticket(void * arg){
    // 每个线程代表一个窗口，执行卖票的操作
    // int tickets = 100; //不能用局部变量
    while(tickets > 0){
        usleep(5000);
        printf("%ld 正在卖第 %d 张门票\n", pthread_self(), tickets);
        tickets--;
    }

    return NULL;
}
int main(){

    // 创建三个子线程
    pthread_t tid1, tid2, tid3;

    pthread_create(&tid1, NULL, sellticket, NULL);
    pthread_create(&tid2, NULL, sellticket, NULL);
    pthread_create(&tid3, NULL, sellticket, NULL);

    // 回收子线程的资源 (这里也可以设置线程分离)
    // pthread_join(tid1, NULL);
    // pthread_join(tid2, NULL);
    // pthread_join(tid3, NULL);

    // 设置线程分离
    pthread_detach(tid1);
    pthread_detach(tid2);
    pthread_detach(tid3);

    // 退出主线程
    pthread_exit(NULL);

    return 0;
}