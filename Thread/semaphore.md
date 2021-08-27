### semaphore

```c++
//仿写semaphore
class Semaphore {
public:
	Semaphore(int x = 1):count(x){}
	~Semaphore(){}
	Semaphore(const Semaphore&) = delete;
	const Semaphore& operator=(const Semaphore&) = delete;
	void P()
	{
		std::unique_lock<std::mutex> locker(mtx);
		if (--count < 0)
		{
			cv.wait(locker);
		}
	}

	void V()
	{
		std::unique_lock<std::mutex> locker(mtx);
		if (++count <= 0)
		{
			cv.notify_one();
		}
	}
private:
	int count;
	std::mutex mtx;
	std::condition_variable cv;
};
```

