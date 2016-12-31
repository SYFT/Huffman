#include "huffman.h"

Huffman::Node::Node(
        Huffman::Node* left,
        Huffman::Node* right,
        char _value) {
    value = _value, child[0] = left, child[1] = right;
}

bool Huffman::Node::isEnd() {
    return !(child[0] || child[1]);
}

bool Huffman::Node::isEOF() {
    return isEnd() && value == 0;
}

Huffman::Node::~Node() {
    if(child[0]) delete child[0];
    if(child[1]) delete child[1];
    child[0] = child[1] = NULL;
}

Huffman::Huffman() {
    root = nowAt = NULL, isInit = false;
    memset(table, 0, sizeof(table));
}

Huffman::~Huffman() {
    delete root;
    nowAt = root = NULL;
    isInit = false;
}

bool Huffman::cmp(const HeapNode& a, const HeapNode& b) {
    if(a.first != b.first) return a.first > b.first;
    return a.second->value > b.second->value;
}

HuffmanState Huffman::init(const vector<HuffmanChar> w) {
    int n = w.size();
    if(n < 1) return wrong;

    Node** nodes = NULL;

    try {
        nodes = new Node* [n];
        // The memory will free by huffman destructor.
        for(int i = 0; i < n; ++i) {
            nodes[i] = new Node();
            nodes[i]->value = w[i].second;
        }
    }
    catch(exception) {
        return wrong;
    }

    typedef bool (*comp)(const HeapNode&, const HeapNode&);
    priority_queue<HeapNode, vector<HeapNode>, comp> heap;
    for(int i = 0; i < n; ++i)
        heap.push(make_pair(w[i].first, nodes[i]));

    while((int) heap.size() > 1) {
        HeapNode left = heap.top();
        heap.pop();
        HeapNode right = heap.top();
        heap.pop();
        Node *father = new Node(left.second, right.second);
        heap.push(make_pair(left.first + right.first, father));
    }
    root = heap.top().second;

    isInit = true;
    return success;
}

HuffmanState Huffman::walk(const bool lr, char &ret) {
    ret = 0;
    if(!isInit) return unInit;
    if(nowAt == NULL) nowAt = root;
    if(nowAt->child[lr] == NULL) return wrong;
    nowAt = nowAt->child[lr];
    if(nowAt->isEOF()) {
        nowAt = NULL;
        return eof;
    }
    if(nowAt->isEnd()) {
        ret = nowAt->value;
        nowAt = root;
        return hit;
    }
    return miss;
}

HuffmanState Huffman::query(char character, const char*& ret) const {
    ret = NULL;
    if(!strlen(table[(int) character])) return wrong;
    ret = table[(int) character];
    return success;
}

