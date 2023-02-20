#ifndef INSERTSORT_H
#define INSERTSORT_H

/**直接插入排序
 * 1.算法思想：每次将一个待排序的记录按其关键字大小插入前面前面已经排好序的子序列,直到全部记录被插入.
 * 2.排序结果：非递减序列
 * 3.算法性能：时间复杂度为o(n^2)，空间复杂度为o(1)
 * 4.适用性：适用于顺序存储和链式存储的线性表
 * 5.稳定性：稳定
 * 6.说明：教材提供的源代码中i、j和temp被重复定义,虽然函数作用域中的3个变量被for语句作用域的隐藏了,在语法上没有错,但是这是一种很不好
 * 的习惯;本人对原代码进行了改写
 * @tparam ElemType     数组元素类型
 * @param a             数组名
 * @param n             数组大小
 */
template<class ElemType>
void DirectInsertionSort(ElemType a[], int n) {   //将下标为1～n-1的元素逐个插入到已排序序列中适当的位置
    int i, j;
    for (i = 1; i < n; i++) {                   //依次将a[1]~a[n-1]插入前面的有序序列中
        if (a[i] < a[i - 1]) {                   //若a[i]小于其前驱,则将其插入有序表中
            ElemType temp = a[i];               //暂存待插入元素
            for (j = i - 1; temp < a[j]; j--)   //从后往前查找待插入位置
                a[j + 1] = a[j];                //向后挪位
            a[j + 1] = temp;                    //复制到插入位置
        }
    }
}

/**折半插入排序
 * 相比与直接插入排序,该算法仅仅减少了元素的比较次数,其他方面与直接插入排序算法一致
 * @tparam ElemType
 * @param a
 * @param n
 */
template<class ElemType>
void BinaryInsertionSort(ElemType a[], int n) {
    int low, high, mid;
    for (int i = 1; i < n; i++) {
        ElemType temp = a[i];
        low = 0, high = i - 1;                   //设置折半查找的范围
        while (low <= high) {
            mid = low + (high - low) / 2;       //此表达式可防止low+hig结果溢出
            if (temp < a[mid])
                high = mid - 1;                 //查找左子表
            else
                low = mid + 1;                  //查找右子表
        }
        for (int j = i - 1; j >= high + 1; j--) //high+1或low就是插入位置
            a[j + 1] = a[j];                    //向后移动元素
        a[high + 1] = temp;
    }
}

/**希尔排序
 * 1.算法思想：先将待排序表分割成若干形如L[i, i+d, i+2d, ..., i+kd]的子表，然后对各个子表分别进行直接插入排序，
 * 当整个表中的元素"基本有序"时，再对整个表进行一次直接插入排序
 * 2.算法性能：其时间复杂度依赖于增量序列的函数，其准确数量级尚无法计算出。当n在某个特定范围内时，其时间复杂度约为o(n^1.3)，
 * 在最坏情况下，其时间复杂度为o(n^2)
 * 3.适用性：顺序存储的线性表，最好是基本有序且数据量不大
 * 4.稳定性：不稳定
 * @tparam ElemType
 * @param a
 * @param n
 */
template<class ElemType>
void ShellSort(ElemType a[], int n) {
    for (int dk = n / 2; dk >= 1; dk /= 2) {     //设置步长dk
        for (int i = dk; i < n; i++) {
            if (a[i] < a[i - dk]) {
                int j;
                ElemType temp = a[i];
                for (j = i - dk; j >= 0 && temp < a[j]; j -= dk)
                    a[j + dk] = a[j];
                a[j + dk] = temp;
            }
        }
    }
}

#endif    //INSERTSORT_H
