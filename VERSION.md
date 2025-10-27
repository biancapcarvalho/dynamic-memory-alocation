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

### Próximos Passos
- Correçao do frag_count em todos os algoritmos
- Tornar operações de alocação atômicas
- gerar requisiçoes de forma dinamica
- criar 3 cenários de carga (25, 50 e 75%)
- gerar relatório