# Linux-C-WebServer
C++ Web Server on Ubuntu1804
1. **项目介绍：框架 + 实现内容 + 成果(学习到了哪些知识)**

   本项目为独立开发的B/S架构，其中服务器部署在linux环境下，客户端通过HTTP协议进行通信。

   webserver的整体框架可以分为三部分：I/O处理，业务逻辑以及存储单元

   - I/O处理模块使用epoll进行多路复用，设置非阻塞

   - 线程池处理具体的业务逻辑

   - 数据库连接池处理存储单元的连接

2. **为什么要做这个项目**

   该项目中包含大量的基础知识，包括linux的常用命令和操作，tcp/ip协议，网路通信，多线程等

3. **select poll  epoll，基本原理，底层结构，有什么区别**

   **select**

   ​	select是一种I/O多路复用机制，可以同时监视多个文件描述符的状态，当其中任意一个文件描述符就绪时，select函数就会返回。

   - 使用流程：
     1. 创建一个文件描述符集合，将需要监视的文件描述符添加到集合中
     2. 调用select函数，等待文件描述符就绪
     3. 当select函数返回时，遍历文件描述符集合，判断哪些文件描述符已经就绪
   - 方法与函数：
     1. FD_ZERO：将文件描述符集合清空
     2. FD_SET：将一个文件描述符添加到集合中
     3. FD_CLR：将一个文件描述符从集合中删除
     4. FD_ISSET：判断一个文件描述符是否在集合中

   **poll**

   **epoll**

   - 基本原理
   - 内核结构

   - 使用流程：
     1. int epoll_create(int size)：创建一个epoll对象，传入一个参数(此参数曾经是指示最大容量，现已废弃，但必须大于零)若操作成功则返回该epoll对象的fd，操作失败返回-1并设置errno。epoll对象内部有两个指针，分别维护其底层的红黑树和双向链表，一个指向RBtree的根节点，一个指向链表的首节点。
     2. int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)：将需要监控的文件描述符添加到epoll对象中。
     3. int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout)：初始化完成后程序会阻塞在epoll_wait这个函数，进入大循环，当某个fd上有事件发生时，内核会将其对应的结构体放入一个链表中，返回有事件发生的链表
   - 与poll和select的区别

4. **LT和ET**

   1. 原理
   2. 区别
   3. 具体读数据的方式
   4. 使用时的注意事项

5. **线程同步机制类封装及线程池实现**

   - 通过什么方式实现的线程同步？(互斥锁、条件变量、信号量)
   - 线程池怎么封装的，使用什么设计模式？
   - 线程池的容量怎么设置的
   - threadpool由任务队列taskqueue和线程池组成，应用了典型的生产者-消费者模型
   - lock类用于实现多线程同步
     - mutex互斥锁用于保证任务添加/获取的互斥性
     - 条件变量cond_var用于保证多个线程获取task的同步性
     - 信号量sem用于通知线程是否有任务需要处理

6. **消息队列**

7. **两种高效的事件处理模式之proactor模式，为什么使用了同步I/O模拟proactor？有试过reactor模式吗？**

   1. reactor模式

      reactor模式中，主线程只负责监听文件描述符上是否有事件发生，有的话就立即将该事件通知工作线程，即它传递的是“事件**就绪**‘状态，除此之外，主线程不做任何其他实质性的工作，读写数据，接受新的连接以及处理客户请求均在工作线程中完成。

8. **定时关闭连接的实现**（双链表结构的定时器）

   - 使用带头尾结点的升序双向链表管理定时器实现

   - 为每个连接创建一个定时器，将其添加到链表中，并按照超时时间升序排列。

   - 执行定时任务时，将到期的定时器从链表中删除。 

   - 添加定时器的事件复杂度是O(n), 删除定时器的事件复杂度是O(1)。

9. **解析HTTP请求报文**

   采用主-从状态机的方式进行报文解析

   从状态机负责读http报文中的一行（遇到换行符结束并返回状态）

   主状态机根据从状态机返回的状态来判断当前整体状态以及下一步需要做的操作

10. **同步/异步日志的实现**

    使用阻塞队列，建立生产者-消费者模型，异步进行日志输出

    同步日志直接由工作线程写入文件

11. **你使用了什么样的方法去提高并发量？**

    1. 使用epoll减少用户态和内核态之间的数据拷贝

    2. 设置listenfd LT + read/writefd ET的模式减少epoll_wait的阻塞时间

    3. 使用定时器定期关闭非活跃连接，释放系统资源(fd)

    4. 使用线程池+任务队列高效处理读写逻辑

    5. 设计异步日志系统，减少工作线程I/O阻塞

12. **项目还可以有哪些扩展/改进**

    1. 链表结构的定时器，查找的复杂度为o(n)，可以改为最小堆/红黑树结构

    2. 如果有多核条件，可以尝试将proactor模式转化为多reactor模式，充分利用多核性能，每一个核的线程负责循环监听一组文件描述符的集合

    3. 增加前端函数调用，扩展业务功能，如二维码扫码等

    4. 异步日志底层结构由阻塞队列可以改为双缓冲区，能够解决生产者消费者异步操作速度不匹配的问题，提升日志系统的整体吞吐率

    5. 实现文件上传功能

    6. 尝试实现HTTPS

    7. 利用cookie实现状态保存

    8. 尝试使用远程数据库，避免本地存储爆炸

    9. valgrind用来检测自己没写完的webserver有没有内存泄露

13. **使用webbench进行性能测试**

    注：下面的测试中均使用ET边沿触发模式

    首先在本地(和服务端共用一个服务器)进行压力测试，模拟4500个用户接入的高并发场景

    ![image-20230412224118484](C:\Users\jyliu\Desktop\学习\实习\面经\理论面经\图片\服务器本地测试，4500clients)

- 然后在远程(linux)上使用webbench，相同参数进行性能测试

  远程linux的环境：centos7 + webbench1.5

  参数：并发量最高能跑到27000左右，QPS可以达到2000左右

  单核2G服务器QPS

  1. 原生php：105
  2. thinkphp5.0：105
  3. laravel5.6：23

  github上star数最多的webserver

  环境：Ubuntu:19.10 cpu:i5-8400(6核) 内存:8G

  QPS：10000左右

- 虚拟机上测试：

  ![image-20230412223123317](C:\Users\jyliu\Desktop\学习\实习\面经\理论面经\图片\虚拟机测试4500)

- 分别对2/4/8个线程状态下的服务器进行压力测试，发现QPS基本相同。

13. **除了webbench以外，你还用过其他测试性能的工具吗？**

    valgrind

14. **遇到的问题：**

    1. 一开始设置ET的时候，发现读到的数据不全，或者有丢失数据，原因是没有正确设置Ewouldblock和eagain
    2. 一开始listenfd设置的也是ET，结果发现性能竟然比LT还差，后来发现是ET的一个bug，当两个请求建立连接的客户端fd同时/相差很小到达服务器时，LT模式容易忽略掉其中一个，所以后面改成了listenfd LT + 工作fd ET
    3. 服务器端口问题：一开始怎么都连接不上服务器，后来发现是云服务器的防火墙没开相应端口
    4. 大文件传输问题：服务器发送文件数据时只调用了writev函数，并对其返回值是否异常做了处理，在发送小文件(调用一次writev就可以发送全部数据时)不会报错。但是当一次writev写不完数据时，会出现文件不全，客户端资源卡住的问题。原因是writev的m_iv结构体有问题，在多次传输中还是会按照原有指针和原有长度发送数据。更新后的代码在每次传输后都要更新下次传输文件的起始位置和长度(偏移量)
