shared_ptr 循环引用问题

```c++
#include <iostream>
#include <string>
#include <atomic>
using namespace  std;

class Child;
class Parent
{
public:
	shared_ptr<Child> c;
	Parent() { cout << "construct parent" << endl; }
	~Parent() { cout << "destory parent" << endl; }
	void hi() { cout << "parent hi func" << endl; }
};
class Child
{
public:
	shared_ptr<Parent> p;
	Child() { cout << "construct child" << endl; }
	~Child() { cout << "destory child" << endl; }
};
int main()
{
	shared_ptr<Child> pc(new Child());
	shared_ptr<Parent> pa(new Parent());
	pc->p = pa;
	pa->c = pc;
	pc->p->hi();
	return 0;
}
```

调用结果，我们只会调用构造函数，并不会调用析构函数

**![image-20210802115537149](C:\Users\lenovo\AppData\Roaming\Typora\typora-user-images\image-20210802115537149.png)**

```c++
/*
* weak_ptr 仿写目前存在的问题，当访问当已经shared_ptr 已经释放的资源的时候，我们weak_ptr还是能看到该值。
* 自己的思考 weak_ptr 中的 mPtr mpRefCnt使用二级指针  指向shared_ptr 中的mPtr mpRefCnt指针的地址。
*/
namespace peter
{
	template<typename T>
	class RefCnt
	{
	private:
		T* mPtr;
		std::atomic<int> mCnt_s;	//主要用于 shared_ptr 引用计数
		std::atomic<int> mCnt_w;	//主要用于 weak_ptr 引用计数
	public:
		RefCnt(T* ptr = nullptr) :mPtr(ptr)
		{
			if (mPtr != nullptr)
			{
				mCnt_s = 1;
				mCnt_w = 0;
			}
		}
		void addRef_s() { mCnt_s++; }
		int delRef_s() { return --mCnt_s; }		//返回值为int  是因为在--后，方柏霓比较其是否为零
		int getCnt_s() { return mCnt_s.load; }

		void addRef_w() { mCnt_w++; }
		int	delRef_w() { return --mCnt_w; }
		int getCnt_w() { return mCnt_w.load; }
	};
	/*
	* 删除器  使用删除器的原因是 我们不仅仅要删除对象
	* 也有可能是文件，信号量等资源
	* 留有该模板接口可以让用户自己完成和设置使用资源的删除器
	*/
	template<typename T>			
	struct MyDeletor {
		void operator()(T* ptr)
		{
			delete ptr;
		}
	};
	template<typename T>			//删除器的特化版本 删除一组对象
	struct MyDeletor<T[]>
	{
		void operator()(T* ptr)
		{
			delete[] ptr;
		}
	};

	template<typename T>class weak_ptr;

	template<typename T, typename Deletor = MyDeletor<T>>
	class shared_ptr
	{
	public:
		shared_ptr(T* ptr = nullptr) :mPtr(ptr)
		{
			if (mPtr != nullptr)
			{
				mpRefCnt = new RefCnt<T>(ptr);
			}
			else 
			{ 
				mpRefCnt = nullptr;
			}
		}
		~shared_ptr()
		{
			if (mPtr != nullptr && 0 == mpRefCnt->delRef_s())
			{
				mDeletor(mPtr);
				delete mpRefCnt;
			}
			mPtr = nullptr;
			mpRefCnt = nullptr;
		}
		T& operator*()const { return *mPtr; }
		T* operator->()const { return mPtr; }
		shared_ptr(const shared_ptr& ptr) :mPtr(ptr.mPtr), mpRefCnt(ptr.mpRefCnt)
		{
			if (mPtr != nullptr)
			{
				mpRefCnt->addRef_s();
			}
		}
		// null null
		// not null
		//null  not
		// not not
		shared_ptr& operator=(const shared_ptr& ptr)
		{
			if (this == &ptr || mPtr == ptr.mPtr) return *this;
			if (mPtr != nullptr && mpRefCnt->delRef_s() == 0)
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
				mpRefCnt->addRef_s();
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
			if (mPtr != nullptr && 0 == mpRefCnt->delRef_s())
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
				return mpRefCnt->getCnt_s();
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
			if (mPtr != nullptr && 0 == mpRefCnt->delRef_s())
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
		shared_ptr(const weak_ptr<T>& src):mPtr(src.mPtr),mpRefCnt(src.mpRefCnt)
		{
			if (mPtr != nullptr)
			{
				mpRefCnt->addRef_s();
			}
		}
		friend class weak_ptr<T>;
	private:
		T* mPtr;
		peter::RefCnt<T>* mpRefCnt;
		Deletor mDeletor;
	};

	template <typename T>
	class weak_ptr
	{
	public:
		weak_ptr():mPtr(nullptr),mpRefCnt(nullptr){}
		weak_ptr(shared_ptr<T>& src ) :mPtr(src.mPtr), mpRefCnt(src.mpRefCnt)
		{
			if (mPtr != nullptr)
			{
				mpRefCnt->addRef_w();
			}
		}
		~weak_ptr()
		{
			release();
		}
		weak_ptr(const weak_ptr& src) :mPtr(src.mPtr), mpRefCnt(src.mpRefCnt)
		{
			if (mPtr != nullptr)
			{
				mpRefCnt->addRef_w();
			}
		}
		weak_ptr& operator=(const shared_ptr<T>& src)
		{
			mPtr = src.mPtr;
			mpRefCnt = src.mpRefCnt;
			if (mPtr != nullptr)
			{
				mpRefCnt->addRef_w();
			}
			return (*this);
		}
		weak_ptr& operator=(const weak_ptr<T>& src)
		{
			mPtr = src.mPtr;
			mpRefCnt = src.mpRefCnt;
			if (mPtr != nullptr)
			{
				mpRefCnt->addRef_w();
			}
			return (*this);
		}
		shared_ptr<T>& lock()
		{
			return shared_ptr<T>(*this);
		}
		bool expried()const
		{
			if (mPtr!=nullptr )
			{
				return mpRefCnt->getCnt_s != 0;
			}
			return true;
		}
		int use_count()const
		{
			if (mPtr != nullptr)
			{
				return mpRefCnt->getCnt_s();
			}
			return 0;
		}
		friend class shared_ptr<T>;
	private:
		void release()
		{
			if (mPtr != nullptr )
			{
				mpRefCnt->delRef_w();
				cout << "该对象有效" << endl;
			}
			else
			{
				cout<<"该对象无效"<<endl;
			}
			mpRefCnt = nullptr;
		}
		T* mPtr;
		RefCnt<T>* mpRefCnt;
	};
}
 
class Child;
class Parent {
public:
	peter:: weak_ptr<Child> c;
	Parent() { cout << "construct Parent" << endl; }
	~Parent() { cout << "detory Parent" << endl; }
	void hi() {
		cout << "hi Parent" << endl;
	}
};
class Child
{
public:
	peter::weak_ptr<Parent> p;
	Child() { cout << "construct Child" << endl; }
	~Child() { cout << "destory Child" << endl; }
};

int main()
{
	peter::shared_ptr<Parent> par(new Parent());
	peter::shared_ptr<Child> pcr(new Child());
	par->c = pcr;
	pcr->p = par;

	return 0;
}
```

上述问题的处理方案是使用weak_ptr,弱引用。