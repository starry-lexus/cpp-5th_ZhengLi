//11_4.cpp
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

int main() {
    double values[] = {1.23, 35.36, 653.7, 4358.24};
    string names[] = {"Zoot", "Jimmy", "Al", "Stan"};
    cout //<< setiosflags(ios_base::fixed)
            //<< fixed
            << setprecision(6)
            << 3466.9768 << endl;
    cout//<< setiosflags(ios_base::scientific)
            << scientific
            << setprecision(6)
            << dec << 3466.9768 << endl;

    for (int i = 0; i < 4; i++)
        cout << setiosflags(ios_base::left)
             << setw(6) << names[i]
             << resetiosflags(ios_base::left)
             << setw(10) << setprecision(1) << values[i] << endl;
    return 0;
}
