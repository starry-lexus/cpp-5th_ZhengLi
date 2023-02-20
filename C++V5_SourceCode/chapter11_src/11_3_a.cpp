//11_3_a.cpp
#include<fstream>
#include <iomanip>
#include <string>

using namespace std;

int main() {
    ofstream txtout("out11_3.txt");
    double values[] = {1.23, 35.36, 653.7, 4358.24};
    string names[] = {"Zoot", "Jimmy", "Al", "Stan"};
    for (int i = 0; i < 4; i++)
        txtout << setiosflags(ios_base::left)
               << setw(6) << names[i]
               << resetiosflags(ios_base::left)
               << setw(10) << values[i] << endl;
    txtout.close();
    return 0;
}
