#include <iostream>
using namespace std;

// A Tree node
struct Node {
  char ch;
  int freq;
  Node *left, *right;
};

// Comparison object to be used to order the heap
struct comp {
  bool operator()(Node *l, Node *r) {
    // highest priority item has lowest frequency
    return l->freq > r->freq;
  }
};

// Function to allocate a new tree node
Node *getNode(char ch, int freq, Node *left, Node *right) {
  Node *node = new Node();

  node->ch = ch;
  node->freq = freq;
  node->left = left;
  node->right = right;

  return node;
}