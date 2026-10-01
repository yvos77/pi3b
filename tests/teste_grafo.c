#include <stdio.h>
#include "grafo.h"

int main(void) {
    Grafo *g = grafo_criar(LISTA, 3, 3);

    grafo_add_aresta(g, 0, 3);
    grafo_add_aresta(g, 0, 4);
    grafo_add_aresta(g, 1, 3);

    printf("%d %d %d\n",
           grafo_grau(g, 0),
           grafo_grau(g, 3),
           grafo_tem_aresta(g, 1, 4));

    grafo_liberar(g);

    return 0;
}