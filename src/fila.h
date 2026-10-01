#ifndef FILA_H
#define FILA_H

typedef struct {
    int *dados;
    int  capacidade;
    int  inicio;
    int  fim;
    int  tamanho;
} Fila;

Fila *fila_criar(int capacidade);
void  fila_liberar(Fila *f);
void  fila_inserir(Fila *f, int v);
int   fila_remover(Fila *f);
int   fila_vazia(Fila *f);

#endif