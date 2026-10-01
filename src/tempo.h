#ifndef TEMPO_H
#define TEMPO_H

double agora_ms(void);

void registrar(const char *arquivo, const char *algoritmo, const char *repr,
               int n, long m, double tempo_ms, long memoria_bytes);

#endif