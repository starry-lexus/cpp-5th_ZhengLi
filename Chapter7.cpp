/**
 *@author 岛中客、眼中你
 *@date 2023/1/28
 */
#include <iostream>
#include <utility>

using namespace std;

class Shape {
public:
    virtual double getArea() const = 0;
};

class Rectangle : public Shape {
public:
    Rectangle(double len, double w) : length(len), width(w) {}

    double getArea() const override { return length * width; }

private:
    double length;
    double width;
};

const double PI = 3.1415926;

class Circle : public Shape {
public:
    explicit Circle(double r) : radius(r) {}

    double getArea() const override { return PI * radius * radius; }

private:
    double radius;
};

class Square : public Rectangle {
public:
    explicit Square(double sideLength) : Rectangle(sideLength, sideLength), a(sideLength) {}

    double getArea() const override { return a * a; }

private:
    double a;
};

class Document {
public:
    explicit Document(string name) : name(std::move(name)) {}

private:
    string name;
};

class Book : private Document {
public:
    Book(string name, unsigned pageCount) : Document(std::move(name)), pageCount(pageCount) {}

private:
    unsigned pageCount;
};

int main() {
    /*7-1:
     * 不同点:
     * ①public继承方式:    基类中的public和protected成员的属性在派生类中均不变
     * ②protected继承方式: 基类中的public和protected成员的属性在派生类中均变为protected
     * ③private继承方式:   基类中的public和protected成员的属性在派生类中均变为private
     */

    /*7-2:
     * 在构造派生类的对象时，会首先调用基类的构造函数，然后按照构造函数初始化列表中指定的方式初始化派生类新增的成员对象，
     * 最后执行派生类构造函数的函数体
     */

    //7-3: A::fn1();   fn2();

    /*7-4:
     * 定义或语法形式:
     *              class 派生类名: virtual 继承方式 基类名
     * 作用:在继承过程中通过防止同名成员产生副本以解决唯一标识问题
     */

    /*7-5:
     * 设计思路:Shape过于抽象,无法定义其属性和具体操作,其派生类Rectangle和Circle都有计算面积的函数getArea(),此时对于基类
     * Shape可有两种设计思路:①在Shape中定义getArea(),函数体为空,让它的派生类隐藏此函数(显然不能虚继承),此法的缺点是浪费空间;
     * ②根据上述分析,类Shape特别适合设计为抽象类(见第8章),即将getArea()定义为纯虚函数;有点遗憾的是,即使派生类不覆盖抽象类的
     * 纯虚函数,编译器也不会报错
     */

    //7-6:这两个类请自行定义并测试,构造函数调用顺序:先调用Mammal,后调用Dog;析构函数的调用顺序与此相反

    //7-7:见教材7.4节的7_4.cpp和7_5.cpp,位于P265~P271

    //7-8:如果你定义的类Document的name属性也是string类型,那么使用std::move()进行列表初始化可减少不必要的复制操作

    //7-9:在派生类Derived中重新定义函数fn1(),在函数体内调用基类的函数fn1(),即采用隐藏策略

    //7-10:请自行定义并观察执行过程

    /*7-11:
     * ①若不用作用域分辨符,则用DerivedClass的对象和指针访问到的函数fn1()和函数fn2()均是自己的成员函数,同理,用类BaseClass
     * 的指针访问到的函数fn1()和函数fn2()均是自己的成员函数
     * ②若使用作用域分辨符,假定DerivedClass的对象名为d,指针名为p,则根据作用域范围,用d.BaseClass::fn1();或p->BaseClass::fn1();
     * 即可访问到基类的函数fn1(),fn2同理;
     * 注意事项:①BaseClass类的对象一般不能转换为DerivedClass类的对象,除非DerivedClass类有接受BaseClass类型或其引用类型的
     * 构造函数;②尽量避免将基类指针转换为派生类指针
     */

    /*7-12:
     * 1.共同点:它们都使得已有对象成为新对象的一部分，从而达到代码复用的目的
     * 2.不同点:组合反映的是has-a的关系,即整体与部分;继承反映的是is-a的关系,即特殊与一般.当然,你也可以从属性与操作的角度进行比较
     * 关系已经说明
     */

    /*7-13:仅需重写7_6.cpp和7_8.cpp的主函数即可,见相关文件
     * 7_6.cpp:
     * 属于多继承的情况,在Derived类的对象中,数据成员存放的顺序依次是Base1::var、Base2::var和var,由于int占4个字节,
     * 所以存放Base1的数据成员的首地址与存放Base2的数据成员的首地址之差为4
     * 7_8.cpp:
     * 属于虚拟继承的情况,在Derived类的对象中,数据成员存放的策略和顺序等都取决于编译器,以g++/gcc 12.2.0为例,
     * pb1b与pb2b之间、pb2b与pb3b之间均相差16,在第36行处设置断点,进入调试模式后单步执行,可以看到调试器中的pd = pb1b =
     * pd2b = 0x1e6b5d717f0(系统分配的空间首地址),据此可以断定,该编译器采用的就是增加隐指针的策略,即在Base1类型对象和
     * Base2类型的对象中都增加一个指向Base0中定义的数据成员的首地址的隐指针
     * 注意:P301中作者所说的"通过相同的方式取得这个隐含的Base0指针值"是对编译器而言的,程序员是无法用指向Base0类型的指针接收
     * 该隐指针的,只能通过调试器等方式查看.如果你能直接打印出来,那么说明本人才疏学浅,没有测试到位
     */

    /*7-14:本题几乎是在抄写7.8.3节内容,注意书中的印刷错误
     * 1.①派生类指针可以隐含转换为基类指针,而基类指针要转换为派生类指针,则转换必须显式进行;
     *   ②由类型兼容可知:通过调用基类的复制构造函数,派生类对象可以转换为基类对象,而要让基类对象显式转换为派生类对象,必须显示定义
     *   接受基类类型(或其引用)参数的构造函数
     *   ③引用与指针的实现方式一致,因此目的类型为引用的转换同①
     * 2.在多重继承的情况下,执行基类指针到派生类指针的显示转换时,有时需要调整指针所存储的地址值已得到新指针的值;在虚继承的情况下,
     * 派生类的指针可以隐含转换为虚基类的指针,但虚基类的指针无法显式转换为派生类的指针。
     */

    /*7-15:
     * 1.运行结果:2 1.4013e-45
     * 2.分析:在直接将pd转换为Base2指针的过程中,需要在原地址的基础上加上一个偏移量,但这里的每步转换都涉及void指针,这个偏移量
     * 始终没有加上,结果当然是错误的.
     * 3.避免此类问题的方法:使用void指针时,必须要保证前后类型一致,而不能仅仅存在继承关系上的兼容性;当然,最好别用void指针
     */

    return 0;
}