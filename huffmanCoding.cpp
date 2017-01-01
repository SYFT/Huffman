#include "huffman.h"
#include "binaryIO.h"
#include <vector>
using namespace std;

void compress(string input, string output) {
    Huffman *huffman = new Huffman();
    ifstream in(input.c_str(), ios::in);
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

    Dumper<2> *dumper = new Dumper<2>(output);
    dumper->writeType((int) numChar - 1);
    for(int i = 0; i < numChar - 1; ++i) {
        dumper->writeType((char) vec[i].second);
        dumper->writeType((int) vec[i].first);
    }
    in.seekg(0, ios::beg);
    while(getline(in, line)) {
        int m = line.length();
        for(int i = 0; i < m; ++i) {
            const char *code;
            HuffmanState s = huffman->query(line[i], code);
            if(s == wrong) {
                return;
            }
            int l = strlen(code);
            for(int j = 0; j < l; ++j)
                dumper->writeBit(code[j] - '0');
        }
    }

    in.close();

    delete huffman;
    delete dumper;
}

void decompress(string com, string decom) {
}

int main(int argc, char *argv[]) {
    ios::sync_with_stdio(false);
    compress("stud.dat", "stud.huf");
    decompress("stud.huf", "stud.out");
    return 0;
}

