//11_8.cpp
#include <iostream>
#include <string>
#include<fstream>

using namespace std;

int main() {
    ifstream roomfile("meetingrm.txt");
    string line;
    cout << "Lines terminated by TAB " << endl;
    while (getline(roomfile, line, '\t'))
        cout << line << endl;
    roomfile.clear();
    roomfile.seekg(0, ios::beg);
    cout << "Lines terminated by neline " << endl;
    while (getline(roomfile, line))
        cout << line << endl;
    return 0;
}
