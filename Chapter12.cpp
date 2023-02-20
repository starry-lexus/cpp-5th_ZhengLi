/**
 *@author 岛中客、眼中你
 *@date 2023/2/17
 */
#include <iostream>
#include <utility>

using namespace std;

class Exception {
public:
    Exception() = default;

    virtual ~Exception() = default;

    virtual void ErrorInfo() = 0;
};

class OutOfMemory : public Exception {
public:
    OutOfMemory() = default;

    ~OutOfMemory() override = default;

    void ErrorInfo() override;
};

void OutOfMemory::ErrorInfo() {
    cerr << "Failed to allocate memory!" << endl;
}

class RangError : public Exception {
public:
    explicit RangError(size_t number) : invalidNum(number) {}

    ~RangError() override = default;

    void ErrorInfo() override;

    virtual size_t getNumber() { return invalidNum; }

    virtual void setNum(size_t number) { invalidNum = number; }

private:
    size_t invalidNum;
};

void RangError::ErrorInfo() {
    cerr << "The number " << invalidNum << " you used is out of range!" << endl;
}

class SomeClass {
public:
    explicit SomeClass(string name) : str(std::move(name)) {
        cout << str << ": in constructor" << endl;
    }

    SomeClass(const SomeClass &) {
        cout << str << ": in copy constructor" << endl;
    }

    ~SomeClass() {
        cout << str << ": in destructor" << endl;
    }

    SomeClass &operator=(const SomeClass &) {
        cout << str << ": in assignment function" << endl;
        return *this;
    }

private:
    string str;
};

void test1();

size_t *test2();

void test3(size_t *);

int main() {
    /*12-1:
     * 1.异常是指函数在执行过程中出现的特殊状况或运行结果无法定义的情况,其会导致函数或程序异常终止
     * 2.异常处理是指用catch语句块捕获throw语句块抛出的用于说明异常情况的对象后,对此做出的恰当处理
     */

    /*12-2
     * C++的异常处理机制使得异常的引发和处理不必在同一个函数中,这样底层的函数可以着重解决具体问题,而不必过多考虑异常的处理.
     * 上层调用者可以针对不同的异常给出不同的处理措施.
     */

    /*12-3:
     * 1.throw语法为"throw 表达式;" 例如throw runtime_error("Invalid arguments");
     * 2.catch语句块用于捕获并处理try语句块中可能引发的异常
     * try{
     *      语句;         //可能引发多种异常
     *  }
     *  catch(参数声明)
     *  {
     *      语句;         //用于处理异常的程序
     *  }
     */

    //12-4:
    try {
        test1();
    } catch (Exception &e) {
        e.ErrorInfo();
    }

    //12-5: 根本就不可能触发异常,因为内存分配是在编译期确定的.数字太大,编译器会报错;数字不够大,编译器就会警告"条件始终为false".
    char *t;
    try {
        t = new char[999999999999999999];
        if (t == nullptr)
            throw "Failed to allocate!";
    } catch (char *address) {   //无法访问的代码
        cerr << "Exception: " << address << endl;
    }

    //12-6: 见Array.h

    //12-7: 显然队列的插入和删除这种涉及资源的操作是非异常安全的,见Queue.h

    /*12-8:
     * 1.本书声称加入了C++14标准,而auto_ptr早在C++11中就已经弃用了,C++17直接将其移除了,不知道作者的想法是什么.
     * 2.为什么要弃用auto_ptr? 这是因为①auto_ptr虽然采用copy语义来转移指针资源,但是它在转移指针所有权的同时却将
     *  原指针置为了nullptr,这就属于言行不一了,就好像在const函数内部用const_cast将常量语义给去除后修改了常对象的值。
     *  ②根据《Effective STL》的说法,如果对auto_ptr使用sort函数,那么排序后的结果会出现大量nullptr,而move语义
     *  就没有这个问题,这也是unique_ptr能够在STL中代替auto_ptr的原因；③auto_ptr会因为赋值语义不清晰而导致传入函数
     *  的拷贝资源丢失等
     * 3.C++11起使用的智能指针有三种,分别是unique_ptr、shared_ptr和weak_ptr
     *   unique_ptr"独占"所指向的对象,shared_ptr允许多个指针指向同一个对象,weak_ptr指向shared_ptr所管理的对象
     */
    shared_ptr<SomeClass> p1(new SomeClass("p1"));      //auto_ptr早就被弃用了
    unique_ptr<SomeClass> p2(new SomeClass("p2"));

    *p2 = *p1;

    {
        shared_ptr<SomeClass> p3(new SomeClass("p3"));
        p1 = p3;
    }

    return 0;
}

size_t *test2() {
    auto *n = new size_t;
    if (n == nullptr)
        throw OutOfMemory();
    return n;
}

void test1() {
    auto p = test2();
    test3(p);
    cout << "The number is " << *p << endl;
    delete p;
}

void test3(size_t *p) {
    unsigned short age;
    cout << "Enter your age(0~150), please:";
    cin >> age;

    if (age > 150)
        throw RangError(age);
    *p = age;
}