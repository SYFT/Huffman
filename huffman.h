// #program once
#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdexcept>
#include <vector>
#include <queue>
using namespace std;

typedef pair<int, char> HuffmanChar;
// character and frequency
typedef enum {hit, miss, eof, wrong, success, unInit} HuffmanState;
// hit/miss: hit/miss a character   eof: end of file
// success: success finish function wrong: wrong data
typedef pair<HuffmanState, char> HuffmanResponse;

class wrongdata : exception {};

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
    };
    Node *root;
    bool isInit;

    Node *nowAt;

public:
    Huffman();

    HuffmanState init(const vector<HuffmanChar> freqAndChar);
    /**
     * freqAndChar: An array with frequency and character. 
     * numOfChar: A positive integer the number of character.
     * Construct a huffman tree by its character(ascii) and frequency.
     * */

    HuffmanResponse walk(const bool zeroOrOne);
    /**
     * zeroOrOne: Indicate the next step is zero or one.
     * Walk in the huffman tree with indication lr.
     * Response the situation of this huffman node.
     * */
};

#endif

