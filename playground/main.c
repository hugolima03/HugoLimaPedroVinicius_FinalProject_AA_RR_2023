#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N_ASCII 255
int elemetos[N_ASCII];
int n_elementos;

typedef struct Node {
  unsigned char caractere;
  int frequencia;
  struct Node* esq;
  struct Node* dir;
} node;

typedef struct filaP {
  int tamanhoHeap;
  node* vetor[N_ASCII];
} filaP;

filaP* iniciaFila() {
  filaP* f = malloc(sizeof(filaP));
  f->tamanhoHeap = 0;
  return f;
}

int parent(int i) { return (i - 1) / 2; }
int left(int i) { return (i * 2 + 1); }
int rigth(int i) { return (i * 2 + 2); }

node* criarNode(int caractere, int frequencia) {
  node* root = malloc(sizeof(node));
  root->caractere = (char)caractere;
  root->frequencia = frequencia;
  root->esq = root->dir = NULL;
  return root;
}

void MinHeapify(filaP* f, int pos) {
  int esq = left(pos);
  int dir = rigth(pos);
  int menor = pos;

  if (esq < f->tamanhoHeap &&
      f->vetor[esq]->frequencia < f->vetor[pos]->frequencia)
    menor = esq;

  if (dir < f->tamanhoHeap &&
      f->vetor[dir]->frequencia < f->vetor[menor]->frequencia)
    menor = dir;

  node* aux;
  if (menor != pos) {
    aux = f->vetor[pos];
    f->vetor[pos] = f->vetor[menor];
    f->vetor[menor] = aux;
    MinHeapify(f, menor);
  }
}

node* extraindoMinFila(filaP* f) {
  node* aux;
  aux = f->vetor[0];
  f->vetor[0] = f->vetor[f->tamanhoHeap - 1];
  f->tamanhoHeap--;

  MinHeapify(f, 0);  // falha aqui
  return aux;
}

unsigned int pegandoFrequencias(FILE* f, unsigned int vetor[]) {
  int r, i;
  for (i = 0;; i++) {
    r = fgetc(f);
    if (feof(f)) break;
    vetor[r]++;
  }
  return i;
}

void adicionarNaFila(filaP* f, node* root) {
  f->tamanhoHeap++;
  int pos = f->tamanhoHeap - 1;

  while ((pos > 0) && (f->vetor[parent(pos)]->frequencia > root->frequencia)) {
    f->vetor[pos] = f->vetor[parent(pos)];
    pos = parent(pos);
  }
  f->vetor[pos] = root;
}

node* criarHuffman(int vetorFrequencia[]) {
  filaP* f = iniciaFila();
  for (int i = 0; i < N_ASCII; i++) {
    if (vetorFrequencia[i]) {
      node* auxNode = criarNode(i, vetorFrequencia[i]);
      adicionarNaFila(f, auxNode);
    }
  }
  node *x, *y, *z;
  int n = f->tamanhoHeap - 1;
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

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Use: %s <arquivo.html>\n", argv[0]);
    exit(1);
  }
  FILE *f, *F_registro;
  unsigned int n;  // numero de bytes
  unsigned int vetorFrequencia[N_ASCII];
  char* codes[N_ASCII];  // vetor de codigos, um por caractere
  char code[N_ASCII];    // vetor para carregar um codigo

  memset(vetorFrequencia, 0, sizeof(vetorFrequencia));

  f = fopen(argv[1], "r");
  if (!f) {
    perror(argv[1]);
    exit(1);
  }
  n = pegandoFrequencias(f, vetorFrequencia);
  fclose(f);

  node* huff = criarHuffman(vetorFrequencia);
  n_elementos = 0;  // contagem de elemento do array master
  traverse(huff, 0, code, codes);

  F_registro = fopen("registro.txt", "w");
  fprintf(F_registro, "Key\tValor\n");

  int i, j;
  for (i = 0; i < n_elementos; i++) {
    j = elemetos[i];  // pega a numeração em  decimal para usar como CHAR
    fprintf(F_registro, "%c -> %s\n", (char)j, codes[j]);
  }
  fclose(F_registro);

  f = fopen(argv[1], "r");
  if (!f) {
    perror(argv[1]);
    exit(1);
  }

  char compress[1024];
  int r;
  for (i = 0; i < n; i++) {
    r = fgetc(f);
    // printf("r:%d\n",r);
    // printf("%s\n",codes[r]);
    printf("%s", codes[r]);
    if (feof(f)) break;
  }
  printf("\n");
}