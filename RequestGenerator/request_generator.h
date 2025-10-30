#ifndef REQUEST_GENERATOR_H
#define REQUEST_GENERATOR_H

#include "../config.h"

// Estrutura da requisição
typedef struct {
    char type;  // 'A' para alocar, 'D' para desalocar, 'N' para inválida
    int pid;    // PID do processo
    int size;   // tamanho em unidades de memória (apenas para alocação)
} Request;

/**
 * Inicializa o gerador de requisições com um cenário de carga específico
 */
void init_request_generator(int load_scenario);

/**
 * Gera uma requisição aleatória
 * Retorno:
 * - Request: estrutura contendo os detalhes da requisição gerada
 *   - type: 'A' (alocar), 'D' (desalocar), 'N' (inválida)
 */
Request generate_request(void);

// para depurar qual foi a quantidade de requisiçoes de alocaçao e desalocaçao geradas
void print_request_summary(void);

#endif