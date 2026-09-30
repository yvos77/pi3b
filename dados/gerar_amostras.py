import os
import random
from collections import deque

random.seed(42)
VIZINHOS_POR_VEZ = 6

PASTA = os.path.dirname(os.path.abspath(__file__))
TAMANHOS = [100, 500, 1000, 2000]


def carregar():
    with open(os.path.join(PASTA, "meta.txt"), encoding="utf-8") as f:
        n_usuarios, n_produtos, _ = map(int, f.read().split())

    usuarios = {}
    with open(os.path.join(PASTA, "usuarios.csv"), encoding="utf-8") as f:
        next(f)
        for linha in f:
            i, nome = linha.rstrip("\n").split(",", 1)
            usuarios[int(i)] = nome

    produtos = {}
    with open(os.path.join(PASTA, "produtos.csv"), encoding="utf-8") as f:
        next(f)
        for linha in f:
            partes = linha.rstrip("\n").split(",")
            produtos[int(partes[0])] = partes[1:]

    arestas = []
    with open(os.path.join(PASTA, "compras.csv"), encoding="utf-8") as f:
        next(f)
        for linha in f:
            u, p = linha.split(",")
            arestas.append((int(u), int(p)))

    return n_usuarios, n_produtos, usuarios, produtos, arestas


def bola_de_neve(adj, semente, alvo):
    visitados = {semente}
    fila = deque([semente])
    while fila and len(visitados) < alvo:
        v = fila.popleft()
        vizinhos = [w for w in adj.get(v, ()) if w not in visitados]
        random.shuffle(vizinhos)
        adicionados = 0
        for w in vizinhos:
            if len(visitados) >= alvo or adicionados >= VIZINHOS_POR_VEZ:
                fila.append(v)
                break
            visitados.add(w)
            fila.append(w)
            adicionados += 1
    return visitados


def completar_nicho(adj, produtos, selecionados, minimo):
    presentes = [v for v in selecionados
                 if v in produtos and produtos[v][4] == "2"]
    if len(presentes) >= minimo:
        return selecionados

    candidatos = sorted(v for v in produtos
                        if produtos[v][4] == "2" and v not in selecionados
                        and adj.get(v))
    for v in candidatos[:minimo - len(presentes)]:
        compradores = adj[v]
        ja_dentro = [w for w in compradores if w in selecionados]
        selecionados.add(v)
        if not ja_dentro:
            selecionados.add(compradores[0])
    return selecionados


def salvar(destino, usuarios, produtos, arestas, selecionados):
    os.makedirs(destino, exist_ok=True)

    us = sorted(v for v in selecionados if v in usuarios)
    ps = sorted(v for v in selecionados if v in produtos)

    novo = {}
    for i, v in enumerate(us):
        novo[v] = i
    for i, v in enumerate(ps):
        novo[v] = len(us) + i

    with open(os.path.join(destino, "usuarios.csv"), "w", encoding="utf-8") as f:
        f.write("indice,nome\n")
        for v in us:
            f.write(f"{novo[v]},{usuarios[v]}\n")

    with open(os.path.join(destino, "produtos.csv"), "w", encoding="utf-8") as f:
        f.write("indice,nome,categoria,grau,score,rotulo\n")
        for v in ps:
            f.write(f"{novo[v]}," + ",".join(produtos[v]) + "\n")

    internas = [(u, p) for u, p in arestas if u in selecionados and p in selecionados]
    with open(os.path.join(destino, "compras.csv"), "w", encoding="utf-8") as f:
        f.write("usuario,produto\n")
        for u, p in internas:
            f.write(f"{novo[u]},{novo[p]}\n")

    with open(os.path.join(destino, "meta.txt"), "w", encoding="utf-8") as f:
        f.write(f"{len(us)} {len(ps)} {len(internas)}\n")

    n_nicho = sum(1 for v in ps if produtos[v][4] == "2")
    n_padrao = sum(1 for v in ps if produtos[v][4] == "0")
    return len(us), len(ps), len(internas), n_nicho, n_padrao


def main():
    n_usuarios, n_produtos, usuarios, produtos, arestas = carregar()

    adj = {}
    for u, p in arestas:
        adj.setdefault(u, []).append(p)
        adj.setdefault(p, []).append(u)

    candidatos = sorted((v for v in adj if v < n_usuarios),
                        key=lambda v: len(adj[v]))
    semente = candidatos[len(candidatos) // 2]

    print(f"{'amostra':<10}{'|U|':>6}{'|P|':>6}{'arestas':>9}"
          f"{'nicho':>7}{'padrao':>8}")
    for alvo in TAMANHOS:
        selecionados = bola_de_neve(adj, semente, alvo)
        selecionados = completar_nicho(adj, produtos, selecionados, 5)
        destino = os.path.join(PASTA, "amostras", f"n{alvo}")
        nu, np_, na, nn, npd = salvar(destino, usuarios, produtos,
                                      arestas, selecionados)
        print(f"n{alvo:<9}{nu:>6}{np_:>6}{na:>9}{nn:>7}{npd:>8}")
        if nn < 5 or npd < 10:
            print(f"  ATENCAO: n{alvo} tem produtos rotulados de menos")


if __name__ == "__main__":
    main()