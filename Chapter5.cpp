/**
 *@author 岛中客、眼中你
 *@date 2023/1/8
 */
#include <iostream>

using namespace std;

class Cat {
public:
    explicit Cat(unsigned age = 0) : age(age) { numOfCats++; }

    Cat(Cat &cat) {
        age = cat.age;
        numOfCats++;
    }

    ~Cat() { numOfCats--; }

    static unsigned int getNumOfCats() {
        return numOfCats;
    }

private:
    unsigned age;
    static unsigned numOfCats;
};

unsigned Cat::numOfCats = 0;

class Car;

class Boat {
    friend double getTotalWeight(const Boat &boat, const Car &car);

    double weight;
};

class Car {
    friend double getTotalWeight(const Boat &boat, const Car &car);

    double weight;
};

//为避免调用复制构造函数,宜用引用型参数;为防止函数修改类的属性,宜用常引用形参
double getTotalWeight(const Boat &boat, const Car &car) {
    return boat.weight + car.weight;
}

int main() {
    //5-1:作用域是一个标识符在程序正文中有效的区域.有函数原型作用域、局部(块)作用域、类作用域、文件作用域命名空间和限定作用域的enum枚举类

    /*5-2:
     * 1.概念:程序运行到某一点,能够引用到的标识符,就是该处可见的标识符
     * 2.一般规则
     * ①标识符要声明在前,引用在后
     * ②在同一作用域中,不能声明同名的标识符
     * ③在没有互相包含关系的不同的作用域中声明的同名标识符,互补影响
     * ④如果在两个或多个具有包含关系的作用域中声明了同名标识符,则外层标识符在内层不可见
     */

    //5-3:运行结果为5 7 5 10 5 7;主要考查作用域、可见性和生存周期

    //5-4:让Engine类将Fuel类声明为友元类

    /*5-5:
     * 1.概念:由static关键字声明的数据成员称为静态数据成员.
     * 2.特点:静态数据成员属于类而非该类的某个具体对象,它由所有对象共同维护,一半用法为"类名::标识符"
     */

    /*5-6:
     * 1.概念:静态函数成员就是static使用关键字声明的函数成员
     * 2.特点
     * ①可通过类名和对象名两种方式调用,习惯上用类名调用
     * ②可以直接访问该类的静态数据和函数成员,但在访问非静态成员时,只能通过对象名
     */

    //5-7:
    Cat Persian;
    cout << "The number of cats: " << Cat::getNumOfCats() << endl;
    Cat Birman(Persian);
    cout << "The number of cats: " << Cat::getNumOfCats() << endl;

    //5-8:友元函数是在类中用friend关键字修饰的非成员函数,友元类是在类中用friend关键字修饰的其他类

    /*5-9:
     * ①友元不具有双向性,所以类B不是类A的友元类
     * ②友元不具有传递性,所以类C不是类A的友元类
     * ③友元不能被继承,所以类D不是类A的友元类
     */

    //5-10:由5-5定义的类Cat可知,静态成员变量可以是私有的,声明见类Cat

    /*5-11:
     * 1.文件安排:main函数和全局变量n定义在Chapter_5.cpp,在test.h中声明外部变量n和函数fn1,在test.cpp中定义函数fn1
     * 2.运行结果:最后的n值取决于fn1函数对n所赋的值
     */

    /*5-12:
     * 1.说明:题目描述信息有误或指代不明,若将静态变量n定义在函数fn1中,则其虽然具有静态生存周期,但其仅可在函数fn1中可见,
     * 到底是让谁"显示n的值"?若按指代关系的就近原则,则n的值应由main函数输出,但main函数对n不可见;
     * 2.修改:若将静态变量n定义为全局静态变量,则静态属性就是多余的,所以应将输出语句也放在函数fn1中
     * 3.运行结果:n的初值加10
     */

    //5-13:类的定义和普通函数h的声明在eg5_13.h,实现在eg5_13.cpp,main函数既可定义在其中(符合题目要求),也可定义在其他文件中

    //5-14:见类Boat、类Car和函数getTotalWeight

    /*5-15:
     * 1.功能上的不同点:在函数内部定义的普通局部变量具有动态生存周期,每次调用都需要初始化;而在函数内部定义的静态局部变量具有
     * 静态生存周期,仅初始化一次
     * 2.底层处理:已定义的静态局部变量位于未初始化的数据段中(.bss),其标识符与相对地址存储在符号表中,其初始化代码一般会内嵌
     * 在函数体中,并用一些静态的标志变量来表示其是否已初始化,从而保证其只会被初始化一次;而局部变量只有在程序执行到此语句时才
     * 为其分配存储空间并进行(自动)初始化
     */

    /*5-16:以Windows系统为例,在编译阶段输入的文件为.cpp和.h文件,输出的文件为.o文件;在连接阶段,输入文件为.o文件,输出文件
     * 为.exe文件。编译阶段是将源文件编译为目标代码文件,连接阶段是将所有目标代码文件和系统运行库连接在一起,生成可执行文件。
     * (1)会在编译阶段出错:没有匹配的函数调用f
     * (2)会在连接阶段出错:对"f(int)"的未定义引用
     * (3)不会出错
     * (4)会在连接阶段出错:重复定义了"f(int)"
     */

    return 0;
}