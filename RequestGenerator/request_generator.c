#include <stdio.h>
#include <stdlib.h>
#include "../config.h"
#include "request_generator.h"
#include "../MemoryManager/allocated_processes.h"

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
 * Obs.: nao entendi direito como que a carga influencia na geraçao das requisiçoes
 * Aqui ele só gera 10.000 requisiçoes aleatórias de alocaçao e desalocaçao sem considerar a carga
 */
Request* generate_requests() {
    Request* requests = (Request*)malloc(sizeof(Request) * NUM_REQUESTS);
    if (requests == NULL) {
        fprintf(stderr, "Erro: Falha ao alocar memória para requisições\n");
        exit(1);
    }

    int req_type;
    int pid;

    for (int i = 0; i < NUM_REQUESTS; i++)
    {
        req_type = rand() % 2; // 0 para alocação, 1 para desalocação
        
        if (req_type == 0) {
            // Alocação
            requests[i].type = 'A';
            requests[i].pid = get_random_pid();
            requests[i].size = get_request_size();
        } else {
            // Desalocação
            pid = get_random_allocated_pid();
            if (pid == -1) {
                // Nenhum processo alocado -> gerar requisição de alocação
                requests[i].type = 'A';
                requests[i].pid = get_random_pid();
                requests[i].size = get_request_size();
            } else {
                requests[i].type = 'D';
                requests[i].pid = pid;
                requests[i].size = 0;
            }
        }
    }

    return requests;
}