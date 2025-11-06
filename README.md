# Atividade Prática de Sistemas Operacionais 2025/2 - Gerenciamento de Memória com Paginação

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

- **Gerenciador de Relatório**
    - Concentra as funções para extrair os dados das simulações
    - Exporta arquivos CSV, um para cada simulação, com os dados de tempo médio, fragmentação externa, e taxa de falha a cada nova requisição
    - Exporta arquivos TXT com as estatísticas da memória ao final de cada simulação

- **Relatório Final**
    - O diretório contém os arquivos de requisições, CSV e TXT utilizados para gerar os gráficos e fazer a análise dos algoritmos
    - O diretório também contém o PDF do relatório

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
    - Os arquivos com as requisições são salvos aqui
- ReportManager/
    - **`report.h/.c`**: Implementação do componente de relatório
    - Os arquivos com os dados das simulação são salvos aqui

## Como Compilar e Executar

O projeto utiliza um `Makefile` para automatizar o processo de compilação, execução e limpeza. Os principais comandos, a serem executados no terminal a partir da raiz do projeto, são:

```bash
# Compila tudo e executa o processo completo - gera os arquivos de carga e roda as 9 simulações (3 algoritmos x 3 cenários de carga)
make run_all

# Limpa todos os arquivos gerados (executáveis, .o, arquivos de carga, etc.)
make clean