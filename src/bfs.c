#include "bfs.h"
#include "fila.h"
#include <stdlib.h>
#include <stdio.h>

#ifndef NICHO
#define NICHO 1
#endif

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

void saltos_ate_nicho(Grafo *g, const char *arquivo_saida) {
    int *origens_nicho = malloc(g->n * sizeof(int));
    int n_origens = 0;

    for (int v = 0; v < g->n; v++) {
        if (g->rotulo[v] == NICHO) {
            origens_nicho[n_origens++] = v;
        }
    }

    int *dist = malloc(g->n * sizeof(int));
    int *pai = malloc(g->n * sizeof(int));

    bfs(g, origens_nicho, n_origens, dist, pai);

    FILE *f = fopen(arquivo_saida, "w");
    if (f) {
        fprintf(f, "vertice,tipo,rotulo,distancia,saltos\n");
        for (int v = 0; v < g->n; v++) {
            int saltos = (dist[v] == INALCANCAVEL) ? -1 : dist[v] / 2;
            const char *tipo = (v < g->n_usuarios) ? "usuario" : "produto";
            fprintf(f, "%d,%s,%d,%d,%d\n", v, tipo, g->rotulo[v], dist[v], saltos);
        }
        fclose(f);
    }

    free(origens_nicho);
    free(dist);
    free(pai);
}

void imprimir_caminho(Grafo *g, int *pai, int destino) {
    if (pai[destino] == -1) {
        printf("Sem caminho para o vertice %d\n", destino);
        return;
    }

    int *caminho = malloc(g->n * sizeof(int));
    int tam = 0;
    int atual = destino;

    while (atual != -1) {
        caminho[tam++] = atual;
        atual = pai[atual];
    }

    printf("Caminho ate ao nicho:\n");
    for (int i = tam - 1; i >= 0; i--) {
        int v = caminho[i];
        const char *tipo = (v < g->n_usuarios) ? "[U]" : "[P]";
        printf(" %s Vertice %d\n", tipo, v);
        if (i > 0) printf("  -> ");
    }
    printf("= %d saltos\n", (tam - 1) / 2);

    free(caminho);
}

int contar_componentes(Grafo *g) {
    int *visitado = calloc(g->n, sizeof(int));
    int *dist = malloc(g->n * sizeof(int));
    int *pai = malloc(g->n * sizeof(int));
    int componentes = 0;

    for (int v = 0; v < g->n; v++) {
        if (!visitado[v]) {
            componentes++;
            int orig[] = {v};
            bfs(g, orig, 1, dist, pai);

            for (int i = 0; i < g->n; i++) {
                if (dist[i] != INALCANCAVEL) {
                    visitado[i] = 1;
                }
            }
        }
    }

    free(visitado);
    free(dist);
    free(pai);
    return componentes;
}