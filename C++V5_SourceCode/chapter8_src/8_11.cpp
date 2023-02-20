#include <iostream>

using namespace std;

class Base {
public:
    virtual void f1(int) const;

    virtual ~Base() {
    };
};

void Base::f1(int) const {
    cout << "Base f1" << endl;
    return;
}

class Derived : public Base {
public:
    void f1(int);

    ~Derived() {
    };
};

void Derived::f1(int) {
    cout << "derived f1" << endl;
}

int main() {
    Base *b;
    b = new Base;
    b->f1(1);
    delete b;
    b = new Derived;
    b->f1(1);
    delete b;
    return 0;
}
