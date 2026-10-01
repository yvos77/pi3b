#include <stdio.h>
#include "grafo.h"
#include "bipartido.h"

int main() {
    printf("--- Teste 1: Bipartido Simples ---\n");
    Grafo *g1 = grafo_criar(LISTA, 4, 2);
    grafo_add_aresta(g1, 0, 2);
    grafo_add_aresta(g1, 1, 3);
    
    int cor1[4], u1, v1;
    if (testar_bipartido(g1, cor1, &u1, &v1)) {
        printf("Bipartido? SIM\n");
        validar_particao(g1, cor1);
    }

    printf("\n--- Teste 2: Triangulo (Nao bipartido) ---\n");
    Grafo *g2 = grafo_criar(LISTA, 3, 1);
    grafo_add_aresta(g2, 0, 1);
    grafo_add_aresta(g2, 1, 2);
    grafo_add_aresta(g2, 2, 0);
    
    int cor2[3], u2, v2;
    if (!testar_bipartido(g2, cor2, &u2, &v2)) {
        printf("Bipartido? NAO. Conflito entre %d e %d\n", u2, v2);
    }

    printf("\n--- Teste 3: Desconexo com ciclo impar ---\n");
    Grafo *g3 = grafo_criar(LISTA, 5, 2);
    grafo_add_aresta(g3, 0, 3); // Comp 1 normal
    grafo_add_aresta(g3, 1, 2); // Comp 2 com ciclo
    grafo_add_aresta(g3, 2, 4);
    grafo_add_aresta(g3, 4, 1);
    
    int cor3[5], u3, v3;
    if (!testar_bipartido(g3, cor3, &u3, &v3)) {
        printf("Bipartido? NAO. Conflito entre %d e %d\n", u3, v3);
    }

    return 0;
}