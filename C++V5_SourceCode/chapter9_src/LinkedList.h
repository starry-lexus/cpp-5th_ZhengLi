//LinkedList.h
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "Node.h"

#include<iostream>

using namespace std;

template<class T>
class LinkedList {
private:
    //数据成员：
    Node<T> *front, *rear;    //表头和表尾指针
    Node<T> *prevPtr, *currPtr;   //记录表当前遍历位置的指针，由插入和删除操作更新
    int size;    //表中的元素个数
    int position;    //当前元素在表中的位置序号。由函数reset使用

    //函数成员：
    //生成新结点，数据域为item，指针域为ptrNext
    Node<T> *newNode(const T &item, Node<T> *ptrNext = NULL);

    //释放结点
    void freeNode(Node<T> *p);

    //将链表L 拷贝到当前表（假设当前表为空）。
    //被拷贝构造函数、operator = 调用
    void copy(const LinkedList<T> &L);

public:
    LinkedList();    //构造函数
    LinkedList(const LinkedList<T> &L);  //拷贝构造函数
    ~LinkedList();    //析构函数
    LinkedList<T> &operator=(const LinkedList<T> &L); //重载赋值运算符

    int getSize() const;    //返回链表中元素个数
    bool isEmpty() const;    //链表是否为空

    void reset(int pos = 0);//初始化游标的位置
    void next();    //使游标移动到下一个结点
    bool endOfList() const;    //游标是否到了链尾
    int currentPosition() const;    //返回游标当前的位置

    void insertFront(const T &item);    //在表头插入结点
    void insertRear(const T &item);        //在表尾添加结点
    void insertAt(const T &item);        //在当前结点之前插入结点
    void insertAfter(const T &item);    //在当前结点之后插入结点

    T deleteFront();    //删除头结点
    void deleteCurrent();    //删除当前结点

    T &data();                //返回对当前结点成员数据的引用
    const T &data() const;   //返回对当前结点成员数据的常引用

    //清空链表：释放所有结点的内存空间。被析构函数、operator= 调用
    void clear();
};

template<class T>
Node<T> *LinkedList<T>::newNode(const T &item, Node<T> *ptrNext)    //生成新结点
{
    Node<T> *p;
    p = new Node<T>(item, ptrNext);
    if (p == NULL) {
        cout << "Memory allocation failure!\n";
        exit(1);
    }
    return p;
}

template<class T>
void LinkedList<T>::freeNode(Node<T> *p) //释放结点
{
    delete p;
}

template<class T>
void LinkedList<T>::copy(const LinkedList<T> &L) //链表复制函数
{
    Node<T> *p = L.front;    //P用来遍历L
    int pos;
    while (p != NULL)    //将L中的每一个元素插入到当前链表最后
    {
        insertRear(p->data);
        p = p->nextNode();
    }
    if (position == -1)    //如果链表空,返回
        return;
    //在新链表中重新设置prevPtr和currPtr
    prevPtr = NULL;
    currPtr = front;

    for (pos = 0; pos != position; pos++) {
        prevPtr = currPtr;
        currPtr = currPtr->nextNode();
    }
}

template<class T>
//构造一个新链表，将有关指针设置为空，size为0，position为-1
LinkedList<T>::LinkedList() : front(NULL), rear(NULL),
                              prevPtr(NULL), currPtr(NULL), size(0), position(-1) {}

template<class T>
LinkedList<T>::LinkedList(const LinkedList<T> &L)  //拷贝构造函数
{
    //先重置链表
    front = rear = NULL;
    prevPtr = currPtr = NULL;
    size = 0;
    position = -1;

    //然后复制
    copy(L);
}

template<class T>
LinkedList<T>::~LinkedList()    //析构函数
{
    clear();
}

template<class T>
LinkedList<T> &LinkedList<T>::operator=(const LinkedList<T> &L)//重载"="
{
    if (this == &L)    // 不能将链表赋值给它自身
        return *this;
    clear();
    copy(L);
    return *this;
}

template<class T>
int LinkedList<T>::getSize() const    //返回链表大小的函数
{
    return size;
}

template<class T>
bool LinkedList<T>::isEmpty() const    //判断链表为空否
{
    return size == 0;
}

template<class T>
void LinkedList<T>::reset(int pos)    //将链表当前位置设置为pos
{
    int startPos;
    if (front == NULL)    // 如果链表为空，返回
        return;
    if (pos < 0 || pos > size - 1)    // 如果指定位置不合法，中止程序
    {
        cerr << "Reset: Invalid list position: " << pos << endl;
        return;
    }
    // 设置与遍历链表有关的成员
    if (pos == 0)    // 如果pos为0，将指针重新设置到表头
    {
        prevPtr = NULL;
        currPtr = front;
        position = 0;
    } else    // 重新设置 currPtr, prevPtr, 和 position
    {
        currPtr = front->nextNode();
        prevPtr = front;
        startPos = 1;
        for (position = startPos; position != pos; position++) {
            prevPtr = currPtr;
            currPtr = currPtr->nextNode();
        }
    }
}

template<class T>
void LinkedList<T>::next()    //将prevPtr和currPtr向前移动一个结点
{
    if (currPtr != NULL) {
        prevPtr = currPtr;
        currPtr = currPtr->nextNode();
        position++;
    }
}

template<class T>
bool LinkedList<T>::endOfList() const    // 判断是否已达表尾
{
    return currPtr == NULL;
}

template<class T>
int LinkedList<T>::currentPosition() const  // 返回当前结点的位置
{
    return position;
}

template<class T>
void LinkedList<T>::insertFront(const T &item)   // 将item插入在表头
{
    if (front != NULL)    // 如果链表不空则调用Reset
        reset();
    insertAt(item);    // 在表头插入
}


template<class T>
void LinkedList<T>::insertRear(const T &item)   // 在表尾插入结点
{
    Node<T> *nNode;
    prevPtr = rear;
    nNode = newNode(item);    // 创建新结点
    if (rear == NULL)    // 如果表空则插入在表头
        front = rear = nNode;
    else {
        rear->insertAfter(nNode);
        rear = nNode;
    }
    currPtr = rear;
    position = size;
    size++;
}


template<class T>
void LinkedList<T>::insertAt(const T &item)    // 将item插入在链表当前位置
{
    Node<T> *nNode;
    if (prevPtr == NULL)    // 插入在链表头，包括将结点插入到空表中
    {
        nNode = newNode(item, front);
        front = nNode;
    } else    // 插入到链表之中. 将结点置于prevPtr之后
    {
        nNode = newNode(item);
        prevPtr->insertAfter(nNode);
    }
    if (prevPtr == rear)    //正在向空表中插入，或者是插入到非空表的表尾
    {
        rear = nNode;    //更新rear
        position = size;    //更新position
    }
    currPtr = nNode;    //更新currPtr
    size++;    //使size增值
}


template<class T>
void LinkedList<T>::insertAfter(const T &item)  // 将item 插入到链表当前位置之后
{
    Node<T> *p;
    p = newNode(item);
    if (front == NULL)     // 向空表中插入
    {
        front = currPtr = rear = p;
        position = 0;
    } else    // 插入到最后一个结点之后
    {
        if (currPtr == NULL)
            currPtr = prevPtr;
        currPtr->insertAfter(p);
        if (currPtr == rear) {
            rear = p;
            position = size;
        } else
            position++;
        prevPtr = currPtr;
        currPtr = p;
    }
    size++;              // 使链表长度增值
}


template<class T>
T LinkedList<T>::deleteFront()    // 删除表头结点
{
    T item;
    reset();
    if (front == NULL) {
        cerr << "Invalid deletion!" << endl;
        exit(1);
    }
    item = currPtr->data;
    deleteCurrent();
    return item;
}


template<class T>
void LinkedList<T>::deleteCurrent()    // 删除链表当前位置的结点
{
    Node<T> *p;
    if (currPtr == NULL)    // 如果表空或达到表尾则出错
    {
        cerr << "Invalid deletion!" << endl;
        exit(1);
    }
    if (prevPtr == NULL)    // 删除将发生在表头或链表之中
    {
        p = front;    // 保存头结点地址
        front = front->nextNode();    //将其从链表中分离
    } else    //分离prevPtr之后的一个内部结点，保存其地址
        p = prevPtr->deleteAfter();

    if (p == rear)    // 如果表尾结点被删除
    {
        rear = prevPtr;    //新的表尾是prevPtr
        position--;    //position自减
    }
    currPtr = p->nextNode();    // 使currPtr越过被删除的结点
    freeNode(p);    // 释放结点，并
    size--;    //使链表长度自减
}

template<class T>
T &LinkedList<T>::data()    //返回一个当前结点数值的引用
{
    if (size == 0 || currPtr == NULL)    // 如果链表为空或已经完成遍历则出错
    {
        cerr << "Data: invalid reference!" << endl;
        exit(1);
    }
    return currPtr->data;
}

template<class T>
void LinkedList<T>::clear()    //清空链表
{
    Node<T> *currPosition, *nextPosition;
    currPosition = front;
    while (currPosition != NULL) {
        nextPosition = currPosition->nextNode(); //取得下一结点的地址
        freeNode(currPosition);    //删除当前结点
        currPosition = nextPosition;    //当前指针移动到下一结点
    }
    front = rear = NULL;
    prevPtr = currPtr = NULL;
    size = 0;
    position = -1;
}


#endif  //LINKEDLIST_H
