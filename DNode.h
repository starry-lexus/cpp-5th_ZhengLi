/**
 *@author 岛中客、眼中你
 *@date 2023/2/1
 */
#ifndef DNODE_H
#define DNODE_H

//类模板的定义
template<class DataType>
class DNode {
private:
    DNode<DataType> *next;        //指向后继结点的指针
    DNode<DataType> *prior;     //指向前驱结点的指针
    DataType data;                //数据域
public:
    explicit DNode(const DataType &data, DNode<DataType> *next = nullptr, DNode<DataType> *prior = nullptr);

    void insertAfter(DNode<DataType> *p);         //在本结点之后插入一个同类结点p
    DNode<DataType> *deleteAfter();                 //删除本结点的后继结点，并返回其地址
    DNode<DataType> *nextDNode();                 //获取后继结点的地址
    const DNode<DataType> *nextDNode() const;     //获取后继结点的地址
    DNode<DataType> *priorDNode();                 //获取前驱结点的地址
    const DNode<DataType> *priorDNode() const;     //获取前驱结点的地址
};


//构造函数，初始化数据和指针成员
template<class DataType>
DNode<DataType>::DNode(const DataType &data, DNode<DataType> *next, DNode<DataType> *prior)
        : data(data), next(next), prior(prior) {}

//返回后继结点的指针
template<class DataType>
DNode<DataType> *DNode<DataType>::nextDNode() {
    return next;
}

//返回后继结点的指针(const)
template<class T>
const DNode<T> *DNode<T>::nextDNode() const {
    return next;
}

//返回前驱结点的指针
template<class DataType>
DNode<DataType> *DNode<DataType>::priorDNode() {
    return prior;
}

//返回前驱结点的指针(const)
template<class DataType>
const DNode<DataType> *DNode<DataType>::priorDNode() const {
    return prior;
}

//在当前结点之后插入一个结点p
template<class T>
void DNode<T>::insertAfter(DNode<T> *p) {
    /* 1.如果觉得比较难理解,不放画个图试试
     * 2.如果你之前学习过用C语言描述的数据结构,那么你可以试试加入this指针,例如this->next,看会会对你的理解有所帮助
     * 3.注意:①和②步必须在④步之前,否则*this的后继结点的指针就会丢失,从而导致插入失败
     */
    p->next = next;         //①将当前结点的后继作为p的后继
    next->prior = p;        //②将p作为当前结点的后继的前驱
    p->prior = this;        //③当前结点成为p的前驱
    next = p;               //④p成为当前结点的后继
}

//删除当前结点的后继结点，并返回其地址
template<class T>
DNode<T> *DNode<T>::deleteAfter() {
    DNode<T> *tempPtr = next;    //将欲删除的结点地址存储到tempPtr中
    if (next == nullptr)        //如果当前结点没有后继结点，则返回空指针
        return nullptr;
    next = tempPtr->next;
    tempPtr->next->prior = this;
    delete tempPtr;
}

#endif //DNODE_H