#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <queue>
#include <algorithm>
#include <sstream> 
using namespace std; 
//map(ºìºÚÊ÷) Ð¡¶¥¶Ñ huffmanÊ÷
struct HuffmanNode {
    int weight;
    unsigned char data;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(int w, unsigned char d)
        : weight(w), data(d), left(nullptr), right(nullptr) {
    }
    HuffmanNode(int w, HuffmanNode* l, HuffmanNode* r)
        : weight(w), data(0), left(l), right(r) {
    }
};

class NodeMinHeap {
public:
    NodeMinHeap() : _root(nullptr), _cap(0), _size(0) {}
    ~NodeMinHeap() { delete[] _root; }

    void Expand() {
        HuffmanNode** tmp = new HuffmanNode * [this->_cap];
        if (_root != nullptr) {
            for (int i = 0; i < this->_size; ++i) tmp[i] = this->_root[i];
            delete[] _root;
        }
        this->_root = tmp;
    }
    bool swap(HuffmanNode** a, HuffmanNode** b) {
        HuffmanNode* tmp = *a; *a = *b; *b = tmp; return true;
    }
    void adjustUp(int childIndex) {
        int parentIndex = (childIndex - 1) / 2;
        while (childIndex > 0 && this->_root[childIndex]->weight < this->_root[parentIndex]->weight) {
            swap(this->_root + childIndex, this->_root + parentIndex);
            childIndex = parentIndex;
            parentIndex = (childIndex - 1) / 2;
        }
    }
    bool adjustDown() {
        if (this->_size <= 1) return true;
        HuffmanNode** root = this->_root;
        int parent = 0; int leftChild = 1;
        while (leftChild < this->_size) {
            int smallerChild = leftChild;
            int rightChild = leftChild + 1;
            if (rightChild < this->_size && root[rightChild]->weight < root[leftChild]->weight) {
                smallerChild = rightChild;
            }
            if (root[parent]->weight <= root[smallerChild]->weight) break;
            swap(root + parent, root + smallerChild);
            parent = smallerChild;
            leftChild = 2 * parent + 1;
        }
        return true;
    }
    bool push(HuffmanNode* data) {
        if (this->_size == this->_cap) {
            this->_cap = (this->_cap == 0) ? 1 : this->_cap * 2;
            Expand();
        }
        this->_root[this->_size] = data; this->_size++;
        adjustUp(this->_size - 1); return true;
    }
    bool pop() {
        if (this->_size == 0) return false;
        swap(this->_root, this->_root + (this->_size - 1));
        this->_size--; adjustDown(); return true;
    }
    HuffmanNode* peek() { return (this->_size > 0) ? this->_root[0] : nullptr; }
    bool isEmpty() { return this->_size == 0; }
    int getSize() { return this->_size; }
private:
    HuffmanNode** _root; int _cap; int _size;
};

class HuffmanTree {
private:
    HuffmanNode* root;
    void deleteTree(HuffmanNode* node) {
        if (node) {
            deleteTree(node->left); deleteTree(node->right); delete node;
        }
    }
    void generateCodesHelper(HuffmanNode* node, string currentCode, map<unsigned char, string>& codes) {
        if (node == nullptr) return;
        if (!node->left && !node->right) {
            codes[node->data] = currentCode.empty() ? "0" : currentCode;
            return;
        }
        generateCodesHelper(node->left, currentCode + "0", codes);
        generateCodesHelper(node->right, currentCode + "1", codes);
    }
public:
    HuffmanTree(const map<unsigned char, int>& frequencies) {
        this->root = nullptr;
        NodeMinHeap minHeap;
        for (auto const& pair : frequencies) {
            if (pair.second > 0) {
                minHeap.push(new HuffmanNode(pair.second, pair.first));
            }
        }
        if (minHeap.getSize() == 0) return;
        while (minHeap.getSize() > 1) {
            HuffmanNode* left = minHeap.peek(); minHeap.pop();
            HuffmanNode* right = minHeap.peek(); minHeap.pop();
            int combinedWeight = left->weight + right->weight;
            HuffmanNode* parent = new HuffmanNode(combinedWeight, left, right);
            minHeap.push(parent);
        }
        this->root = minHeap.peek();
    }
    ~HuffmanTree() { deleteTree(this->root); }
    HuffmanNode* get_root() { return this->root; }
    map<unsigned char, string> getCodes() {
        map<unsigned char, string> codes;
        generateCodesHelper(this->root, "", codes);
        return codes;
    }
};

class HuffmanCompressor {
public:
    string compress(const string& inputData);
    string decompress(const string& inputData);
};