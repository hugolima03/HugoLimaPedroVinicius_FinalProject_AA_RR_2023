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

int printCompactedSize(unordered_map<char, int> freq,
                       unordered_map<char, std::__1::string> huffmanCode,
                       string userInput, string encodedInput) {
  int compactedSize = 0;
  for (auto pair : huffmanCode) {
    compactedSize = compactedSize + 8;
    compactedSize = compactedSize + freq[pair.first] * pair.second.length();
  }
  cout << "Texto codificado(" << compactedSize << " bits): " << encodedInput
       << '\n';
  return compactedSize + userInput.length();
}

int printOriginalSize(string userInput) {
  int originalSize = userInput.size() * sizeof(char) * 8;
  cout << "Texto original(" << originalSize << " bits): " << userInput << '\n';
  return originalSize;
}

void printDiff(int compactedSize, int originalSize) {
  float diff =
      100 - ((static_cast<double>(compactedSize) / originalSize) * 100);
  if (diff > 0) {
    cout << GREEN_COLOR << "Redução de " << diff << "%" << RESET_COLOR << '\n';
  } else {
    cout << RED_COLOR << "Aumento de " << abs(diff) << "%" << RESET_COLOR
         << '\n';
  }
  cout << '\n';
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