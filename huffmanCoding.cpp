#include "huffman.h"
#include "binaryIO.h"
#include <vector>
using namespace std;

void compress(string input, string output) {
    Huffman *huffman = new Huffman();
    ifstream in(input, ios::in);
    int cnt[256] = {0};
    string line;
    while(getline(in, line)) {
        int m = line.length();
        for(int i = 0; i < m; ++i) ++cnt[(int) line[i]];
    }
    vector<pair<int, char> > vec;
    int numChar = 0;
    for(int i = 0; i < 256; ++i)
        if(cnt[i]) {
            vec.push_back(make_pair(cnt[i], i));
            ++numChar;
        }
    ++numChar;
    vec.push_back(make_pair(1, 0));
    huffman->init(vec);

    Dumper *dumper = new Dumper(output);
    for(int i = 0; i < numChar - 1; ++i) {
        char c = vec[i].second;
        // not finish
        // write headers.
        dump.
    }
    in->seekg(0, ios::beg);
    while(getline(in, line)) {
        int m = line.length();
        for(int i = 0; i < m; ++i) {
            // write codes.
        }
    }

    delete huffman;
    delete dumper;
}

int main(int argc, char *argv[]) {
    ios::sync_with_stdio(false);
    compress("stud.dat", "stud.huf");
    decompress("stud.huf", "stud.out");
    return 0;
}

