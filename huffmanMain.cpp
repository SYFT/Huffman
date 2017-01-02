#include "huffmanCode.h"
using namespace std;

int main(int argc, char *argv[]) {
    ios::sync_with_stdio(false);
    HuffmanCode::compress("stud.dat", "stud.huf");
    HuffmanCode::decompress("stud.huf", "stud.out");
    return 0;
}


