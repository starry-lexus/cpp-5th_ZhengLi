/**
 *@author 岛中客、眼中你
 *@date 2023/2/1
 */
#ifndef ZHENGLI_ORDERLIST_H
#define ZHENGLI_ORDERLIST_H

#include "LinkedList.h"

template<typename ElemType>
class OrderList : public LinkedList<ElemType> {
public:
    void insert(const ElemType &item);
};

template<typename ElemType>
void OrderList<ElemType>::insert(const ElemType &item) {
    this->reset();                   //从头开始寻找插入点
    while (!this->endOfList()) {
        if (item <= this->data())    //如果待插入数据不大于当前结点的数据,则将其插入当前结点之前
            break;
        this->next();
    }
    this->insertAt(item);
}

#endif //ZHENGLI_ORDERLIST_H
