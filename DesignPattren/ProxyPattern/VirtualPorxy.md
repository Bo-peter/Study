### 虚拟代理(Virtual Proxy)

目的：帮助我们控制访问创建开销大的资源

虚拟代理作为创建开销大的对象的代表，经常会直到我们真正需要一个对象的时候才创建它。当对象在创建前和创建中时，由虚拟代理地来扮演对象的替身。对象创建后，代理就会将请求直接委托给对象。

示例：

​	

![image-20210716181420324](../image/image-1.png)

**![image-20210716182123785](../image/image-2.png)**

