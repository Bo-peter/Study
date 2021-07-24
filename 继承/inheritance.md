struct  和class  没有属性时创建

```c++
#include <iostream>
#include <string>
#include <queue>
#include <thread>
#include <list>
#include <mutex>
using namespace  std;

// 1 函数友元
// 2 成员函数友元
// 

// 单向  A-> B      // b -> A
//不具有传递性 ： A->B,B->C     //A->C 
//不具有继承性；



template<int i>
class Object
{
private:
	static int num;
public:
	Object() { cout << "construct obj" << ++num << endl; }
	~Object() { cout << "deconstruct obj" << num-- << endl; }
};



class Base :public Object<0>
{
public:
	Base() { cout << "Base:" << endl; }
};

int  Object<0> :: num = 0;
class  Test :public Object<1> {
public:
	Test() { cout << "Test:" << endl; }
};
int  Object<1> ::num = 0;

int  main()
{
	Base base[3];
	Test test[2];
	return  0;
}


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

