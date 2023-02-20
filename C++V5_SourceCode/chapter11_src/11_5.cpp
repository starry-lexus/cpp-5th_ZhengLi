//11_5.cpp
#include <fstream>

using namespace std;
struct Date {
    int mondy, day, year;
};

int main() {
    Date dt = {6, 10, 92};
    ofstream file("date.dat", ios_base::binary);
    file.write(reinterpret_cast<char *>(&dt), sizeof(dt));
    file.close();
    /*file.open("integer", ios_base::binary);
    int i=12;
    file.write(reinterpret_cast<char *>(&i), sizeof(int));
    int j=20;
    file.write(reinterpret_cast<char *>(&j), sizeof(int));
    int k=0;
    file.write(reinterpret_cast<char *>(&k), sizeof(int));
    file.close();*/
    return 0;
}
