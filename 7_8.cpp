/**
 *@author 岛中客、眼中你
 *@date 2023/1/28
 */
//7_8.cpp
#include <iostream>

using namespace std;

class Base0 {    //定义基类Base0
public:
    int var0;

    void fun0() { cout << "Member of Base0" << endl; }
};

class Base1 : virtual public Base0 {    //定义派生类Base1
public:    //新增外部接口
    int var1;

    void printAddress() {
        cout << this << endl;
    }
};

class Base2 : virtual public Base0 {    //定义派生类Base2
public:    //新增外部接口
    int var2;

    void printAddress() {
        cout << this << endl;
    }
};

class Derived : public Base1, public Base2 {    //定义派生类Derived
public:    //新增外部接口
    int var;

    void fun() { cout << "Member of Derived" << endl; }
};

int main() {    //程序主函数
    auto *pd = new Derived();
    Base1 *pb1b = pd;
    Base2 *pb2b = pd;
    Base0 *pb0b = pb1b;
    cout << "  pd = " << pd << endl;
    cout << "pb1b = " << pb1b << endl;
    cout << "pb2b = " << pb2b << endl;
    cout << "pb0b = " << pb0b << endl;

    return 0;
}