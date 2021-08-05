![image-20210801161449579](C:\Users\lenovo\AppData\Roaming\Typora\typora-user-images\image-20210801161449579.png)

```c++
namespace peter
{
	template<typename T>
	class RefCnt
	{
	private:
		T* mPtr;
		std::atomic<int> mCnt;
	public:
		RefCnt(T* ptr = nullptr) :mPtr(ptr)
		{
			if (mPtr != nullptr)
			{
				mCnt = 1;
			}
		}
		void addRef() { mCnt++; }
		int delRef() { return --mCnt; }
		int getCnt() { return mCnt.load; }
	};
	template<typename T>
	struct MyDeletor {
		void operator()(T* ptr)
		{
			delete ptr;
		}
	};
	template<typename T>
	struct MyDeletor<T[]>
	{
		void operator()(T* ptr)
		{
			delete[] ptr;
		}
	};
	template<typename T,typename Deletor = MyDeletor<T>>
	class shared_ptr
	{
	public:
		shared_ptr(T* ptr = nullptr) :mPtr(ptr)
		{
			if (mPtr != nullptr)
			{
				mpRefCnt = new RefCnt<T>(ptr);
			}
		}
		~shared_ptr()
		{
			if (mPtr != nullptr&&0 == mpRefCnt->delRef())
			{
				mDeletor(mPtr);
				delete mpRefCnt;
			}
			mPtr = nullptr;
			mpRefCnt = nullptr;
		}
		T& operator*()const { return *mPtr; }
		T* operator->()const { return mPtr; }
		shared_ptr(const shared_ptr& ptr):mPtr(ptr.mPtr),mpRefCnt(ptr.mpRefCnt)
		{
			if (mPtr != nullptr)
			{
				mpRefCnt->addRef();
			}
		}
		// null null
		// not null
		//null  not
		// not not
		shared_ptr& operator=(const shared_ptr& ptr)
		{
			if (this == &ptr || mPtr == ptr.mPtr) return *this;
			if (mPtr != nullptr && mpRefCnt->delRef() == 0)
			{
				mDeletor(mPtr);
				delete mpRefCnt;
				mpRefCnt = nullptr;
				mPtr = nullptr;
			}
			mPtr = ptr.mPtr;
			mpRefCnt = ptr.mpRefCnt;
			if (mPtr != nullptr)
			{
				mpRefCnt->addRef();
			}
			return *this;
		}
		shared_ptr(shared_ptr&& _u)
		{
			mPtr = _u.mPtr;
			mpRefCnt = _u.mPRefCnt;
			_u.mPtr = nullptr;
			_u.mpRefCnt = nullptr;
		}
		shared_ptr& operator=(shared_ptr&& _u)
		{
			if (this == &_u) return *this;
			if (mPtr != nullptr && 0 == mpRefCnt->delRef())
			{
				mDeletor(mPtr);
				mPtr = nullptr;
				delete mpRefCnt;
				mpRefCnt = nullptr;
			}
			mPtr = _u.Ptr;
			mpRefCnt = _u.mpRefCnt;
			_u.mPtr = nullptr;
			_u.mpRefCnt = nullptr;
			return *this;
		}
		const int use_count()const
		{
			if (mPtr != nullptr)
			{
				return mpRefCnt->getCnt();
			}
			return 0;
		}
		operator bool()
		{
			return mPtr != nullptr;
		}
		void reset(T* ptr)
		{
			if (mPtr == ptr) return;
			if (mPtr != nullptr && 0 == mpRefCnt->delRef())
			{
				mDeletor(mPtr);
				mPtr = nullptr;
				delete mpRefCnt;
				mpRefCnt = nullptr;
			}
			mPtr = ptr;
			mpRefCnt = nullptr;
			if (mPtr != nullptr)
			{
				mpRefCnt = new RefCnt<T>(ptr);
			}
		}
		void swap(shared_ptr& src)
		{
			std::swap(this->mPtr, src.mPtr);
			std::swap(this->mpRefCnt, src.mpRefCnt);
		}
	private:
		T* mPtr;
		peter::RefCnt<T>* mpRefCnt;
		Deletor mDeletor;
	};
	template<typename T, typename Deletor>
	class shared_ptr<T[],Deletor>
	{
	public:
		shared_ptr(T* ptr = nullptr) :mPtr(ptr)
		{
			if (mPtr != nullptr)
			{
				mpRefCnt = new RefCnt<T>(ptr);
			}
		}
		~shared_ptr()
		{
			if (mPtr != nullptr && 0 == mpRefCnt->delRef())
			{
				mDeletor(mPtr);
				delete mpRefCnt;
			}
			mPtr = nullptr;
			mpRefCnt = nullptr;
		}
		//T& operator*()const { return *mPtr; }
		//T* operator->()const { return mPtr; }
		T& operator[](const int index)const { return *mPtr[index]; }
		shared_ptr(const shared_ptr& ptr) :mPtr(ptr.mPtr), mpRefCnt(ptr.mpRefCnt)
		{
			if (mPtr != nullptr)
			{
				mpRefCnt->addRef();
			}
		}
		// null null
		// not null
		//null  not
		// not not
		shared_ptr& operator=(const shared_ptr& ptr)
		{
			if (this == &ptr || mPtr == ptr.mPtr) return *this;
			if (mPtr != nullptr && mpRefCnt->delRef() == 0)
			{
				mDeletor(mPtr);
				delete mpRefCnt;
				mpRefCnt = nullptr;
				mPtr = nullptr;
			}
			mPtr = ptr.mPtr;
			mpRefCnt = ptr.mpRefCnt;
			if (mPtr != nullptr)
			{
				mpRefCnt->addRef();
			}
			return *this;
		}
		shared_ptr(shared_ptr&& _u)
		{
			mPtr = _u.mPtr;
			mpRefCnt = _u.mPRefCnt;
			_u.mPtr = nullptr;
			_u.mpRefCnt = nullptr;
		}
		shared_ptr& operator=(shared_ptr&& _u)
		{
			if (this == &_u) return *this;
			if (mPtr != nullptr && 0 == mpRefCnt->delRef())
			{
				mDeletor(mPtr);
				mPtr = nullptr;
				delete mpRefCnt;
				mpRefCnt = nullptr;
			}
			mPtr = _u.Ptr;
			mpRefCnt = _u.mpRefCnt;
			_u.mPtr = nullptr;
			_u.mpRefCnt = nullptr;
			return *this;
		}
		const int use_count()const
		{
			if (mPtr != nullptr)
			{
				return mpRefCnt->getCnt();
			}
			return 0;
		}
		operator bool()
		{
			return mPtr != nullptr;
		}
		void reset(T* ptr)
		{
			if (mPtr == ptr) return;
			if (mPtr != nullptr && 0 == mpRefCnt->delRef())
			{
				mDeletor(mPtr);
				mPtr = nullptr;
				delete mpRefCnt;
				mpRefCnt = nullptr;
			}
			mPtr = ptr;
			mpRefCnt = nullptr;
			if (mPtr != nullptr)
			{
				mpRefCnt = new RefCnt<T>(ptr);
			}
		}
		void swap(shared_ptr& src)
		{
			std::swap(this->mPtr, src.mPtr);
			std::swap(this->mpRefCnt, src.mpRefCnt);
		}
	private:
		T* mPtr;
		peter::RefCnt<T>* mpRefCnt;
		Deletor mDeletor;
	};
}

class Object
{
private:
	int value;
public:
	Object(int _x = 0) :value(_x) { cout << "Construct Object" << endl; }
	~Object() { cout << "Destory Object" << endl; }
	int& Value() { return value; }
	const int& Value()const { return value; }
};

int main()
{
	peter::shared_ptr<Object> obja(new Object(10));
	peter::shared_ptr<Object> objb;
	peter::shared_ptr<Object> objc;
	obja = objc;
	return 0;
}
```

![image-20210802110603877](C:\Users\lenovo\AppData\Roaming\Typora\typora-user-images\image-20210802110603877.png)

