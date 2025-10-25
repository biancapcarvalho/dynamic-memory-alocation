# Versionamento

## v0.1.0 - Implementação Inicial
- (config.h) Definidas as características do sistema
- (first_fit.c) Implementação do First-fit
    - Inicialização de memória
    - Alocação de memória
- (allocated_processes) Implementação de lista de processos alocados
    - Funçoes adicionar e buscar
- (requisitions.txt) Arquivo com as requisiçoes

### Próximos Passos
- Implementar função de desalocação de memória
- Adicionar medição de fragmentação externa
- Tornar operações de alocação atômicas
- Implementar algoritmos adicionais de alocação (best-fit, worst-fit)
- gerar requisiçoes de forma dinamica
- criar 3 cenários de carga (25, 50 e 75%)
- gerar relatório