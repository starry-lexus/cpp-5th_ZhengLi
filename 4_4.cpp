/**
 * @author 岛中客、眼中你
 * @date 2023/1/2
 */
#include <iostream>
#include <cmath>

using namespace std;

class Point {
public:
    explicit Point(int xx = 0, int yy = 0) {    //构造函数
        x = xx;
        y = yy;
    }

    Point(Point &p);       //复制构造函数
    int getX() {
        return x;
    }

    int getY() {
        return y;
    }

private:
    int x, y;
};

Point::Point(Point &p) {
    //复制构造函数须只完成复制对象的任务，不要有其他任何能产生副作用的行为；否则程序的运行结果会因编译器的优化程度而有所差异
    x = p.x;
    y = p.y;
    cout << "Calling the copy constructor of Point" << endl;
}

//形参为Point类对象的函数
void f1(Point p) {
    cout << p.getX() << endl;
}

//返回值类型为Point类对象的函数
Point f2() {
    Point a(1, 2);
    return a;
}

//类的组合
class Line {
public:
    Line(Point xp1, Point xp2);

    Line(Line &line);

    double getLen() { return len; }

private:
    Point p1, p2;
    double len;
};

/**Line类构造函数的执行过程
 * 1.由于Line类构造函数的形参是2个Point类对象，所以用实参初始化形参时会调用两次Point类的复制构造函数
 * 2.初始化列表是用Point类对象xp1/2初始化Point类对象p1/2，调用了两次Point类的复制构造函数
 * 3.最后执行本函数体，即Line类的构造函数
 * @param xp1 Line的一个端点
 * @param xp2 Line的另一个端点
 */
Line::Line(Point xp1, Point xp2) : p1(xp1), p2(xp2) {
    cout << "Calling constructor of Line" << endl;
    double x = static_cast<double> (p1.getX() - p2.getX());
    double y = static_cast<double> (p1.getY() - p2.getY());
    len = sqrt(x * x + y * y);
}

/**
 * 1.初始化列表会调用两次Point类的复制构造函数
 * 2.执行本函数体，即Line类的复制构造函数
 * @param line
 */
Line::Line(Line &line) : p1(line.p1), p2(line.p2) {
    cout << "Calling the copy constructor of Line" << endl;
    len = line.len;
}

int main() {   /*测试复制构造函数被调用的三种情况
    Point a(4, 5);
    Point b = a;                    //情况一:用一个对象初始化另一同类对象
    cout << b.getX() << endl;
    f1(b);                      //情况二:形参为对象
    b = f2();                      //情况三:返回值类型为对象(现代编译器会优化掉)
    cout << b.getX() << endl;
    */
    Point myp1(1, 1), myp2(4, 5);     //建立Point类对象
    Line line(myp1, myp2);               //建立Line类对象
    Line line2(line);                             //利用复制构造函数建立Line2
    cout << "the length of line is " << line.getLen() << endl;
    cout << "the length of line2 is " << line2.getLen() << endl;
    return 0;
}