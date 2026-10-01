#ifndef LEITURA_H
#define LEITURA_H

#include "grafo.h"

Grafo *ler_dataset(const char *pasta, int tipo);
void   imprimir_resumo(Grafo *g);

#endif