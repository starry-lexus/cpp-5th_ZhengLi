/**交换排序
 * 1.冒泡排序
 * 2.快速排序
 *@author 岛中客、眼中你
 *@date 2023/2/2
 */
#ifndef ZHENGLI_SWAPSORT_H
#define ZHENGLI_SWAPSORT_H

/**冒泡排序
 * 1.算法思想：从后往前(或从前往后)两两比较相邻元素的值,若为逆序,则交换它们,直到序列比较完,我们称其为一趟冒泡,结果是关键字最小或最大的
 * 元素放到了序列的最终位置。这样最多做n-1趟冒泡就能把所有元素排好序
 * 2.算法性能：空间复杂度为o(n),时间复杂度为o(n^2)
 * 3.稳定性：稳定
 * 4.适用性：顺序存储的和链式存储的线性表
 * @tparam ElemType
 * @param a
 * @param n
 */
template<typename ElemType>
void bubbleSort(ElemType a[], int n) {
    for (int i = 0; i < n - 1; i++) {
        bool flag = false;                      //表示本趟冒泡是否发生交换的标志
        for (int j = n - 1; j > i; j--) {        //一趟冒泡过程
            if (a[j - 1] > a[j]) {                 //若为逆序:将>改为<即可变为降序排序
                std::swap(a[j - 1], a[j]);        //则交换
                flag = true;
            }
        }
        if (!flag) return;                      //如果本趟冒泡没有发生交换，那么说明表已经有序
    }
}

template<typename ElemType>
void bubbleSort(ElemType *a[], int n) {
    bubbleSort(*a, n);
}

#endif //ZHENGLI_SWAPSORT_H
