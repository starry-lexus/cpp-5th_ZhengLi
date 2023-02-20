//4_3.cpp
#include <iostream>

using namespace std;

const double PI = 3.141593;        //给出p的值
const double FENCE_PRICE = 35;    //栅栏的单价
const double CONCRETE_PRICE = 20;//过道水泥单价

class Circle {    //声明定义类Circle 及其数据和方法
public:        //外部接口
    Circle(double r);        //构造函数
    double circumference();    //计算圆的周长
    double area();            //计算圆的面积
private:    //私有数据成员
    double radius;            //圆半径
};

//类的实现

//构造函数初始化数据成员radius
Circle::Circle(double r) {
    radius = r;
}

//计算圆的周长
double Circle::circumference() {
    return 2 * PI * radius;
}

//计算圆的面积
double Circle::area() {
    return PI * radius * radius;
}

//主函数实现
int main() {
    double radius;
    cout << "Enter the radius of the pool: ";     // 提示用户输入半径
    cin >> radius;

    Circle pool(radius);        //游泳池边界
    Circle poolRim(radius + 3);    //栅栏

    //计算栅栏造价并输出
    double fenceCost = poolRim.circumference() * FENCE_PRICE;
    cout << "Fencing Cost is $" << fenceCost << endl;

    //计算过道造价并输出
    double concreteCost = (poolRim.area() - pool.area()) * CONCRETE_PRICE;
    cout << "Concrete Cost is $" << concreteCost << endl;

    return 0;
}
