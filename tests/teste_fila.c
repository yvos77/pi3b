#include <stdio.h>
#include "fila.h"

int main() {
    Fila *f = fila_criar(10);
    
    printf("Inserindo: 10, 20, 30, 40, 50\n");
    fila_inserir(f, 10);
    fila_inserir(f, 20);
    fila_inserir(f, 30);
    fila_inserir(f, 40);
    fila_inserir(f, 50);

    printf("Removidos: %d, %d, %d\n", fila_remover(f), fila_remover(f), fila_remover(f));

    printf("Inserindo: 60, 70, 80, 90\n");
    fila_inserir(f, 60);
    fila_inserir(f, 70);
    fila_inserir(f, 80);
    fila_inserir(f, 90);

    printf("Removendo o resto: ");
    while (!fila_vazia(f)) {
        printf("%d ", fila_remover(f));
    }
    printf("\n");

    fila_liberar(f);
    return 0;
}