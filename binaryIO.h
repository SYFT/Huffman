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

    void reset();
    /**
     * Clear buffer.
     * */

    ~Dumper();

public:
    Dumper(ostream& outputStream);
    /**
     * Use this ostream object as output stream.
     * */

    Dumper(const string& filename);
    /**
     * Open a file named filename as output stream.
     * */

    bool isBad() const;
    /**
     * Check wheather the loader is bad or not.
     * */

    IOState writeBit(bool nextBit);
    /**
     * Write next bit as nextBit.
     * */

    IOState writeInt(int nextInt);
    /**
     * Write a integer in binary.
     * */
};

template<int T>
class Loader {
private:
    Block now[T];
    int bitIndex, bitLimit;
    istream* in;
    bool bad;

    void reset();
    /**
     * Load buffer.
     * */

    ~Loader();

public:
    Loader(istream& inputStream);
    /**
     * Use this istream object as input stream.
     * */

    Loader(const string& filename);
    /**
     * Open a file named filename as input stream.
     * */

    bool isBad();
    /**
     * Check wheather the loader is bad or not.
     * */

    IOState readBit(bool& nextBit);
    /**
     * Set nextBit as istream next bit.
     * */

    IOState readInt(int &nextInt);
    /**
     * Read binary bits as an int.
     * */
};

#endif

