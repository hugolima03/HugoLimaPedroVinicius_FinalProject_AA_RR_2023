// g++ -std=c++11 main.cpp -o main && ./main ./tests/basic.html
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <queue>
using namespace std;

#include "utils.h"

// traverse the Huffman Tree and store Huffman Codes
// in a map.
void encode(Node *root, string str, unordered_map<char, string> &huffmanCode) {
  if (root == nullptr) return;

  // found a leaf node
  if (!root->left && !root->right) {
    huffmanCode[root->ch] = str;
  } else {
    // GERANDO DOT LANGUAGE
    generateDotLanguage(root);
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

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cout << "Digite o caminho para o arquivo HTML." << endl;
    return 1;
  }

  ifstream targetFile(argv[1]);
  string text;
  if (targetFile.is_open()) {
    char ch;
    while (targetFile.get(ch)) {
      text += ch;
    }
    targetFile.close();
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

  // Cria fila de prioridade para os nós ativos da árvore de Huffman
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

  // traverse the Huffman Tree and store Huffman Codes
  // in a map. Also prints them
  graphFile << "digraph G {" << endl;
  unordered_map<char, string> huffmanCode;
  encode(root, "", huffmanCode);
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

      int originalSize = printOriginalSize(userInput);
      int compactedSize =
          printCompactedSize(freq, huffmanCode, userInput, encodedInput);

      printDiff(compactedSize, originalSize);
    } else {
      break;
    }
  }

  return 0;
}