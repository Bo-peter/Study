#include <iostream>
#include <string>
#include <queue>

using namespace std;

struct Work
{
    virtual void transaction() = 0;
};

class Boss : public Work
{
private:
    queue<string> orders;

public:
    Boss() { cout << "The boss is here" << endl; }
    Boss(queue<string> &_orders) : orders(_orders) { cout << "The boss is here" << endl; }
    void addTransaction(const string &order)
    {
        orders.push(order);
        cout << "The order is add by boss" << endl;
    }
    void transaction()
    {
        while (!orders.empty())
        {
            cout << "the boss deal with " << orders.front() << endl;
            orders.pop();
        }
    }
};

class Assistant : public Work
{
private:
    Boss *boss;
    queue<string> orders;
    void inviteBoss(queue<string> orders)
    {
        boss = new Boss(orders);
    }

public:
    Assistant() : boss(nullptr) {}

    void addTransaction(const string &order)
    {
        if (boss == nullptr)
        {
            orders.push(order);
            cout << "The order is add by assitant" << endl;
        }
        else
        {
            boss->addTransaction(order);
        }
    }
    void transaction()
    {
        if (boss == nullptr)
        {
            inviteBoss(orders);
        }
        boss->transaction();
    }
};
int main()
{
    Assistant *assistant = new Assistant();
    assistant->addTransaction("吃饭");
    assistant->addTransaction("喝酒");
    assistant->addTransaction("洗脚");
    assistant->addTransaction("打麻将");
    assistant->transaction();
    assistant->addTransaction("吃饭");
    assistant->addTransaction("喝酒");
    assistant->addTransaction("洗脚");
    assistant->addTransaction("打麻将");
    assistant->transaction();
    return 0;
}