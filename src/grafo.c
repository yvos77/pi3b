#include <stdlib.h>
#include "grafo.h"

Grafo *grafo_criar(int tipo, int n_usuarios, int n_produtos) {
    Grafo *g = malloc(sizeof(Grafo));

    if (g == NULL) {
        return NULL;
    }

    g->tipo = tipo;
    g->n_usuarios = n_usuarios;
    g->n_produtos = n_produtos;
    g->n = n_usuarios + n_produtos;
    g->m = 0;

    g->lista = NULL;
    g->matriz = NULL;

    if (tipo == LISTA) {
        g->lista = calloc(g->n, sizeof(No *));

        if (g->lista == NULL) {
            free(g);
            return NULL;
        }
    } else if (tipo == MATRIZ) {
        g->matriz = calloc((size_t)g->n * g->n, sizeof(char));

        if (g->matriz == NULL) {
            free(g);
            return NULL;
        }
    }

    g->rotulo = calloc(g->n, sizeof(int));

    if (g->rotulo == NULL) {
        free(g->lista);
        free(g->matriz);
        free(g);
        return NULL;
    }

    return g;
}

void grafo_add_aresta(Grafo *g, int u, int v) {
    if (g->tipo == LISTA) {
        No *novo_v = malloc(sizeof(No));

        if (novo_v == NULL) {
            return;
        }

        novo_v->v = v;
        novo_v->prox = g->lista[u];
        g->lista[u] = novo_v;

        No *novo_u = malloc(sizeof(No));

        if (novo_u == NULL) {
            g->lista[u] = novo_v->prox;
            free(novo_v);
            return;
        }

        novo_u->v = u;
        novo_u->prox = g->lista[v];
        g->lista[v] = novo_u;

        g->m++;
    } else if (g->tipo == MATRIZ) {
        g->matriz[(size_t)u * g->n + v] = 1;
        g->matriz[(size_t)v * g->n + u] = 1;

        g->m++;
    }
}

int grafo_tem_aresta(Grafo *g, int u, int v) {
    if (g->tipo == LISTA) {
        No *atual = g->lista[u];

        while (atual != NULL) {
            if (atual->v == v) {
                return 1;
            }

            atual = atual->prox;
        }
    } else if (g->tipo == MATRIZ) {
        return g->matriz[(size_t)u * g->n + v];
    }

    return 0;
}

int grafo_grau(Grafo *g, int u) {
    int grau = 0;

    if (g->tipo == LISTA) {
        No *atual = g->lista[u];

        while (atual != NULL) {
            grau++;
            atual = atual->prox;
        }
    } else if (g->tipo == MATRIZ) {
        for (int v = 0; v < g->n; v++) {
            if (g->matriz[(size_t)u * g->n + v]) {
                grau++;
            }
        }
    }

    return grau;
}

int grafo_vizinhos(Grafo *g, int u, int *buffer) {
    int quantidade = 0;

    if (g->tipo == LISTA) {
        No *atual = g->lista[u];

        while (atual != NULL) {
            buffer[quantidade] = atual->v;
            quantidade++;
            atual = atual->prox;
        }
    } else if (g->tipo == MATRIZ) {
        for (int v = 0; v < g->n; v++) {
            if (g->matriz[(size_t)u * g->n + v]) {
                buffer[quantidade] = v;
                quantidade++;
            }
        }
    }

    return quantidade;
}

int grafo_eh_usuario(Grafo *g, int v) {
    return v >= 0 && v < g->n_usuarios;
}

void grafo_liberar(Grafo *g) {
    if (g == NULL) {
        return;
    }

    if (g->tipo == LISTA) {
        for (int i = 0; i < g->n; i++) {
            No *atual = g->lista[i];

            while (atual != NULL) {
                No *proximo = atual->prox;
                free(atual);
                atual = proximo;
            }
        }

        free(g->lista);
    } else if (g->tipo == MATRIZ) {
        free(g->matriz);
    }

    free(g->rotulo);
    free(g);
}