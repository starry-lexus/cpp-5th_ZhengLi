#include "date.h"

std::istream &operator>>(std::istream &in, Date &date) {
    int year, month, day;
    char c1, c2;
    in >> year >> c1 >> month >> c2 >> day;
    date = Date(year, month, day);
    return in;
}

std::ostream &operator<<(std::ostream &out, const Date &date) {
    out << date.getYear() << "-" << date.getMonth() << "-" << date.getDay();
    return out;
}

Date::Date(int year, int month, int day) : year(year), month(month), day(day) {
    if (day <= 0 || day > getMaxDay()) {
        std::cout << "Invalid date: " << *this << std::endl;
        exit(1);
    }
    int years = year - 1;
    totalDays = years * 365 + years / 4 - years / 100 + years / 400
                + DAYS_BEFORE_MONTH[month - 1] + day;
    if (isLeapYear() && month > 2) totalDays++;
}

int Date::getMaxDay() const {
    if (isLeapYear() && month == 2)
        return 29;
    else
        return DAYS_BEFORE_MONTH[month] - DAYS_BEFORE_MONTH[month - 1];
}