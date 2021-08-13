### Thread

```c++
//std::mutex g_tex;
//可以与 lock_guard 愉快的玩耍 RAll 行程自动话资源释放
//std::lock_guard<mutex> lock(g_tex);

//std::thread::joinable
//检查 std::thread 对象是否标识活跃的执行线程。具体而言，若 get_id() != std::thread::id() 则返回 true 。故默认构造的 thread 不可结合。
//即就是检查线程是否存在，与有无执行代码无关
//结束执行代码，但仍未结合的线程仍被当作活跃的执行线程，从而可结合。

void fun()
{
	std::this_thread::sleep_for(std::chrono::seconds(2));
}

int main()
{
	std::thread t1;
	cout << t1.joinable() << endl;
	t1 = std::thread(fun);
	std::this_thread::sleep_for(std::chrono::seconds(4));
	cout << t1.joinable() << endl;
	t1.join();
	cout<<t1.joinable() << endl;
}


int main()
{
	//返回实现支持的并发线程数
	//虚拟的，可能与真实的不相符
	cout<<std::thread::hardware_concurrency() << endl; 
	return 0;
}



//该段代码印证我对 detach 的猜想
//一旦分离，两个线程不在有任何关系，运行完毕后自行释放资源
void independentThread()
{
	std::cout << "Starting concurrent thread.\n";
	std::this_thread::sleep_for(std::chrono::seconds(2));
	std::cout << "Exiting concurrent thread.\n";
}

void threadCaller()
{
	std::cout << "Starting thread caller.\n";
	std::thread t(independentThread);
	t.detach();
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "Exiting thread caller.\n";
}

int main()
{
	threadCaller();
	std::this_thread::sleep_for(std::chrono::seconds(5));
}


class Object
{
private:
	int value;
public:
	Object(int x = 0):value(x){}
	~Object(){}
	void funa(int a)
	{
		value += a;
		cout << value  << endl;
	}
	int& Value()
	{
		return value;
	}
};
struct functor {
	void operator()(){
		cout << "functor" << endl;
	}
};
int main()
{
	Object obj(5);
	functor fun;
	//这样传递参数只能是拷贝一个新的对象，值传递
	std::thread t1(&Object::funa,obj, 4);
	std::thread t2(&Object::funa, obj, 5);
	//地址传递，
	std::thread t3(&Object::funa, &obj, 4);
	std::thread t4(&Object::funa, &obj, 5);
	//仿函数的调用
	std::thread t5(fun);
	//资源转移
	//std::thread t6(std::move(t1));
	std::thread t6 = std::move(t1);
	//t3 = std::move(t6);
	//t1.join();  //如果发生资源转移，则不需要调用join释放资源
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
	cout << obj.Value() << endl;
	return 0;
}

const int n = 5;
int g_max = 0;
void fun(int& x)
{
	for (int i = 0; i < n; i++)
	{
		cout << x++ << endl;
	}
}

int main()
{
	int x = 10;
	//std::thread t1(fun, x); //编译无法通过
	//std::thread t2(fun, x);

	std::thread t1(fun, std::ref(x));
	std::thread t2(fun, std::ref(x));
	std::this_thread::sleep_for(std::chrono::seconds(1));
	t1.join();
	t2.join();
	cout << x << endl;
	return 0;

}


const int n = 5;
int g_max = 0;
void fun(int x,int *p)
{
	if (p == nullptr)return;
	for (int i = 0; i < n; i++)
	{
		cout << x++ << "  " << (*p)++ << endl;
	}
}

int main()
{
	int x = 10, y = 20;
	std::thread t1(fun,x,&y);
	std::thread t2(fun,x,&y);

	std::this_thread::sleep_for(std::chrono::seconds(1));
	t1.join();
	t2.join();
	return 0;

}


const int n = 5;
int g_max = 0;
void fun()
{
	for (int i = 0; i < 5; i++)
	{
		cout << "thread x:" << g_max++ << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}

int main()
{
	std::thread t1(fun);
	std::thread t2(fun);

	std::this_thread::sleep_for(std::chrono::seconds(1));
	t1.join();
	t2.join();
	return 0;

}



const int n = 5;
void fun()
{
	int x = 0;
	for (int i = 0; i < 5; i++)
	{
		cout << "thread x:" << x++ << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}

int main()
{
	std::thread t1(fun);
	std::thread t2(fun);

	std::this_thread::sleep_for(std::chrono::seconds(1));
	t1.join();
	t2.join();
	return 0;

}
```

