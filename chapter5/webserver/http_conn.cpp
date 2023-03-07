#include "http_conn.h"
#include <sys/epoll.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/uio.h>
#include "locker.h"

int http_conn::m_epollfd = -1;
int http_conn::m_user_count = 0;

void setnonblocking(int fd){
    int old_flag = fcntl(fd, F_GETFL);
    int new_flag = old_flag | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_flag);
}
// 向epoll中添加需要监听的文件描述符
void addfd(int epollfd, int fd, bool one_shot){
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLRDHUP;

    if(one_shot){
        event.events |= EPOLLONESHOT;
    }

    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);

    // 设置文件描述符非阻塞
    setnonblocking(fd);
}


// 从epoll中移除监听的文件描述符
void removefd(int epollfd, int fd){
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL);
    close(fd);
}

// 修改epoll中的文件描述符
// 重置socket上EPOLLONESHOT事件，以确保下一次可读时，EPOLLIN事件能被触发
void modfd(int epollfd, int fd, int ev){
    epoll_event event;
    event.data.fd = fd;
    event.events = ev | EPOLLONESHOT | EPOLLRDHUP;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
}

// 初始化新接入的连接
void http_conn::init(int sockfd, const sockaddr_in &addr){
    m_sockfd = sockfd;
    m_address = addr;

    //　端口复用
    int reuse = 1;
    setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    // 添加到 epoll对象中
    addfd(m_epollfd, m_sockfd, true);
    m_user_count++;

    init_state();
}

void http_conn::init_state(){
    m_check_state = CHECK_STATE_REQUESTLINE;
    m_checked_index = 0;
    m_start_line = 0;
    m_read_index = 0;
}

//　关闭连接
void http_conn::close_conn(){
    if(m_sockfd != -1){
        removefd(m_epollfd, m_sockfd);
        m_sockfd = -1;
        m_user_count--;
    }
}

// 主线程一次性读完所有数据
bool http_conn::read(){
    
    if(m_read_index >= read_buffer_size){
        return false;
    }

    // 读取到的字节
    int bytes_read = 0;
    while(true){
        bytes_read = recv(m_sockfd, m_read_buf + m_read_index, read_buffer_size - m_read_index, 0);
        if(bytes_read == -1){
            if(errno == EAGAIN | errno == EWOULDBLOCK){
                break;
            }
            return false;
        }else if(bytes_read == 0){
            // 对方关闭连接
            printf("client closed...\n\n");
            return false;

        }
        m_read_index += bytes_read;
    }

    printf("----- 读取到了数据: -----\n%s\n", m_read_buf);

    return true;
}



// 由线程池中的工作线程调用，这是处理 HTTP请求的入口函数
void http_conn::process(){
   
    // 解析HTTP请求
    HTTP_CODE read_ret = process_read();
    if(read_ret == NO_REQUEST){
        modfd(m_epollfd, m_sockfd, EPOLLIN);
        return;
    }else if()

    printf("parse request, create response\n");
    
    // 生成响应

}

// 主状态机,从大的范围解析请求
http_conn::HTTP_CODE http_conn::process_read(){

    // 定义初始状态
    LINE_STATUS line_status = LINE_OK;
    HTTP_CODE ret = NO_REQUEST;
    
    char * text = 0;

    while((m_check_state == CHECK_STATE_CONTENT) && (line_status == LINE_OK)
        || (line_status = parse_line()) == LINE_OK){
        // 解析到了一行完整的数据，或请求到了
        text = getline();

        m_start_line = m_checked_index;
        printf("got 1 http line : %s\n", text);

        switch ( m_check_state ) {
            case CHECK_STATE_REQUESTLINE: 
            {
                ret = parse_request_line( text );
                if ( ret == BAD_REQUEST ) {
                    return BAD_REQUEST;
                }
                break;
            }
            case CHECK_STATE_HEADER: 
            {
                ret = parse_headers( text );
                if ( ret == BAD_REQUEST ) {
                    return BAD_REQUEST;
                } else if ( ret == GET_REQUEST ) {
                    return do_request();
                }
                break;
            }
            case CHECK_STATE_CONTENT: 
            {
                ret = parse_content( text );
                if ( ret == GET_REQUEST ) {
                    return do_request();
                }
                line_status = LINE_OPEN;
                break;
            }
            default: 
            {
                return INTERNAL_ERROR;
            }
        }

    }
    return NO_REQUEST;
}

http_conn::HTTP_CODE http_conn::do_request(){
    
}

http_conn::HTTP_CODE http_conn::parse_request_line(char * text){

}

http_conn::HTTP_CODE http_conn::parse_headers(char * text){

}

http_conn::HTTP_CODE http_conn::parse_content(char *text){

}        

http_conn::LINE_STATUS parse_line(){

}                 



// 主线程一次性写完所有数据
bool http_conn::write(){
    printf("一次性写完数据\n");
    return true;
}