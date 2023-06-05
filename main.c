#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "heap.h"

#define N_ASCII 255

int elemetos[N_ASCII];
int n_elementos;

int getFrequency(FILE* f, int vetor[]) {
  int r, i;
  for (i = 0;; i++) {
    r = fgetc(f);
    if (feof(f)) break;
    vetor[r]++;
  }
  return i;
}

node* criarHuffman(int vetorFrequencia[]) {
  Heap* f = iniciaFila();
  for (int i = 0; i < N_ASCII; i++) {
    if (vetorFrequencia[i]) {
      node* auxNode = criarNode(i, vetorFrequencia[i]);
      adicionarNaFila(f, auxNode);
    }
  }
  node *x, *y, *z;
  int n = f->size - 1;
  for (int i = 0; i < n; i++) {
    z = malloc(sizeof(node));
    x = extraindoMinFila(f);
    y = extraindoMinFila(f);
    z->esq = x;
    z->dir = y;
    z->frequencia = x->frequencia + y->frequencia;
    adicionarNaFila(f, z);
  }
  return extraindoMinFila(f);
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Erro: Caminho não especificado.");
    exit(1);
  }

  FILE* targetFile;
  int n;
  int vetorFrequencia[N_ASCII];
  char* codes[N_ASCII];  // vetor de codigos, um por caractere
  char code[N_ASCII];    // vetor para carregar um codigo

  memset(vetorFrequencia, 0, sizeof(vetorFrequencia));

  targetFile = fopen(argv[1], "r");
  if (!targetFile) {
    printf("Erro: Falha ao abrir arquivo.");
    exit(1);
  }

  n = getFrequency(targetFile, vetorFrequencia);
  fclose(targetFile);

  node* huff = criarHuffman(vetorFrequencia);
  n_elementos = 0;  // contagem de elemento do array master
  traverse(huff, 0, code, codes);

  int i, j;
  for (i = 0; i < n_elementos; i++) {
    j = elemetos[i];  // pega a numeração em  decimal para usar como CHAR
    printf("%c -> %s\n", (char)j, codes[j]);
  }
  printf("\n");
}