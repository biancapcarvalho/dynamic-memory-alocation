#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../config.h"
#include "request_generator.h"
#include "../MemoryManager/allocated_processes.h"

void generation_loop(FILE *file, int load) {
    int num_requests = NUM_REQUESTS;
    init_request_generator(load);

        for (int i = 0; i < num_requests; i++) {
            Request request = generate_request();
            if (request.type == 'A') {
                fprintf(file, "A %d %d\n", request.pid, request.size);
            } else if (request.type == 'D') {
                fprintf(file, "D %d\n", request.pid);
            }
        }
    
    print_request_summary();
    free_allocated_processes_list();
}

int main(int argc, char* argv[]) {
    // seed para geração de números aleatórios do gerador de requisições
    srand(time(NULL));

    FILE *file_baixa, *file_media, *file_alta;

    // Carga baixa

    file_baixa = fopen("RequestGenerator/carga_baixa.txt", "w");
    if (file_baixa == NULL) {
        fprintf(stderr, "Erro mortal: Não foi possível criar carga_baixa.txt\n");
        return 1;
    }
    generation_loop(file_baixa, 25);
    fclose(file_baixa);

    // Carga média

    file_media = fopen("RequestGenerator/carga_media.txt", "w");
    if (file_media == NULL) {
        fprintf(stderr, "Erro mortal: Não foi possível criar carga_media.txt\n");
        return 1;
    }
    generation_loop(file_media, 50);
    fclose(file_media);

    // Carga baixa

    file_alta = fopen("RequestGenerator/carga_alta.txt", "w");
    if (file_alta == NULL) {
        fprintf(stderr, "Erro mortal: Não foi possível criar carga_alta.txt\n");
        return 1;
    }
    generation_loop(file_alta, 75);
    fclose(file_alta);

    fprintf(stderr, "\n\n*** Arquivos gerados: carga_baixa.txt, carga_media.txt, carga_alta.txt ***\n");
    return 0;
}