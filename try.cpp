#include <fstream>
#include <iostream>
using namespace std;
struct student
{
    char name[20];
    int num;
    int age;
    char sex;
};
int main( )
{
    ofstream out("stud.dat",ios::binary | ios::out);
    bool ch[2] = {true, false};
    cout << sizeof(bool) << endl;
    out.write((char *) ch, sizeof(bool) * 2);
    out.close();
    return 0;
}

