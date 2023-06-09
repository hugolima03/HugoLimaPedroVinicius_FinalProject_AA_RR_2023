// g++ -std=c++11 main.cpp -o main && ./main
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <queue>
using namespace std;

#include "heap.h"

// ANSI color codes
const string RESET_COLOR = "\033[0m";
const string RED_COLOR = "\033[31m";
const string GREEN_COLOR = "\033[32m";

ofstream graphFile("graph.dot");

// traverse the Huffman Tree and store Huffman Codes
// in a map.
void encode(Node *root, string str, unordered_map<char, string> &huffmanCode) {
  if (root == nullptr) return;

  // found a leaf node
  if (!root->left && !root->right) {
    huffmanCode[root->ch] = str;
  } else {
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

  encode(root->left, str + "0", huffmanCode);
  encode(root->right, str + "1", huffmanCode);
}

// traverse the Huffman Tree and decode the encoded string
void decode(Node *root, int &index, string str) {
  if (root == nullptr) {
    return;
  }

  // found a leaf node
  if (!root->left && !root->right) {
    cout << root->ch;
    return;
  }

  index++;

  if (str[index] == '0')
    decode(root->left, index, str);
  else
    decode(root->right, index, str);
}

bool isForbiddenChar(char c) {
  // Define the forbidden characters
  std::string forbiddenChars = "!$*^@\"";

  // Check if the character is in the forbidden characters string
  return forbiddenChars.find(c) != std::string::npos;
}

string removeForbiddenChars(const string &input) {
  string result = input;

  // Use remove_if with a lambda function to remove forbidden characters
  result.erase(remove_if(result.begin(), result.end(), isForbiddenChar),
               result.end());

  return result;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cout << "Digite o caminho para o arquivo HTML." << endl;
    return 1;
  }

  ifstream file(argv[1]);
  string text;
  if (file.is_open()) {
    char ch;
    while (file.get(ch)) {
      text += ch;
    }
    file.close();
  } else {
    cout << "Falha ao abrir o arquivo." << endl;
    return 1;
  }
  text = removeForbiddenChars(text);
  // conta a frequência de aparição de cada personagem
  // e armazena em um map(objeto)
  unordered_map<char, int> freq;
  for (char ch : text) {
    freq[ch]++;
  }

  // Crie uma fila de prioridade para armazenar os nós ativos da árvore de
  // Huffman
  priority_queue<Node *, vector<Node *>, comp> pq;

  // Crie um nó folha para cada letra e adiciona na fila de prioridade.
  for (auto pair : freq) {
    pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
  }

  while (pq.size() != 1) {
    // Remove os dois nós de menor frequência da fila
    Node *left = pq.top();
    pq.pop();
    Node *right = pq.top();
    pq.pop();

    // Cria um novo nó interno com esses dois nós
    // como filhos e com frequência igual à soma
    // das frequências dos dois nós. Adicione o novo nó
    // para a fila de prioridade.
    int sum = left->freq + right->freq;
    pq.push(getNode('\0', sum, left, right));
  }

  // raiz da Huffman Tree
  Node *root = pq.top();

  if (!graphFile) {
    cerr << "Erro criando arquivo." << endl;
    return 1;
  }
  // Generate the graphviz dot graph
  graphFile << "digraph G {" << endl;
  // traverse the Huffman Tree and store Huffman Codes
  // in a map. Also prints them
  unordered_map<char, string> huffmanCode;
  encode(root, "", huffmanCode);
  //
  graphFile << "}" << endl;
  graphFile.close();
  string command = "dot -Tpng graph.dot -o graph.png && rm graph.dot";
  int result = system(command.c_str());
  if (result != 0) {
    cerr << "Erro ao gerar a imagem do grafo." << endl;
    return 1;
  }

  cout << "Tabela de tradução:\n" << '\n';
  for (auto pair : huffmanCode) {
    cout << pair.first << " " << pair.second << '\n';
  }

  string userInput;

  while (true) {
    cout << "Digite uma nova sentença(exit para sair): ";
    cin >> userInput;

    string encodedInput = "";
    if (userInput != "exit") {
      for (char ch : userInput) {
        bool notExistsInTable = (huffmanCode.find(ch) == huffmanCode.end());
        if (notExistsInTable) {
          throw invalid_argument(
              "Caracter fornecido não existe na tabela de tradução");
        }
        encodedInput += huffmanCode[ch];
      }

      int compactedSize = 0;
      for (auto pair : huffmanCode) {
        compactedSize = compactedSize + 8;
        compactedSize = compactedSize + freq[pair.first] * pair.second.length();
      }
      compactedSize = compactedSize + userInput.length();
      int originalSize = userInput.size() * sizeof(char) * 8;
      cout << "Texto original(" << originalSize << " bits): " << userInput
           << '\n';
      cout << "Texto codificado(" << compactedSize << " bits): " << encodedInput
           << '\n';
      float reduction =
          100 - ((static_cast<double>(compactedSize) / originalSize) * 100);
      if (reduction > 0) {
        cout << GREEN_COLOR << "Redução de " << reduction << "%" << RESET_COLOR
             << '\n';
      } else {
        cout << RED_COLOR << "Aumento de " << abs(reduction) << "%"
             << RESET_COLOR << '\n';
      }
      cout << '\n';
    } else {
      break;
    }
  }

  return 0;
}