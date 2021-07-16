#include <iostream>

using namespace std;

//含有纯虚函数的类，被称为抽象类
//仅含有纯虚函数的类被称为接口
struct Subject
{
    virtual void request() = 0; //纯虚函数
};

class RealSubject : Subject
{
public:
    void request() { cout << "RealSubject::request" << endl; }
};

class Proxy : public Subject
{
private:
    RealSubject *realSubject;

public:
    Proxy() : realSubject(new RealSubject()) {}
    void preRequst() { cout << "pre" << endl; }
    void postRequest() { cout << "post" << endl; }
    void request()
    {
        preRequst();
        realSubject->request();
        postRequest();
    }
};


int main()
{
    Subject* sub = new Proxy();
    sub->request();
    return 0;
}