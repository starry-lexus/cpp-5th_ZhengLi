//11_1_a.cpp
#include<fstream>

using namespace std;

int main() {
    double values[] = {1.23, 35.36, 653.7, 4358.24};
    ofstream txtout("out11_1.txt");

    for (int i = 0; i < 4; i++) {
        txtout.width(10);
        txtout << values[i] << endl;
    }
    txtout.close();
    return 0;
}
