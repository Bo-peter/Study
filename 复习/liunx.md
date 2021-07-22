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

fork+exec()

##### 信号：

