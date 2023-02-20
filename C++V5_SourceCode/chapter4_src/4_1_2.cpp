#include <iostream>

using namespace std;

class Clock { //时钟类的定义
public:        //外部接口，公有成员函数
    Clock(int newH, int newM, int newS);    //构造函数
    Clock(); //默认构造函数
    void setTime(int newH = 0, int newM = 0, int newS = 0);

    void showTime();

private:    //私有数据成员
    int hour, minute, second;
};

Clock::Clock() : hour(0), minute(0), second(0) {}//默认构造函数
Clock::Clock(int newH, int newM, int newS) : hour(newH), minute(newM), second(newS) {
}

//时钟类成员函数的具体实现
void Clock::setTime(int newH, int newM, int newS) {
    hour = newH;
    minute = newM;
    second = newS;
}

inline void Clock::showTime() {
    cout << hour << ":" << minute << ":" << second << endl;
}

int main() {
    Clock c(0, 0, 0); //自动调用构造函数
    Clock c2;
    c.showTime();
    return 0;
}
