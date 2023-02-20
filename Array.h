//Array.h
#ifndef ARRAY_H
#define ARRAY_H

#include <stdexcept>

//数组类模板定义
template<class T>
class Array {
private:
    T *list;    //T类型指针，用于存放动态分配的数组内存首地址
    int size;    //数组大小（元素个数）
public:
    explicit Array(int sz = 50);            //构造函数
    Array(const Array<T> &a);                //拷贝构造函数
    ~Array();    //析构函数
    Array<T> &operator=(const Array<T> &rhs); //重载"="使数组对象可以整体赋值
    T &operator[](int n);    //重载"[]"，使Array对象可以起到C++普通数组的作用
    const T &operator[](int n) const;    //"[]"运算符的const版本
    explicit operator T *();    //重载到T*类型的转换，使Array对象可以起到C++普通数组的作用
    explicit operator const T *() const;    //到T*类型转换操作符的const版本
    int getSize() const;    //取数组的大小
    void resize(int sz);    //修改数组的大小
    void swap(Array &right);
};

template<class T>
void Array<T>::swap(Array<T> &right) {
    std::swap(list, right.list);        //调用std::swap分别交换两个成员
    std::swap(size, right.size);
}

namespace std {  //特化std::swap
    template<typename T>
    inline void swap(Array <T> &right, Array <T> &left) {
        left.swap(right);
    }
}

//构造函数
template<class T>
Array<T>::Array(int sz) {
    if (sz >= 0) {
        size = sz;              // 将元素个数赋值给变量size
        list = new T[size];    //动态分配size个T类型的元素空间
    } else
        throw std::invalid_argument("The array length must be a positive integer!");
}

//析构函数
template<class T>
Array<T>::~Array() {
    delete[] list;
}

//拷贝构造函数
template<class T>
Array<T>::Array(const Array<T> &a) {
    //从对象x取得数组大小，并赋值给当前对象的成员
    size = a.size;
    //为对象申请内存并进行出错检查
    list = new T[size];    // 动态分配n个T类型的元素空间
    //从对象X复制数组元素到本对象
    for (int i = 0; i < size; i++)
        list[i] = a.list[i];
}

//重载"="运算符，将对象rhs赋值给本对象。实现对象之间的整体赋值
template<class T>
Array<T> &Array<T>::operator=(const Array<T> &rhs) {
    if (&rhs != this) {
        //如果本对象中数组大小与rhs不同，则删除数组原有内存，然后重新分配
        if (size != rhs.size) {
            delete[] list;        //删除数组原有内存
            size = rhs.size;    //设置本对象的数组大小
            list = new T[size];    //重新分配n个元素的内存
        }
        //从对象X复制数组元素到本对象
        for (int i = 0; i < size; i++)
            list[i] = rhs.list[i];
    }
    return *this;    //返回当前对象的引用
}

//重载下标运算符，实现与普通数组一样通过下标访问元素，并且具有越界检查功能
template<class T>
T &Array<T>::operator[](int n) {
    if (n >= 0 && n < size)
        return list[n];            //返回下标为n的数组元素
    else
        throw std::out_of_range("Invalid index");
}

template<class T>
const T &Array<T>::operator[](int n) const {
    if (n >= 0 && n < size)
        return list[n];            //返回下标为n的数组元素
    else
        throw std::out_of_range("Invalid index");
}

//重载指针转换运算符，将Array类的对象名转换为T类型的指针，
//指向当前对象中的私有数组。
//因而可以象使用普通数组首地址一样使用Array类的对象名
template<class T>
Array<T>::operator T *() {
    return list;    //返回当前对象中私有数组的首地址
}

template<class T>
Array<T>::operator const T *() const {
    return list;    //返回当前对象中私有数组的首地址
}

//取当前数组的大小
template<class T>
int Array<T>::getSize() const {
    return size;
}

// 将数组大小修改为sz
template<class T>
void Array<T>::resize(int sz) {
    if (sz < 0)
        throw std::invalid_argument("The array length must be a positive integer!");
    if (sz == size)    //如果指定的大小与原有大小一样，什么也不做
        return;
    T *newList = new T[sz];    //申请新的数组内存
    int n = (sz < size) ? sz : size;    //将sz与size中较小的一个赋值给n
    //将原有数组中前n个元素复制到新数组中
    for (int i = 0; i < n; i++)
        newList[i] = list[i];
    delete[] list;        //删除原数组
    list = newList;    // 使list指向新数组
    size = sz;    //更新size
}

#endif  //ARRAY_H
