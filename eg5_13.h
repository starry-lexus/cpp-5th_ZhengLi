/**
 *@author 岛中客、眼中你
 *@date 2023/1/8
 */

#ifndef ZHENGLI_EG5_13_H
#define ZHENGLI_EG5_13_H

class X;    //前向引用声明

class Y {
public:
    void g(X *x);   //声明成员函数
};

class Z {
    void f(X *x);
};

class X {
public:
    friend void Y::g(X *x);     //声明类Y的成员函数g为友元函数
    friend class Z;

    friend void h(X *x);

private:
    int i;
};

void h(X *x);

#endif //ZHENGLI_EG5_13_H
