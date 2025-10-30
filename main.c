#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "MemoryManager/memory.h"

void print_memory_list(void);
void print_memory_map(void);

int main(int argc, char* argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Erro: ceve ser informado o arquivo de requisições.\n");
        fprintf(stderr, "Ex: %s carga_alta.txt\n", argv[0]);
        return 1;
    }
    const char* request_file = argv[1];

    char full_path[512];
    snprintf(full_path, sizeof(full_path), "RequestGenerator/%s", request_file);
    FILE* file = fopen(full_path, "r");
    if (file == NULL) {
        fprintf(stderr, "Erro: Não foi possível abrir o arquivo '%s'\n", request_file);
        return 1;
    }
    
    printf("Iniciando a simulação com o arquivo: %s\n", request_file);

    init_memory();

    char line[100];
    char req_type;
    int pid, size;

    long long nodes_traversed_total = 0;
    int nodes_traversed = 0;
    int failures = 0;
    int alloc_requests = 0;

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "A %d %d", &pid, &size) == 2) {
            nodes_traversed = alloc_mem(pid, size);
            if (nodes_traversed_total == -2) {
                failures++;
            } else {
                nodes_traversed_total += nodes_traversed;
            }
            
            alloc_requests++;
        } else if (sscanf(line, "D %d", &pid) == 1) {
            dealloc_mem(pid);
        }
    }

    fclose(file);
    printf("\n... Simulação concluída.\n");

    int avg_nodes_traversed = (alloc_requests > 0) ? (nodes_traversed_total / alloc_requests) : 0;
    double failure_rate = (alloc_requests > 0) ? ((double)failures / alloc_requests) * 100.0 : 0.0;

    printf("--> Estatísticas finais:");
    printf("\n  > Fragmentação externa: %d blocos livres de 1 ou 2 páginas", frag_count());
    printf("\n  > Tamanho médio dos fragmentos externos: %.2f KB", avg_ext_frag_size());
    printf("\n  > Tamanho médio dos fragmentos internos: %.2f KB", avg_int_frag_size());
    printf("\n  > Tempo médio (média de nós atravessados até a alocação) %d nós", avg_nodes_traversed);
    printf("\n  > Percentual de falha por falta de memória contígua: %.2f%%\n", failure_rate);

    // print_memory_list();
    print_memory_map();

    return 0;
}