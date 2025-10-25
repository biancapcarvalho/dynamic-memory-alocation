#include "allocated_processes.h"
#include <stdio.h>
#include <stdlib.h>

static AllocatedProcess* allocated_list_head = NULL;

void add_allocated_process(int PID) {
    AllocatedProcess* new_process = (AllocatedProcess*)malloc(sizeof(AllocatedProcess));
    if (new_process == NULL) {
        fprintf(stderr, "  Erro ao adicionar processo á lista de processos alocados\n");
        exit(1);
    }

    new_process->PID = PID;
    new_process->next = allocated_list_head;
    allocated_list_head = new_process;

    printf("  Processo %d adicionado à lista com sucesso.\n", PID);
}

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