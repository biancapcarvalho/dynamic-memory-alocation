#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "MemoryManager/memory.h"

void print_memory_list(void);
void print_memory_map(void);

int main(int argc, char* argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Erro: ceve ser informado o arquivo de requisições.\n");
        fprintf(stderr, "Ex: %s carga_alta.txt\n", argv[0]);
        return 1;
    }
    const char* request_file = argv[1];

    char full_path[512];
    snprintf(full_path, sizeof(full_path), "RequestGenerator/%s", request_file);
    FILE* file = fopen(full_path, "r");
    if (file == NULL) {
        fprintf(stderr, "Erro: Não foi possível abrir o arquivo '%s'\n", request_file);
        return 1;
    }
    
    printf("Iniciando a simulação com o arquivo: %s\n", request_file);

    init_memory();

    char line[100];
    char req_type;
    int pid, size;

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "A %d %d", &pid, &size) == 2) {
            alloc_mem(pid, size);
        } else if (sscanf(line, "D %d", &pid) == 1) {
            dealloc_mem(pid);
        }
    }

    fclose(file);
    printf("\n... Simulação concluída.\n");

    print_memory_list();
    print_memory_map();
    printf("\n> Contagem final de fragmentação externa: %d blocos de 1 ou 2 páginas\n", frag_count());
    printf("\n> Média final de fragmentação interna: %d KB\n", avg_int_frag());

    return 0;
}