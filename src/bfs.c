#include "bfs.h"
#include "fila.h"
#include <stdlib.h>

void bfs(Grafo *g, int *origens, int n_origens, int *dist, int *pai) {
    Fila *fila = fila_criar(g->n);
    int *buffer = malloc(g->n * sizeof(int));

    // Inicialização
    for (int v = 0; v < g->n; v++) {
        dist[v] = INALCANCAVEL;
        pai[v] = -1;
    }

    // Múltiplas origens
    for (int i = 0; i < n_origens; i++) {
        int o = origens[i];
        dist[o] = 0;
        fila_inserir(fila, o);
    }

    // Busca
    while (!fila_vazia(fila)) {
        int u = fila_remover(fila);
        int k = grafo_vizinhos(g, u, buffer);
        
        for (int i = 0; i < k; i++) {
            int w = buffer[i];
            if (dist[w] == INALCANCAVEL) {
                dist[w] = dist[u] + 1;
                pai[w] = u;
                fila_inserir(fila, w);
            }
        }
    }

    free(buffer);
    fila_liberar(fila);
}