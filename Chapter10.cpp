/**
 *@author 岛中客、眼中你
 *@date 2023/2/5
 */
#include <iostream>
#include <ctime>
#include <vector>
#include <deque>
#include <list>
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <random>

using namespace std;

template<class Container>
int JosephProblem(int n, int m) {
    /*直接计算法
     int survivor = 0;
     for (int i = 1; i <= n; i++)
         survivor = (survivor + m) % i;
     return survivor;
     */

    /*符合题目要求法: 便于输出出局顺序
     * 由于双向迭代器不支持直接移动n位,所以list不能像vector和deque那样直接用seq.erase(seq.begin()+number),
     * 现就如何高效实现此函数模板进行讨论:
     * 1.自定义迭代器移动函数:①用typeid在运行时获取迭代器类型,根据不同类型编写操作步骤,例如以下判定是针对随机迭代器的
     * typeid(std::iterator_traits<Iterator>::iterator_category) == typeid(std::random_access_iterator_tag);
     * 如何不借助辅助函数(类似P471的做法),那本函数是无法调用的.
     * 原因是我们无法直接传入迭代器类,Container是类模板参数,具体化的 list<int>是list类,v是对象,v.begin()是迭代器,
     * auto无法根据v.begin获取其迭代器类型
     * 2.自己实现advance函数: 你可以模仿P471页的做法,也可以模仿<iterator>库的做法,模仿别人而造出一模一样的轮子没有意义
     * 3.函数模板特化:
     * ①特化list: 因为vector和deque的迭代器类型均为随机访问迭代器,而list的是双向访问迭代器
     * ②特化vector: 让vector预留n份空间,以免运行时重新动态分配空间【注: ①和②的性能表现几乎一样】
     * ③全部特化: 那模板还有什么存在的必要呢?
     * 4.定义3参数辅助函数模板: 其最后一个参数为迭代器类型,本函数只需调用辅助函数模板即可,类似书上所给advance的实现
     */
    Container v;

    for (int i = 1; i <= n; i++)
        v.push_back(i);

    ptrdiff_t number = 0;       //报数:初始为0

    while (v.size() != 1) {
        number = (number + m - 1) % static_cast<ptrdiff_t>(v.size());    //计算出局者的编号
        v.erase(v.begin() + number);
    }

    return v[0];
}

template<>
int JosephProblem<list<int>>(int n, int m) {
    list<int> v;

    for (int i = 1; i <= n; i++)
        v.push_back(i);

    ptrdiff_t number = 0;

    while (v.size() != 1) {
        number = (number + m - 1) % static_cast<ptrdiff_t>(v.size());
        auto iter = v.begin();      //iter为工作迭代器
        advance(iter, number);         //让iter指向出局者
        v.erase(iter);                  //删除元素
    }

    return v.front();
}

template<class T>
void exchange(list<T> &l1, typename list<T>::iterator p1, list<T> &l2, typename list<T>::iterator p2) {
    //题目所给得函数原型有误,应该在list<T>::iterator前加上typename: 类模板不能作为参数,其实例才可以
    l1.splice(p1, l2, p2, l2.end());
    l2.splice(l2.end(), l1, p1, l1.end());
}

template<class Container>
void t10_7(Container c) {
    c.push(5);
    c.push(1);
    c.push(4);
    c.push(6);
    cout << c.top() << " ";
    c.pop();
    cout << c.top() << " ";
    c.pop();
    c.push(2);
    c.push(3);
    cout << c.top() << " ";
    c.pop();
    cout << c.top() << endl;
}

template<>
void t10_7(queue<int> c) {
    c.push(5);
    c.push(1);
    c.push(4);
    c.push(6);
    cout << c.front() << " ";
    c.pop();
    cout << c.front() << " ";
    c.pop();
    c.push(2);
    c.push(3);
    cout << c.front() << " ";
    c.pop();
    cout << c.front() << endl;
}

double pow_int(int base, int indices) {
    double result = 1.0;
    int cycles = abs(indices);
    if (indices == 0)           //x^0 = 1, 0^0 = 1
        return 1;
    else if (base == 0)         //0^x = 0 (x !=0)
        return 0;
    else {
        while (cycles--)
            result *= base;
    }
    return indices > 0 ? result : 1.0 / result;     //x^-a = 1 / x^a
}

int main() {
    /*10-1:
     * 1.各种子概念
     *   容器: ①基于容器中元素的组织方式: 顺序容器、关联容器;②基于容器所关联的迭代器类型: 可逆容器-->随机访问容器
     *   迭代器: 输入迭代器、输出迭代器-->前向迭代器-->双向迭代器-->随机访问迭代器
     *   算法: 不可变序列算法、可变序列算法、排序算法、搜索算法、数值算法
     * 2.划分
     * vector/deque --- 随机访问迭代器
     * list/set/multiset/map/multimap  --- 双向迭代器
     */

    /*10-2:
     * 1.[s1+1, s+4)包括s[1]、s[2]和s[3]
     * 2.除[s+4, s+4)外均为非法区间
     */

    //10-3: 实例自行建立.现象是往s中添加的元素个数小于等于capacity时,s.capacity()不变;之后会因重新分配空间而变大

    //10-4: vector  deque   list

    /*10-5:
     * 1.约瑟夫问题: 又称丢手绢问题,它的一般形式是n个人围成一圈,第一个人从1开始报数,报到第m时,此人就会自杀,最后剩一个人存活.
     * 例如当n = 4, m = 3时, 自杀的顺序是3,2,4,最后活下来的是1
     * 2.解决方法:
     *   ①直接用for循环
     *   ②按照题目要求操作顺序容器,简直多此一举
     * 3.计算程序运行时间的方法,其余见getRunTime.cpp
     * 运行环境说明:
     * 处理器:     Intel(R) Core(TM) i5-7300HQ CPU @ 2.50GHz   2.50 GHz
     * 机带RAM:    8.00 GB
     * 操作系统版本: Windows 10专业版 22H2
     * GCC:        12.2.0
     * MinGW-w64:  10.0.0 (linked with ucrt)
     * IDE:        CLion 2022.3.2
     * 运算结果:
     * vector: 1s左右
     * deque:  6s左右
     * list:   70s左右
     */
    time_t start_time, end_time;

    start_time = clock();
    cout << JosephProblem<vector<int>>(100000, 5) << endl;
    end_time = clock();
    cout << "vector: " << (end_time - start_time) << "ms" << endl;

    start_time = clock();
    cout << JosephProblem<deque<int>>(100000, 5) << endl;
    end_time = clock();
    cout << "deque: " << (end_time - start_time) << "ms" << endl;

    start_time = clock();
    cout << JosephProblem<list<int>>(100000, 5) << endl;
    end_time = clock();
    cout << "list: " << (end_time - start_time) << "ms" << endl;

    //10-6: 调用list的splice成员函数即可实现
    list<int> list1, list2;
    for (int i = 0; i < 10; i++) {
        list1.push_back(i);
        list2.push_front(i);
    }
    auto iter1 = next(list1.begin(), 5);
    auto iter2 = next(list2.begin(), 7);
    exchange(list1, iter1, list2, iter2);

    cout << "list1: ";
    for (auto i: list1)
        cout << i << " ";
    cout << endl;

    cout << "list2: ";
    for (auto i: list2)
        cout << i << " ";
    cout << endl;

    /*10-7: 对栈、队列和优先级队列的操作特点的认识
     * 栈:       后进先出
     * 队列:      先进先出
     * 优先级队列: 弹出的是最大的元素
     */
    stack<int> s;
    queue<int> q;
    priority_queue<int> pq;
    t10_7(s);   //以_开头的全局变量名为保留关键字,用户不能使用
    t10_7(q);
    t10_7(pq);

    /*10-8:I do not believe in taking the right decision . I take a decision and make it right .
     * 如何输入EOF?
     * Linux/Unix/Mac:      ctrl + D
     * cmd/Visual Studio:   ctrl + Z
     * CLion + MinGW:       ctrl + C(没反应,应该是键位冲突),将\CLion\bin下的idea.properties文件打开,将
     * run.processes.with.pty=TRUE的TRUE改为FALSE,重启CLion,然后就可以用ctrl + D了
     * CLion + Cygwin:      ctrl + D
     */
    map<string, int> word_map;
    cout << "Enter several words please (end up with EOF):";
    string word;
    while (cin >> word)
        ++word_map[word];
    for (const auto &pair: word_map)   //C++17允许将pair替换为[word, count]
        cout << "{" << pair.first << ", " << pair.second << "} ";
    cout << endl;

    /*10-9: 直接看标准头文件中的函数声明或模板实现(以MSVC编译器提供的库为例),请自行编写程序对比
     * 微软一般喜欢用left和right命名,本人改为了a和b, T和_FwdIt1都是模板参数
     * void swap(T& a, T& b){       //容器特花版就是在内部调用自己的成员函数swap
     *     C++11前、C++11和C++14、C++17起这三个阶段,该函数所在头文件不一样,定义也略有不同,但主体实现还是
     *     T temp = a;
     *     a = b;
     *     b = temp;
     *     详见https://zh.cppreference.com/w/cpp/algorithm/swap或直接看头文件中的源代码,从命名也能看出来设计者比我们
     *     多做了那些工作
     * }
     * void iter_swap(_FwdIt1 _Left, _FwdIt2 _Right){
     *     swap(*_Left, *_Right);
     * }
     * _FwdIt2 swap_ranges(const _FwdIt1 _First1, const _FwdIt1 _Last1, _FwdIt2 _First2);
     * 分析: swap的形参是引用,剩下两个的形参都是指针,swap_ranges实现起来比较复杂,但原理很简单,就是先验证参数的合法性,然后在
     *      范围[first1, last1)和始于first2的另一范围间交换元素,最后返回指向始于 first2 的范围中被交换的最末元素后一元素的
     *      迭代器。其具体实现涉及的函数模板比较多,感兴趣的可自行研究,微软在关键代码处写了注释.
     * 说明: 本人也不知道如何编写程序对比这三个函数对数组中的元素执行的交换操作,打印地址?或许直接看源码是最佳选择.
     */

    //10-10:本题完全没必要用sort函数,因为set内的元素本身就是有序的
    set<int> s1, s2;

    cout << "Enter a set of integers please (end up with 0):";    //49 36 59 50 49 65 52 78 67 96 0
    while (true) {
        int v;
        cin >> v;
        if (v == 0) break;
        auto r = s1.insert(v);
        if (!r.second)
            cout << v << " is duplicated." << endl;
    }
    cout << "Enter a set of integers again, please (end up with 0):"; //89 4 79 23 49 75 18 47 89 21 0
    while (true) {
        int v;
        cin >> v;
        if (v == 0) break;
        auto r = s2.insert(v);
        if (!r.second)
            cout << v << " is duplicated." << endl;
    }

    cout << "The intersection of s1 and s2 = {";
    set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), ostream_iterator<int>(cout, ","));
    cout << "}" << endl;
    cout << "The union of s1 and s2 = {";
    set_union(s1.begin(), s1.end(), s2.begin(), s2.end(), ostream_iterator<int>(cout, ","));
    cout << "}" << endl;
    cout << "The difference of s1 and s2 = {";
    set_difference(s1.begin(), s1.end(), s2.begin(), s2.end(), ostream_iterator<int>(cout, ","));
    cout << "}" << endl;

    /*10-11:
     * (1).输出结果是1 4 5 4 5,没有真正达到去除重复元素的目的.修改如代码所示:
     * (2).list自己的unique成员函数就是更高效的方法
     * 运行结果分析:
     * ①unique删除的是[first, last)中连续重复的元素(只保留第一个),并返回范围的新逻辑结尾的尾后迭代器(例如,
     * 它在本程序中返回的是指向s的第4个元素的迭代器).它其实是通过用覆写的方式对重复元素进行逻辑上的删除的,容器的迭代器、引用
     * 和指针都没有失效,所以我们可以调用erase来真正删除那些重复元素.
     * ②list是直接删除结点
     */
    int arr[] = {1, 1, 4, 4, 5};

    vector<int> v(arr, arr + 5);
    v.erase(unique(v.begin(), v.end()), v.end());
    copy(v.begin(), v.end(), ostream_iterator<int>(cout, "\n"));

    list<int> ls(arr, arr + 5);
    ls.unique();
    copy(ls.begin(), ls.end(), ostream_iterator<int>(cout, "\n"));

    //10-12:用srand()和rand()定义的函数对象的返回值不变,所以只能用C++的<random>库来实现
    vector<int> v1(5);

    generate(v1.begin(), v1.end(), [] {
        random_device seed;                         //产生随机数种子: 硬件生成的真随机数,开销很大,不适合多次调用以生成随机数序列
        default_random_engine gen(seed());      //随机数生成器: MSVC的默认随机数引擎是梅森旋转算法mersenne_twister
        uniform_int_distribution<int> dis(0, 9);    //随机数分布器:只能用均匀分布,其他分布无法指定范围
        return dis(gen);
    });
    for (const auto &i: v1)
        cout << i << " ";
    cout << endl;

    //10-13: 当指数不大,用循环连乘要比系统的pow函数执行效率高
    cout << "5 to the of power of n:" << endl;
    transform(v1.begin(), v1.end(), ostream_iterator<int>(cout, " "),
              [](int n) { return pow_int(5, n); });
    cout << endl;

    cout << "n to the of power of 7:" << endl;
    transform(v1.begin(), v1.end(), ostream_iterator<int>(cout, " "),
              [](int n) { return pow_int(n, 7); });
    cout << endl;

    cout << "n to the of power of n:" << endl;
    transform(v1.begin(), v1.end(), ostream_iterator<int>(cout, " "),
              [](int n) { return pow_int(n, n); });
    cout << endl;

    //10-14: 见Array.h

    //10-15: 见IncrementIterator.h

    //10-16: 对模板特化的简单应用,详见mySort.cpp

    /*10-17:Boost编译和导入到IDE还是比较难的,现提供Compile&Include_Boost.txt以供参考
     * 1.10_3.cpp修改前后对比：修改后的数组看起来就像STL容器
     * 2.见10_18.cpp
     * 3.见10_18.cpp
     * 4.见10_10.cpp
     */

    return 0;
}