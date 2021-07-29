```c++
/*
* unique_ptr 
* unique_ptr 对象不能进行复制操作只能进行移动操作 //move
* 
*/

namespace peter
{
template <class _Tp>
class unique_ptr
{
private:
	_Tp* _Ptr;
	unique_ptr(const unque_ptr&) = delete;
	operator=(const unque_Ptr&) = delete;
public:
	typedef _Tp* pointer;
	typedef _Tp element_type;
public:
	unique_ptr(_Tp* p = nullptr):_Ptr(p){}
	~unique_ptr()
	{
		if (_Ptr != nullptr)
		{
			delete _Ptr;
		}
		_Ptr = nullptr;
	}
	pointer get()const { return _Ptr; }
	_Tp& operator*() { return *get(); }
	const _Tp& operator*()const { return *get(); }
	pointer  operator->() { retrun  get(); }
	opearator bool()const { return _Ptr != nullptr; }
	void reset(pointer p = nullptr)
	{
		if (_Ptr != nullptr)
		{
			delete _Ptr;
		}
		_Ptr = p;
	}
	pointer release()
	{
		pointer p = _Ptr;
		_Ptr = nullptr;
		return p;
	}
	void swap(unique& _p)
	{
		std::swap(this->_Ptr, _p->_Ptr);
	}
};

}
```

