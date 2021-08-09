### Thread

#### 第一天

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
void fun(int x,int p)
{
	if (p == nullptr)return;
	for (int i = 0; i < n; i++)
	{
		cout << x++ << "  " << (p)++ << endl;
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

#### 第二天

```c++
//使用线程和观察者模式，完成猫狗老鼠和人的问题
std::mutex mtx;
std::condition_variable cv;
int flag = 1;
int value = 1;
void print_A()
{
	unique_lock<std::mutex> locker(mtx);
	while (value<=100)
	{
		while (flag != 1)
		{
			cv.wait(locker);
			if (flag != 1)
			{
				cv.notify_one();
			}
		}
		cout << "A:"<<value++ << endl;
		flag = 2;
		cv.notify_one();
	}
}

void print_B()
{
	unique_lock<std::mutex> locker(mtx);
	while (value <= 100)
	{
		while (flag != 2)
		{
			cv.wait(locker);
			if (flag != 2)
			{
				cv.notify_one();
			}
		}
		cout <<"B:"<< value++ << endl;
		flag = 3;
		cv.notify_one();
	}
	
}

void print_C()
{
	unique_lock<std::mutex> locker(mtx);
	while (value <= 100)
	{
		while (flag != 3)
		{
			cv.wait(locker);
			if (flag != 3)
			{
				cv.notify_one();
			}
		}
		cout << "C:" <<value++ << endl;
		flag = 1;
		cv.notify_one();
	}
}

int main()
{
	std::thread p1(print_A);
	std::thread p2(print_B);
	std::thread p3(print_C);

	p1.join();
	p2.join();
	p3.join();
	return 0;
}
#if 0
std::mutex mtx;
std::condition_variable cv;
int flag = 1;
using namespace std;

void print_A() 
{
	unique_lock<std::mutex> locker(mtx);
	for (int i = 0; i < 10; i++)
	{
		while (flag != 1 )
		{
			cv.wait(locker);
			if (flag != 1)
			{
				cv.notify_one();
			}
		}
		cout << "A" ;
		flag = 2;
		cv.notify_one();
	}
}

void print_B()
{
	unique_lock<std::mutex> locker(mtx);
	for (int i = 0; i < 10; i++)
	{
		while (flag != 2)
		{
			cv.wait(locker);
			if (flag != 2)
			{
				cv.notify_one();
			}
		}
		cout << "B" ;
		flag = 3;
		cv.notify_one();
	}
}

void print_C()
{
	unique_lock<std::mutex> locker(mtx);
	for (int i = 0; i < 10; i++)
	{
		while (flag != 3)
		{
			cv.wait(locker);
			if (flag != 1)
			{
				cv.notify_one();
			}
		}
		cout << "C" << endl;
		flag = 1;
		cv.notify_one();
	}
}

int main()
{
	std::thread p1(print_A);
	std::thread p2(print_B);
	std::thread p3(print_C);

	p1.join();
	p2.join();
	p3.join();
	return 0;
}

//惊群现象怎么处理
class Queue
{
public:
	Queue(size_t max):maxsize(max){}
	queue<int> qu;
	size_t maxsize;
};

std::mutex mtx;
std::condition_variable cv;

Queue Q(10);

void productor_thread()
{
	srand(time(0));
	unique_lock<std::mutex> locker(mtx);
	for (int i = 0; i < rand()%20+10; i++)
	{
		//判断缓冲区满不满
		while (Q.maxsize == Q.qu.size())
		{
			cv.wait(locker);
		}
		Q.qu.push(1);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		cout << "product: 1" << endl;
		cv.notify_all();
	}
}
void consumer_thread()
{
	unique_lock<std::mutex> locker(mtx);
	while (true)
	{
		for (int i = 0; i < 5; i++)
		{
			while (Q.qu.empty())
			{
				cv.wait(locker);
			}
			Q.qu.pop();
			cout << "consumer: 1" << endl;
		}
		cv.notify_all();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

int main()
{
	std::thread pro1(productor_thread);
	std::thread pro2(productor_thread);
	std::thread pro3(productor_thread);

	std::thread con1(consumer_thread);
	//std::thread con2(consumer_thread);

	pro1.join();
	pro2.join();
	pro3.join();

	con1.join();
	//con2.join();
	return 0;
}

//条件变量 condition_varialbe
//一般需要 unique_lock<std::mutex> 配合使用

std::mutex mtx;
bool flag = false;
std::condition_variable cv;
int number = 0;
const int n = 10;
void producer_thread()
{
	std::unique_lock<std::mutex> locker(mtx);
	for (int i = 1; i < n; i++)
	{
		//if (flag)
		while(flag)	//使用while的原因是防止虚假唤醒
		{
			cv.wait(locker);
		}
		number = i;
		cout << "product:" << number << endl;
		flag = true;
		cv.notify_one();
	}
}

void consumer_thread()
{
	std::unique_lock<std::mutex> locker(mtx);
	for (int i = 1; i < n; i++)
	{
		//if (!flag ) 
		while(!flag)
		{
			cv.wait(locker);
		}
		cout << "consumer:" << number << endl;
		flag = false;
		cv.notify_one();
	}
}

int main()
{
	std::thread pro(producer_thread);
	std::thread con(consumer_thread);

	pro.join();
	con.join();
	return 0;
}
```

