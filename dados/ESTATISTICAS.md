# Estatísticas do dataset

Grafo bipartido cliente–produto. Vértices são usuários e produtos; cada aresta é
uma compra.

## Grafo completo

| Métrica | Valor |
|---|---|
| Usuários | 1.500 |
| Produtos | 600 |
| Vértices (V) | 2.100 |
| Arestas (E) | 11.107 |
| Grau médio dos produtos | 18,5 |
| Grau máximo | 94 |
| Grau mínimo | 1 |
| Tipos de produto | 24 |
| Maior tipo | 159 produtos |
| Menor tipo | 7 produtos |

Densidade: num grafo bipartido o máximo possível é `|U| × |P| = 900.000` arestas.
Com 11.107, a densidade é de **1,23%** — esparso, como se espera de dados de
compra.

## Verificações de integridade

| Verificação | Resultado |
|---|---|
| Arestas duplicadas | 0 |
| Índices fora da faixa válida | 0 |
| Arestas dentro da mesma partição | 0 |
| Vértices de grau zero | 0 |

A terceira linha é a que importa: nenhuma aresta liga usuário a usuário nem
produto a produto. O grafo é bipartido por construção, e o teste de 2-coloração
implementado em C deve confirmá-lo.

## Distribuição dos rótulos

Cada produto recebe um rótulo, calculado antes de qualquer execução de algoritmo:

```
score = 0,6 × impopularidade + 0,4 × raridade do tipo
```

| Rótulo | Valor no CSV | Produtos | Faixa de grau | Critério |
|---|---|---|---|---|
| Nicho extremo | `2` | 30 | 1 a 10 | score no percentil 95 ou acima |
| Padrão | `0` | 150 | 28 a 94 | entre os 25% de maior grau |
| Intermediário | `1` | 420 | 1 a 27 | nenhum dos anteriores |

As duas classes de interesse **não se sobrepõem em grau**: o produto de nicho mais
vendido tem 10 compras; o produto padrão menos vendido tem 28. A separação é
limpa, o que dá sentido à pergunta de negócio — os dois extremos são de fato
extremos.

Os 420 intermediários não são descartados: eles formam a maior parte do grafo e
são os vértices por onde os caminhos entre padrão e nicho efetivamente passam.

## Subamostras

Geradas por bola de neve (expansão em largura a partir de um usuário de grau
mediano), para os testes de crescimento assintótico exigidos no protocolo
experimental.

| Amostra | Usuários | Produtos | Vértices | Arestas | Nicho | Padrão |
|---|---|---|---|---|---|---|
| n100 | 46 | 60 | 106 | 215 | 5 | 31 |
| n500 | 360 | 145 | 505 | 1.902 | 5 | 80 |
| n1000 | 758 | 245 | 1.003 | 4.462 | 5 | 120 |
| n2000 | 1.441 | 559 | 2.000 | 10.830 | 26 | 150 |

Cada pasta de amostra contém os quatro arquivos do dataset com os índices
renumerados a partir de zero, e constitui um grafo completo por si só.

A amostragem não é aleatória por vértice. Sortear vértices ao acaso destruiria a
conectividade e tornaria os resultados de busca em largura sem significado; a
expansão em largura preserva a vizinhança local do grafo original.

Quando uma amostra não alcança cinco produtos de nicho pela expansão, o script
acrescenta os que faltam junto de um comprador, o que faz o total de vértices
ultrapassar ligeiramente o alvo nominal.

## Reprodução

```bash
python3 dados/gerar_dataset.py
python3 dados/gerar_amostras.py
```

Semente fixa em 42. A origem e o tratamento do catálogo estão em `FONTE.md`.