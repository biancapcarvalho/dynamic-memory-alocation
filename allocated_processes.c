#include "allocated_processes.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Esse arquivo implementa uma lista encadeada para rastrear os processos que
 * têm memória alocada
 */

static AllocatedProcess* allocated_list_head = NULL;

// Função para adicionar à lista
void add_allocated_process(int PID) {
    AllocatedProcess* new_process = (AllocatedProcess*)malloc(sizeof(AllocatedProcess));
    if (new_process == NULL) {
        fprintf(stderr, "Erro ao adicionar processo á lista de processos alocados\n");
        exit(1);
    }

    new_process->PID = PID;
    new_process->next = allocated_list_head;
    allocated_list_head = new_process;

    printf("Processo %d adicionado à lista com sucesso.\n", PID);
}

// Função para verificar se um processo já está alocado
bool is_process_allocated(int PID) {
    AllocatedProcess* current = allocated_list_head;
    while (current != NULL) {
        if (current->PID == PID) {
            return TRUE;
        }
        current = current->next;
    }
    return FALSE;
}