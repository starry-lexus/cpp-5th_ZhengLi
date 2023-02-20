//8_5_2.cpp
#include <iostream>

using namespace std;

class Base {
public:
    virtual ~Base();
};

Base::~Base() {
    cout << "Base destructor" << endl;
}

class Derived : public Base {
public:
    Derived();

    virtual ~Derived();

private:
    int *p;
};

Derived::Derived() {
    p = new int(0);
}

Derived::~Derived() {
    cout << "Derived destructor" << endl;
    delete p;
}

int main() {
    Base *basePtrs[10];
    for (int i = 0; i < 10; i++) {
        basePtrs[i] = new Derived();
    }
    for (int i = 0; i < 10; i++) {
        delete basePtrs[i];
    }
    return 0;
}
