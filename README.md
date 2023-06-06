# Alocação de Salas de Aula - Modelo de Programação Linear Inteira

Este repositório contém o código e a documentação referente ao trabalho de alocação de salas de aula (Pasa) do Instituto de Computação (IC) da Universidade Federal de Alagoas (UFAL). O objetivo principal do projeto é reduzir o desperdício de energia por meio de um modelo de programação linear inteira.

## Visão geral

O problema de alocação de salas de aula é uma questão recorrente em instituições de ensino, onde é necessário otimizar a distribuição das salas disponíveis para as disciplinas e atividades acadêmicas. Neste trabalho, propomos um modelo de programação linear inteira que aborda esse problema específico no IC da UFAL.

O trabalho foi desenvolvido com base na alocação de salas de aulas para o período letivo 2021.2. Utilizamos os dados de alocação existentes como referência para resolver o problema e comparar os resultados com a solução atualmente adotada pelo instituto.

## Estrutura do repositório

O repositório está organizado da seguinte maneira:

- `cplex/`: Esta pasta contém os arquivos com o código fonte do modelo de programação linear inteira desenvolvido. 
- `instância/alocacao-professor-20212_EC_CC-v28/`: Nesta pasta, mostra a alocação de salas de aula do período letivo 2021.2.
- `docs/`: Aqui encontram-se os documentos e arquivos de documentação relacionados ao trabalho, incluindo manuais, especificações técnicas e relatórios de resultados.
- `solução/alocacao-professor-20212_EC_CC-v28/`: Esta pasta armazena os resultados obtidos a partir da resolução do modelo proposto.
- `README.md`: Este arquivo contém informações gerais sobre o repositório e as instruções para utilização.

## Tabela de Salas do IC

A tabela a seguir apresenta as salas disponíveis no Instituto de Computação (IC) e seus respectivos usos:

| Sala               | Uso           |
|--------------------|---------------|
| Auditório          | Aula Teórica  |
| Lab 02             | Misto         |
| Lab 03             | Misto         |
| Sala 02            | Aula Teórica  |
| Sala 03            | Aula Teórica  |
| Lab Robótica       | Aula Prática  |
| Sala 204 Bloco 12  | Aula Teórica  |
| Sala 205 Bloco 12  | Aula Teórica  |
| Sala 206 Bloco 12  | Aula Teórica  |
| Sala 207 Bloco 12  | Aula Teórica  |
| Mini sala 01       | Aula Teórica  |
| Lab Controle       | Aula Prática  |
| Lab 01 Bloco 12    | Aula Prática  |
| Auditório CEPETEC  | Aula Teórica  |
| Lab Eletrônica     | Aula Prática  |
| Lab 01 AB          | Misto         |

## Formulação

Definições dos conjuntos usados:

- T = {1, 2, ..., Tn} - Turmas
- S = {1, 2, ..., Sn} - Salas
- H = {1, 2, ..., Hn} - Intervalos de tempo
- H^1 ⊆ H - Horários fictícios
- H^2 = H - {Hn}
- CH_h ⊆ T - Conflito de horários das turmas do intervalo h ∈ H

Variáveis binárias:

- xs,t - indica que a turma t ∈ T é alocada à sala s ∈ S
- zs,h - indica que a sala s ∈ S é utilizada no intervalo h ∈ H ∪ H^1
- ys,h - indica que a sala s ∈ S é aberta do intervalo h ∈ H ∪ H^1 para o intervalo h+1

Apresenta-se, a seguir, o modelo de programação linear inteira para minimização da soma da quantidade de vezes que as salas são abertas:

2 / 2

Aqui está o trecho adicionado ao README, que inclui a seção "Formulação" com as definições dos conjuntos e o modelo de programação linear inteira:

markdown

## Formulação

Definições dos conjuntos usados:

- T = {1, 2, ..., Tn} - Turmas
- S = {1, 2, ..., Sn} - Salas
- H = {1, 2, ..., Hn} - Intervalos de tempo
- H^1 ⊆ H - Horários fictícios
- H^2 = H - {Hn}
- CH_h ⊆ T - Conflito de horários das turmas do intervalo h ∈ H

Variáveis binárias:

- xs,t - indica que a turma t ∈ T é alocada à sala s ∈ S
- zs,h - indica que a sala s ∈ S é utilizada no intervalo h ∈ H ∪ H^1
- ys,h - indica que a sala s ∈ S é aberta do intervalo h ∈ H ∪ H^1 para o intervalo h+1

Apresenta-se, a seguir, o modelo de programação linear inteira para minimização da soma da quantidade de vezes que as salas são abertas:

{\allowdisplaybreaks\begin{align}
\min \quad & \sum_{s \in S}^{}\sum_{h \in H^{} \cup H^{1}}^{}y_{s,h} &\forall \ h \in H \cup H^{1}, \ \forall \ s \in S
& \
& \sum_{s \in S}^{} x_{s,t} = 1 &\forall \ t \in T\
&\sum_{t \in CH_{h}}^{} x_{s,t} = z_{s,h} &\forall \ h \in H \cup H^{1}, \ \forall \ s \in S\
& z_{s,h+1} - z_{s,h} \leq y_{s,h} &\forall \ s \in S, \ h \in H^{} \cup H^{1}\
&x_{s,t} \in \left { 0,1 \right } &\forall \ s \in S, \ t \in T\
&z_{s,h} \in \left { 0,1 \right } &\forall \ s \in S, \ h \in H \cup H^{1}\
&y_{s,h} \in \left { 0,1 \right } &\forall \ s \in S, \ h \in H^{*} \cup H^{1}
\end{align}}


Nesta formulação são utilizados os horários fictícios, que precedem os primeiros horários de cada dia e os de pausa de 40 minutos. Pois, não se deve contabilizar a primeira vez que a sala é aberta a cada dia e às vezes que a sala fica muito tempo sem ser usada.

Resumidamente, as restrições são:

1. Função Objetivo, minimiza o número de vezes que as salas são abertas.
2. Cada turma deve ser alocada a uma única sala.
3. Não pode haver conflitos de horários.
4. Relação entre a variável ys,h e zs,h.
5. xs,t é uma variável binária.
6. zs,h é uma variável binária.
7. ys,h é uma variável binária.

