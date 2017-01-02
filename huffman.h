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
typedef enum {hit, miss, eof, wrong, good, unInit} HuffmanState;
// hit/miss: hit/miss a character   eof: end of file
// good: good finish function wrong: wrong data
typedef pair<HuffmanState, char> HuffmanResponse;

class Huffman {

public:
    struct Node {
        int weight;
        Node *child[2];
        char value;

        Node(Node* left = NULL, Node* right = NULL, char value = (char) 0);

        bool isEnd() const;
        /**
         * Check wheather this node is an end node.
         */

        bool isEOF() const;
        /**
         * Check wheather this node means the EOF flag.
         */

        ~Node();
    };
    typedef pair<int, Node*> HeapNode;

    Node *root;
    bool isInit;

    Node *nowAt;

    char table[256][300];

    HuffmanState buildTable(const Node *x, char* buffer, int n, int m);

public:
    Huffman();

    HuffmanState init(const vector<HuffmanChar> freqAndChar);
    /**
     * freqAndChar: An array with frequency and character. 
     * Each character occurs once in vector.
     * numOfChar: A positive integer the number of character.
     * Construct a huffman tree by its character(ascii) and frequency.
     * Return good if normal, otherwise wrong.
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

    ~Huffman();

    struct cmp {
        bool operator ()(const HeapNode&, const HeapNode&);
    };
};

#endif

