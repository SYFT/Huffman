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
private:
    Block now[T];
    int bitIndex;
    ostream* out;
    bool bad;

    void reset() {
        int blockNum = bitIndex / 8;
        out->write((char*) now, sizeof(Block) * blockNum);
        memset(now, 0, sizeof(now)), bitIndex = 0;
    }
    /**
     * Clear buffer.
     * */

public:
    Dumper(ostream& outputStream) {
        out = &outputStream;
        bad = false;
        reset();
    }
    /**
     * Use this ostream object as output stream.
     * */

    Dumper(const string& filename) {
        out = new ofstream(filename.c_str(), ios::binary | ios::out);
        bad = false;
        reset();
    }
    /**
     * Open a file named filename as output stream.
     * */

    bool isBad() const {
        return bad || !out->good();
    }
    /**
     * Check wheather the loader is bad or not.
     * */

    IOState writeBit(bool nextBit) {
        if(isBad()) return fail;

        int blockIndex = bitIndex / 8;
        int reminder = bitIndex % 8;
        now[blockIndex] |= 1 << reminder;
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

        bool bits[32];
        int len = sizeof(K) * 8;
        for(int i = 0; i < len; ++i, nex >>= 1)
            bits[i] = nex & 1;
        for(int i = len - 1; i >= 0; --i) {
            IOState s = writeBit(bits[i]);
            if(s == fail) return fail;
        }
        return success;
    }
    /**
     * Write a integer in binary.
     * K must be char, byte, int, short or other type represents in number.
     * */

    ~Dumper() {
        reset();
        out->flush();
        delete out;
        out = NULL;
    }
};

template<int T>
class Loader {
private:
    Block now[T];
    int bitIndex, bitLimit;
    istream* in;
    bool bad;

    void reset() {
        bitLimit = in->read((char*) now, sizeof(Block) * T);
        bitLimit *= 8;
        memset(now, 0, sizeof(now)), bitIndex = 0;
    }
    /**
     * Load buffer.
     * */

public:
    Loader(istream& inputStream) {
        in = &inputStream;
        reset();
    }
    /**
     * Use this istream object as input stream.
     * */

    Loader(const string& filename) {
        in = new ifstream(filename.c_str(), ios::in | ios::binary);
        reset();
    }
    /**
     * Open a file named filename as input stream.
     * */

    bool isBad() {
        if(bad) return true;
        if(in->bad() || in->fail()) return true;
        return false;
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
     * Set nextBit as istream next bit.
     * */

    template<class K>
    IOState readType(K &nex) {
        if(isBad()) return fail;

        nex = (K) 0;
        for(int i = sizeof(nex) * 8; i >= 1; --i) {
            bool x;
            IOState s = readBit(x);
            if(s == fail) return fail;
            nex = (nex << 1) | x;
        }

        return success;
    }
    /**
     * Read binary bits as a K type.
     * K must be char, byte, int, short or other type represents in number.
     * */

    ~Loader() {
        delete in;
        in = NULL;
    }
};

#endif

