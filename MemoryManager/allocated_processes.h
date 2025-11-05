#ifndef ALLOCATED_PROCESSES_H
#define ALLOCATED_PROCESSES_H

#include "../config.h"

 /**
  * Estrutura que representa um processo na lista
  * -> Cada nó representa um processo que possui memória alocada
  */
typedef struct AllocatedProcess {
    int PID;
    struct AllocatedProcess* next;
} AllocatedProcess;

/**
 * Função para adicionar um processo à lista de processos alocados
 * Parametros:
 *  - PID: O identificador do processo a ser adicionado
 */
void add_allocated_process(int PID);

/**
 * Função para remover um processo da lista de processos alocados
 * Parametros:
 *  - PID: O identificador do processo a ser removido
 */
void remove_allocated_process(int PID);

/**
 * Função para verificar se um processo está na lista de processos alocados
 * Parametros:
 *  - PID: O identificador do processo a ser verificado
 * Retorno:
 *  - true: Se o processo estiver na lista
 *  - false: Se o processo não estiver na lista
 */
bool is_process_allocated(int PID);

/**
 * Função para obter, aleatoriamente, o PID de um processo alocado
 * Retorno:
 *  - PID de um processo alocado
 */
int get_random_allocated_pid();

/**
 * Função para liberar toda a memória alocada para a lista de processos alocados
 */
void free_allocated_processes_list(void);

#endif