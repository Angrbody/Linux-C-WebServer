#include "threadpool.h"
#include "http_conn.h"

template<typename T>
threadpool<T>::threadpool(int thread_number, int max_requests):
    m_thread_number(thread_number),
    m_max_requests(max_requests),
    m_stop(false),
    m_threads(NULL){
        if(thread_number <= 0 || max_requests <= 0){
            throw std::exception();
        }
        // 创建线程数组
        m_threads = new pthread_t(m_thread_number);
        if(!m_threads){
            throw std::exception();
        } 

        // 创建thread_number个线程，设置线程脱离
        for(int i = 0; i < m_thread_number; i++){
            printf("create the %dth thread\n", i);

            if(pthread_create(m_threads+i, NULL, worker, this) != 0){
                delete [] m_threads;
                throw std::exception();
            }

            if(pthread_detach(m_threads[i])){
                delete [] m_threads;
                throw std::exception();
            }
        }  
    }

template<typename T>
threadpool<T>::~threadpool(){
    delete [] m_threads;
    m_stop = true;
}

// 添加新任务到任务队列中
template<typename T>
bool threadpool<T>::append(T* request){

    m_queuelocker.lock();   //上锁
    if(m_workqueue.size() > m_max_requests){
        m_queuelocker.unlock(); //队列满则操作失败，返回
        return false;
    }

    m_workqueue.push_back(request); // 请求队列尾插
    m_queuelocker.unlock();         // 解锁
    m_queueStat.post();             // 为此任务添加信号量

}

template<typename T>
void* threadpool<T>::worker(void* arg){

    threadpool* pool = (threadpool*) arg;
    pool->run();
    return pool;

}

template<typename T>
void threadpool<T>::run(){
    while(!m_stop){
        m_queueStat.wait();
        m_queuelocker.lock();
        if(m_workqueue.empty()){
            m_queuelocker.unlock();
            continue;
        }

        T* request = m_workqueue.front();
        m_workqueue.pop_front();
        m_queuelocker.unlock();

        if(!request){
            continue;
        }

        request->process();
    }
}

template class threadpool<http_conn>;