#include <stdlib.h>
#include "fila.h"

Fila *fila_criar(int capacidade) {
    Fila *f = (Fila *)malloc(sizeof(Fila));
    f->dados = (int *)malloc(capacidade * sizeof(int));
    f->capacidade = capacidade;
    f->inicio = 0;
    f->fim = 0;
    f->tamanho = 0;
    return f;
}

void fila_liberar(Fila *f) {
    free(f->dados);
    free(f);
}

void fila_inserir(Fila *f, int v) {
    f->dados[f->fim] = v;
    f->fim = (f->fim + 1) % f->capacidade;
    f->tamanho++;
}

int fila_remover(Fila *f) {
    int v = f->dados[f->inicio];
    f->inicio = (f->inicio + 1) % f->capacidade;
    f->tamanho--;
    return v;
}

int fila_vazia(Fila *f) {
    return f->tamanho == 0;
}