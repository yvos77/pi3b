#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"
#include "bfs.h"

int main() {
    Grafo *g = grafo_criar(LISTA, 3, 3);
    grafo_add_aresta(g, 0, 3);
    grafo_add_aresta(g, 1, 3);
    grafo_add_aresta(g, 1, 4);

    int dist[6];
    int pai[6];
    int origens[] = {0};

    bfs(g, origens, 1, dist, pai);

    printf("dist[0]=%d (esperado: 0)\n", dist[0]);
    printf("dist[3]=%d (esperado: 1)\n", dist[3]);
    printf("dist[1]=%d (esperado: 2)\n", dist[1]);
    printf("dist[4]=%d (esperado: 3)\n", dist[4]);
    printf("dist[2]=%d (esperado: -1)\n", dist[2]);
    printf("dist[5]=%d (esperado: -1)\n", dist[5]);

    // Limpeza (opcional para o teste, mas boa prática)
    // grafo_liberar(g);
    return 0;
}