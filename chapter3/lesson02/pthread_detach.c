/*
    #include <pthread.h>

    int pthread_detach(pthread_t thread);
        - 功能：分离一个线程
        - 参数：
            thread：线程id
        被分离过的线程结束时期资源将被自动释放给系统，无需pthread_join()处理
        已经分离过的线程将无法被分离（禁止！）
        已经分离过的线程不能再去连接，会报错
        - 返回值：
            成功：0
            失败：错误号
    Compile and link with -pthread.


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


    // 设置子线程分离
    ret = pthread_detach(tid);
    if(ret != 0){
        char * errstr = strerror(ret);
        printf("error2: %s\n", errstr);
    }

    // 对分离的子线程进行连接
    ret = pthread_join(tid, NULL);
    if(ret != 0){
        char * errstr = strerror(ret);
        printf("error3: %s\n", errstr);
    }
    // 退出主线程
    pthread_exit(NULL);

    return 0;   
}