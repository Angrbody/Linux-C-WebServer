/*

    #include <pthread.h>

    void pthread_exit(void *retval);
        - 功能：终止线程，谁调用终止谁
    Compile and link with -pthread.


*/
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void * callback(void * arg){
    printf("iam child thread\n");
    printf("child thread id : %ld\n", pthread_self());
    return NULL; // 相当于pthread_exit(NULL)
}
int main(){

    pthread_t tid;
    int ret = pthread_create(&tid, NULL, callback, NULL);
    if(ret != 0){
        char* str = strerror(ret);
        printf("%s\n", str);
    }
    
    // pthread_exit(NULL);

    for(int i = 0; i < 5; i++){
        printf("%d\n",i);
    }

    pthread_t maintid = pthread_self();
    printf("tid : %ld, main thread id : %ld\n", tid, maintid);

    ret = pthread_equal(maintid, tid);
    if(ret == 0){
        printf("diffrent tid\n");
    }else{
        printf("equal tid\n");
    }

    // 退出主线程，其退出时不会影响其他正在运行的线程
    pthread_exit(NULL);

    printf("main thread exit\n");

    return 0;
}