#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"
#include "bfs.h"

int main() {
    Grafo *g = grafo_criar(LISTA, 3, 3);
    
    g->rotulo[5] = 1; 

    grafo_add_aresta(g, 0, 3);
    grafo_add_aresta(g, 1, 3);
    grafo_add_aresta(g, 1, 5);

    int n_comp = contar_componentes(g);
    printf("Componentes encontradas: %d\n", n_comp);

    int dist[6], pai[6];
    int origens[] = {5};
    bfs(g, origens, 1, dist, pai);
    imprimir_caminho(g, pai, 0);

    return 0;
}