# Versionamento

## v0.1.0 - Implementação Inicial
- (config.h) Definidas as características do sistema
- (first_fit.c) Implementação do First-fit
    - Inicialização de memória
    - Alocação de memória
- (allocated_processes) Implementação de lista de processos alocados
    - Funçoes adicionar e buscar
- (requisitions.txt) Arquivo com as requisiçoes

## v0.1.1 - Concluindo First Fit
- Implementação do dealloc_mem e frag_count do First-fit
- Atualizaçao do README

## v0.2.0 - Implementaçao do next fit
- Implementação do alloc_mem, dealloc_mem e frag_count do next-fit
- Correçao na funçao dealloc_mem do first-fit
- Atualizaçao do README

## v0.3.0 - Implementaçao do best fit
- Implementação do alloc_mem, dealloc_mem e frag_count do best-fit
- Atualizaçao do README

## v0.3.1 - Correçao da Alocaçao
- Correçao da alocaçao -> Implementação de alocaçao por paginas
- Alteraçao na struct da memória -> indicar KB de fragmentaçao
- Alteraçao no print_memory_list -> exibir a fragmentaçao
- Atualizaçao do README

## v0.4.0 - Implementação inicial do gerador de requisições
- Arquivos renomeados e reorganizados em diretórios
- (config.h) Definido número de requisições a serem gerados
- (request_generator) Implementação do gerador de requisições aleatórias
- (main.c) Alterado para processar um array de requisições, não mais um arquivo
- (first_fit.c, best_fit.c e next_fit.c) Implementada função de contagem de fragmentos internos
- Atualizaçao do README

## v0.4.1
- (config.h) Inclusão de <stdbool.h> para variáveis booleanas
- (main.c)
    - Inserção da chamada srand() antes de chamar o gerador de requisições, para ser chamada uma única vez
    - Chamada print_memory_list() fora do loop, somente ao final do processamento de todas as requisições
- (allocated_processes.c)
    - Remoção dos printf
    - Nova função: get_random_allocated_pid() para não gerar requisições de desalocação de processos não alocados
- (request_generator.c)
    - Remoção do srand() (foi para main.c)
    - Correção do rand() para escolher o tipo de requisição
    - Alteração na desalocação para não gerar requisição de processo não alocado

## v0.5.0
- (main.c) Reformulaçao para processar as requsições a partir de um arquivo
- (allocated_processes.c/.h) Função para liberar a memória da lista de processos alocados
- (first, best e next_fit.c) Correção de frag_count(), nova função avg_int_frag(), nova função print_memory_map()
- (next_fit.c) Correção na alocação e desalocação
- (request_generator.c/.h)
    - Implementação dos 3 cenários de carga
    - Reformulação de generate_request() para lidar com casos de requisição inválida (alocar processo já alocado e desalocar processo não alocado)
    - Inclusão de contadores de requisições inválidas, de alocação e de desalocação
- (RequestGenerator/main.c) Programa para gerar os arquivos de requisição

### Próximos Passos
- Fazer o componente de relatório