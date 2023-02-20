/**Individual Bank Account Management
 * 就我而言,此题异常复杂,除了版本控制之外,需要用到serialization的其他所有功能,还要对MSVC或gcc(Window中的MinGW或Cygwin等)
 * 有所了解,知道如何改变文件的编译顺序,知道如何捕获并解决boost::archive::archive_exception和读写冲突等文件输入输出流的异常……
 * 注意事项：
 * 1.以下两个宏函数必须放在main函数中所有的基类指针积累之前
 * 此宏函数实际是对编译器进行提示,它会改变编译文件的顺序,所以如果将其放在头文件中,那程序就会因为符号重定义而异常终止;如果将其放在
 * 实现文件中,也就是account.cpp中时,那程序也会异常终止,此时Visual Studio 2022给出的异常提示是"未处理的异常,在内存位置...",
 * 检查后发现这是archive抛出的异常,将其捕获后并输出异常信息,显示为"unregistered class-derived class",这时你在百度/Google
 * /Stack Overflow中是找不到或很难找到原因的.即使抛弃IDE而采用命令行,你也必须清楚编译的先后顺序
 * 2.既然这么麻烦,干嘛不用hpp文件呢?
 * 你能问出这个问题,说明你了解过hpp文件,但只了解了皮毛,甚至可以说只是知道它的定义.的确,hpp文件就是将类的定义和实现全部放在一个文件中,
 * 不仅可以减少文件数量,不用考虑如何放置处理多态的宏函数,还可以让release版本没有lib和dll,实在是非常方便,但是,hpp文件中不能有全局
 * 对象和函数,也不允许展开或定义命名空间,静态成员也不能有,而本程序刚好全占,与其处理这些问题,倒不如老老实实分文件编写类
 * 3.可以归档成文本文件吗?
 * 应该可以,但没必要: ①转换处理问题的逻辑不后,程序可能会变得更复杂;②而且这也无法保护账户信息(最起码用户打开文件之后是无法解析内容的,
 * 当然,条件允许的话,上传到服务器是最保险的)
 * 4.11-11参考答案.pdf是官方参考答案吗?
 * 这是我迫不得已在清华大学出版社购买的电子书截屏,因为本题折磨了我一周多之后,我也未能解决数据冗余的问题,但不知是我理解不了书中的描述信息
 * 还是她的代码本身就有问题,本人根据指示修改代码后程序会异常终止;而且参考答案只保存了Account类的id、balance和total,根本没有保存账单,
 * 这就等于修改了程序的表现(无法输出历史账单);不过这给了我一些启发,让我发现了数据冗余的根源所在.除本题及第12章习题之外,其余答案均为本人
 * 独立撰写
 * 5.尽管解决了许多令人头疼的问题,但本人又在不经意间引入了一个新问题---query函数失效了,本人尝试了能想到的各种方法,但都无济于事,但愿你能
 * 从此解法或官方参考答案的指示中找到完美的解决方案
 * @author 岛中客、眼中你
 * @date 2023-2-30
 */
#include "account.h"
#include <iostream>
#include <fstream>
#include <boost/serialization/vector.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

/*BOOST_SERIALIZATION_ASSUME_ABSTRACT(Account)        //声明虚基类,不可置于头文件中,但可置于account.cpp中
BOOST_CLASS_EXPORT_GUID(SavingsAccount, "SavingsAccount")
BOOST_CLASS_EXPORT_GUID(CreditAccount, "CreditAccount")*/

using namespace std;

class Command {
    friend boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar.template register_type<SavingsAccount>();    //生成的文件较小
        ar.template register_type<CreditAccount>();
        ar & accounts;
        ar & bills;
    }

private:
    vector<Account *> accounts;
    Statements bills;
public:
    void run();

    void statements();

    Command() = default;

    ~Command();
};

Command::~Command() {
    for (auto item: accounts)
        delete item;
}

void Command::run() {
    //如果不是为了复刻书上内容，你完全可以选择去掉Date参数，然后在函数内部获取本地时间，作为相关参数，防止用户杜撰信息
    cout << "(a)add account (d)deposit (w)withdraw (s)show (c)change day (n)next month (q)query (e)exit" << endl;
    Date date = Statements::lastDate();
    char cmd;
    do {
        //显示日期和总金额
        cout << date << "\t" << "\tTotal: " << Statements::getTotal() << "\tcommand> ";

        char type;
        int index, day;
        double amount, credit, rate, fee;
        string id, desc;
        Account *account;
        Date date1, date2;

        cin >> cmd;
        switch (cmd) {
            case 'a':    //增加账户
                cin >> type >> id;
                if (type == 's') {
                    cin >> rate;
                    account = new SavingsAccount(date, id, rate);
                } else {
                    cin >> credit >> rate >> fee;
                    account = new CreditAccount(date, id, credit, rate, fee);
                }
                accounts.push_back(account);
                break;
            case 'd':    //存入现金
                cin >> index >> amount;
                getline(cin, desc);
                accounts[index]->deposit(date, amount, desc);
                break;
            case 'w':    //取出现金
                cin >> index >> amount;
                getline(cin, desc);
                accounts[index]->withdraw(date, amount, desc);
                break;
            case 's':    //查询各账户信息
                for (size_t i = 0; i < accounts.size(); i++) {
                    cout << "[" << i << "] ";
                    accounts[i]->show(cout);
                    cout << endl;
                }
                break;
            case 'c':    //改变日期
                cin >> day;
                if (day < date.getDay())
                    cout << "You cannot specify a previous day";
                else if (day > date.getMaxDay())
                    cout << "Invalid day";
                else
                    date = Date(date.getYear(), date.getMonth(), day);
                break;
            case 'n':    //进入下个月
                if (date.getMonth() == 12)
                    date = Date(date.getYear() + 1, 1, 1);
                else
                    date = Date(date.getYear(), date.getMonth() + 1, 1);
                for (const auto i: accounts)
                    i->settle(date);
                break;
            case 'q':    //查询一段时间内的账目
                cin >> date1 >> date2;
                Statements::query(date1, date2);
                break;
            case 'e':
                break;
            default:
                cerr << "Invalid command: " << cmd << endl;
        }
    } while (cmd != 'e');
}

void Command::statements() {
    Statements::query(Statements::startDate(), Statements::lastDate());

    cout << "\nSummary:" << endl;
    for (const auto item: accounts) {
        item->show(cout);
        cout << endl;
    }
    cout << "Total balance: " << Statements::getTotal() << endl;
}

int main() {
    Command c;
    const char *filename = "../accounts.dat";

    ifstream fin(filename, ios_base::binary);
    if (fin) {       //尝试恢复数据
        try {
            boost::archive::binary_iarchive ia(fin);
            ia >> c;
        } catch (boost::archive::archive_exception &e) {
            cerr << e.what() << endl;
        }

        //查询账单
        c.statements();
    }
    fin.close();

    c.run();
    ofstream fout(filename, ios_base::trunc | ios_base::binary);
    boost::archive::binary_oarchive oa(fout);
    oa << c;
    fout.close();

    return 0;
}