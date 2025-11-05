#ifndef REQUEST_GENERATOR_H
#define REQUEST_GENERATOR_H

#include "../config.h"

/**
 * Estrutura que representa uma requisição de memória
 */
typedef struct {
    char type;  // 'A' para alocar, 'D' para desalocar, 'N' para inválida
    int pid;    // PID do processo
    int size;   // tamanho em unidades de memória (apenas para alocação)
} Request;

/**
 * Função para gerar um tamanho de requisição aleatório em unidades de memória
 * Retorno:
 *  - Tamanho da requisição em unidades de memória
 */
int get_request_size();

/**
 * Função para gerar um PID aleatório entre 0 e 9999
 * Retorno:
 *  - PID gerado
 */
int get_random_pid();

/**
 * Função para gerar uma requisição de memória
 * Parametros:
 *  - load_scenario: Percentual de requisições de alocação (25, 50 ou 75%)
 * Retorno:
 *  - Requisição gerada
 */
Request generate_request(int load_scenario);

/**
 * Função para inicializar o arquivo de requisições
 * Parametros:
 *  - path: Caminho do arquivo onde as requisições serão salvas
 */
FILE* init_request_file(const char* path);

/**
 * Função para gerar todas as requisições de memória com base na carga especificada e salvar em um arquivo
 */
void generation_loop(FILE *file, int load);

#endif