#ifndef ZHENGLI_PRINTARRAY_H
#define ZHENGLI_PRINTARRAY_H

#include <ostream>

template<class ElemType>
void printArray(const ElemType a[], int n) {
    for (int i = 0; i < n; i++)
        std::cout << a[i] << " ";
    std::cout << std::endl;
}

#endif //ZHENGLI_PRINTARRAY_H
