# Gerenciamento de Memória

Este projeto é um exercício prático sobre gerenciamento de memória. Foi desenvolvido como atividade da disciplina de Sistemas Operacionais para o curso de Engenharia de Computação da Universidade Federal de Goiás (UFG).

O objetivo principal é desenvolver um sistema para compreender o gerenciamento de espaço livre através de listas encadeadas e paginação, e avaliar o desempenho dos algoritmos de alocação dinâmica de memória: *first fit*, *next fit* e *best fit*.

## Especificações do Sistema

As especificações do sistema implementado são:
* **Memória Total:** 1 MB
* **Tamanho da Página:** 4 KB
* **Total de Páginas:** 256
* **Unidade de Memória**: 1 KB
* **Tamanho do Processo:** Varia de 3 a 15 unidades de memória
* **PID:** Um número de 0 a 9.999

## Estado Atual do Projeto

O simulador implementa os seguintes componentes:

- **Gerador de Requisições** (`request_generator.h` e `request_generator.c`)
    - Gera as requisições dinamicamente
    - Os PIDs e tamanhos das requisições são gerados aleatoriamente
    - A quantidade de requisições gerada é especificada por `NUM_REQUESTS` e definida em `config.h`

- **Gerenciamento de Memória** (`memory.h`)
    - A memória é uma lista duplamente encadeada (`MemorySegment`) para facilitar a fusão de segmentos livres adjacentes durante a desalocação
    - Algoritmos de alocação implementados
        - First Fit (`first_fit.c`): percorre a lista e aloca o primeiro segmento livre que seja grande o suficiente
        - Next Fit (`next_fit.c`): percorre a lista a partir do último segmento alocado e aloca o primeiro segmento livre que seja grande o suficiente
        - Best Fit (`best_fit.c`): percorre toda a lista e aloca o menor segmento livre que seja gande o suficiente
    - Controle de Processos (`allocated_processes.h` e `allocated_processes.c`)
        - Lista simplesmente encadeada para rastrear quais PIDs já possuem memória alocada

## Estrutura de Arquivos do Projeto

- **`config.h`**: Define todas as constantes globais do sistema (tamanho da memória, páginas, processos, etc.)
- **`main.c`**: O programa principal que inicializa a memória, chama o gerador de requisições e processa cada requisição (alocando ou desalocando) em um loop
- MemoryManager/
    - **`memory.h`**: Interface do gerenciador de memória
    - **`first_fit.c`**: Implementação do algoritmo do First Fit
    - **`next_fit.c`**: Implementação do algoritmo do Next Fit
    - **`best_fit.c`**: Implementação do algoritmo do Best Fit
    - **`allocated_processes.h/.c`**: Implementação da lista de processos alocadosprocessos alocados
- RequestGenerator/
    - **`request_generator.h/.c`**: Implementação do gerador de requisições

## TODO

### 1. Geração de Requisições

- [ ] **Cenários de Carga**: O gerador deve simular diferentes cargas:
    - Baixa Carga (25%)
    - Média Carga (50%)
    - Alta Carga (75%)
    - A carga é definida pela probabilidade de uma requisição ser para alocar memória

### 2. Relatório e Análise

- [ ] **Coleta de Estatísticas**: Implementar o componente de relatório.
- [ ] **Métricas de Desempenho**: Para cada algoritmo e cada cenário de carga, o simulador deve calcular e registrar:
    - O tamanho médio dos fragmentos externos.
    - O tempo médio de alocação (medido pelo número de nós atravessados na lista ligada).
    - O percentual de vezes que uma requisição de alocação falhou por falta de memória contígua.
- [ ] **Salvar Resultados**: Os valores estatísticos devem ser guardados em arquivos (ex: .csv ou .txt).
- [ ] **Análise Final**: Usar os dados salvos para plotar gráficos (Excel, gnuplot, etc.) e escrever as considerações finais sobre o desempenho comparativo dos algoritmos avaliados.

## Dúvidas
- Confirmar se frag_count deve ser de fragmentação interna ou externa
- Como implementar os cenários de carga?

## Melhorias

- Implementar atomicidade nas operações de alocação e desalocação de memória, para que não haja inconsistência entre as listas de memória e de processos alocados
- Fazer uso de processos ou de threads para executar os 3 algoritmos de alocação com o mesmo comando no terminal

## Como Compilar e Executar

```bash
## FIRST FIT
# Compilar o programa
gcc main.c MemoryManager/allocated_processes.c MemoryManager/first_fit.c RequestGenerator/request_generator.c -o first_fit

# Executar a simulação
./first_fit

## NEXT FIT
# Compilar o programa
gcc main.c MemoryManager/allocated_processes.c MemoryManager/next_fit.c RequestGenerator/request_generator.c -o next_fit

# Executar a simulação
./next_fit

## BEST FIT
# Compilar o programa
gcc main.c MemoryManager/allocated_processes.c MemoryManager/best_fit.c RequestGenerator/request_generator.c -o best_fit

# Executar a simulação
./best_fit