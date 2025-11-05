#include "report.h"
#include <stdio.h>
#include <string.h>
#include "report.h"
#include "../MemoryManager/memory.h"

static FILE* csv_file = NULL;

double get_avg_nodes_traversed(int alloc_requests, long long nodes_traversed) {
    return (alloc_requests > 0) ? ((double)nodes_traversed / alloc_requests) : 0;
}

double get_failure_rate(int alloc_requests, int failure_count) {
     return (alloc_requests > 0) ? ((double)failure_count / alloc_requests) * 100.0 : 0.0;
}

int get_ext_frag_count() {
    return frag_count();
}

double get_avg_ext_frag() {
    return avg_ext_frag_size();
}

double get_avg_int_frag() {
    return avg_int_frag_size();
}

void init_csv_logger(const char* algorithm, const char* request_file) {
    char csv_filename[256];
    
    // tira ".txt" do nome do arquivo
    char load_name[100];
    strncpy(load_name, request_file, sizeof(load_name) - 1);
    load_name[sizeof(load_name) - 1] = '\0';
    char* dot = strrchr(load_name, '.');
    if (dot) {
        *dot = '\0';
    }
    
    snprintf(csv_filename, sizeof(csv_filename), "ReportManager/%s_%s.csv", algorithm, load_name);
    csv_file = fopen(csv_filename, "w");
    if (csv_file == NULL) {
        fprintf(stderr, "Aviso: Não foi possível criar o arquivo CSV '%s'\n", csv_filename);
        return;
    }

    fprintf(csv_file, "RequestNum,AvgNodesTraversed,FailureRate,AvgExtFragSize\n");
}

void log_csv_data(SimulationStats stats, int request_num) {
    if (csv_file == NULL) {
        return;
    }

    double avg_nodes = get_avg_nodes_traversed(stats.alloc_count, stats.nodes_traversed_count);
    double failure_rate = get_failure_rate(stats.alloc_count, stats.failure_count);
    double avg_ext_frag = get_avg_ext_frag();

    fprintf(csv_file, "%d,%.2f,%.2f,%.2f\n",
        request_num,
        avg_nodes,
        failure_rate,
        avg_ext_frag
    );
}

void close_csv_logger(void) {
    if (csv_file != NULL) {
        fclose(csv_file);
        csv_file = NULL;
    }
}

void export_final_statistics(
    const char* algorithm,
    const char* request_file,
    SimulationStats stats,
    int request_count
) {
    char relatorio_filename[256];
    snprintf(relatorio_filename, sizeof(relatorio_filename), "ReportManager/estatisticas_%s.txt", algorithm);

    FILE* relatorio_txt = fopen(relatorio_filename, "a");
    if (relatorio_txt == NULL) {
        fprintf(stderr, "Aviso: Não foi possível criar o arquivo '%s'\n", relatorio_filename);
    } else {
        fprintf(relatorio_txt, "\n-------------------------------------------------------------------------------\n\n"); 
        fprintf(relatorio_txt, "-> %s %s [%d Alocações e %d Desalocações] \n\n", algorithm, request_file, stats.alloc_count, request_count - stats.alloc_count);
        fprintf(relatorio_txt, "  > Fragmentação externa: %d blocos livres de 1 ou 2 páginas\n", get_ext_frag_count());
        fprintf(relatorio_txt, "  > Tamanho médio dos fragmentos externos: %.2f páginas\n", get_avg_ext_frag());
        fprintf(relatorio_txt, "  > Tamanho médio dos fragmentos internos: %.2f KB\n", get_avg_int_frag());
        fprintf(relatorio_txt, "  > Tempo médio (média de nós atravessados até a alocação) %.2f nós\n", get_avg_nodes_traversed(stats.alloc_count, stats.nodes_traversed_count));
        fprintf(relatorio_txt, "  > Percentual de falha por falta de memória contígua: %.2f%%\n", get_failure_rate(stats.alloc_count, stats.failure_count));

        print_memory_map(relatorio_txt);

        fclose(relatorio_txt);
    }
}