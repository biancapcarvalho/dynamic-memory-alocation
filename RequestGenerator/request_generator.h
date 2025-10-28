#ifndef REQUEST_GENERATOR_H
#define REQUEST_GENERATOR_H

#include "../config.h"

// Estrutura da requisição
typedef struct {
    char type;  // 'A' para alocar, 'D' para desalocar
    int pid;    // PID do processo
    int size;   // tamanho em unidades de memória (apenas para alocação)
} Request;

/**
 * Gera um array de requisições aleatórias
 * Retorno:
 * - Um ponteiro para o array de 'Request' alocado dinamicamente
 */
Request* generate_requests();

#endif