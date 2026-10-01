#include "bipartido.h"
#include "fila.h"
#include <stdlib.h>
#include <stdio.h>

int testar_bipartido(Grafo *g, int *cor, int *u_conflito, int *v_conflito) {
    Fila *fila = fila_criar(g->n);
    int *buffer = malloc(g->n * sizeof(int));

    for (int v = 0; v < g->n; v++) {
        cor[v] = -1;
    }

    for (int v = 0; v < g->n; v++) {
        if (cor[v] == -1) {
            cor[v] = 0; // Nova componente
            fila_inserir(fila, v);

            while (!fila_vazia(fila)) {
                int u = fila_remover(fila);
                int k = grafo_vizinhos(g, u, buffer);

                for (int i = 0; i < k; i++) {
                    int w = buffer[i];
                    if (cor[w] == -1) {
                        cor[w] = 1 - cor[u];
                        fila_inserir(fila, w);
                    } else if (cor[w] == cor[u]) {
                        *u_conflito = u;
                        *v_conflito = w;
                        free(buffer);
                        fila_liberar(fila);
                        return 0; // Não é bipartido
                    }
                }
            }
        }
    }

    free(buffer);
    fila_liberar(fila);
    return 1; // É bipartido
}

int validar_particao(Grafo *g, int *cor) {
    int divergencias = 0;
    int n_produtos = g->n - g->n_usuarios;
    
    // Assume que a cor[0] dita a cor dos utilizadores
    int cor_usuario = cor[0];
    int cor_produto = 1 - cor_usuario;

    for (int v = 0; v < g->n; v++) {
        int cor_esperada = (v < g->n_usuarios) ? cor_usuario : cor_produto;
        if (cor[v] != cor_esperada) {
            divergencias++;
        }
    }

    printf("PARTICAO VALIDADA: %d usuarios, %d produtos, %d divergencias\n", 
           g->n_usuarios, n_produtos, divergencias);
    return divergencias;
}