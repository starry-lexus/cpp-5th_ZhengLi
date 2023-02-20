/**
 *@author 岛中客、眼中你
 *@date 2023/2/10
 */
#include <iterator>
#include <algorithm>
#include <iostream>

using namespace std;

//将迭代器视为索引,这样实现起来就会觉得得心应手
template<class T>        //随机访问迭代器是输入迭代器的子概念(适配器),只需继承一个即可
class IncrementIterator : public iterator<random_access_iterator_tag, T> {
private:
    T value;
public:
    using iterator = IncrementIterator<T>;
    using value_type = T;
    using difference_type = ptrdiff_t;
    using const_reference = const value_type &;
    using const_pointer = const value_type *;

    //为了迭代器的隐蔽性,不允许隐式类型转换
    explicit IncrementIterator(const T &value = T()) : value(value) {}

    /* 1.理论上以非成员函数形式重载==和<后就可以通过namespace std::rel_ops自动生成其他关系运算符的重载,但copy算法却不买账.
     * 2.可以用更通用的Boost.operators代替namespace std::rel_ops
     * 3.C++20前MSVC编译器其实是用成员函数的形式重载vector的所有关系运算符的,这与cppreference的描述不符,估计这本身就是由
     * 实现定义的,从C++20开始,STL库是用==和<=>(三路比较)两个重载运算符函数合成其他的运重载算符函数的
     */
    bool operator==(const iterator &right) const { return value == right.value; }

    bool operator<(const iterator &right) const { return value < right.value; }

    bool operator!=(const iterator &right) const { return !(*this == right); }

    bool operator>(const iterator &right) const { return right < *this; }

    bool operator<=(const iterator &right) const { return !(right < *this); }

    bool operator>=(const iterator &right) const { return !(*this < right); }

    /*后缀"++",后缀--同理
     * 1.这里允许出现连续后缀++这种无意义的操作,STL的容器也允许,但内置表达式不允许
     * 2.clang-tidy自相矛盾的说法:
     * ①返回iterator: Overloaded 'operator++' returns a non-constant object instead of a constant object type
     * ②返回const_iterator: Return type 'const iterator' (aka 'const IncrementIterator<T>') is 'const'-qualified
     *   at the top level, which may reduce code readability without improving const correctness.
     * ③按照Stack Overflow的建议,返回左值引用: 警告信息同①
     * 总之,不管改成什么样子,clang-tidy都不满意,那干脆就别管了,按常规做法来.
     */
    iterator &operator++() {    //前缀++
        ++value;
        return *this;
    }

    iterator operator++(int) {   //后缀++
        iterator tmp = *this;
        ++value;
        return tmp;
    }

    iterator &operator--() {
        --value;
        return *this;
    }

    iterator operator--(int) {
        iterator temp = *this;
        --value;
        return temp;
    }

    iterator &operator+=(const difference_type offset) {
        value += offset;
        return *this;
    }

    iterator &operator-=(const difference_type offset) {
        value -= offset;
        return *this;
    }

    iterator operator+(const difference_type offset) const {
        iterator temp = *this;
        temp.value += offset;
        return temp;
    }

    friend iterator operator+(const difference_type offset, iterator next) {
        next.value += offset;
        return next;
    }

    iterator operator-(const difference_type offset) const {
        iterator temp = *this;
        temp.value -= offset;
        return temp;
    }

    difference_type operator-(const iterator &right) const {
        return value - right.value;
    }

    const_reference operator[](const unsigned int index) {
        return *(*this + index);
    }

    const_reference operator*() const { return value; }

    const_pointer operator->() const { return &value; }
};

int main() {
    //将[0, 10)范围内的整数输出
    copy(IncrementIterator<int>(), IncrementIterator<int>(10), ostream_iterator<int>(cout, " "));
    cout << endl;

    //将下面数组中的数分别加上0、1、2、3、……，然后输出
    int s[] = {5, 8, 7, 4, 2, 6, 10, 3};
    transform(s, s + sizeof(s) / sizeof(int), IncrementIterator<int>(), ostream_iterator<int>(cout, " "), plus<>());
    cout << endl;

    //测试迭代器是否完全支持随机访问
    IncrementIterator<int> x, y;
    cout << *x << endl;
    cout << *(x + 10) << endl;
    cout << *(10 + x) << endl;
    cout << *(x - 10) << endl;
    cout << *(y++) << endl;
    cout << *(++y) << endl;
    cout << *(y--) << endl;
    cout << *(--y) << endl;
    cout << y - x << endl;
    cout << *(y += 4) << endl;
    cout << *(y -= 2) << endl;
    cout << y[2] << endl;
    cout << "t == y ? " << (x == y) << endl;
    cout << "t < y ? " << (x < y) << endl;

    return 0;
}