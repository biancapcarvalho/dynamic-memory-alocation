#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "config.h"

void print_memory_list(void);

int main() {
    init_memory();

    FILE* file = fopen("requisitions.txt", "r");

    char line[100];
    int req, pid, size;

    // Le as requisições linha por linha
    while (fgets(line, sizeof(line), file)) {
        if (strlen(line) <= 1) continue;

        char* token = strtok(line, ", ");
        if (token == NULL) continue;
        req = token[0];

        token = strtok(NULL, ", ");
        if (token == NULL) continue;
        pid = atoi(token);
        
        if (req == 'A')
        {
            token = strtok(NULL, ", ");
            if (token == NULL) continue;
            size = atoi(token);

            alloc_mem(pid, size);
        }
        else if (req == 'D')
        {
            dealloc_mem(pid);
        }
        
        print_memory_list();
    }

    fclose(file);
    frag_count();

    return 0;
}