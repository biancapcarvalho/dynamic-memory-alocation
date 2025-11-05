#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "MemoryManager/memory.h"
#include "ReportManager/report.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Erro: Uso incorreto.\n");
        fprintf(stderr, "Uso: %s <algoritmo> <arquivo_requisicoes>\n", argv[0]);
        fprintf(stderr, "Algoritmos disponíveis: first_fit, best_fit, next_fit\n");
        fprintf(stderr, "Arquivos disponíveis: carga_baixa.txt, carga_media.txt, carga_alta.txt\n");
        return 1;
    }

    const char* algorithm = argv[1];
    const char* request_file = argv[2];

    char full_path[512];
    snprintf(full_path, sizeof(full_path), "RequestGenerator/%s", request_file);
    FILE* file = fopen(full_path, "r");
    if (file == NULL) {
        fprintf(stderr, "Erro: Não foi possível abrir o arquivo '%s'\n", request_file);
        return 1;
    }

    printf("Iniciando a simulação do %s com o arquivo %s\n", algorithm, request_file);

    AllocAlgorithm algorithm_enum;

    if (strcmp(algorithm, "first_fit") == 0) {
        algorithm_enum = FIRST_FIT;
    } else if (strcmp(algorithm, "best_fit") == 0) {
        algorithm_enum = BEST_FIT;
    } else if (strcmp(algorithm, "next_fit") == 0) {
        algorithm_enum = NEXT_FIT;
    }

    init_memory(algorithm_enum);
    init_csv_logger(algorithm, request_file); // para o relatório

    char line[100];
    int pid, size;

    SimulationStats stats = {0}; // nodes_traversed_count, failure_count, alloc_count
    int nodes_traversed = 0;
    int request_count = 0;

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "A %d %d", &pid, &size) == 2) {
            nodes_traversed = alloc_mem(pid, size);
            request_count++;
            if (nodes_traversed == -2) {
                stats.failure_count++;
            } else {
                stats.nodes_traversed_count += nodes_traversed;
            }
            stats.alloc_count++;
        } else if (sscanf(line, "D %d", &pid) == 1) {
            dealloc_mem(pid);
            request_count++;
        }

        log_csv_data(stats, request_count);
    }

    fclose(file);
    close_csv_logger();
    printf("\n... Simulação concluída.\n");
    export_final_statistics(algorithm, request_file, stats, request_count);
    cleanup_memory();
    
    return 0;
}