#ifndef ALLOCATED_PROCESSES_H
#define ALLOCATED_PROCESSES_H

#include "config.h"

/**
 * Implementação de uma lista encadeada para rastrear os processos que têm memória alocada
 */

typedef struct AllocatedProcess {
    int PID;
    struct AllocatedProcess* next;
} AllocatedProcess;

/**
 * Funçao para adicionar um processo à lista de processos alocados
 */
void add_allocated_process(int PID);

/**
 * Funçao para verificar se um processo está na lista de processos alocados
 */
bool is_process_allocated(int PID);

#endif