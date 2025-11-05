#ifndef REPORT_H
#define REPORT_H

/**
 * Estrutura para armazenar as estatísticas da simulação
 */
typedef struct SimulationStats {
    long long nodes_traversed_count; // quantidade de nós atravessados até a alocação
    int failure_count;               // quantidade de falhas por falta de memória contígua
    int alloc_count;                 // quantidade total de requisições de alocação
} SimulationStats;

/**
 * Função para calcular a média de nós atravessados até a alocação
 * Parametros:
 *  - alloc_requests: número total de requisições de alocação
 *  - nodes_traversed: número total de nós atravessados
 * Retorno:
 *  - Média de nós atravessados por requisição
 */
double get_avg_nodes_traversed(int alloc_requests, long long nodes_traversed);

/**
 * Função para calcular a taxa de falha por falta de memória contígua
 * Parametros:
 *  - alloc_requests: número total de requisições de alocação
 *  - failure_count: número total de falhas por falta de memória contígua
 * Retorno:
 *  - Taxa de falha em porcentagem
 */
double get_failure_rate(int alloc_requests, int failure_count);

/**
 * Função para obter a contagem de fragmentação externa
 * Retorno:
 *  - Número de fragmentos externos de tamanho 1 ou 2 unidades
 */
int get_ext_frag_count();

/**
 * Função para obter o tamanho médio da fragmentação externa
 * Retorno:
 *  - Tamanho médio em páginas de segmentos livres (fragmentação externa)
 */
double get_avg_ext_frag();

/**
 * Função para obter o tamanho médio da fragmentação interna
 * Retorno:
 *  - Tamanho médio em KB de fragmentação interna
 */
double get_avg_int_frag();

/**
 * Função para inicializar o CSV
 * Parametros:
 *  - algorithm: Nome do algoritmo de alocação utilizado
 *  - request_file: Nome do arquivo de requisições utilizado
 * Obs.: O arquivo será salvo em "ReportManager/<algorithm>_<request_file>.csv"
 */
void init_csv_logger(const char* algorithm, const char* request_file);

/**
 * Função para registrar dados no CSV
 * Parametros:
 *  - stats: Estrutura contendo as estatísticas da simulação até o momento
 *  - request_num: Número da requisição atual
 */
void log_csv_data(SimulationStats stats, int request_num);

/**
 * Função para fechar o arquivo CSV
 */
void close_csv_logger(void);

/**
 * Função para exportar as estatísticas finais para um arquivo de texto
 * Parametros:
 *  - algorithm: Nome do algoritmo de alocação utilizado
 *  - request_file: Nome do arquivo de requisições utilizado
 *  - stats: Estrutura contendo as estatísticas da simulação
 * Obs.: O arquivo será salvo em "ReportManager/estatisticas_<algorithm>.txt"
 * Obs.: As estatísticas salvas são referentes ao estado final da memória após todas as requisições
 */
void export_final_statistics(
    const char* algorithm,
    const char* request_file,
    SimulationStats stats,
    int request_count
);

#endif