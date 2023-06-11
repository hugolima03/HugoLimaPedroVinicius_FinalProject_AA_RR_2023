#include <fstream>
#include <iostream>
using namespace std;

#include "heap.h"

// ANSI color codes
const string RESET_COLOR = "\033[0m";
const string RED_COLOR = "\033[31m";
const string GREEN_COLOR = "\033[32m";

ofstream graphFile("graph.dot");

bool isForbiddenChar(char c) {
  // Define the forbidden characters
  string forbiddenChars = "!$*^@\"";

  // Check if the character is in the forbidden characters string
  return forbiddenChars.find(c) != string::npos;
}

string removeForbiddenChars(const string &input) {
  string result = input;

  // Use remove_if with a lambda function to remove forbidden characters
  result.erase(remove_if(result.begin(), result.end(), isForbiddenChar),
               result.end());

  return result;
}

void generateDotLanguage(Node *root) {
  string leftNode = to_string(root->left->freq);
  string rightNode = to_string(root->right->freq);
  if (root->left->ch) {
    leftNode = root->left->ch;
  }
  if (root->right->ch) {
    rightNode = root->right->ch;
  }
  string leftEdge = '"' + to_string(root->freq) + '"' + " -> " + '"' +
                    leftNode + '"' + "[ label=0 ]";
  string rightEdge = '"' + to_string(root->freq) + '"' + " -> " + '"' +
                     rightNode + '"' + "[ label=1 ]";
  graphFile << leftEdge << endl;
  graphFile << rightEdge << endl;
}