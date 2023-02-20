/**
 *@author 岛中客、眼中你
 *@date 2023/2/13
 */
#include <iostream>
#include <fstream>
#include <string>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <unordered_map>

using namespace std;

/**用boost::serialization存档
 * 前提：通过序列化保存的每个类都必须有一个函数可以保存或恢复类的状态(所有属性或希望恢复的属性)
 * 一、定义模板函数serialize的方式
 * 1.作为需存档类的成员模板函数：会破坏类的定义
 * class Demo{
 *      friend class boost::serialization::access;
 *      template<class Archive>
 *      void serialize(Archive &ar, const unsigned int version}{
 *          ar & 数据成员1;
 *          ....
 *      }
 * };
 * 2.作为需存档类的非成员模板函数：①声明友元函数模板；②提供足够多的接口；③将数据成员声明为public(破坏了封装性)。都不方便
 * namespace boost{
 *    namespace serialization{
 *       template<class Archive>
 *       void serialize(Archive &ar, youClass &instance, const unsigned int version}{
             ar & instance.数据成员1;
             ....
         }
       }
     }
 * 二、可序列化的类型：基类、派生类、指针、数组、STL容器
 * 三、其他特性：类版本控制、将序列化拆分为save/load
 * 更多详细信息请阅读官方文档 https://www.boost.org/doc/libs/1_81_0/libs/serialization/doc/index.html
 */
class Dog {
    friend boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & _age;
        ar & _weight;
    }

public:
    Dog() = default;

    Dog(unsigned age, unsigned weight) : _age(age), _weight(weight) {}

    unsigned age() const { return _age; }

    unsigned weight() const { return _weight; }

private:
    unsigned _age;
    unsigned _weight;
};

int main() {
    /*11-1
     * 1.流是一种抽象,它负责在数据的生产者和消费者之间建立联系,并管理数据的流动
     * 2.流的提取是指从流中获取数据,流的插入是指在流中写入数据
     * 3.I/O流在C++语言中起着帮助程序与外界进行数据交换的作用
     */

    /*11-2
     * cout是标准输出流,可用'>'进行输出重定向
     * cerr是标准错误输出流,没有缓冲,发送给它的内容被立即输出,可用'2>'进行输出重定向
     * clog类似于cerr,但是有缓冲,缓冲区慢时输出
     */

    //11-3:
    /*locale GBK(".936");
    string filename = "../test1.txt";
    wofstream out;
    out.imbue(GBK);

    out.open(filename);
    out << L"已成功写入文件！";
    out.close();

    //11-4:
    wifstream in;
    in.imbue(GBK);

    in.open(filename);
    if (in){
        wcout.imbue(GBK);
        wstring test1;
        in >> test1;
        wcout << test1 << endl;
    }else
        cout << "Error: cannot open the file" << endl;
    in.close();

    //11-5
    out.open(filename, ios_base::out | ios_base::app);
    out.imbue(GBK);
    out << L"已成功添加字符！";
    out.close();

    in.open(filename);
    if (in){
        wcout.imbue(GBK);
        wstring test1;
        in >> test1;
        wcout << test1 << endl;
    }else
        cout << "Error: cannot open the file" << endl;
    in.close();*/

    /*11-6:
     * 1.都可以正确归档和恢复,以16进制形式打开两文件,你就会发现仅中间的serialization::archive的ASCⅡ码相同,其余均不同
     * 注意: serialization库只会链接静态库,文本归档和二进制归档时不能使用同一个Dog对象(可以先注释掉文本归档的所有代码)
     */
    /*Dog dog1(10, 5), dog2{};
    ofstream ofs;
    ifstream ifs;
    ofs.open("../dogInfo.txt");
    boost::archive::text_oarchive oa(ofs);
    oa << dog1;
    ofs.close();

    ifs.open("../dogInfo.txt");
    boost::archive::text_iarchive ia(ifs);
    ia >> dog2;
    cout << "age: " << dog2.age() << "\nweight: " << dog2.weight() << endl;

    ofs.open("../dogInfo.dat", ios_base::binary);
    boost::archive::binary_oarchive boa(ofs);
    boa << dog1;
    ofs.close();

    ifs.open("../dogInfo.dat", ios_base::binary);
    boost::archive::binary_iarchive bia(ifs);
    bia >> dog2;
    cout << "age: " << dog2.age() << "\nweight: " << dog2.weight() << endl;*/

    //11-7:
    /*ios_base::fmtflags original_flags = cout.flags();     //保存cout的默认输出格式
    cout << 812 << '|';                                     //输出"812|"
    cout.setf(ios_base::left, ios_base::adjustfield);       //左对齐值,用适当的填充字符填充右边
    cout.width(10);                                         //设置字符输出宽度为10
    cout << 813 << 815 << '\n';
    cout.unsetf(ios_base::adjustfield);                     //清除格式
    cout.precision(2);                                      //设置输出精度
    cout.setf(ios_base::uppercase | ios_base::scientific);  //设置输出格式为科学计数法,底数字母为大写E
    cout << 831.0;
    cout.flags(original_flags);                             //恢复最初的输出格式*/

    //11-8:
    /*cout << "Enter a decimal integer please:";
    int dec;
    cin >> dec;

    ios_base::fmtflags origin= cout.flags();
    cout << "Decimal:     " << dec << endl;
    cout << "Octal:       " << oct << showbase << dec << endl;
    cout << "Hexadecimal: " << hex << showbase << dec << endl;
    cout.flags(origin);*/

    //11-9:
    /*cout << "Enter a name of txt file please:";
    string fileName, str;
    cin >> fileName;
    ifstream input(fileName);   //"../test2.txt"
    ofstream output("../result.txt");
    int lineNo = 0;
    if (input){
        while (!input.eof()){
            getline(input, str);
            output << ++lineNo << " " << str;
            //只有在文件末尾前的才输出换行符,显然如果简单地将换行符放在str后,则结果文件的末尾就会多出一个空行
            if (!input.eof())
                output << endl;
        }
    }else{
        cerr << "cannot open this file" << endl;
    }
    input.close();
    output.close();*/

    //11-10:
    locale GDB(".936");
    wifstream ToTheOak("../致橡树.txt");
    wofstream ToTheOak_wordFrequency("../致橡树_汉字频率.txt");
    ToTheOak.imbue(GDB);
    ToTheOak_wordFrequency.imbue(GDB);

    if (ToTheOak) {
        wchar_t word;
        int total = 0;
        unordered_map<wchar_t, int> word_map;
        while (ToTheOak >> word) {
            if (ispunct(word, GDB))     //如果你想统计标点符号的频率,那就删除此语句
                continue;
            ++word_map[word];
            ++total;
        }
        ToTheOak_wordFrequency << L"汉字\t频率" << endl;
        for (const auto &pair: word_map)
            ToTheOak_wordFrequency << pair.first << "\t" << pair.second << endl;
        ToTheOak_wordFrequency << L"总字数(不含标点符号)：" << total;
    } else
        wcerr << L"无法打开文件\"致橡树.txt\"" << endl;
    ToTheOak.close();
    ToTheOak_wordFrequency.close();

    //11-11: 见IBAM.cpp、account.h/.cpp、date.h/.cpp和accumulator.h

    return 0;
}