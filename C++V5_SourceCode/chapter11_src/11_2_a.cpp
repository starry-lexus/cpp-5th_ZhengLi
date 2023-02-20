//11_2_a.cpp
#include<fstream>
#include <iomanip>
#include <string>

using namespace std;

int main() {
    ofstream txtout("out11_2.txt");
    double values[] = {1.23, 35.36, 653.7, 4358.24};
    string names[] = {"Zoot", "Jimmy", "Al", "Stan"};
    for (int i = 0; i < 4; i++)
        txtout << setw(6) << names[i] << setw(10) << values[i] << endl;
    txtout.close();
    return 0;
}
