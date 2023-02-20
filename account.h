/**
 *@author 岛中客、眼中你
 *@date 2023/2/17
 */
#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "accumulator.h"
#include "date.h"
#include <utility>
#include <cmath>
#include <boost/serialization/map.hpp>              //for std::multimap
#include <boost/serialization/string.hpp>           //for std::string
#include <boost/serialization/assume_abstract.hpp>  //for assuming class Account is a virtual base class
#include <boost/serialization/export.hpp>           //for polymorphic
#include <boost/serialization/base_object.hpp>      //for register derived class

class Account;      //前置声明

class AccountRecord {    //账目记录
    friend boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & date;
        ar & account;
        ar & amount;
        ar & balance;
        ar & desc;
    }

private:
    Date date;          //日期
    const Account *account;   //账户
    double amount;            //交易额
    double balance;           //余额
    std::string desc;         //描述
public:
    AccountRecord();    //添加默认构造函数
    AccountRecord(const Date &date, const Account *account, double amount, double balance, std::string desc);

    void show() const;    //输出当前记录
};

using RecordMap = std::multimap<Date, AccountRecord>;

/**有关Statements类的可能疑惑
 * 1.为什么要将Account类的静态数据成员单独拿出来创建新类?这不是多此一举吗?
 *   这绝不是多此一举.如果你对boost::serialization有较为深刻的理解或者你之前试验过,那么你就会发现,派生类在
 *   归档新增加的数据成员时必须调用基类的serialize函数,而如果基类存在静态数据成员,那么boost就会为每个子类
 *   存储一份静态数据成员,这无疑是在浪费存储空间.在本例中,这种隐藏陷阱就会导致个人银行账户管理程序在恢复数据后
 *   发现账目列表有许多冗余数据(它的分布规律与你实现的save和load功能有关),存储的文件大小也会随程序的执行次数
 *   呈指数增长.而这种异常现象产生的根源一般很难被新手发现: 这不符合我们的一般认识,即使存在继承关系,静态数据成员
 *   也只有一份
 * 2.将所有账户的总余额total放在Statements中不违和吗?
 *   如前所述,静态数据成员会导致归档文件大小急剧增加,虽然double所占空间只有8字节,但是我们必须得考虑程序长期或多次
 *   运行的隐患;况且,这种分离也是符合实际情况的
 * 3.既然这样,那何不将Account类的record函数也迁移过来?它也和账单有关呀
 *   如你所见,Account类是抽象类,如果将record也迁移过来,那么即使Account的将Statements类声明为自己的友元类,record
 *   也无法访问Account的私有数据成员id和balance,因为它俩不是静态数据成员,无法通过类访问,Account类还是抽象类,不能被实例化,
 *   也就无法通过对象访问,所以这也是不得已.
 */
class Statements final {     //禁止继承此类
    friend class Account;

    friend boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & recordMap;
        ar & total;
    }

public:
    //查询一段时间内的账单
    static void query(const Date &begin, const Date &end);

    static double getTotal() { return total; }

    //返回账单最近一次变更的时间：主要是为了复现书上的输出内容
    static Date lastDate();

    //返回账单最早变更的时间
    static Date startDate();

private:
    static RecordMap recordMap;     //不可改为非静态成员
    static double total;
};

class Account { //抽象账户类
    friend boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & id;
        ar & balance;
    }

private:
    std::string id;    //帐户
    double balance;    //余额
protected:
    //供派生类调用的构造函数，id为账户
    Account() : balance(0) {}   //添加默认构造函数
    Account(const Date &date, std::string id);

    //记录一笔帐，date为日期，amount为金额，desc为说明
    void record(const Date &date, double amount, const std::string &desc);

    //报告错误信息
    void error(const std::string &msg) const;

public:
    const std::string &getId() const { return id; }

    double getBalance() const { return balance; }

    //存入现金，date为日期，amount为金额，desc为款项说明
    virtual void deposit(const Date &date, double amount, const std::string &desc) = 0;

    //取出现金，date为日期，amount为金额，desc为款项说明
    virtual void withdraw(const Date &date, double amount, const std::string &desc) = 0;

    //结算（计算利息、年费等），每月结算一次，date为结算日期
    virtual void settle(const Date &date) = 0;

    //显示账户信息
    virtual void show(std::ostream &out) const;

    //给基类定义虚析构函数
    virtual ~Account() = default;
};

class SavingsAccount : public Account { //储蓄账户类
    friend boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<Account>(*this); //调用父类的serialize
        ar & rate;
    }

private:
    Accumulator acc;    //辅助计算利息的累加器: 不用归档,以后同理
    double rate;        //存款的年利率
public:
    SavingsAccount();   //添加默认构造函数
    SavingsAccount(const Date &date, const std::string &id, double rate);

    double getRate() const { return rate; }

    void deposit(const Date &date, double amount, const std::string &desc) override;

    void withdraw(const Date &date, double amount, const std::string &desc) override;

    void settle(const Date &date) override;
};

class CreditAccount : public Account { //信用账户类
    friend boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<Account>(*this); //调用父类的serialize
        ar & credit;
        ar & rate;
        ar & fee;
    }

private:
    Accumulator acc;    //辅助计算利息的累加器
    double credit;        //信用额度
    double rate;        //欠款的日利率
    double fee;            //信用卡年费

    double getDebt() const {    //获得欠款额
        double balance = getBalance();
        return (balance < 0 ? balance : 0);
    }

public:
    //构造函数
    CreditAccount();

    CreditAccount(const Date &date, const std::string &id, double credit, double rate, double fee);

    double getCredit() const { return credit; }

    double getRate() const { return rate; }

    double getFee() const { return fee; }

    double getAvailableCredit() const {    //获得可用信用
        if (getBalance() < 0)
            return credit + getBalance();
        else
            return credit;
    }

    void deposit(const Date &date, double amount, const std::string &desc) override;

    void withdraw(const Date &date, double amount, const std::string &desc) override;

    void settle(const Date &date) override;

    void show(std::ostream &out) const override;
};

#endif //ACCOUNT_H