#include <iostream>

using namespace std;

int main() {
    //2-10:在函数声明中给出默认形参，在定义中以注释说明默认形参，这是一个好习惯
    int i = 0, j = 0, n;
    cout << "Enter some integers please (enter 0 to quit):" << endl;
    cin >> n;
    while (n != 0) {
        if (n > 0) i++;
        else j++;
        cin >> n;
    }
    cout << "Count of positive integers: " << i << endl;
    cout << "Count of negative integers: " << j << endl;


    return 0;
}
