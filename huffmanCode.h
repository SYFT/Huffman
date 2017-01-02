#include "huffman.h"
#include "binaryIO.h"
#include <vector>
using namespace std;

namespace HuffmanCode {
    typedef enum {finished, wrong_file, wrong_data} HCState;

    HCState compress(string input, string output) {
        ifstream in(input.c_str(), ifstream::in);
        // Can not open file or other file wrong.
        if(in.fail()) return wrong_file;

        // Count frequency
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
        vec.push_back(make_pair(1, 0)); // insert a null char as eof.

        // build huffman tree.
        Huffman *huffman = new Huffman();
        huffman->init(vec);

        // write headers.
        Dumper<2> *dumper = new Dumper<2>(output);
        dumper->writeType((int) numChar - 1);
        for(int i = 0; i < numChar - 1; ++i) {
            dumper->writeType((char) vec[i].second);
            dumper->writeType((int) vec[i].first);
        }
        in.close();
        // Compress file
        in.open(input.c_str(), ifstream::in);
        while(!in.eof()) {
            char c = in.get();
            if(in.fail()) break;
            const char *code;
            HuffmanState s = huffman->query(c, code);
            if(s == wrong) {
                in.close();
                delete dumper;
                delete huffman;
                return wrong_data;
            }
            int l = strlen(code);
            for(int i = 0; i < l; ++i)
                dumper->writeBit(code[i] - '0');
        }
        const char *eofCode;
        HuffmanState s = huffman->query(0, eofCode);
        if(s == wrong) {
            in.close();
            delete dumper;
            delete huffman;
            return wrong_data;
        }
        int l = strlen(eofCode);
        for(int i = 0; i < l; ++i) dumper->writeBit(eofCode[i] - '0');

        in.close();
        delete dumper;
        delete huffman;

        return finished;
    }

    HCState decompress(string com, string decom) {
        int numChar;
        Loader<2>* loader = new Loader<2>(com);
        // Can not operate file
        if(loader->isBad()) {
            delete loader;
            return wrong_file;
        }

        IOState state;
        state = loader->readType(numChar);
        if(state == fail) return wrong_data;
        vector<pair<int, char> > vec;
        // read header
        vec.push_back(make_pair(1, 0)); // insert null char as eof.
        for(int i = 0; i < numChar; ++i) {
            char c;
            int freq;
            state = loader->readType(c);
            if(state == fail) {
                delete loader;
                return wrong_data;
            }
            state = loader->readType(freq);
            if(state == fail) {
                delete loader;
                return wrong_data;
            }
            vec.push_back(make_pair(freq, c));
        }

        // build huffman tree
        Huffman *huffman = new Huffman();
        huffman->init(vec);

        ofstream out(decom.c_str(), ofstream::out);

        // decompressing
        while(!loader->isBad()) {
            bool leftOrRight;
            state = loader->readBit(leftOrRight);
            if(state == fail) {
                delete loader;
                return wrong_data;
            }
            char c;
            HuffmanState wState = huffman->walk(leftOrRight, c);
            if(wState == eof) break;
            if(wState == wrong) {
                delete huffman;
                delete loader;
                return wrong_data;
            }
            if(wState == miss) continue;
            if(wState == hit) out << c;
        }

        out.close();
        delete huffman;
        delete loader;

        return finished;
    }
};

