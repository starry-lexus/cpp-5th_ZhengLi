#ifndef DATE_H
#define DATE_H

#include <iostream>
#include <boost/serialization/access.hpp>

namespace {    //namespace使下面的定义只在当前文件中有效
    //存储平年中某个月1日之前有多少天，为便于getMaxDay函数的实现，该数组多出一项
    const int DAYS_BEFORE_MONTH[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
}

class Date {    //日期类
    friend boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & year;
        ar & month;
        ar & day;
    }

private:
    int year;        //年
    int month;        //月
    int day;        //日
    int totalDays;    //该日期是从公元元年1月1日开始的第几天
public:
    Date() : Date(1, 1, 1) {}

    Date(int year, int month, int day);    //用年、月、日构造日期
    int getYear() const { return year; }

    int getMonth() const { return month; }

    int getDay() const { return day; }

    int getMaxDay() const;        //获得当月有多少天
    bool isLeapYear() const {    //判断当年是否为闰年
        return year % 4 == 0 && year % 100 != 0 || year % 400 == 0;
    }

    //计算两个日期之间差多少天
    int operator-(const Date &date) const {
        return totalDays - date.totalDays;
    }

    //判断两个日期的前后顺序
    bool operator<(const Date &date) const { return totalDays < date.totalDays; }

    bool operator==(const Date &date) const { return totalDays == date.totalDays; }

    bool operator!=(const Date &date) const { return !(*this == date); }

    bool operator>(const Date &date) const { return date < *this; }

    bool operator<=(const Date &date) const { return !(*this > date); }

    bool operator>=(const Date &date) const { return !(*this < date); }
};

std::istream &operator>>(std::istream &in, Date &date);

std::ostream &operator<<(std::ostream &out, const Date &date);

#endif  //DATE_H