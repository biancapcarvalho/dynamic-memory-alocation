#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "config.h"

void print_memory_list(void);

int main() {
    init_memory();
    print_memory_list();

    FILE* file = fopen("requisitions.txt", "r");

    char line[100];
    int pid, size;

    // Pula o cabeçalho
    if (fgets(line, sizeof(line), file) == NULL) {
        fprintf(stderr, "Erro: Arquivo de requisições vazio ou inválido.\n");
        fclose(file);
        return 1;
    }

    // Le as requisições linha por linha
    while (fgets(line, sizeof(line), file)) {
        if (strlen(line) <= 1) continue;

        char* token = strtok(line, ", ");
        if (token == NULL) continue;
        pid = atoi(token);

        token = strtok(NULL, ", ");
        if (token == NULL) continue;
        size = atoi(token);
        
        int nodes_traversed = alloc_mem(pid, size);
        
        print_memory_list();
    }

    fclose(file);
    printf("Simulação concluída.\n");
    print_memory_list();

    return 0;
}