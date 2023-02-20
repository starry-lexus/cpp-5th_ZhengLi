//10_18.cpp
#include <functional>
#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>

using namespace std;

bool g(int x, int y) {
    return x > y;
}

int main() {
    /*boost库没有取反器,标准库的not1与boost的bind无法搭配使用，但它有更好的函数或重载的关系运算符
     * std::bind和boost::bind的区别
     * 1.由boost::bind生成的函数对象会重载关系运算符
     * 2.boost::bind支持非默认的调用约定
     * 3.boost::protect(boost/bind/protect.hpp)可以在求值时保护bind函数对象,让bind无法对其进行识别和求值,
     * 用法为protect(bind(f, ...))
     * 4.用户可以通过std::is bind expression指示自定义的函数对象为std::bind表达式,从而实现std::bind的嵌套,
     * 具体示例见https://zh.cppreference.com/w/cpp/utility/functional/is_bind_expression
     * 关于boost::bind更多详细信息,请参阅 https://www.boost.org/doc/libs/1_81_0/libs/bind/doc/html/bind.html
     */
    int intArr[] = {30, 90, 10, 40, 70, 50, 20, 80};
    const int N = sizeof(intArr) / sizeof(int);
    vector<int> a(intArr, intArr + N);

    vector<int>::iterator p;
    //p = find_if(a.begin(), a.end(), bind2nd(ptr_fun(g), 40));
    p = find_if(a.begin(), a.end(), boost::lambda::_1 > 40);
    if (p == a.end())
        cout << "no element greater than 40" << endl;
    else
        cout << "first element greater than 40 is: " << *p << endl;
    /*编译器警告信息分析
     * 1.clang-tidy建议使用greater<>()，但是由于boost::bind没有类型特征信息,编译器是推断不出类型的,所以必须指出greater模板参数
     * 2.clang-tidy热衷于lambda表达式,而不是适配器
     *替换语句分析
     * not1是对一元函数对象或一元谓词的返回值进行非操作
     * not2是对二元函数对象或二元谓词的返回值进行非操作
     * 1.not1(bind2nd(greater<int>(), 15))作用: 先绑定greater的第二个参数以实现x > 15的功能,然而对greater返回值进行非运算
     * 同义替换为!boost::bind(greater<int>(),_1, 15)
     * 2.bind2nd(not2(greater<int>()), 15)作用: 相当于先通过greater定义出less_equal),然后绑定它的第二个参数
     * 同义替换为boost::bind(!boost::bind(greater<int>(), _1, _2), _1, 15))   或
     *         boost::bind(not2(greater<int>()), _1, 15)
     * 说明: ①如果logical_not()是一个持有参数x并返回!x的函数对象,则!boost::bind(greater<int>(),_1, 15)等价于
     *      boost::bind(logical_not(), boost::bind(greater<int>(),_1, 15))
     *      ②functional和boost\lambda\core.hpp中均有placeholders(_1),所以通过不能展开boost::lambda命名空间
     *      来简化boost::lambda::_1
     */
    //p = find_if(a.begin(), a.end(), not1(bind2nd(greater<int>(), 15)));
    p = find_if(a.begin(), a.end(), !boost::bind(greater<int>(), _1, 15));
    if (p == a.end())
        cout << "no element is not greater than 15" << endl;
    else
        cout << "first element that is not greater than 15 is: " << *p << endl;
    //p = find_if(a.begin(), a.end(), bind2nd(not2(greater<int>()), 15));
    p = find_if(a.begin(), a.end(), boost::bind(!boost::bind(greater<int>(), _1, _2), _1, 15));
    if (p == a.end())
        cout << "no element is not greater than 15" << endl;
    else
        cout << "first element that is not greater than 15 is: " << *p << endl;
    return 0;
}
