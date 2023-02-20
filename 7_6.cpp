/**
 *@author 岛中客、眼中你
 *@date 2023/1/28
 */
//7_6.cpp
#include <iostream>

using namespace std;

class Base1 {    //定义基类Base1
public:
    int var;

    void fun() { cout << "Member of Base1" << endl; }
};

class Base2 {    //定义基类Base2
public:
    int var;

    void fun() { cout << "Member of Base2" << endl; }
};

class Derived : public Base1, public Base2 {    //定义派生类Derived
public:
    int var;    //同名数据成员
    void fun() { cout << "Member of Derived" << endl; }    //同名函数成员
};

int main() {
    auto *pd = new Derived();
    Base1 *pb1b = pd;
    Base2 *pb2b = pd;
    pd->var = 0;
    pd->Base1::var = 1;
    pd->Base2::var = 2;

    cout << "  pd = " << pd << endl;
    cout << "pb1b = " << pb1b << endl;
    cout << "pb2b = " << pb2b << endl;

    pb1b->fun();
    cout << "Base1::var = " << pb1b->var << endl;
    pb2b->fun();
    cout << "Base2::var = " << pb2b->var << endl;

    return 0;
}
