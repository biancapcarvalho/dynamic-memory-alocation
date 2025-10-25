#ifndef ALLOCATED_PROCESSES_H
#define ALLOCATED_PROCESSES_H

#include "config.h"

// Estrutura para um processo alocado
typedef struct AllocatedProcess {
    int PID;
    struct AllocatedProcess* next;
} AllocatedProcess;

// Funções para gerenciar a lista
void add_allocated_process(int PID);
bool is_process_allocated(int PID);

#endif