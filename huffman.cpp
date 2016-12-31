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

Huffman::Huffman() {
    root = nowAt = NULL, isInit = false;
}

HuffmanState Huffman::init(const vector<HuffmanChar> w) {
    int n = w.size();
    if(n < 1) return wrong;

    Node** nodes = NULL;

    try {
        nodes = new Node* [n];
        for(int i = 0; i < n; ++i) {
            nodes[i] = new Node();
            nodes[i]->value = w[i].second;
        }
    }
    catch(exception) {
        return wrong;
    }

    typedef pair<int, Huffman::Node*> HeapNode;;
    priority_queue<HeapNode> heap;
    for(int i = 0; i < n; ++i)
        heap.push(make_pair(-w[i].first, nodes[i]));

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

HuffmanResponse Huffman::walk(const bool lr) {
    if(!isInit) return make_pair(unInit, 0);
    if(nowAt == NULL) nowAt = root;
    if(nowAt->child[lr] == NULL) return make_pair(wrong, 0);
    nowAt = nowAt->child[lr];
    if(nowAt->isEOF()) {
        nowAt = NULL;
        return make_pair(eof, 0);
    }
    if(nowAt->isEnd()) {
        char character = nowAt->value;
        nowAt = root;
        return make_pair(hit, character);
    }
    return make_pair(miss, 0);
}

