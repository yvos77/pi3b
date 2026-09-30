# Origem do catálogo de produtos

## Procedência

O arquivo `dataset3b.csv` foi obtido em **30/09/2026**, a partir do material da
disciplina de Projeto Integrador PI 2A, ofertada no primeiro semestre de 2026.

Conteúdo: **200.003 produtos** de varejo eletrônico, com as colunas
`id`, `nome`, `categoria` e `valor`.

## Verificação de integridade

O arquivo foi conferido antes do uso:

| Verificação | Resultado |
|---|---|
| Identificadores duplicados | nenhum |
| Campos vazios em `nome` ou `categoria` | nenhum |
| Vírgulas dentro do campo `nome` | nenhuma |
| Codificação | UTF-8, quebra de linha CRLF |

Nenhuma linha foi editada manualmente. O arquivo bruto está versionado como
recebido, e todo o tratamento é feito por `gerar_dataset.py`.

## Por que foi preciso extrair 24 tipos a partir do nome

O catálogo original traz apenas **três categorias** — Comunicação, Informática e
Eletrônicos — e as três têm praticamente o mesmo tamanho (cerca de 66 mil
produtos cada). Essa granularidade inviabiliza duas etapas centrais da
modelagem.

### Problema 1 — a afinidade do usuário perde sentido

No modelo de compras, cada cliente recebe de uma a três categorias de afinidade,
e é dentro delas que a maior parte das suas compras acontece. É esse mecanismo
que produz as **bolhas de consumo** cuja distância o trabalho se propõe a medir.

Com apenas três categorias disponíveis, um cliente com três afinidades cobriria o
catálogo inteiro, e mesmo um cliente com uma única afinidade teria acesso a um
terço de todos os produtos. Não haveria separação entre grupos de consumo: o
grafo resultante seria quase homogêneo, e a pergunta de negócio — a quantos
saltos um usuário está de um produto de nicho — não teria resposta informativa,
porque todos estariam a um ou dois saltos de tudo.

### Problema 2 — o score de nicho fica sem um dos seus componentes

A classificação de um produto como *nicho extremo* combina dois sinais:

```
score = 0,6 × impopularidade + 0,4 × raridade do tipo
```

A raridade do tipo é medida como `1 − (produtos do tipo ÷ produtos do maior tipo)`.
Com três categorias de tamanho equivalente, esse termo vale aproximadamente zero
para todos os produtos, e o score passa a depender só da popularidade. A noção de
nicho ficaria reduzida a "produto pouco vendido", perdendo a dimensão de
pertencer a um segmento de mercado restrito, que é o que caracteriza consumo de
nicho em e-commerce.

### Solução adotada

Os nomes dos produtos seguem um padrão regular — `marca + tipo + modelo + código`,
como em *Realme Smartwatch Global 5151* ou *TCL Home Theater Crystal 4419*. O
tipo do produto, que é a informação de segmento que faltava em coluna própria, é
recuperável do nome: o script extrai o termo imediatamente posterior à marca e o
normaliza contra uma tabela de 24 tipos (`Smartwatch`, `Roteador Wi-Fi`,
`Power Bank`, `Placa de Vídeo`, `Micro-ondas`, entre outros).

Não se trata de informação inventada — ela já estava presente no dado, apenas não
estava estruturada. A categoria original de três valores é descartada em favor
desse tipo mais fino, que passa a ocupar a coluna `categoria` do
`produtos.csv` gerado.

## Por que a amostra de 600 produtos é estratificada

Os 24 tipos aparecem no catálogo bruto com frequências praticamente idênticas,
em torno de 8.300 produtos cada. Catálogos reais de e-commerce não se comportam
assim: apresentam poucas categorias volumosas e uma cauda de categorias
pequenas, e é essa desigualdade que dá origem aos produtos de nicho.

Por isso, a seleção dos 600 produtos utilizados no grafo não é uniforme. O script
ordena os 24 tipos aleatoriamente e distribui as vagas segundo uma lei de
potência de expoente 1,0, de modo que o tipo mais representado receba cerca de
159 produtos e o menos representado cerca de 7.

A desigualdade é, portanto, uma **decisão de amostragem documentada**, aplicada
sobre dados reais — não uma alteração dos dados de origem. A escolha está
registrada aqui e deve constar da seção de Metodologia do artigo.

## Reprodutibilidade

O script usa semente fixa (`random.seed(42)`). Executado sobre o mesmo
`dataset3b.csv`, produz sempre os mesmos arquivos. A reprodução foi verificada em
máquinas distintas.

```bash
python3 dados/gerar_dataset.py
python3 dados/gerar_amostras.py
```