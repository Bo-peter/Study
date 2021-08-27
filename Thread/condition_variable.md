```c++



int number = 0;
std::mutex mtx;
std::condition_variable cv;
void fun0()
{
	while (number <= 98)
	{
		std::unique_lock<std::mutex> locker(mtx);
		while (number % 3 != 0 && number <= 98)
		{
			cv.wait(locker);
			if (number % 3 != 0)
			{
				cv.notify_one();
			}
		}
		cout << number << " ";
		number++;
		cv.notify_one();
	}
}

void fun1()
{
	while (number <= 98)
	{
		std::unique_lock<std::mutex> locker(mtx);
		while (number % 3 != 1 && number <= 98)
		{
			cv.wait(locker);
			if (number % 3 != 1)
			{
				cv.notify_one();
			}
		}
		cout << number << " ";
		number++;
		cv.notify_one();
	}
}

void fun2()
{
	while (number <= 98)
	{
		std::unique_lock<std::mutex> locker(mtx);
		while (number % 3 != 2 && number <= 98)
		{
			cv.wait(locker);
			if (number % 3 != 2)
			{
				cv.notify_one();
			}
		}
		cout << number << " ";
		number++;
		cv.notify_one();
	}
}
int  main()
{
	std::thread t1(fun0);
	std::thread t2(fun1);
	std::thread t3(fun2);
	t1.join();
	t2.join();
	t3.join();
	return 0;
}
```

```c++
//避免了惊群现象
//但是不能并发生产
//完成runbuf的实现，模拟视频流

std::counting_semaphore bufStatus_full(1);
std::counting_semaphore bufStatus_use(1);
std::counting_semaphore bufGet(0);

list<int> buf;
const int MAX = 10;
void prudutor_thread()
{
	while (true)
	{
		bufStatus_full.acquire();
		bufStatus_use.acquire();
		int index = 0;
		if ((index = MAX - buf.size()) > 3)
		{
			for (int i = 0; i < 3; i++)
			{
				buf.push_back(1);
				if (buf.size() == 1)
				{
					bufGet.release();
				}
			}
			bufStatus_full.release();
		}
		else
		{
			for (int i = 0; i < index; i++)
			{
				buf.push_back(1);
			}
		}
		bufStatus_use.release();
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void consumer_thread()
{
	while (true)
	{
		bufGet.acquire();
		bufStatus_use.acquire();
		int index = 0;
		if ((index = buf.size()) >= 3)
		{
			for (int i = 0; i < 3; i++)
			{
				cout << buf.front() << "  ";
				buf.pop_front();
				cout << " $" << buf.size() << "$   ";
				if (buf.size() == MAX - 1)
				{
					bufStatus_full.release();
				}
			}
			bufGet.release();
		}
		else
		{
			for (int i = 0; i < index; i++)
			{
				cout << buf.front() << endl;
				buf.pop_front();
			}
		}
		bufStatus_use.release();
	}
}



int main()
{
	std::thread t1(prudutor_thread);
	std::thread t2(consumer_thread);
	std::thread t3(consumer_thread);
	t1.join();
	t2.join();
	t3.join();
	return 0;
}
```

