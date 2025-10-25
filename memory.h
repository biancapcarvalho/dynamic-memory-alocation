#ifndef MEMORY_H
#define MEMORY_H

#include "config.h"

/**
 * TRECHO DO LIVRO
 * Cada entrada na lista especifica se é um espaço livre (L) ou alocado a um processo
 * (P), o endereço no qual se inicia esse segmento, o comprimento e um ponteiro para o
 * item seguinte
 * [...]
 * Como a vaga da tabela de processos para o que está sendo concluído geralmente aponta
 * para a entrada da lista do próprio processo, talvez seja mais conveniente ter a lista
 * como uma lista duplamente encadeada, em vez daquela com encadeamento simples. Essa
 * estrutura torna mais fácil encontrar a entrada anterior e ver se a fusão é possível.
 */

typedef struct MemorySegment {
    int PID;                        // PID do processo alocado (ou -1 se for um espaço livre)
    int start_unit;                 // unidade inicial do segmento
    int size;                       // tamanho do segmento em unidades (KB)
    struct MemorySegment* prev;     // ponteiro para o segmento anterior
    struct MemorySegment* next;     // ponteiro para o próximo segmento
} MemorySegment;

/**
 * Função para inicializar a memória
 * - Cria o primeiro segmento livre que representa toda a memória disponível
 */
void init_memory(void);

/**
 * Funçao para alocar um segmento de memória para um processo
 * Parametros:
 *  - PID : O identificador do processo para o qual alocar
 *  - mem_units: A quantidade de unidades de memória requisitadas
 * Retorno:
 *  - Em caso de falha: -1
 *  - Em caso de sucesso: nº de nós atravessados na lista até encontrar um segmento
 */
int alloc_mem(int PID, int mem_units);

/**
 * Função para desalocar a memória de um processo
 * Parâmetros:
 *  - PID: O identificador do processo que irá liberar a memória
 * Retorno:
 *  - Em caso de falha: -1
 *  - Em caso de sucesso: 1
 */
int dealloc_mem(int PID);

/**
 * Função para contar fragmentação externa
 * Retorno:
 *  - Quantidade de fragmentos externos de tamanho 1 ou 2 unidades
 */
int frag_count();

/**
 * Função para imprimir o estado atual da lista
 */
void print_memory_list(void);

#endif