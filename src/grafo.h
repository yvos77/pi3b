#ifndef GRAFO_H
#define GRAFO_H

#define LISTA  0
#define MATRIZ 1

#define PADRAO        0
#define INTERMEDIARIO 1
#define NICHO         2

typedef struct No {
    int v;
    struct No *prox;
} No;

typedef struct {
    int    tipo;
    int    n;
    long   m;
    int    n_usuarios;
    int    n_produtos;
    No   **lista;
    char  *matriz;
    int   *rotulo;
} Grafo;

Grafo *grafo_criar(int tipo, int n_usuarios, int n_produtos);
void   grafo_liberar(Grafo *g);
void   grafo_add_aresta(Grafo *g, int u, int v);
int    grafo_tem_aresta(Grafo *g, int u, int v);
int    grafo_grau(Grafo *g, int u);
int    grafo_vizinhos(Grafo *g, int u, int *buffer);
long   grafo_memoria_bytes(Grafo *g);
int    grafo_eh_usuario(Grafo *g, int v);

#endif