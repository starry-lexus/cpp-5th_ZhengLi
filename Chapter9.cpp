/**关于多文件中模板的组织
 * 1.将模板的声明和实现都放在头文件中:当多个源文件都引用此头文件时,会造成冲突,但C++标准允许这种冲突的存在,所以问题就是代码冗余
 * 2.使用export关键字:①C++11标准以前用于标注一个模板定义被导出,C++11 ~ C++17不再使用,但保留该关键字,C++20重新启用,但一般
 * 是结合模块(module)使用;②目前对module支持最好的是Visual Studio 2022,CLion在IDE层面不完全支持,gcc对C++20标准的实现
 * 不太好,bug较多;③本书是根据C++11和C++14标准撰写的,所以可以认为gcc和VS均不支持export,尽管只有Comeau C/C++和Intel 7.x
 * 编译器实现了这个功能,但它们也是对export持反对态度
 * 3.在头文件的末尾include源文件:多此一举,本质上还是方法一
 * 4.显式实例化:需要穷举所有可能被其他函数实例化的实例
 * 建议:企业基本用的都是C++11,顶多到C++14,所以不需要太过关心新特性,完全可以在用的时候学习或查询;C++20标准之前的STL库也是把模板
 * 的声明和定义都放到了头文件中
 * 关于代码测试
 * 查找和排序算法题有代码复用,运行时需要注释
 *@author 岛中客、眼中你
 *@date 2023/2/1
 */
#include <iostream>
#include "Array.h"
#include "printArray.h"
#include "insertSort.h"
#include "selectSort.h"
#include "swapSort.h"
#include "orderSearch.h"

using namespace std;

template<typename T>
void print_error(const T &msg) {
    std::cout << msg << std::endl;
}

template<typename T, typename... TypeArgs>
void print_error(const T &msg, const TypeArgs &... args) {
    std::cout << msg << std::endl;
    return print_error(args...);
}

/**排列数
 * 1.定义：从M个不同元素中任取N个元素的所有排列的个数(M >= N)
 * 2.计算公式
 *   ①M*(M-1)*(M-2)*(M-N+1)
 *   ②M! / (M-N)! , 规定0! = 1
 * 3.说明:如果先定义阶乘模板,再用阶乘计算排列数,那么性能会较差
 * @tparam M
 * @tparam N
 */
template<unsigned M, unsigned N>
struct Permutation {
    enum {
        VALUE = M * Permutation<M - 1, N - 1>::VALUE
    };
};
template<unsigned M>
struct Permutation<M, 0> {
    enum {
        VALUE = 1
    };
};
template<unsigned N>
struct Permutation<0, N> {       //若M < N(参数错误),则VALUE = 0
    enum {
        VALUE = 0
    };
};
template<unsigned M>
struct Permutation<M, M> {       //若M = N(特殊情况),则直接返回VALUE = 1
    enum {
        VALUE = 1
    };
};

template<unsigned M, unsigned N>
struct GCD {
    enum {
        VALUE = GCD<N % M, M>::VALUE
    };
};
template<unsigned M>
struct GCD<0, M> {
    enum {
        VALUE = M
    };
};

int main() {
    //9-1:
    cout << "Enter the count of students in your class:";
    int n;
    double total = 0.0;
    cin >> n;
    Array<double> scores(n);
    cout << "Enter their scores one by one:";
    for (int i = 0; i < n; i++) {
        cin >> scores[i];
        total += scores[i];
    }
    cout << "The average score is " << total / n << endl;

    /*9-2:
     * 1.链表的结点类至少应该包含数据域和指针域
     * 2.单链表的结点只有指向其后继结点的指针域,而双连表的结点还有指向其前驱结点的指针域.
     * 注意:在非循环链表中,除第一个结点外,其余结点有且仅有一个前驱结点,除最后一个结点外,其余结点有且仅有一个后继结点.
     */

    //9-3:链表中的元素的最大数目取决于系统所能分配的内存空间

    //9-4:在双向链表中所使用的的结点类应该比在单链表中使用的结点类多一个指针域,用于指示其前驱结点.具体实现见DNode.h

    //9-5:见9_5answer.cpp

    //9-6:见9_6answer.cpp

    //9-7:栈是一种只能在一端进行插入或删除操作的线性表.栈的操作特性可以概括为后进先出(Last In First Out, LIFO)

    //9-8:队列是一种只能在一端进行插入,在另一端进行删除操作的线性表.队列的操作特性可以概括为先进先出(FIFO)

    //9-9:插入排序的基本思想是每次将一个待排序的记录按其关键字大小插入前面前面已经排好序的子序列,直到全部记录被插入.

    //9-10:若需要打印排序算法的每一步执行结果,请导入printArray.h,下同
    int data[] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19,
                  2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
    DirectInsertionSort(data, 20);

    //9-11:选择排序的基本思想是每一趟(如第i趟)在后面n-i+1(i=1,2,..,n-1)个待排序元素中选取最小的，作为有序子序列的第i个元素

    //9-12:本题沿用了9-10的data数组
    selectSort(data, 20);

    //9-13:交换排序的基本思想是根据序列中两个元素关键字的比较结果来对换这两个记录在序列中的位置

    //9-14:
    bubbleSort(data, 20);

    //9-15:只需修改一下交换元素的条件即可,详见swapSort.h

    //9-16:顺序查找的基本思想是根据元素之间的位置关系查找,例如根据数组下标或链表的next指针域

    //9-17:
    int key;
    cout << "Enter an integer to get its index in array data:";
    cin >> key;
    int index = seqSearch(data, 20, key);
    if (index == -1)
        cout << "Not find!" << endl;
    else
        cout << "Its index is " << index << endl;

    /*9-18:折半查找的基本思想:首先将给定值key与表中中间位置的元素比较,若相等,则查找成功,返回该元素的位置;若不等,则所需查找的元素
     * 只能在中间元素的前半部分或后半部分.然后在缩小的范围内继续进行同样的查找,如此重复,直到找到为止,或确定表中没有此元素,则查找失败,
     * 返回查找失败的信息.
     */

    //9-19:利用一下9-17的前三行代码
    int index1 = binSearch(data, 20, key);
    if (index1 == -1)
        cout << "Not find!" << endl;
    else
        cout << "Its index is " << index1 << endl;

    /*9-20:
     * 只有给模板提供了正确的参数,它才会实例化
     * 9_1.cpp中的实例:outputArray(a, A_COUNT)、outputArray(b, B_COUNT)、outputArray(c, C_COUNT)
     * 9_2.cpp中的实例:s1、s2、s3及其所调用过的所有成员函数
     * 注意:对类模板成员函数定义的实例化也是按需进行的,参数相同的实例是不同的实例
     */

    /*9-21:
     * 1.为模板定义特殊的实现：模板的特化、类模板的偏特化、函数模板的重载、模板的默认实参
     * 2.①模板特化是指为特定参数提供特殊的模板定义；②类模板的偏特化是指为部分参数可变部分参数不可变的模板提供特殊定义
     */

    /*9-22:
     * 优点:对问题的处理更加灵活,进一步提高了模板的复用程度
     * 缺点:执行速度较差(递归程序都会重复执行)
     */

    //9-23:调用之前的bubbleSort,参数为(*a,n),详见swapSort.h

    //9-24:
    cout << Permutation<7, 2>::VALUE << endl;
    cout << Permutation<2, 7>::VALUE << endl;
    cout << Permutation<7, 7>::VALUE << endl;
    cout << GCD<12, 16>::VALUE << endl;
    cout << GCD<16, 12>::VALUE << endl;
    cout << GCD<13, 15>::VALUE << endl;

    return 0;
}