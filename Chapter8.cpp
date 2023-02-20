/**
 *@author 岛中客、眼中你
 *@date 2023/1/29
 */
#include <iostream>

using namespace std;

class Counter {
public:
    Counter(int cnt = 0) : cnt(cnt) {}       //允许进行隐式转换,所以不用explicit标记
    friend Counter operator+(const Counter &c1, const Counter &c2);

private:
    int cnt;
};

Counter operator+(const Counter &c1, const Counter &c2) {
    return {c1.cnt + c2.cnt};       //为避免重复声明返回类型,这里返回初始化列表而非临时无名对象
}

class Mammal {
public:
    virtual void speak() const;
};

void Mammal::speak() const {
    cout << "Mammal::speak()" << endl;
}

class Dog : public Mammal {
public:
    void speak() const override;
};

void Dog::speak() const {
    cout << "woof" << endl;
}

class Shape {
public:
    virtual ~Shape() = default;;

    virtual double getArea() const = 0;

    virtual double getPerim() const = 0;

    virtual int getVertexCount() const = 0;
};

class Rectangle : public Shape {
public:
    Rectangle(double len, double w) : length(len), width(w) {}

    Rectangle() : length(0.0), width(0.0) {}

    double getArea() const override { return length * width; }

    double getPerim() const override { return 2 * (length + width); }

    int getVertexCount() const override;

private:
    double length;
    double width;
};

int Rectangle::getVertexCount() const {
    return 4;
}

const double PI = 3.1415926;

class Circle : public Shape {
public:
    explicit Circle(double r = 0.0) : radius(r) {}

    double getArea() const override { return PI * radius * radius; }

    double getPerim() const override { return 2 * PI * radius; }

    int getVertexCount() const override;

private:
    double radius;
};

int Circle::getVertexCount() const {
    return 0;
}

class Square : public Rectangle {
public:
    explicit Square(double a = 0.0) : Rectangle(a, a) {};

    int getVertexCount() const override;
};

int Square::getVertexCount() const {
    return 4;
}

int getVertexCount1(Shape *shape) {
    auto *rectangle = dynamic_cast<Rectangle *>(shape);
    if (rectangle) return rectangle->getVertexCount();
    auto *square = dynamic_cast<Square *>(shape);
    if (square) return square->getVertexCount();
    auto *circle = dynamic_cast<Circle *>(shape);
    if (circle) return circle->getVertexCount();
    return -1;
}

int getVertexCount2(Shape *shape) {
    //注意:指针不具有多态类型
    if (typeid(*shape) == typeid(Rectangle))
        return dynamic_cast<Rectangle *>(shape)->getVertexCount();
    else if (typeid(*shape) == typeid(Square))
        return dynamic_cast<Square *>(shape)->getVertexCount();
    else if (typeid(*shape) == typeid(Circle))
        return dynamic_cast<Circle *>(shape)->getVertexCount();
    else
        return -1;
}

int getVertexCount3(Shape *shape) {
    return shape->getVertexCount();
}

class Point {
public:
    explicit Point(double xx = 0, double yy = 0) {    //构造函数
        x = xx;
        y = yy;
    }

    double getX() const {
        return x;
    }

    double getY() const {
        return y;
    }

    Point &operator++();        //前置++
    Point operator++(int);      //后置++
    Point &operator--();

    Point operator--(int);

    friend ostream &operator<<(ostream &out, const Point &p);

    friend Point operator+(const Point &p1, const Point &p2);

private:
    double x, y;
};

Point &Point::operator++() {
    x++;
    y++;
    return *this;
}

Point Point::operator++(int) {
    Point old = *this;
    ++(*this);
    return old;
}

Point &Point::operator--() {
    x--;
    y--;
    return *this;
}

Point Point::operator--(int) {
    Point old = *this;
    --(*this);
    return old;
}

ostream &operator<<(ostream &out, const Point &p) {
    out << "(" << p.x << ", " << p.y << ")";
    return out;
}

Point operator+(const Point &p1, const Point &p2) {
    return Point(p1.x + p2.x, p1.y + p2.y);
}

class BaseClass {
public:
    virtual void fn1() const;

    void fn2() const {
        cout << "BaseClass::fn2()" << endl;
    }
};

void BaseClass::fn1() const {
    cout << "BaseClass::fn1()" << endl;
}

class DerivedClass : public BaseClass {
public:
    void fn1() const override;

    void fn2() const {
        cout << "DerivedClass::fn2()" << endl;
    }
};

void DerivedClass::fn1() const {
    cout << "DerivedClass::fn1()" << endl;
}

int main() {
    /*8-1:
     * 1.概念:多态是指同样的消息被不同类型的对象接收时会导致完全不同的行为,是对类的特定成员函数的再抽象.
     * 2.实现原理:
     * ①单继承:C++会为每个多态类型创建一个虚表(Virtual Table),其中存储有指向虚函数的指针和运行时类型信息等,每个多态类型的对象
     * 只需保存一个指向虚表首地址的指针---虚表指针.当通过基类的指针或引用调用一个虚函数时,就可以通过虚表指针找到该对象的虚表,进而找到
     * 存放该虚函数的指针的虚函数条目,将该条目中存放的指针读出后,就可获得应该调用的函数的入口地址,然后调用该虚函数即可.
     * ②多继承:继承了多个基类的可能会有多个虚表,也可能是一个分段的虚表(每个基类的虚表指针指向其中一段的首地址)
     */

    /*8-2
     * 1.概念:抽象类是类中存在纯虚函数的类
     * 2.作用:为一个类族提供统一的操作界面(接口),以便多态地使用其中的成员函数
     * 抽象类的派生类不一定要给出纯虚函数的实现,但如果不去实现,则派生类仍然是抽象类,无法实例化
     */

    /*8-3:
     * 1.在C++中,不能声明虚构造函数,原因如下:
     *   ①构造函数构造对象是在编译阶段完成的,而虚函数是在程序执行时动态确定的;
     *   ②如果基类的构造函数为虚函数,那么派生类的构造函数就会覆盖掉基类的构造函数,这样子类在创建对象时就会由于找不到
     *   基类构造函数而发生错误;
     *   ③虚函数的动态绑定是通过虚表实现的,而在虚表的初始化工作是在构造完对象之后才进行的;创建对象时需要为其分配资源
     *   和确定类型等,而这些都是不能推迟到运行时才执行的,即这些工作必须在编译时就完成(其实是对①的具体解释)
     * 2.析构函数是可以声明为虚函数的,这样可以保证使用基类类型的指针就能够调用适当的析构函数对不同的对象进行清理工作,
     * 防止出现不确定的结果.例如P320的8_5.cpp和8.7.1节的例子
     */

    //8-4:将运算符+重载为非成员函数,这样整型数值也可以与Count类的对象相加

    //8-5:Dog类对象调用的speak函数是本类的speak函数
    Dog lele;
    lele.speak();

    //8-6:思路与上一章的完全相同

    //8-7:为方便输出点坐标,可以重载<<运算符
    Point point(1, 2);
    cout << point << endl;
    cout << point++ << endl;
    cout << ++point << endl;
    cout << point-- << endl;
    cout << --point << endl;

    /*8-8:
     * 运行结果分析:派生类DerivedClass覆盖了基类BaseClass的虚函数fn1(),隐藏了fn2().①根据赋值兼容规则,基类类型的指针可以
     * 指向派生类对象,从而通过此指针访问某个与基类同名的成员函数,所以pb->fn1()访问的是DerivedClass::fn1();②函数fn2()是
     * 在编译阶段就确定了的,经过地址变换,fd存储的就是DerivedClass从BaseClass中继承来的数据成员的首地址,所以pd->fn2()访问
     * 的是BaseClass::fn2()
     */
    DerivedClass d;
    BaseClass *pb = &d;
    DerivedClass *pd = &d;
    pb->fn1();
    pd->fn1();
    pb->fn2();
    pd->fn2();

    //8-9:见P320页的例8-5及其修改方法或见\C++V5_SourceCode\chapter8_src\8_5.cpp

    //8-10:给8-7中定义的Point类增加一个友元函数实现重载"+"即可
    Point point1(1.2, 2.3);
    cout << point1 + point << endl;

    //8-11:
    Rectangle r;
    cout << getVertexCount1(&r) << endl;
    cout << getVertexCount2(&r) << endl;
    cout << getVertexCount3(&r) << endl;
    Circle c;
    cout << getVertexCount1(&c) << endl;
    cout << getVertexCount2(&c) << endl;
    cout << getVertexCount3(&c) << endl;
    Square s;
    cout << getVertexCount1(&s) << endl;
    cout << getVertexCount2(&s) << endl;
    cout << getVertexCount3(&s) << endl;

    return 0;
}