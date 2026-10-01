#include <stdio.h>
#include "leitura.h"

int main(void) {
    Grafo *g = ler_dataset("dados", MATRIZ);

    if (g == NULL) {
        printf("Erro ao carregar dataset.\n");
        return 1;
    }

    imprimir_resumo(g);

    grafo_liberar(g);

    return 0;
}