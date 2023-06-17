// Huffman Coding in C++

#include <iostream>

#include "heap.h"
#include "utils.h"

using namespace std;

void encode(MinHNode *root, string str,
            unordered_map<char, string> &huffmanCode) {
  if (root == nullptr) return;

  // encontrou nó folha
  if (!root->left && !root->right) {
    huffmanCode[root->item] = str;
  } else {
  }

  encode(root->left, str + "0", huffmanCode);
  encode(root->right, str + "1", huffmanCode);
}

struct MinH *createAndBuildMinHeap(unordered_map<char, int> freq, int size) {
  struct MinH *minHeap = createMinH(size);

  int i = 0;
  for (auto pair : freq) {
    minHeap->array[i] = newNode(pair.first, pair.second);
    i++;
  }

  minHeap->size = size;
  buildMinHeap(minHeap);

  return minHeap;
}

struct MinHNode *buildHfTree(unordered_map<char, int> freq, int size) {
  struct MinHNode *left, *right, *top;
  struct MinH *minHeap = createAndBuildMinHeap(freq, size);

  graphFile << "digraph G {" << endl;
  while (!checkSizeOne(minHeap)) {
    left = extractMin(minHeap);
    right = extractMin(minHeap);

    top = newNode('$', left->freq + right->freq);
    top->left = left;
    top->right = right;

    generateDotFileEdges(top);
    insertMinHeap(minHeap, top);
  }
  graphFile << "}" << endl;
  graphFile.close();

  string command = "dot -Tpng graph.dot -o graph.png";
  int result = system(command.c_str());
  if (result != 0) {
    cerr << "Erro ao gerar a imagem do grafo." << endl;
  }

  return extractMin(minHeap);
}

int main(int argc, char *argv[]) {
  clock_t start, end;

  if (argc < 2) {
    cout << "Digite o caminho para o arquivo HTML." << endl;
    return 1;
  }
  string text = getTextFromFile(argv[1]);

  // conta a frequência de aparição de cada personagem
  // e armazena em um map(objeto)
  unordered_map<char, int> freq;
  for (char ch : text) {
    freq[ch]++;
  }

  int size = freq.size();
  start = clock();
  struct MinHNode *root = buildHfTree(freq, size);
  end = clock();
  // Calculating total time taken by the program.
  double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
  cout << "Time taken by program is : " << fixed << time_taken
       << setprecision(5);
  cout << " sec " << endl;

  unordered_map<char, string> tabelaTraducao;
  encode(root, "", tabelaTraducao);

  printTranslationTable(root, freq, tabelaTraducao);

  string userInput = text;

  string encodedInput = "";
  for (char ch : userInput) {
    bool notExistsInTable = (tabelaTraducao.find(ch) == tabelaTraducao.end());
    if (notExistsInTable) {
      throw invalid_argument(
          "Caracter fornecido não existe na tabela de tradução");
    }
    encodedInput += tabelaTraducao[ch];
  }
  int originalSize = printOriginalSize(userInput);
  int compactedSize = printCompactedSize(encodedInput);

  printDiff(compactedSize, originalSize);
}