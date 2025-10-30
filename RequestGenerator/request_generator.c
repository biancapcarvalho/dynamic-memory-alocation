#include <stdio.h>
#include <stdlib.h>
#include "../config.h"
#include "request_generator.h"
#include "../MemoryManager/allocated_processes.h"

// para depurar a quantidade de requisiçoes de alocaçao e desalocaçao geradas
static int alloc_count = 0;
static int dealloc_count = 0;
static int invalid_count = 0;
static int load = 0;

/**
 * Inicializa o gerador de requisições com um cenário de carga específico
 */
void init_request_generator(int load_scenario) {
    alloc_count = 0;
    dealloc_count = 0;
    invalid_count = 0;
    load = load_scenario;
}

/**
 * Gera um tamanho de requisição aleatório em unidades de memória
 */
int get_request_size() {
    // MAX_REQUEST_SIZE - MIN_REQUEST_SIZE + 1 = 15 - 3 + 1 = 13
    // rand() % 13 gera um número entre 0 e 12
    // rand() % 13 + 3, transforma para um número entre 3 e 15
    return ((rand() % (MAX_REQUEST_SIZE - MIN_REQUEST_SIZE + 1)) + MIN_REQUEST_SIZE);
}

/**
 * Gera um PID aleatório
 */
int get_random_pid() {
    return (rand() % MAX_PID) ;
}

/**
 * Gera uma requisição
 */
Request generate_request(void) {
    Request request;
    int req_type = rand() % 100;
    int pid;

    if (req_type < load) {
        // Alocação
        pid = get_random_pid();

        if (is_process_allocated(pid)) {
            // processo já alocado, gerar requisição inválida
            request.type = 'N';
            invalid_count++;
        } else {
            request.type = 'A';
            request.pid = pid;
            request.size = get_request_size();
            alloc_count++;
            add_allocated_process(request.pid);
        }
    } else if (req_type >= load) {
        // Desalocação
        pid = get_random_allocated_pid();
        if (pid == -1) {
            // nenhum processo alocado, gerar requisição inválida
            request.type = 'N';
            invalid_count++;
        } else {
            request.type = 'D';
            request.pid = pid;
            request.size = 0;
            dealloc_count++;
            remove_allocated_process(request.pid);
        }
    }

    return request;
}

// Depurar a quantidade de requisiçoes de alocaçao e desalocaçao geradas
void print_request_summary(void) {
    printf("\n[Depurador do Gerador de Requisições]\n");
    printf("  - Total de Requisições de Alocação Geradas: %d\n", alloc_count);
    printf("  - Total de Requisições de Desalocação Geradas: %d\n\n", dealloc_count);
    printf("  - Total de Requisições Inválidas Geradas: %d\n\n", invalid_count);
}