#ifndef BIPARTIDO_H
#define BIPARTIDO_H

#include "grafo.h"

int testar_bipartido(Grafo *g, int *cor, int *u_conflito, int *v_conflito);
int validar_particao(Grafo *g, int *cor);

#endif