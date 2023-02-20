/**
 * @author 岛中客、眼中你
 * @date 2022/12/28
 */
#include <iostream>

using namespace std;

short int f3_7(unsigned short a, unsigned short b);

double transferFtoC(double F);

bool isPrime(unsigned int n);

int GCD(int a, int b);

int LCM(int a, int b);

int sum(int n);

int Fibonacci(int n);

int Legendre(int x, int n);

int getPower(int x, int y);

double getPower(double x, int y);

int main() {
    /*3-1:
     * C++中的函数是功能抽象的基本单位，主调函数是调用其他函数的函数，被调函数是被其他函数调用的函数，它们之间的关系是调用与被调用的关系。
     * 调用函数的方法：函数名(实参列表)
     */

    /*3-2:
     * rSomeRef是对intOne的引用(别名)，两者同步变化。所以，当intOne = 5时，rSomeRef = 5；
     * 当rSomeRef = intTwo时，intOne = intTwo
     */

    /*3-3:
     * 相同点:都是用实参初始化形参
     * 不同点:①在引用传递中，形参的初始化发生在执行主调函数的调用表达式时；而在值传递中，形参的初始化在类型说明时进行
     *       ②在引用传递中，形参和实参同步变化；而在值传递中，形参的变化不会引起实参的变化
     * */

    //3-4: 内联函数是在编译时将函数体嵌入到每一个调用处的函数，其特点是功能简单、规模较小又使用频繁

    //3-5: 函数原型、函数定义和函数调用中的参数都是局部变量，作用域分别为各自的函数体，所以它们的参数名不必一致

    //3-6: 调用被重载的函数时，编译器是通过形参的个数和类型来匹配函数的

    //3-7:
    cout << f3_7(256, 2) << endl;

    //3-8:
    int F;
    cout << "Enter a Fahrenheit please:";
    cin >> F;
    cout << F << "℉" << " is equal to " << transferFtoC(F) << "℃" << endl;

    //3-9:
    unsigned int n;
    cout << "Enter a number please:";
    cin >> n;
    if (isPrime(n))
        cout << n << " is a prime" << endl;
    else
        cout << n << " is not a prime" << endl;

    //3-10
    int a, b;
    cout << "Enter two integers please:";
    cin >> a >> b;
    cout << "The GCD of " << a << " and " << b << " is " << GCD(a, b) << ", and the LCM is " << LCM(a, b) << endl;

    //3-11:嵌套调用是指函数1调用了函数2，函数2又调用了函数3，则这三个函数之间形成了嵌套调用。递归是指函数直接或间接调用自身

    //3-12:
    int num;
    cout << "Enter a positive integers please:";
    cin >> num;
    cout << "1 + ... + " << num << " = " << sum(num) << endl;

    //3-13
    int fi;
    cout << "Enter a positive integers please:";
    cin >> fi;
    cout << "Fibonacci(" << fi << ") = " << Fibonacci(fi) << endl;

    //3-14:
    int x, order;
    cout << "Enter the arguments x and n for Legendre's polynomial in order:";
    cin >> x >> order;
    cout << "p(x" << x << ") of order " << order << " is " << Legendre(x, order) << endl;

    //3-15:
    int ai, m;
    double bi;
    cout << "Please enter an integer a and a real number b as the base, and an int m as the exponent:";
    cin >> ai >> bi >> m;
    cout << ai << "^" << m << " = " << getPower(ai, m) << "\t";
    cout << bi << "^" << m << " = " << getPower(bi, m) << endl;

    //3-16:利用函数调用栈保存信息
    return 0;
}

short int f3_7(unsigned short a, unsigned short b) {
    if (b == 0) return -1;
    else return a / b;
}

double transferFtoC(double F) {
    return 5.0 / 9 * (F - 32);
}

bool isPrime(unsigned int n) {
    //0和1既不是素数也不是合数
    if (n == 0 || n == 1) return false;

    for (int i = 2; i <= n / 2; i++) {
        if (n % i == 0) return false;
    }

    return true;
}

int GCD(int a, int b) {  //求最大公约数的方法很多，主要有质因数分解法、短除法、辗转相除法和更相减损法等
    //辗转相除法
    if (b % a == 0) return a;
    while (a % b) {
        int r = a % b;
        a = b;
        b = r;
    }
    return b;
}

int LCM(int a, int b) {  //求最大公倍数
    //a和b最大公约数与最小公倍数之积等于a*b
    return a * b / GCD(a, b);
}

int sum(int n) { //写法不唯一
    //这里假定用户会按照提示输入正整数，故不判断n的取值范围；若要判断，则将下列方法稍加改造即可
    /*方法一:利用与运算的短路特性
    int result = n;
    result && (result += sum(n - 1));
    return result;
    */

    //方法二:利用三目运算符
    return n == 1 ? 1 : (n + sum(n - 1));

    /*方法三: 常规思路
    if(n == 1)
        return 1;
    else
        return n + sum(n - 1);
    */
}

int Fibonacci(int n) {
    if (n < 1) {
        cerr << "\nError: Non-positive integer" << endl;
        return -1;
    } else if (n == 1 || n == 2)
        return 1;
    else
        return Fibonacci(n - 1) + Fibonacci(n - 2);
}

int Legendre(int x, int n) {
    if (n == 0)
        return 1;
    else if (n == 1)
        return x;
    else
        return ((2 * n - 1) * x * Legendre(x, n - 1) - (n - 1) * Legendre(x, n - 2)) / n;
}

int getPower(int x, int y) {
    if (y < 0)
        return 0;
    else if (y == 0)
        return 1;
    else
        return x * getPower(x, y - 1);
}

double getPower(double x, int y) {
    double result = x;
    int t = abs(y);
    if (t == 0)
        return 1;
    else
        result *= getPower(x, t - 1);
    return y > 0 ? result : 1.0 / result;
}