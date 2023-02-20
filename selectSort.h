/**选择排序
 *@author 岛中客、眼中你
 *@date 2023/2/1
 */
#ifndef SELECTSORT_H
#define SELECTSORT_H

/**简单(直接)选择排序算法
 * 1.算法思想
 * 2.算法性能：
 *   空间复杂度为o(1)
 *   时间复杂度为o(n^2),元素移动次数为0 ~ 3(n-1)次,元素比较次数为n(n-1)/2
 * 3.适用性：顺序存储和链式存储的线性表
 * 4.稳定性：不稳定
 * @tparam ElemType
 * @param a
 * @param n
 */
template<typename ElemType>
void selectSort(ElemType a[], int n) {
    for (int i = 0; i < n; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++)
            if (a[j] < a[min]) min = j;
        if (min != i)
            std::swap(a[i], a[min]);
    }
}

/**堆排序
 * 1.相关概念：n个关键字序列L[1...n]称为堆,当且仅当该序列满足L(i) >= L(2i) && L(i) >= L(2i+1)时,该堆为大根堆(max heap);
 * 当且仅当该序列满足L(i) <= L(2i) && L(i) <= L(2i+1)时,该堆为小根堆(min heap).其中1 <= i <= floor(n/2)
 * 2.算法思想：
 * 3.算法性能：
 *   空间复杂度为o(1)
 *   时间复杂度为o(n*log2 n)：，建堆时间为o(n)，之后又n-1次调整时间，每次调整堆的时间复杂度为o(h)，所以在最好、最坏和平均情况下，
 *   堆排序的时间复杂度均为o(n*log2 n)
 * 4.适用性：适合关键字较多的情况，例如从10亿个数据中筛选出前100个最大值
 * 5.稳定性：不稳定
 */
template<typename ElemType>
void HeapAdjust(ElemType a[], int k, int len) {  //将以元素a[k]为根的子树调整为大根堆
    ElemType temp = a[k];       //暂存子树的根结点
    for (int i = 2 * k + 1; i < len; i = 2 * i + 1) {    //沿key较大的子结点向下筛选
        if (i < (len - 1) && a[i] < a[i + 1])
            i++;                                        //取key较大的子结点的下标
        if (temp >= a[i])
            break;                                      //筛选结束
        else {
            a[k] = a[i];                                //尝试将a[i]调整到双亲结点上
            k = i;                                      //修改k值,以便继续向下调整
        }
    }
    a[k] = temp;                                        //放入最终位置
}

template<typename ElemType>
void BuildMaxHeap(ElemType a[], int len) {
    for (int i = len / 2 - 1; i >= 0; i--)
        HeapAdjust(a, i, len);                      //从i = (n/2-1) ~ 0反复调整堆
}

template<typename ElemType>
void heapSort(ElemType a[], int len) {
    BuildMaxHeap(a, len);                              //初始建堆
    for (int i = len - 1; i > 0; i--) {                 //n-1趟的交换和建堆过程
        std::swap(a[i], a[0]);                         //输出堆顶元素(和堆底元素交换)
        HeapAdjust(a, 0, i - 1);                //调整剩余i-1个元素
    }
}

#endif //SELECTSORT_H