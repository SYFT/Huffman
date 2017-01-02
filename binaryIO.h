// #program once
#ifndef BINARYIO_H
#define BINARYIO_H

#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

typedef unsigned char Block;
typedef enum {success, fail, interrupt} IOState;

template<int T>
class Dumper {
public:
    Block now[T];
    int bitIndex;
    ofstream out;
    bool bad;

    void reset() {
        int blockNum = (bitIndex - 1) / 8;
        if(out.good() && bitIndex) {
            out.write((char*) now, sizeof(Block) * (blockNum + 1));
            if(out.fail()) bad = true;
        }
        memset(now, 0, sizeof(now)), bitIndex = 0;
    }
    /**
     * Clear buffer.
     * */

public:
    Dumper(const string& filename) {
        out.open(filename.c_str(), ofstream::binary | ofstream::out);
        bad = false, bitIndex = 0;
        reset();
    }
    /**
     * Open a file named filename as output stream.
     * */

    bool isBad() const {
        return bad;
    }
    /**
     * Check wheather the loader is bad or not.
     * */

    IOState writeBit(bool nextBit) {
        if(isBad()) return fail;

        int blockIndex = bitIndex / 8;
        int reminder = bitIndex % 8;
        if((now[blockIndex] >> reminder) & 1)
            now[blockIndex] ^= 1 << reminder;
        now[blockIndex] |= (nextBit ? 1 : 0) << reminder;
        ++bitIndex;
        if(bitIndex >= T * 8) reset();
        return success;
    }
    /**
     * Write next bit as nextBit.
     * */

    template<class K>
    IOState writeType(K nex) {
        if(isBad()) return fail;

        // out.write((char*) &nex, sizeof(K));
        int bits = sizeof(K) * 8;
        for(int i = 0; i < bits; ++i) {
            bool b = (nex >> i) & 1;
            IOState s = writeBit(b);
            if(s == fail) return fail;
        }

        if(!out.good()) return fail;
        return success;
    }
    /**
     * Write a integer in binary.
     * K must be char, byte, int, short or other type represents in number.
     * */

    ~Dumper() {
        reset();
        out.flush();
        out.close();
    }
};

template<int T>
class Loader {
public:
    Block now[T];
    int bitIndex, bitLimit;
    ifstream in;
    bool bad;

    void reset() {
        memset(now, 0, sizeof(now)), bitIndex = 0;
        if(!isBad()) {
            in.read((char*) now, sizeof(Block) * T);
            bitLimit = in.gcount() * 8;
        } else bitLimit = 0;
    }
    /**
     * Load buffer.
     * */

public:
    Loader(const string& filename) {
        in.open(filename.c_str(), ifstream::in | ifstream::binary);
        bad = false;
        reset();
    }
    /**
     * Open a file named filename as input stream.
     * */

    bool isBad() {
        return bad;
    }
    /**
     * Check wheather the loader is bad or not.
     * */

    IOState readBit(bool& nextBit) {
        if(isBad()) return fail;

        int blockIndex = bitIndex / 8;
        int offset = bitIndex % 8;
        nextBit = (now[blockIndex] >> offset) & 1;

        ++bitIndex;
        if(bitIndex >= 8 * T) reset();
        else if(bitIndex >= bitLimit) {
            reset();
            bad = true;
        }

        return success;
    }
    /**
     * Set nextBit as ifstream next bit.
     * */

    template<class K>
    IOState readType(K &nex) {
        if(isBad()) return fail;

        // in.read((char*) &nex, sizeof(K));
        int bits = sizeof(K) * 8;
        nex = 0;
        for(int i = 0; i < bits; ++i) {
            bool b;
            IOState s = readBit(b);
            if(s == fail) return fail;
            nex |= b << i;
        }
        if(!in.good()) return fail;
        return success;
    }
    /**
     * Read binary bits as a K type.
     * K must be char, byte, int, short or other type represents in number.
     * */

    ~Loader() {
        in.close();
    }
};

#endif

