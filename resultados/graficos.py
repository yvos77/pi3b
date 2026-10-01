import csv
import os
import statistics
from collections import defaultdict

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

PASTA = os.path.dirname(os.path.abspath(__file__))
FIGURAS = os.path.join(PASTA, "figuras")
TEMPOS = os.path.join(PASTA, "tempos.csv")
SALTOS = os.path.join(PASTA, "saltos.csv")

AQUECIMENTO = 2


def carregar_tempos():
    """Agrupa as execucoes por (algoritmo, repr, n), descartando o aquecimento."""
    grupos = defaultdict(list)

    with open(TEMPOS, encoding="utf-8") as f:
        for linha in csv.DictReader(f):
            chave = (linha["algoritmo"], linha["repr"], int(linha["n"]))
            grupos[chave].append((
                float(linha["tempo_ms"]),
                int(linha["memoria_bytes"])
            ))

    resumo = {}

    for chave, execucoes in grupos.items():
        uteis = execucoes[AQUECIMENTO:] or execucoes
        tempos = [t for t, _ in uteis]

        resumo[chave] = {
            "media": statistics.mean(tempos),
            "desvio": statistics.stdev(tempos) if len(tempos) > 1 else 0.0,
            "memoria": uteis[0][1],
            "n_execucoes": len(uteis),
        }

    return resumo


def grafico_tempo(resumo, algoritmo):
    plt.figure(figsize=(8, 5))

    for repr_ in ("lista", "matriz"):
        pontos = sorted(
            (n, d)
            for (a, r, n), d in resumo.items()
            if a == algoritmo and r == repr_
        )

        if not pontos:
            continue

        xs = [n for n, _ in pontos]
        ys = [d["media"] for _, d in pontos]
        erros = [d["desvio"] for _, d in pontos]

        plt.errorbar(
            xs,
            ys,
            yerr=erros,
            marker="o",
            capsize=4,
            label=f"{repr_} de adjacência"
        )

    plt.xlabel("Número de vértices (N)")
    plt.ylabel("Tempo de execução (ms)")
    plt.title(f"Crescimento do tempo — {algoritmo}")
    plt.legend()
    plt.grid(alpha=0.3)
    plt.tight_layout()

    saida = os.path.join(FIGURAS, f"tempo_{algoritmo}.png")
    plt.savefig(saida, dpi=300)
    plt.close()

    print(f"  {saida}")


def grafico_memoria(resumo):
    plt.figure(figsize=(8, 5))

    for repr_ in ("lista", "matriz"):
        pontos = sorted({
            (n, d["memoria"])
            for (a, r, n), d in resumo.items()
            if r == repr_
        })

        if not pontos:
            continue

        xs = [n for n, _ in pontos]
        ys = [m / 1024 for _, m in pontos]

        plt.plot(
            xs,
            ys,
            marker="o",
            label=f"{repr_} de adjacência"
        )

    plt.xlabel("Número de vértices (N)")
    plt.ylabel("Memória (KB, escala log)")
    plt.yscale("log")
    plt.title("Consumo de memória por representação")
    plt.legend()
    plt.grid(alpha=0.3, which="both")
    plt.tight_layout()

    saida = os.path.join(FIGURAS, "memoria.png")
    plt.savefig(saida, dpi=300)
    plt.close()

    print(f"  {saida}")


def grafico_saltos():
    if not os.path.exists(SALTOS):
        print(f"  (pulando histograma: {SALTOS} nao existe)")
        return

    usuarios = []
    produtos = []

    with open(SALTOS, encoding="utf-8") as f:
        for linha in csv.DictReader(f):
            if int(linha["distancia"]) < 0:
                continue

            saltos = int(linha["saltos"])

            if linha["tipo"] == "usuario":
                usuarios.append(saltos)

            elif linha["rotulo"] == "padrao":
                produtos.append(saltos)

    plt.figure(figsize=(8, 5))

    maximo = max(usuarios + produtos + [1])
    bins = range(0, maximo + 2)

    plt.hist(
        [usuarios, produtos],
        bins=bins,
        align="left",
        label=["Usuários", "Produtos padrão"]
    )

    plt.xlabel("Saltos de consumo até o nicho extremo mais próximo")
    plt.ylabel("Quantidade de vértices")
    plt.title("Distância até produtos de nicho")
    plt.xticks(range(0, maximo + 1))
    plt.legend()
    plt.grid(alpha=0.3, axis="y")
    plt.tight_layout()

    saida = os.path.join(FIGURAS, "saltos.png")
    plt.savefig(saida, dpi=300)
    plt.close()

    print(f"  {saida}")


def tabela(resumo):
    print()

    print(
        f"{'algoritmo':<14}"
        f"{'repr':<9}"
        f"{'N':>6}"
        f"{'media (ms)':>13}"
        f"{'desvio':>10}"
        f"{'memoria (KB)':>15}"
    )

    for chave in sorted(resumo):
        a, r, n = chave
        d = resumo[chave]

        print(
            f"{a:<14}"
            f"{r:<9}"
            f"{n:>6}"
            f"{d['media']:>13.3f}"
            f"{d['desvio']:>10.3f}"
            f"{d['memoria'] / 1024:>15.1f}"
        )


def main():
    if not os.path.exists(TEMPOS):
        print(
            f"ERRO: {TEMPOS} nao existe. "
            "Rode 'bash scripts/rodar.sh' antes."
        )
        return

    os.makedirs(FIGURAS, exist_ok=True)
    resumo = carregar_tempos()

    print("figuras geradas:")

    for algoritmo in sorted({a for a, _, _ in resumo}):
        grafico_tempo(resumo, algoritmo)

    grafico_memoria(resumo)
    grafico_saltos()

    tabela(resumo)


if __name__ == "__main__":
    main()