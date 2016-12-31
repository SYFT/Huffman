#include "binaryIO.h"

template<int T>
void Dumper<T>::reset() {
    int blockNum = bitIndex / 8;
    out->write((char*) now, sizeof(Block) * blockNum);
    memset(now, 0, sizeof(now)), bitIndex = 0;
}

template<int T>
Dumper<T>::Dumper(ostream &_out) {
    out = &_out;
    bad = false;
    reset();
}

template<int T>
Dumper<T>::Dumper(const string& filename) {
    out = new ofstream(filename.c_str(), ios::binary | ios::out);
    bad = false;
    reset();
}

template<int T>
bool Dumper<T>::isBad() const {
    return bad || !out->good();
}

template<int T>
IOState Dumper<T>::writeBit(bool nextBit) {
    if(isBad()) return fail;

    int blockIndex = bitIndex / 8;
    int reminder = bitIndex % 8;
    now[blockIndex] |= 1 << reminder;
    ++bitIndex;
    if(bitIndex >= T * 8) reset();
    return success;
}

template<int T>
IOState Dumper<T>::writeInt(int nextInt) {
    if(isBad()) return fail;

    bool bits[32];
    for(int i = 0, x = nextInt; i < 32; ++i, x >>= 1)
        bits[i] = x & 1;
    for(int i = 31; i >= 0; --i) {
        IOState s = writeBit(bits[i]);
        if(s == fail) return fail;
    }
    return success;
}

template<int T>
Dumper<T>::~Dumper() {
    reset();
    out->flush();
    delete out;
    out = NULL;
}

template<int T>
void Loader<T>::reset() {
    bitLimit = in->read((char*) now, sizeof(Block) * T);
    bitLimit *= 8;
    memset(now, 0, sizeof(now)), bitIndex = 0;
}

template<int T>
Loader<T>::~Loader() {
    delete in;
    in = NULL;
}

template<int T>
Loader<T>::Loader(istream& _in) {
    in = &_in;
    reset();
}

template<int T>
Loader<T>::Loader(const string& filename) {
    in = new ifstream(filename.c_str(), ios::in | ios::binary);
    reset();
}

template<int T>
bool Loader<T>::isBad() {
    if(bad) return true;
    if(in->bad() || in->fail()) return true;
    return false;
}

template<int T>
IOState Loader<T>::readBit(bool& nextBit) {
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

template<int T>
IOState Loader<T>::readInt(int& nextInt) {
    if(isBad()) return fail;

    nextInt = 0;
    for(int i = 31; i >= 0; --i) {
        bool x;
        IOState s = readBit(x);
        if(s == fail) return fail;
        nextInt = (nextInt << 1) | x;
    }

    return success;
}

