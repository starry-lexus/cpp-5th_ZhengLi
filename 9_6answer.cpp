/**
 *@author 岛中客、眼中你
 *@date 2023/2/1
 */
#include "OrderList.h"
#include <iostream>

using namespace std;

int main() {
    OrderList<int> a, b;
    cout << "Enter 5 integers for OrderList a:";
    for (int i = 0; i < 5; i++) {
        int item;
        cin >> item;
        a.insert(item);
    }
    cout << "Enter 5 integers for OrderList b:";
    for (int i = 0; i < 5; i++) {
        int item;
        cin >> item;
        b.insert(item);
    }

    //将b中的元素插入a中
    b.reset();      //重置b的游标
    for (int i = 0; i < 5; i++) {
        a.insert(b.data());
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
