```c++
#include <iostream>
#include <string>

using namespace  std;

/*
* unique_ptr
* unique_ptr 对象不能进行复制操作只能进行移动操作 //move
*
*/

namespace peter
{
	template<typename _Tp>
	struct default_delete
	{
		void operator()(_Tp* ptr)const
		{
			delete ptr;
		}
	};
	template<typename _Tp>
	struct default_delete<_Tp[]>
	{
		void operator()(_Tp* ptr)const
		{
			delete[] ptr;
		}
	};
	template <typename _Tp,typename _Dp=default_delete<_Tp>>
	class unique_ptr
	{
	private:
		_Tp* _Ptr;
		unique_ptr(const unique_ptr&) = delete;
		unique_ptr& operator=(const unique_ptr&) = delete;
	public:
		typedef _Tp* pointer;
		typedef _Tp element_type;
		typedef _Dp delete_type;
		delete_type get_deleter() { return delete_type(); }
	public:
		unique_ptr(_Tp* p = nullptr) :_Ptr(p) {}
		~unique_ptr()
		{
			if (_Ptr != nullptr)
			{
				//delete _Ptr;
				get_deleter()(_Ptr);
			}
			_Ptr = nullptr;
		}
		unique_ptr(unique_ptr&& _ptr)
		{
			_Ptr = _ptr->_Ptr;
			_ptr._Ptr = nullptr;
		}
		unique_ptr& operator=(unique_ptr&& _ptr)
		{
			_Ptr = _ptr->_Ptr;
			_ptr._Ptr = nullptr;
		}
		pointer get()const { return _Ptr; }
		_Tp& operator*() { return *get(); }
		const _Tp& operator*()const { return *get(); }
		pointer  operator->() { return  get(); }
		operator bool()const { return _Ptr != nullptr; }
		void reset(pointer p = nullptr)
		{
			if (_Ptr != nullptr)
			{
				//delete _Ptr;
				get_deleter()(_Ptr);
			}
			_Ptr = p;
		}
		pointer release()
		{
			pointer p = _Ptr;
			_Ptr = nullptr;
			return p;
		}
		void swap(unique_ptr& _p)
		{
			std::swap(this->_Ptr, _p._Ptr);
		}
	};


//特化版本

template <typename _Tp, typename _Dp >
class unique_ptr<_Tp[],_Dp>
{
private:
	_Tp* _Ptr;
	unique_ptr(const unique_ptr&) = delete;
	unique_ptr& operator=(const unique_ptr&) = delete;
public:
	typedef _Tp* pointer;
	typedef _Tp element_type;
	typedef _Dp delete_type;
	delete_type get_deleter() { return delete_type(); }
public:
	unique_ptr(_Tp* p = nullptr) :_Ptr(p) {}
	~unique_ptr()
	{
		if (_Ptr != nullptr)
		{
			//delete _Ptr;
			get_deleter()(_Ptr);
		}
		_Ptr = nullptr;
	}
	unique_ptr(unique_ptr&& _ptr)
	{
		//_Ptr = _ptr->_Ptr;
		//_ptr._Ptr = nullptr;
		_Ptr = _ptr.realeas();
	}
	unique_ptr& operator=(unique_ptr&& _ptr)
	{
		_Ptr = _ptr->_Ptr;
		_ptr._Ptr = nullptr;
	}
	pointer get()const { return _Ptr; }
	//_Tp& operator*() { return *get(); }
	//const _Tp& operator*()const { return *get(); }
	_Tp& operator[](size_t index)
	{
		return _Ptr[index];
	}
	pointer  operator->() { return  get(); }
	operator bool()const { return _Ptr != nullptr; }
	void reset(pointer p = nullptr)
	{
		if (_Ptr != nullptr)
		{
			//delete _Ptr;
			get_deleter()(_Ptr);
		}
		_Ptr = p;
	}
	pointer release()
	{
		pointer p = _Ptr;
		_Ptr = nullptr;
		return p;
	}
	void swap(unique_ptr& _p)
	{
		std::swap(this->_Ptr, _p._Ptr);
	}
};

}

class Object {
private:
	int value;
public:
	Object(int _x = 0) :value(_x) { cout << "construct Object" << endl; }
	~Object() { cout << "destory Object" << endl; }
	int get()const { return value; }
	void set(int _val) { value = _val; }
};


int  main()
{
	//peter::unique_ptr<Object> obja(new Object(10));
	peter::unique_ptr<Object[]> objb(new Object[10]);

	cout << objb[1].get() << endl;;


	//FILE* fp = fopen("test.c", "r");
	//struct file_deleter {
	//	void operator() (FILE* fp)const
	//	{
	//		fclose(fp);
	//	}
	//};
	//unique_ptr<FILE,file_deleter> fp(fopen("test.c", "r"));
	return 0;
}

```

