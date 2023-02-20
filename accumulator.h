/**本类仅用于辅助计算,故而不用保存其数据
 *@author 岛中客、眼中你
 *@date 2023/2/17
 */
#ifndef ACCUMULATOR_H
#define ACCUMULATOR_H

#include "date.h"

class Accumulator {    //将某个数值按日累加
private:
    Date lastDate;    //上次变更数值的时期
    double value;    //数值的当前值
    double sum;        //数值按日累加之和
public:
    Accumulator() {
        Date date;
        lastDate = date;
        value = 0;
        sum = 0;
    }      //添加默认构造函数
    //构造函数，date为开始累加的日期，value为初始值
    Accumulator(const Date &date, double value) : lastDate(date), value(value), sum(0) {}

    //获得到日期date的累加结果
    double getSum(const Date &date) const {
        return sum + value * (date - lastDate);
    }

    //在date将数值变更为value
    void change(const Date &date, double d) {
        sum = getSum(date);
        lastDate = date;
        this->value = d;
    }

    //初始化，将日期变为date，数值变为value，累加器清零
    void reset(const Date &date, double d) {
        lastDate = date;
        this->value = d;
        sum = 0;
    }
};

#endif //ACCUMULATOR_H