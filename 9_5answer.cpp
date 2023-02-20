/**
 *@author 岛中客、眼中你
 *@date 2023/2/1
 */
#include "LinkedList.h"
#include <iostream>

using namespace std;

int main() {
    //作者设计的链表类很不实用,不符合人的直观感受,过于繁琐
    LinkedList<int> a, b;
    cout << "Enter 5 integers for LinkedList a:";
    for (int i = 0; i < 5; i++) {
        int item;
        cin >> item;
        a.insertRear(item);
    }
    cout << "Enter 5 integers for LinkedList b:";
    for (int i = 0; i < 5; i++) {
        int item;
        cin >> item;
        b.insertRear(item);
    }

    //将b中的元素加入a的尾部
    b.reset();      //重置b的游标
    for (int i = 0; i < 5; i++) {
        a.insertRear(b.data());
        b.next();   //将b的游标移动到下一个结点
    }

    //输出结果
    cout << "a + b:" << endl;
    a.reset();
    for (int i = 0; i < 10; i++) {
        cout << a.data() << " ";
        a.next();
    }
    cout << endl;
    return 0;
}