# Atividade Prática de Sistemas Operacionais 2020/2 - Gerenciamento de Memória com Paginação

``Discentes: Ana Beatriz Borges (202201604), Bianca Carvalho (202004706), Júlia Moreira (202204837)``

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
    - Gera as requisições de forma aleatória
        - Os tamanhos das requisições são gerados aleatoriamente entre 3 e 15 unidades de memória
        - Os PIDs são gerados aleatoriamente entre 0 e 9999
            - Para alocação, se o PID já tiver memória alocada, a requisição é descartada
            - Para desalocação, é obtido um PID aleatório da lista de processos alocados. E se não houver processo alocado a requisição é descartada
    - A quantidade de requisições (válidas e inválidas) gerada é especificada por `NUM_REQUESTS` e definida em `config.h`
    - São criados três arquivos de requisições, um para cada cenário de carga (baixa 25%, média 50% e alta 75%)
        - Assim os três algoritmos de alocação recebem a mesma sequência de requisições

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
- **`main.c`**: O programa principal que inicializa a memória, lê o arquivo de requisições e processa cada requisição (alocando ou desalocando) em um loop
- MemoryManager/
    - **`memory.h`**: Interface do gerenciador de memória
    - **`first_fit.c`**: Implementação do algoritmo do First Fit
    - **`next_fit.c`**: Implementação do algoritmo do Next Fit
    - **`best_fit.c`**: Implementação do algoritmo do Best Fit
    - **`allocated_processes.h/.c`**: Implementação da lista de processos alocadosprocessos alocados
- RequestGenerator/
    - **`request_generator.h/.c`**: Implementação do gerador de requisições
    - **`main.c`**: O programa principal do gerador de requisições, que chama o gerador de requisições e cria os arquivos de requisição

## TODO

### 1. Relatório e Análise

- [ ] **Coleta de Estatísticas**: Implementar o componente de relatório.
- [ ] **Métricas de Desempenho**: Para cada algoritmo e cada cenário de carga, o simulador deve calcular e registrar:
    - O tamanho médio dos fragmentos externos.
    - O tempo médio de alocação (medido pelo número de nós atravessados na lista ligada).
    - O percentual de vezes que uma requisição de alocação falhou por falta de memória contígua.
- [ ] **Salvar Resultados**: Os valores estatísticos devem ser guardados em arquivos (ex: .csv ou .txt).
- [ ] **Análise Final**: Usar os dados salvos para plotar gráficos (Excel, gnuplot, etc.) e escrever as considerações finais sobre o desempenho comparativo dos algoritmos avaliados.

## Dúvidas
- Confirmar se frag_count deve ser de fragmentação interna ou externa

## Melhorias

- Implementar atomicidade nas operações de alocação e desalocação de memória, para que não haja inconsistência entre as listas de memória e de processos alocados

## Como Compilar e Executar

```bash
### PRIMEIRO GERAR OS ARQUIVOS DE REQUISIÇÃO -> gera os arquivos carga_baixa.txt, carga_media.txt e carga_alta.txt

# Compilar o programa
gcc RequestGenerator/main.c RequestGenerator/request_generator.c MemoryManager/allocated_processes.c -o generate_requests

# Executar a simulação
./generate_requests


### SEGUNDO, EXECUTAR OS TRÊS ALGORITMOS DE ALOCAÇÃO PARA CADA CENÁRIO DE CARGA

## FIRST FIT
# Compilar o programa
gcc main.c MemoryManager/allocated_processes.c MemoryManager/first_fit.c RequestGenerator/request_generator.c -o first_fit

# Executar a simulação com carga baixa
./first_fit carga_baixa.txt

# Executar a simulação com carga média
./first_fit carga_media.txt

# Executar a simulação com carga alta
./first_fit carga_alta.txt

## NEXT FIT
# Compilar o programa
gcc main.c MemoryManager/allocated_processes.c MemoryManager/next_fit.c RequestGenerator/request_generator.c -o next_fit

# Executar a simulação com carga baixa
./next_fit carga_baixa.txt

# Executar a simulação com carga média
./next_fit carga_media.txt

# Executar a simulação com carga alta
./next_fit carga_alta.txt

## BEST FIT
# Compilar o programa
gcc main.c MemoryManager/allocated_processes.c MemoryManager/best_fit.c RequestGenerator/request_generator.c -o best_fit

# Executar a simulação com carga baixa
./best_fit carga_baixa.txt

# Executar a simulação com carga média
./best_fit carga_media.txt

# Executar a simulação com carga alta
./best_fit carga_alta.txt