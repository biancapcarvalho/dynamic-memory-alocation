# Gerenciamento de Memória

Este projeto é um exercício prático sobre gerenciamento de memória. Foi desenvolvido como atividade da disciplina de Sistemas Operacionais para o curso de Engenharia dea Computação da Universidade Federal de Goiás (UFG).

O objetivo principal é desenvolver um sistema para compreender o gerenciamento de espaço livre através de listas encadeadas e implementar os algoritmos de alocação dinâmica de memória: *first fit*, *next fit* e *best fit*.

## Especificações do Sistema

As especificações do sistema implementado são:
* **Memória Total:** 1 MB
* **Unidade de Alocação:** 4 KB
* **Total de Unidades de Memória:** 256 (calculado como 1 MB / 4 KB)
* **Tamanho do Processo:** Varia de 3 a 15 unidades de memória
* **PID:** Um número de 0 a 9.999

## Versão Atual (v0.1.1)

A versão atual do projeto inclui:
* **`config.h`**: Definição das características do sistema (tamanhos de memória, processos, etc.).
* **`first_fit.c`**: Implementação inicial do algoritmo *First Fit*, incluindo a inicialização da memória e as funçoes `alloc_mem`, `dealloc_mem` e `frag_count`.
* **`allocated_processes.c`**: Uma lista encadeada auxiliar para rastrear processos que já possuem memória alocada, com funções para adicionar e buscar PIDs.
* **`main.c`**: O programa principal que inicializa a memória e processa um arquivo estático (`requisitions.txt`) de requisições de alocação.

## TODO

### 1. Gerenciamento de Memória

* [ ] **Implementar demais algoritmos de alocação**:
    * [ ] Implementar o algoritmo *Next Fit*.
    * [ ] Implementar o algoritmo *Best Fit*.

### 2. Geração de Requisições

* [ ] **Gerador Dinâmico**: Criar um componente para gerar requisições de alocação e desalocação.
    * O PID deve variar de 0 a 9.999.
    * O tamanho da memória requisitada deve ser gerado aleatoriamente entre 3 e 15 unidades.
* [ ] **Cenários de Carga**: O gerador deve simular diferentes cargas:
    * Baixa Carga (25%)
    * Média Carga (50%)
    * Alta Carga (75%)
    * A carga é definida pela probabilidade de uma requisição ser para alocar memória (ex: carga baixa = 50% alocar, 50% desalocar).

### 3. Relatório e Análise

* [ ] **Coleta de Estatísticas**: Implementar o componente de relatório.
* [ ] **Métricas de Desempenho**: Para cada algoritmo e cada cenário de carga, o simulador deve calcular e registrar:
    * O tamanho médio dos fragmentos externos.
    * O tempo médio de alocação (medido pelo número de nós atravessados na lista ligada).
    * O percentual de vezes que uma requisição de alocação falhou por falta de memória contígua.
* [ ] **Salvar Resultados**: Os valores estatísticos devem ser guardados em arquivos (ex: .csv ou .txt).
* [ ] **Análise Final**: Usar os dados salvos para plotar gráficos (Excel, gnuplot, etc.) e escrever as considerações finais sobre o desempenho comparativo dos algoritmos avaliados.

## Como Compilar e Executar

```bash
# Compilar o programa
gcc main.c first_fit.c allocated_processes.c -o simulator

# Executar a simulação
./simulator