#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"
#include "leitura.h"
#include "bfs.h"
#include "bipartido.h"
#include "tempo.h"

static void uso(const char *prog) {
    printf("uso: %s <pasta_dados> <lista|matriz> <algoritmo>\n", prog);
    printf("algoritmos: resumo | bipartido | nicho | componentes\n\n");
    printf("exemplo: %s dados lista nicho\n", prog);
}

static void executar_bipartido(Grafo *g) {
    int *cor = malloc((size_t)g->n * sizeof(int));

    if (!cor) {
        printf("erro: memoria insuficiente\n");
        return;
    }

    int u = -1, v = -1;

    if (testar_bipartido(g, cor, &u, &v)) {
        int divergencias = validar_particao(g, cor);

        printf("GRAFO BIPARTIDO\n");
        printf("particao: %d usuarios, %d produtos, %d divergencias\n",
               g->n_usuarios, g->n_produtos, divergencias);
    } else {
        printf("GRAFO NAO BIPARTIDO\n");
        printf("aresta em conflito: %d -- %d (mesma cor)\n", u, v);
    }

    free(cor);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        uso(argv[0]);
        return 1;
    }

    int tipo = strcmp(argv[2], "matriz") == 0 ? MATRIZ : LISTA;

    Grafo *g = ler_dataset(argv[1], tipo);

    if (!g) {
        printf("erro ao ler o dataset em %s\n", argv[1]);
        return 1;
    }

    double t0 = agora_ms();

    if (strcmp(argv[3], "resumo") == 0) {
        imprimir_resumo(g);

    } else if (strcmp(argv[3], "bipartido") == 0) {
        executar_bipartido(g);

    } else if (strcmp(argv[3], "nicho") == 0) {
        saltos_ate_nicho(g, "resultados/saltos.csv");

    } else if (strcmp(argv[3], "componentes") == 0) {
        printf("componentes conexos: %d\n", contar_componentes(g));

    } else {
        printf("algoritmo desconhecido: %s\n\n", argv[3]);
        uso(argv[0]);
        grafo_liberar(g);
        return 1;
    }

    double tempo = agora_ms() - t0;

    registrar("resultados/tempos.csv", argv[3], argv[2],
              g->n, g->m, tempo, grafo_memoria_bytes(g));

    printf("\ntempo: %.3f ms   memoria: %ld bytes\n",
           tempo, grafo_memoria_bytes(g));

    grafo_liberar(g);

    return 0;
}