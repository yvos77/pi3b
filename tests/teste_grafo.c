#include <stdio.h>
#include "grafo.h"

static void testar_lista_basica(void) {
    Grafo *g = grafo_criar(LISTA, 3, 3);

    grafo_add_aresta(g, 0, 3);
    grafo_add_aresta(g, 0, 4);
    grafo_add_aresta(g, 1, 3);

    printf("Lista basica: %d %d %d\n",
           grafo_grau(g, 0),
           grafo_grau(g, 3),
           grafo_tem_aresta(g, 1, 4));

    grafo_liberar(g);
}

static void testar_matriz_basica(void) {
    Grafo *g = grafo_criar(MATRIZ, 3, 3);

    grafo_add_aresta(g, 0, 3);
    grafo_add_aresta(g, 0, 4);
    grafo_add_aresta(g, 1, 3);

    printf("Matriz basica: %d %d %d\n",
           grafo_grau(g, 0),
           grafo_grau(g, 3),
           grafo_tem_aresta(g, 1, 4));

    grafo_liberar(g);
}

static void testar_grafo_vazio(void) {
    Grafo *g = grafo_criar(LISTA, 2, 2);

    printf("Grafo vazio: arestas=%ld grau0=%d\n",
           g->m,
           grafo_grau(g, 0));

    grafo_liberar(g);
}

static void testar_vertice_isolado(void) {
    Grafo *g = grafo_criar(LISTA, 3, 2);

    grafo_add_aresta(g, 0, 3);

    printf("Vertice isolado: grau2=%d\n",
           grafo_grau(g, 2));

    grafo_liberar(g);
}

static void testar_aresta_repetida(void) {
    Grafo *g = grafo_criar(LISTA, 2, 2);

    grafo_add_aresta(g, 0, 2);
    grafo_add_aresta(g, 0, 2);

    printf("Aresta repetida: arestas=%ld grau0=%d\n",
           g->m,
           grafo_grau(g, 0));

    grafo_liberar(g);
}

static void testar_n_igual_1(void) {
    Grafo *g = grafo_criar(LISTA, 1, 0);

    printf("n=1: vertices=%d grau0=%d usuario0=%d\n",
           g->n,
           grafo_grau(g, 0),
           grafo_eh_usuario(g, 0));

    grafo_liberar(g);
}

int main(void) {
    testar_lista_basica();
    testar_matriz_basica();
    testar_grafo_vazio();
    testar_vertice_isolado();
    testar_aresta_repetida();
    testar_n_igual_1();

    return 0;
}