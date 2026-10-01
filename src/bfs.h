#ifndef BFS_H
#define BFS_H

#include "grafo.h"

#define INALCANCAVEL -1

void bfs(Grafo *g, int *origens, int n_origens, int *dist, int *pai);
void saltos_ate_nicho(Grafo *g, const char *arquivo_saida);
void imprimir_caminho(Grafo *g, int *pai, int destino);
int  contar_componentes(Grafo *g);

#endif