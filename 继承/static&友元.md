### static  & 友元

```c++
#include <iostream>
#include <string>
#include <queue>
#include <thread>
#include <list>
#include <mutex>
using namespace  std;

//static
//类中静态函数只能访问静态成员不能访问非静态成员
//原因 静态函数没有this指针

template<int i>
class Object
{
private:
	static int num;
public:
	Object() { cout << "construct obj" << ++num << endl; }
	~Object() { cout << "deconstruct obj" << num-- << endl; }
};

template<int i>
int Object<i>::num = 0;

class Base :public Object<0>
{
public:
	Base() { cout << "Base:" << endl; }
};



class  Test :public Object<1> {
public:
	Test() { cout << "Test:" << endl; }
};


int  main()
{
	Base base[3];
	Test test[2];
	return  0;
}

// 1 函数友元
// 2 成员函数友元
// 3 类友元

// 单向  A-> B      // b -> A
//不具有传递性 ： A->B,B->C     //A->C 
//不具有继承性；



// 类友元
class Object {
public:
	void fun(Int& it);
};

class Int
{
	friend class Object;
private:
	int value;
public:
	Int(int x = 0) :value(x) {}
	~Int() {}
};

void Object::fun(Int& it)
{
	it.value = 100;
}

//成员函数友元
class Int;

class Object {
public:
	void fun(Int& it);
};

class Int
{
	friend void Object::fun(Int& it);
private:
	int value;
public:
	Int(int x = 0) :value(x) {}
	~Int() {}
};

void Object::fun(Int& it)
{
	it.value = 100;
}




//函数友元
class Int
{
private:
	int value;
public:
	Int(int x = 0):value(x){}
	~Int(){}
	friend void fun(Int& a);
};

void fun(Int& a)
{
	a.value = 10;
}
```

