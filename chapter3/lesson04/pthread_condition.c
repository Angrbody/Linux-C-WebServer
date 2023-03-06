/*

    条件变量的使用
        条件变量类型：pthread_cond_t

*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


// 创建互斥量
pthread_mutex_t mutex;

// 创建条件变量
pthread_cond_t cond;

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
        pthread_mutex_lock(&mutex);
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->next = head;
        head = newNode;
        newNode->value = rand()%1000;
        printf("add node, value = %d, tid = %ld\n", newNode->value, pthread_self());

        // 只要生产了一个，就通知消费者消费
        pthread_cond_signal(&cond);
        
        pthread_mutex_unlock(&mutex);
        usleep(100);
    }

    return NULL;
}

void * purchase(void * arg){

    // 不断的删除节点
    while(1){
        pthread_mutex_lock(&mutex);
        if(head != NULL){
            // 链表中有节点
            struct Node* tmp = head;
            head = head->next;
            printf("delete node, value = %d, pid = %ld\n", tmp->value, pthread_self());
            free(tmp);
            usleep(100);
            
        }else{
            // 没有节点，需要等待信号
            // 当 wait调用时会进行解锁
            // 不阻塞继续执行时会加锁
            pthread_cond_wait(&cond, &mutex);
        }
       pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main(){


    // 互斥锁初始化
    pthread_mutex_init(&mutex, NULL);
    // 条件变量初始化
    pthread_cond_init(&cond, NULL);

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
    pthread_cond_destroy(&cond);

    // 主线程退出
    pthread_exit(NULL);

    

    return 0;
}