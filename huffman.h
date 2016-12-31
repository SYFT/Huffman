// #program once
#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdexcept>
#include <vector>
#include <queue>
#include <cstring>
using namespace std;

typedef pair<int, char> HuffmanChar;
// character and frequency
typedef enum {hit, miss, eof, wrong, success, unInit} HuffmanState;
// hit/miss: hit/miss a character   eof: end of file
// success: success finish function wrong: wrong data
typedef pair<HuffmanState, char> HuffmanResponse;

class Huffman {

private:
    struct Node {
        int weight;
        Node *child[2];
        char value;

        Node(Node* left = NULL, Node* right = NULL, char value = (char) 0);

        bool isEnd();
        /**
         * Check wheather this node is an end node.
         */

        bool isEOF();
        /**
         * Check wheather this node means the EOF flag.
         */

        ~Node();
    };
    Node *root;
    bool isInit;

    Node *nowAt;

    char table[256][300];

    ~Huffman();

    typedef pair<int, Node*> HeapNode;
    bool cmp(const HeapNode&, const HeapNode&);

public:
    Huffman();

    HuffmanState init(const vector<HuffmanChar> freqAndChar);
    /**
     * freqAndChar: An array with frequency and character. 
     * Each character occurs once in vector.
     * numOfChar: A positive integer the number of character.
     * Construct a huffman tree by its character(ascii) and frequency.
     * Return success if normal, otherwise wrong.
     * */

    HuffmanState walk(const bool zeroOrOne, char &resultCharacter);
    /**
     * zeroOrOne: Indicate the next step is zero or one.
     * resultCharacter: If hit a character, it will store in it.
     * Walk in the huffman tree with indication lr.
     * Response the situation of this huffman node.
     * hit for hit a character
     * wrong for wrong input data (this step encode nothing).
     * eof for end of file.
     * miss for other.
     * */

    HuffmanState query(char character, const char*& huffmanCode) const;
    /**
     * character: query the huffman code correspond to it.
     * huffmanCode: character pointer point to huffmancode.
     * If not this character encrypt, return wrong flag.
     * */
};

#endif

