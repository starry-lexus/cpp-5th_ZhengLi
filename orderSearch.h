/**顺序查找
 * 1.按序查找
 * 2.折半查找
 * 3.分块查找
 *@author 岛中客、眼中你
 *@date 2023/2/2
 */
#ifndef ZHENGLI_ORDERSEARCH_H
#define ZHENGLI_ORDERSEARCH_H

template<typename ElemType>
int seqSearch(const ElemType list[], int n, const ElemType &key) {
    for (int i = 0; i < n; i++)
        if (list[i] == key)
            return i;
    return -1;
}

template<class T>
int binSearch(const T list[], int n, const T &key) {
    int low = 0;
    int high = n - 1;
    while (low <= high) {                    //low <= high表示整个数组尚未查找完
        int mid = low + (high - low) / 2;    //求中间元素的下标
        if (key == list[mid])
            return mid;                        //若找到,返回下标
        else if (key < list[mid])
            high = mid - 1;                    //若key < midvalue将查找范围缩小到数组的前一半
        else
            low = mid + 1;                    //否则将查找范围缩小到数组的后一半
    }
    return -1;                              //没有找到返回-1
}

#endif //ZHENGLI_ORDERSEARCH_H
