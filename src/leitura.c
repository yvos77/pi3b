#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leitura.h"

Grafo *ler_dataset(const char *pasta, int tipo) {
    char caminho[256];

    int n_usuarios;
    int n_produtos;
    long n_arestas;

    sprintf(caminho, "%s/meta.txt", pasta);

    FILE *f = fopen(caminho, "r");

    if (f == NULL) {
        return NULL;
    }

    if (fscanf(f, "%d %d %ld", &n_usuarios, &n_produtos, &n_arestas) != 3) {
        fclose(f);
        return NULL;
    }

    fclose(f);

    Grafo *g = grafo_criar(tipo, n_usuarios, n_produtos);

    if (g == NULL) {
        return NULL;
    }

    sprintf(caminho, "%s/produtos.csv", pasta);

    f = fopen(caminho, "r");

    if (f == NULL) {
        grafo_liberar(g);
        return NULL;
    }

    char linha[256];

    if (fgets(linha, sizeof(linha), f) == NULL) {
        fclose(f);
        grafo_liberar(g);
        return NULL;
    }

    int indice;
    int grau;
    int rotulo;
    char nome[128];
    char categoria[64];
    float score;

    while (fscanf(f,
                  "%d,%127[^,],%63[^,],%d,%f,%d\n",
                  &indice,
                  nome,
                  categoria,
                  &grau,
                  &score,
                  &rotulo) == 6) {

        if (indice >= 0 && indice < g->n) {
            g->rotulo[indice] = rotulo;
        }
    }

    fclose(f);

    sprintf(caminho, "%s/compras.csv", pasta);

    f = fopen(caminho, "r");

    if (f == NULL) {
        grafo_liberar(g);
        return NULL;
    }

    if (fgets(linha, sizeof(linha), f) == NULL) {
        fclose(f);
        grafo_liberar(g);
        return NULL;
    }

    int usuario;
    int produto;

    while (fscanf(f, "%d,%d\n", &usuario, &produto) == 2) {
        grafo_add_aresta(g, usuario, produto);
    }

    fclose(f);

    return g;
}

void imprimir_resumo(Grafo *g) {
    if (g == NULL) {
        return;
    }

    int quantidade_nicho = 0;
    int quantidade_padrao = 0;

    for (int i = g->n_usuarios; i < g->n; i++) {
        if (g->rotulo[i] == NICHO) {
            quantidade_nicho++;
        }

        if (g->rotulo[i] == PADRAO) {
            quantidade_padrao++;
        }
    }

    printf("vertices: %d (%d usuarios, %d produtos)\n",
           g->n,
           g->n_usuarios,
           g->n_produtos);

    printf("arestas:  %ld\n", g->m);

    printf("nicho: %d   padrao: %d\n",
           quantidade_nicho,
           quantidade_padrao);

    long memoria = grafo_memoria_bytes(g);

    if (g->tipo == LISTA) {
        printf("memoria: %ld KB (lista)\n", memoria / 1024);
    } else if (g->tipo == MATRIZ) {
        printf("memoria: %ld KB (matriz)\n", memoria / 1024);
    }
}