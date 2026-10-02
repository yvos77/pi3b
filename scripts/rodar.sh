#!/bin/bash
# Executa todos os algoritmos em todas as amostras e representacoes.
# Uso: bash scripts/rodar.sh

set -u

REPETICOES=10
TAMANHOS="100 500 1000 2000"
REPRS="lista matriz"
ALGOS="bipartido nicho componentes"

if [ ! -x bin/pi3b ]; then
    echo "bin/pi3b nao existe. Rode 'make' antes."
    exit 1
fi

mkdir -p resultados
rm -f resultados/tempos.csv

total=0
for n in $TAMANHOS; do
    for repr in $REPRS; do
        for algo in $ALGOS; do
            total=$((total + REPETICOES))
        done
    done
done

feito=0
for n in $TAMANHOS; do
    pasta="dados/amostras/n$n"

    if [ ! -d "$pasta" ]; then
        echo "aviso: $pasta nao existe, pulando"
        continue
    fi

    for repr in $REPRS; do
        for algo in $ALGOS; do
            for i in $(seq 1 $REPETICOES); do
                ./bin/pi3b "$pasta" "$repr" "$algo" > /dev/null 2>&1
                feito=$((feito + 1))
            done

            printf "\r%d/%d  n=%-5s %-7s %s        " \
                   "$feito" "$total" "$n" "$repr" "$algo"
        done
    done
done

echo
echo "pronto: resultados/tempos.csv ($(wc -l < resultados/tempos.csv) linhas)"
echo "agora: python3 resultados/graficos.py"