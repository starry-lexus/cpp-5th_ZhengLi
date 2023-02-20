//11_7_a.cpp
#include <iostream>
#include<fstream>

using namespace std;

int main() {
    ifstream txtin("11_7_a.cpp");
    char ch;
    while ((ch = txtin.get()) != EOF)
        cout.put(ch);
    txtin.close();
    return 0;
}
