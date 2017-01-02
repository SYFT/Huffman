#include "huffman.h"
#include "binaryIO.h"
#include <vector>
using namespace std;

void compress(string input, string output) {
    Huffman *huffman = new Huffman();
    ifstream in(input.c_str(), ifstream::in);
    int cnt[256] = {0};
    while(!in.eof()) {
        char c = in.get();
        if(in.fail()) break;
        ++cnt[(unsigned char) c];
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
    in.close();
    in.open(input.c_str(), ifstream::in);
    while(!in.eof()) {
        char c = in.get();
        if(in.fail()) break;
        const char *code;
        HuffmanState s = huffman->query(c, code);
        if(s == wrong) return;
        int l = strlen(code);
        for(int i = 0; i < l; ++i)
            dumper->writeBit(code[i] - '0');
    }
    const char *eofCode;
    HuffmanState s = huffman->query(0, eofCode);
    if(s == wrong) return;
    int l = strlen(eofCode);
    for(int i = 0; i < l; ++i) dumper->writeBit(eofCode[i] - '0');

    in.close();
    delete huffman;
    delete dumper;
}

void decompress(string com, string decom) {
    int numChar;
    Loader<2>* loader = new Loader<2>(com);
    IOState state;
    state = loader->readType(numChar);
    if(state == fail) return;
    vector<pair<int, char> > vec;
    vec.push_back(make_pair(1, 0));
    for(int i = 0; i < numChar; ++i) {
        char c;
        int freq;
        state = loader->readType(c);
        if(state == fail) return;
        state = loader->readType(freq);
        if(state == fail) return;
        vec.push_back(make_pair(freq, c));
    }
    Huffman *huffman = new Huffman();
    huffman->init(vec);

    ofstream out(decom.c_str(), ofstream::out);

    while(!loader->isBad()) {
        bool leftOrRight;
        state = loader->readBit(leftOrRight);
        if(state == fail) return;
        char c;
        HuffmanState wState = huffman->walk(leftOrRight, c);
        if(wState == eof) break;
        if(wState == wrong) return;
        if(wState == miss) continue;
        if(wState == hit) out << c;
    }

    out.close();
    delete huffman;
    delete loader;
}

int main(int argc, char *argv[]) {
    ios::sync_with_stdio(false);
    compress("stud.dat", "stud.huf");
    // decompress("stud.huf", "stud.out");
    return 0;
}

