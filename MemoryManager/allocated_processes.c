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
}

void remove_allocated_process(int PID) {
    AllocatedProcess* current = allocated_list_head;
    AllocatedProcess* previous = NULL;

    while (current != NULL) {
        if (current->PID == PID) {
            if (previous == NULL) {
                allocated_list_head = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}

bool is_process_allocated(int PID) {
    AllocatedProcess* current = allocated_list_head;
    while (current != NULL) {
        if (current->PID == PID) {
            return true;
        }
        current = current->next;
    }
    return false;
}

int get_random_allocated_pid() {
    AllocatedProcess* current = allocated_list_head;
    int count = 0;
    int pid = -1;

    if (current == NULL) {
        return -1; // nenhum processo alocado
    }

    while (current != NULL) {
        count++;
        if (rand() % count == 0) {
            pid = current->PID;
        }
        current = current->next;
    }

    return pid;
}