import csv
import math
import os
import random

SEED = 42
N_USUARIOS = 1500
N_PRODUTOS = 600
MEDIA_COMPRAS = 8
PROB_RUIDO = 0.05
PERCENTIL_NICHO = 95
PERCENTIL_GRAU_PADRAO = 75
EXPOENTE_TIPOS = 1.0

PASTA = os.path.dirname(os.path.abspath(__file__))
CATALOGO = os.path.join(PASTA, "dataset3b.csv")

TIPOS = {
    "Cabo": "Cabo HDMI",
    "Caixa": "Caixa de Som",
    "Capa": "Capa de Silicone",
    "Carregador": "Carregador Rapido",
    "Fone": "Fone de Ouvido",
    "Headset": "Headset",
    "Home": "Home Theater",
    "Micro-ondas": "Micro-ondas",
    "Modem": "Modem",
    "Monitor": "Monitor Gamer",
    "Mouse": "Mouse",
    "Notebook": "Notebook",
    "Pelicula": "Pelicula de Vidro",
    "Película": "Pelicula de Vidro",
    "Placa": "Placa de Video",
    "Power": "Power Bank",
    "Protetor": "Protetor de Surto",
    "Roteador": "Roteador Wi-Fi",
    "SSD": "SSD",
    "Smart": "Smart TV",
    "Smartphone": "Smartphone",
    "Smartwatch": "Smartwatch",
    "Soundbar": "Soundbar",
    "Teclado": "Teclado Mecanico",
    "Webcam": "Webcam",
}


def tipo_do_produto(nome):
    tokens = nome.split()
    if len(tokens) < 2:
        return None
    return TIPOS.get(tokens[1])


def carregar_catalogo():
    """Lê o catálogo bruto e agrupa os produtos por tipo."""
    por_tipo = {}
    with open(CATALOGO, encoding="utf-8-sig", newline="") as f:
        for linha in csv.DictReader(f):
            nome = linha["nome"].replace(",", " ").strip()
            tipo = tipo_do_produto(nome)
            if tipo:
                por_tipo.setdefault(tipo, []).append(nome)
    return por_tipo


def amostrar_desigual(por_tipo, total):
    """Escolhe `total` produtos distribuindo os tipos por lei de potência.

    O catálogo bruto tem os 24 tipos com o mesmo tamanho. Um catálogo real
    tem categorias grandes e categorias de nicho, então a desigualdade é
    introduzida aqui, na amostragem, e não inventada nos dados.
    """
    tipos = sorted(por_tipo)
    random.shuffle(tipos)

    pesos = [1.0 / ((i + 1) ** EXPOENTE_TIPOS) for i in range(len(tipos))]
    soma = sum(pesos)

    catalogo = []
    for tipo, peso in zip(tipos, pesos):
        quantos = max(2, round(total * peso / soma))
        disponiveis = por_tipo[tipo]
        escolhidos = random.sample(disponiveis, min(quantos, len(disponiveis)))
        for nome in escolhidos:
            catalogo.append((nome, tipo))

    random.shuffle(catalogo)
    return catalogo[:total]


def sortear_com_peso(indices, graus):
    pesos = [graus[i] + 1 for i in indices]
    return random.choices(indices, weights=pesos, k=1)[0]


def main():
    random.seed(SEED)

    if not os.path.exists(CATALOGO):
        print(f"ERRO: nao encontrei {CATALOGO}")
        return

    por_tipo = carregar_catalogo()
    catalogo = amostrar_desigual(por_tipo, N_PRODUTOS)
    n_produtos = len(catalogo)
    base_produto = N_USUARIOS

    agrupado = {}
    for i, (_, tipo) in enumerate(catalogo):
        agrupado.setdefault(tipo, []).append(i)

    tipos = sorted(agrupado)
    todos_produtos = list(range(n_produtos))
    graus = [0] * n_produtos

    compras = []
    for u in range(N_USUARIOS):
        afinidade = random.sample(tipos, random.randint(1, 3))
        candidatos = [i for t in afinidade for i in agrupado[t]]

        quantidade = int(min(40, max(1, random.gauss(MEDIA_COMPRAS, 3))))
        ja_comprou = set()

        for _ in range(quantidade):
            fonte = todos_produtos if random.random() < PROB_RUIDO else candidatos
            escolhido = None
            for _ in range(5):
                p = sortear_com_peso(fonte, graus)
                if p not in ja_comprou:
                    escolhido = p
                    break
            if escolhido is None:
                continue
            ja_comprou.add(escolhido)
            graus[escolhido] += 1
            compras.append((u, base_produto + escolhido))

    for i in range(n_produtos):
        if graus[i] == 0:
            u = random.randrange(N_USUARIOS)
            graus[i] += 1
            compras.append((u, base_produto + i))

    grau_max = max(graus) or 1
    maior_tipo = max(len(v) for v in agrupado.values())

    scores = []
    for i, (_, tipo) in enumerate(catalogo):
        s1 = 1.0 - graus[i] / grau_max
        s2 = 1.0 - len(agrupado[tipo]) / maior_tipo
        scores.append(0.6 * s1 + 0.4 * s2)

    corte_nicho = sorted(scores)[int(n_produtos * PERCENTIL_NICHO / 100)]
    corte_grau = sorted(graus)[int(n_produtos * PERCENTIL_GRAU_PADRAO / 100)]

    rotulos = []
    for i in range(n_produtos):
        if scores[i] >= corte_nicho:
            rotulos.append(2)
        elif graus[i] >= corte_grau:
            rotulos.append(0)
        else:
            rotulos.append(1)

    with open(os.path.join(PASTA, "usuarios.csv"), "w", encoding="utf-8") as f:
        f.write("indice,nome\n")
        for u in range(N_USUARIOS):
            f.write(f"{u},cliente_{u:04d}\n")

    with open(os.path.join(PASTA, "produtos.csv"), "w", encoding="utf-8") as f:
        f.write("indice,nome,categoria,grau,score,rotulo\n")
        for i, (nome, tipo) in enumerate(catalogo):
            f.write(f"{base_produto + i},{nome},{tipo},"
                    f"{graus[i]},{scores[i]:.4f},{rotulos[i]}\n")

    with open(os.path.join(PASTA, "compras.csv"), "w", encoding="utf-8") as f:
        f.write("usuario,produto\n")
        for u, p in compras:
            f.write(f"{u},{p}\n")

    with open(os.path.join(PASTA, "meta.txt"), "w", encoding="utf-8") as f:
        f.write(f"{N_USUARIOS} {n_produtos} {len(compras)}\n")

    n_nicho = rotulos.count(2)
    n_padrao = rotulos.count(0)
    print(f"tipos no catalogo: {len(agrupado)}")
    print(f"  maior: {maior_tipo} produtos   menor: "
          f"{min(len(v) for v in agrupado.values())} produtos")
    print(f"usuarios: {N_USUARIOS}")
    print(f"produtos: {n_produtos}")
    print(f"vertices: {N_USUARIOS + n_produtos}")
    print(f"arestas:  {len(compras)}")
    print(f"nicho_extremo: {n_nicho}   padrao: {n_padrao}")
    print(f"grau medio dos produtos: {sum(graus) / n_produtos:.1f}")
    print(f"grau maximo: {grau_max}   grau minimo: {min(graus)}")

    if N_USUARIOS + n_produtos < 1000:
        print("ATENCAO: menos de 1000 vertices, o RF01 nao foi atendido")
    if n_nicho < 25 or n_padrao < 100:
        print("ATENCAO: poucos produtos rotulados, ajuste os percentis")


if __name__ == "__main__":
    main()