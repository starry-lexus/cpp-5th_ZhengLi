#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>

using namespace std;

//实际上,该函数模板确实用到了,CLion的流分析有问题
template<class InputIterator, class OutputIterator>
void mySort_helper(InputIterator first, InputIterator last, OutputIterator result, output_iterator_tag) {
    vector<typename iterator_traits<InputIterator>::value_type> s(first, last);
    sort(s.begin(), s.end());
    copy(s.begin(), s.end(), result);
}

template<class InputIterator, class OutputIterator>
void mySort_helper(InputIterator first, InputIterator last, OutputIterator result, random_access_iterator_tag) {
    const auto &first2 = result;
    const auto &last2 = copy(first, last, result);
    sort(first2, last2);
    /*可能有人想到了用if语句判断第三个迭代器的类型,从而完成不同的实现,但事实上这种做法是错误的,先看代码
     if (typeid(typename iterator_traits<OutputIterator>::iterator_category) == typeid(random_access_iterator_tag)){
        auto first2 = result;
        auto last2 = copy(first, last, result);
        sort(first2, last2);
     }else{
        vector<typename iterator_traits<InputIterator>::value_type> s(first, last);
        sort(s.begin(), s.end());
        copy(s.begin(), s.end(), result);
     }
     * 1.
     * 调用处警告信息: In instantiation of function template specialization
     *              'mySort<double *, std::ostream_iterator<double>>'
     * sort(first2, last2)处的错误信息: In template: invalid operands to binary expression
     *      ('const std::ostream_iterator<double>' and 'const std::ostream_iterator<double>')
     *      error occurred here in instantiation of function template specialization
     *      'std::sort<std::ostream_iterator<double>, std::less<>>' requested here in instantiation
     *      of function template specialization 'std::sort<std::ostream_iterator<double>>' requested
     *      here in instantiation of function template specialization 'mySort<double *,
     *      std::ostream_iterator<double>>' requested here candidate template ignored: could not match
     *      'reverse_iterator' against 'ostream_iterator' candidate template ignored: could not match
     *      'move_iterator' against 'ostream_iterator'
     * 2.原因分析: 如果把if-else语句块中的sort或全部代码换为输出语句,就会发现mySort第三个参数为非随机访问迭代器时,程序的确进入了
     *            else语句块中,这说明编译器在实例化模板时完全无视分支语句或者有可能编译器采用的就是"高级的"替换,这才导致它直接将
     *            const std::ostream_iterator<double>作为sort的参数类型,从而发生参数类型不匹配的现象
     */
}

//将来自输入迭代器p的n个数值排序，将结果通过输出迭代器result输出
template<class InputIterator, class OutputIterator>
inline void mySort(InputIterator first, InputIterator last, OutputIterator result) {
    mySort_helper(first, last, result, typename iterator_traits<OutputIterator>::iterator_category());
}

int main() {
    //将s数组的内容排序后输出
    double a[5] = {1.2, 2.4, 0.8, 3.3, 3.2};
    mySort(a, a + 5, ostream_iterator<double>(cout, " "));
    cout << endl;

    //从标准输入读入若干个整数，将排序后的结果输出
    mySort(istream_iterator<int>(cin), istream_iterator<int>(), ostream_iterator<int>(cout, " "));
    cout << endl;

    vector<double> v(5, 0);         //必须先对vector初始化,否则其无法生成有效的迭代器,从而让程序从copy函数处异常终止
    auto first = v.begin();
    mySort(a, a + 5, first);
    for (const auto i: v)
        cout << i << " ";

    return 0;
}
