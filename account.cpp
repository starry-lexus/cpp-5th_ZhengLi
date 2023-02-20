/**
 *@author 岛中客、眼中你
 *@date 2023/2/17
 */
#include "account.h"

//AccountRecord类的实现
AccountRecord::AccountRecord(const Date &date, const Account *account, double amount, double balance, std::string desc)
        : date(date), account(account), amount(amount), balance(balance), desc(std::move(desc)) {}

AccountRecord::AccountRecord() : date(Date(1, 1, 1)), account(nullptr), amount(0), balance(0) {}

void AccountRecord::show() const {
    std::cout << date << "\t#" << account->getId() << "\t" << amount << "\t\t" << balance << "\t\t" << desc
              << std::endl;
}

//Statements类的实现
RecordMap Statements::recordMap;    //声明静态数据成员(自动初始化),便于友元类或函数调用

void Statements::query(const Date &begin, const Date &end) {
    if (begin <= end) {
        auto iter1 = recordMap.lower_bound(begin);
        auto iter2 = recordMap.upper_bound(end);
        for (auto iter = iter1; iter != iter2; ++iter)
            iter->second.show();
    }
}

Date Statements::lastDate() {
    if (recordMap.empty())
        return {2008, 11, 1};
    return (--recordMap.end())->first;
}

Date Statements::startDate() {
    if (recordMap.empty())
        return {2008, 11, 1};
    return recordMap.begin()->first;
}

//Account类的实现
double Statements::total = 0;

Account::Account(const Date &date, std::string id) : id(std::move(id)), balance(0) {
    //将开户行为作为账单记录下来,这将允许我们不用额外考虑数据恢复时有关开户信息的获取问题
    record(date, 0, "your bank account has been created");
}

void Account::record(const Date &date, double amount, const std::string &desc) {
    amount = floor(amount * 100 + 0.5) / 100;    //保留小数点后两位
    balance += amount;
    Statements::total += amount;
    AccountRecord record(date, this, amount, balance, desc);
    Statements::recordMap.insert(std::make_pair(date, record));
}

void Account::show(std::ostream &out) const {
    out << id << "\tBalance: " << balance;
}

void Account::error(const std::string &msg) const {
    std::cout << "Error(#" << id << "): " << msg << std::endl;
}

std::ostream &operator<<(std::ostream &out, const Account &account) {
    account.show(out);
    return out;
}

//SavingsAccount类相关成员函数的实现
SavingsAccount::SavingsAccount(const Date &date, const std::string &id, double rate)
        : Account(date, id), rate(rate), acc(date, 0) {}

SavingsAccount::SavingsAccount() : Account({1, 1, 1}, ""), rate(0) {}

void SavingsAccount::deposit(const Date &date, double amount, const std::string &desc) {
    record(date, amount, desc);
    acc.change(date, getBalance());
}

void SavingsAccount::withdraw(const Date &date, double amount, const std::string &desc) {
    if (amount > getBalance()) {
        error("not enough money");
    } else {
        record(date, -amount, desc);
        acc.change(date, getBalance());
    }
}

void SavingsAccount::settle(const Date &date) {
    if (date.getMonth() == 1) {    //每年的一月计算一次利息
        double interest = acc.getSum(date) * rate
                          / (date - Date(date.getYear() - 1, 1, 1));
        if (interest != 0)
            record(date, interest, " interest");
        acc.reset(date, getBalance());
    }
}

//CreditAccount类相关成员函数的实现
CreditAccount::CreditAccount(const Date &date, const std::string &id, double credit, double rate, double fee)
        : Account(date, id), credit(credit), rate(rate), fee(fee) {}

CreditAccount::CreditAccount()
        : Account({1, 1, 1}, ""), credit(0), rate(0), fee(0) {}

void CreditAccount::deposit(const Date &date, double amount, const std::string &desc) {
    record(date, amount, desc);
    acc.change(date, getDebt());
}

void CreditAccount::withdraw(const Date &date, double amount, const std::string &desc) {
    if (amount - getBalance() > credit) {
        error("not enough credit");
    } else {
        record(date, -amount, desc);
        acc.change(date, getDebt());
    }
}

void CreditAccount::settle(const Date &date) {
    double interest = acc.getSum(date) * rate;
    if (interest != 0)
        record(date, interest, " interest");
    if (date.getMonth() == 1)
        record(date, -fee, " annual fee");
    acc.reset(date, getDebt());
}

void CreditAccount::show(std::ostream &out) const {
    Account::show(out);
    out << "\t\tAvailable credit:" << getAvailableCredit();
}