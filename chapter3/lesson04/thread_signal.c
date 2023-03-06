/*

    信号量（信号灯）
        灯亮：线程可访问共享资源
        灯灭：线程不可访问共享资源
    
    类型：sem_t

    方法：
        init
        destroy
        wait
        trywait
        timedwait
        post
        getvalue
        ...

*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// 创建互斥量
pthread_mutex_t mutex;

// 创建两个信号量
sem_t psem;
sem_t csem;

// 创建链表容器
struct Node{
    int value;
    struct Node* next;
};

// 头结点
struct Node* head = NULL;

void * produce(void * arg){

    // 不断的生产节点
    while(1){
        sem_wait(&psem);

        pthread_mutex_lock(&mutex);
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->next = head;
        head = newNode;
        newNode->value = rand()%1000;
        printf("add node, value = %d, tid = %ld\n", newNode->value, pthread_self());

        sem_post(&csem);
        
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void * purchase(void * arg){

    // 不断的删除节点
    while(1){
        sem_wait(&csem);

        pthread_mutex_lock(&mutex);
        // 链表中有节点
        struct Node* tmp = head;
        head = head->next;
        printf("delete node, value = %d, pid = %ld\n", tmp->value, pthread_self());
        free(tmp);

        pthread_mutex_unlock(&mutex);

        sem_post(&psem);
    }

    return NULL;
}

int main(){


    // 互斥锁初始化
    pthread_mutex_init(&mutex, NULL);
    sem_init(&psem, 0, 8);
    sem_init(&csem, 0, 0);

    // 分别创造 5个生产者线程和 5个消费者线程

    // 生产者
    pthread_t producer[5];
    for(int i = 0; i < 5; i++){
        int ret = pthread_create(&producer[i], NULL, produce, NULL);
        if(ret != 0){
            perror("pthread_create");
            exit(0);
        }
        printf("生产者线程 %d 创建成功！\n", i);
    }

    // 消费者
    pthread_t customer[5];
    for(int i = 0; i < 5; i++){
        int ret = pthread_create(&customer[i], NULL, purchase, NULL);
        if(ret != 0){
            perror("pthread_create");
            exit(0);
        }
        printf("消费者线程 %d 创建成功！\n", i);
    }    

    // 设置线程分离
    for(int i = 0; i < 5; i++){
        pthread_detach(customer[i]);
        pthread_detach(producer[i]);
    }

    // 设置循环防止提前释放互斥锁
    while(1){
        sleep(10);
    }

    // 释放互斥锁和条件变量资源
    pthread_mutex_destroy(&mutex);

    // 主线程退出
    pthread_exit(NULL);

    

    return 0;
}