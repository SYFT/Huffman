#include <iostream>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
using namespace std;

const int SIZE = 1024 * 1024 * 8;

int main() {
    freopen("stud.dat", "w", stdout);
    srand(time(0));
    // for(int i = 26; i < 127; ++i) putchar(i);
    for(int i = 0; i < SIZE; ++i) {
        char c = rand() % 126 + 27;
        putchar(c);
    }
    fclose(stdout);
    return 0;
}

