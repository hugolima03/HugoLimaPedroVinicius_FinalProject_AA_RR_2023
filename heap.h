#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N_ASCII 255

int elemetos[N_ASCII];
int n_elementos;

typedef struct Node {
  unsigned char caractere;
  int frequencia;
  struct Node *esq;
  struct Node *dir;
} node;

typedef struct Heap {
  int size;
  node *vetor[N_ASCII];
} Heap;

Heap *iniciaFila() {
  Heap *f = malloc(sizeof(Heap));
  f->size = 0;
  return f;
}

int parent(int i) { return (i - 1) / 2; }
int left(int i) { return (i * 2 + 1); }
int rigth(int i) { return (i * 2 + 2); }

node *criarNode(int caractere, int frequencia) {
  node *root = malloc(sizeof(node));
  root->caractere = (char)caractere;
  root->frequencia = frequencia;
  root->esq = root->dir = NULL;
  return root;
}

void MinHeapify(Heap *f, int pos) {
  int esq = left(pos);
  int dir = rigth(pos);
  int menor = pos;

  if (esq < f->size && f->vetor[esq]->frequencia < f->vetor[pos]->frequencia)
    menor = esq;

  if (dir < f->size && f->vetor[dir]->frequencia < f->vetor[menor]->frequencia)
    menor = dir;

  node *aux;
  if (menor != pos) {
    aux = f->vetor[pos];
    f->vetor[pos] = f->vetor[menor];
    f->vetor[menor] = aux;
    MinHeapify(f, menor);
  }
}

node *extraindoMinFila(Heap *f) {
  node *aux;
  aux = f->vetor[0];
  f->vetor[0] = f->vetor[f->size - 1];
  f->size--;

  MinHeapify(f, 0);
  return aux;
}

void adicionarNaFila(Heap *f, node *root) {
  f->size++;
  int pos = f->size - 1;

  while ((pos > 0) && (f->vetor[parent(pos)]->frequencia > root->frequencia)) {
    f->vetor[pos] = f->vetor[parent(pos)];
    pos = parent(pos);
  }
  f->vetor[pos] = root;
}

void traverse(node* root, int level, char code_so_far[], char* codes[]) {
  // Se estiver em nodo folha
  if ((root->esq == NULL) && (root->dir == NULL)) {
    code_so_far[level] = 0;
    codes[root->caractere] = strdup(code_so_far);
    elemetos[n_elementos++] = (int)root->caractere;
    // printf("letras: %d\n",(int)root->caractere);
  } else {
    // caso nao seja nodo folha, ir para esquerda com bit 0
    code_so_far[level] = '0';
    traverse(root->esq, level + 1, code_so_far, codes);
    // ir para direita com bit 1
    code_so_far[level] = '1';
    traverse(root->dir, level + 1, code_so_far, codes);
  }
}