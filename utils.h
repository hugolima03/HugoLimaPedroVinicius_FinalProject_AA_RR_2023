#include <fstream>
#include <iostream>

using namespace std;

ofstream graphFile("graph.dot");

// ANSI color codes
const string RESET_COLOR = "\033[0m";
const string RED_COLOR = "\033[31m";
const string GREEN_COLOR = "\033[32m";

void printArray(int arr[], int n) {
  int i;
  for (i = 0; i < n; ++i) cout << arr[i];

  cout << "\n";
}

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

string getTextFromFile(string filepath) {
  ifstream targetFile(filepath);
  string text;
  if (targetFile.is_open()) {
    char ch;
    while (targetFile.get(ch)) {
      text += ch;
    }
    targetFile.close();
  } else {
    cerr << "Falha ao abrir o arquivo." << endl;
  }
  // text = removeForbiddenChars(text);
  return text;
}

void printHCodes(struct MinHNode *root, int arr[], int top) {
  if (root->left) {
    arr[top] = 0;
    printHCodes(root->left, arr, top + 1);
  }

  if (root->right) {
    arr[top] = 1;
    printHCodes(root->right, arr, top + 1);
  }
  if (isLeaf(root)) {
    cout << root->item << "  | ";
    printArray(arr, top);
  }
}

void generateDotFileEdges(MinHNode *top) {
  char leftItem = top->left->item;
  char rightItem = top->right->item;
  string leftFreq = to_string(top->left->freq);
  string rightFreq = to_string(top->right->freq);

  if (leftItem == '$') {
    graphFile << '"' + to_string(top->freq) + '"' + " -> " + '"' + leftFreq +
                     '"' + "[ label=0 ]"
              << endl;
  } else {
    graphFile << '"' + to_string(top->freq) + '"' + " -> " + '"' + leftItem +
                     '"' + "[ label=0 ]"
              << endl;
  }

  if (rightItem == '$') {
    graphFile << '"' + to_string(top->freq) + '"' + " -> " + '"' + rightFreq +
                     '"' + "[ label=1 ]"
              << endl;
  } else {
    graphFile << '"' + to_string(top->freq) + '"' + " -> " + '"' + rightItem +
                     '"' + "[ label=1 ]"
              << endl;
  }
}

int printCompactedSize(string encodedInput) {
  int compactedSize = encodedInput.length();
  cout << "Texto codificado(" << compactedSize << " bits): " << encodedInput
       << '\n';
  return compactedSize;
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

void printTranslationTable(MinHNode *root, unordered_map<char, int> freq,
                           unordered_map<char, string> tabelaTraducao) {
  int compactedSize = 0;
  for (auto pair : tabelaTraducao) {
    compactedSize = compactedSize + 8;
    compactedSize = compactedSize + freq[pair.first];
  }

  int arr[MAX_TREE_HT], top = 0;
  cout << "Tabela de tradução(" << compactedSize << " bits):\n";
  printHCodes(root, arr, top);
  cout << endl;
}