/*

    线程池类
        找一个线程去做任务

*/

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>
#include <list>
#include "locker.h"
#include <exception>
#include <cstdio>

// 定义成模板类以便代码复用和功能扩展
template<typename T>
class threadpool{

public:
    threadpool(int thread_number = 8, int max_requests = 10000);
    ~threadpool();
    // 添加任务
    bool append(T* request);

private:
    // 线程的数量
    int m_thread_number;

    // 线程池数组，大小为 m_thread_number
    pthread_t * m_threads;

    // 请求队列中最多允许的，等待处理的数量
    int m_max_requests;

    // 请求队列
    std::list<T*> m_workqueue;

    // 互斥锁
    locker m_queuelocker;

    // 信号量,判断是否有任务需要处理
    sem m_queueStat;

    // 是否结束线程
    bool m_stop;

    // 子线程执行具体的读写任务
    static void* worker(void* arg);

    // 执行任务的方法
    void run();
    
};


#endif

