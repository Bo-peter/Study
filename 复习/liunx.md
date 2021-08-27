### Liunx 基础复习

##### 目录结构

内核 --> boot  

命令 --> bin

库文件 --> lib

配置文件 --> etc

普通用户的家目录  -->home

##### 命令

gerp 

|

find
cat (合并文件)
tail (tail -f <filename>) 跟踪文件更新

more

chome

awk

sed

预编译 编译 汇编  链接

widows PE格式

liunx ELF格式

##### 静态库 和 共享库的区别

ldd 命令可以查看 执行文件所使用的共享库

##### GDB调试

gcc -g 

l   	显示代码

b		+行号  、函数名

info break

delete  断点

r 		启动调试

n		单步执行

s 		进入函数

finish 	退出函数

p  		打印

c		继续执行

bt 		函数调用的栈关系

set	follow-fork-mode <mode>  选择调试跟踪那个进程

info threads 查看线程信息

thread id 	调试目标id指定的线程

set scheduler-locking off|on|step 

"off "  表示不锁定任何线程

"on" 只有当前被调试的线程继续运行

"step" 在但不执行的时候，只有当前线程会执行；



高性能服务器编程

##### 进程 fork

僵死进程：子进程先于父进程结束，父进程没有获取子进程的退出码

处理僵死 进程 父进程 - wait

SIGCHLD 忽略这个信号 可以解决 仅限于 liunx

fork() 的过程

写时拷贝

liunx内核设计与实现第三章

虚拟内存

```c
1.	3
{
	fork()||fork();
    printf("A\n");
}
2.	6
    for(int i=0;i<2;i++)
    {
		fork();
        printf("A\n");
    }
3.	8
    for(int i=0;i<2;i++)
    {
		fork();
        printf("A");
    }
```

fork+exec()  //新进程的产生

##### 信号：

(大概了解) 在什么情况下会发送信号

子进程 结束发送给父进程 -> SIGCHLD

ctrl+c  SIGINT

kill 默认发送15

kill(pid,SIGINT) 

signal 改变信号的相应方式 

signal(SIGINT,fun_sig)

##### 进程间通讯：IPC机制

管道 共享内存 信号量 消息队列 套接字

管道（半双工）写入管道的数据在内存中

有名管道：任意两个进程间，通讯

无名管道：父子

管道的实现。

信号量：临界资源 临界区 p  v

二值信号量，0，1

计数信号量  3

临界资源：同一时刻只允许一个进程访问的资源，我们称为临界资源

临界区：访问临界资源的代码段

我们只能对信号量（一般是正数值）进行原子+1 或者原子 -1 ，信号量是用来同步进程或者线程的。控制我们对临界资源的访问

书：操作系统精髓与原理 第五章

共享内存

![image-20210809173856338](C:\Users\lenovo\AppData\Roaming\Typora\typora-user-images\image-20210809173856338.png)

A 和 B进程共享内存的同一块内存

共享内存一旦创建，只有我们显示的移除，才会消失。

ipcs 命令可以查看我们创建的共享内存、消息队列、信号量

消息队列

![image-20210809174234971](C:\Users\lenovo\AppData\Roaming\Typora\typora-user-images\image-20210809174234971.png)

我们可以根据不同的类型去获取不同的消息 

加锁：<liunx内核csdn>  内核已经加锁同步，不需要我们加锁



##### 线程

进程内部的一条执行路径（序列）调度执行

进程：一个正在运行的程序，资源分配

书：操作系统精髓与原理

多线程实现机制:

从操作系统层面：用户，内核，组合

用户：创建开销非常小    并发运行  内核看到他们 

内核：操作系统直接管理  创建开销较大由内核直接完成 可以并行运行

组合：

 ps -eLf  显示线程信息 

书：操作系统精髓与原理

liunx线程的实现

书：liunx内核设计与实现 3.3

pthread_create->clone->d0_fork()

strace -p  pod可以跟踪在执行过系统调用

ltrace 跟踪执行过的库函数

当发生死锁时，可以用他查找

同步：

对程序得执行流程进行控制，使程序得正确性有保证

信号量，互斥锁，条件变量，读写锁

线程安全：

多线程程序，无论调度次序如何，都能得到正确得结果。

第一要同步，第二引入线程安全的函数

静态变量 和 全局变量 导致程序不安全

死锁 概念



##### 网络编程

ip地址 唯一标识一台主机

端口号 在主机上标识一个进程

小于1024的是知名端口

5000以内 临时端口

以上 临时端口



套接字地址结构

 

网络的字节序列

大端存储

小端存储

协议：约定、规定

分成：tcp/ip   数据链路层 网络层(ip) 传输层（tcp/udp） 应用层（http)

为什么要分层？

tcp 服务器客户端编程流程

 服务器：

socket()->bind()->listen()->accept()->recv()->send()->close()

客户端：

socket()->conncet()(开始三次握手)

 tcp 特点：面向链接的 可靠的 流式服务

udp 特点：无连接 不可靠，数据报

netstat -natp 命令可以查看发送缓冲区和接受接收缓冲区中有多少个数据

time_wait状态

1可靠的终止tcp的链接

2迟来的报文能够被识别并被丢弃



tcp可靠性：应答确认  超时重传 滑动窗口进行流量控制

​					乱序重排，去重

慢启动，快速恢复，快速重排



http 协议 应用层协议

www.baidu.com->通过DNS解析（IP，端口，connect)----三次握手-

短链接 

长连接： 两次及两次以上复用同一个链接

##### IO复用

select  poll  (select 加强版)

1每次循环都需要向内核空间拷贝数据

2内核实现：轮询，O(n)

3返回就绪的数量，需要再次遍历所有描述符O(n)

epoll (liunx下特有的)

```
epoll_create
epoll_ctl
epoll_wait
1 创建内核事件表，每个描述符只需要拷贝一次
2 内核实现：注册回调函数O(1)
3 用户减速到就绪描述符 O(1)
epoll的两种模式
LT 反复提醒，知道用户将数据处理完
ET 提醒一次
```

守护进程点编程流程

1 fork() 退出父进程

2 setsid()

3 fork()退出父进程

4 chdir("/")更改工作路径

5umask(0)

6close关闭描述符

7处理僵死进程



fork() 或者 epoll 的源码
