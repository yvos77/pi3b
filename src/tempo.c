#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <time.h>
#include "tempo.h"

double agora_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000.0 + ts.tv_nsec / 1e6;
}

static int arquivo_existe(const char *caminho) {
    FILE *f = fopen(caminho, "r");
    if (!f)
        return 0;

    fclose(f);
    return 1;
}

void registrar(const char *arquivo, const char *algoritmo, const char *repr,
               int n, long m, double tempo_ms, long memoria_bytes) {
    int precisa_cabecalho = !arquivo_existe(arquivo);

    FILE *f = fopen(arquivo, "a");
    if (!f) {
        fprintf(stderr, "aviso: nao foi possivel gravar em %s\n", arquivo);
        return;
    }

    if (precisa_cabecalho)
        fprintf(f, "algoritmo,repr,n,m,tempo_ms,memoria_bytes\n");

    fprintf(f, "%s,%s,%d,%ld,%.3f,%ld\n",
            algoritmo, repr, n, m, tempo_ms, memoria_bytes);

    fclose(f);
}