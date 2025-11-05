#include <stdio.h>
#include <stdlib.h>
#include "../config.h"
#include "request_generator.h"
#include "../MemoryManager/allocated_processes.h"

int get_request_size() {
    return ((rand() % (MAX_REQUEST_SIZE - MIN_REQUEST_SIZE + 1)) + MIN_REQUEST_SIZE);
}

int get_random_pid() {
    return (rand() % MAX_PID);
}

Request generate_request(int load_scenario) {
    Request request;
    int req_type = rand() % 100;
    int pid;

    if (req_type < load_scenario) {
        // Alocação
        pid = get_random_pid();

        if (is_process_allocated(pid)) {
            // processo já alocado, gerar requisição inválida
            request.type = 'N';
        } else {
            request.type = 'A';
            request.pid = pid;
            request.size = get_request_size();
            add_allocated_process(request.pid);
        }
    } else if (req_type >= load_scenario) {
        // Desalocação
        pid = get_random_allocated_pid();
        if (pid == -1) {
            // nenhum processo alocado, gerar requisição inválida
            request.type = 'N';
        } else {
            request.type = 'D';
            request.pid = pid;
            request.size = 0;
            remove_allocated_process(request.pid);
        }
    }

    return request;
}

FILE* init_request_file(const char* path) {
    FILE* request_file = fopen(path, "w");
    if (request_file == NULL) {
        fprintf(stderr, "Erro: Não foi possível criar %s.txt\n", path);
        exit(1);
    }
    return request_file;
}

void generation_loop(FILE *file, int load) {
    int num_requests = NUM_REQUESTS;

        for (int i = 0; i < num_requests; i++) {
            Request request = generate_request(load);
            if (request.type == 'A') {
                fprintf(file, "A %d %d\n", request.pid, request.size);
            } else if (request.type == 'D') {
                fprintf(file, "D %d\n", request.pid);
            }
        }
    
    free_allocated_processes_list();
}