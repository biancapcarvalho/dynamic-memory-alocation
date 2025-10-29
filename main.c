#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "config.h"
#include "MemoryManager/memory.h"
#include "RequestGenerator/request_generator.h"

void print_memory_list(void);

int main() {
    init_memory();

    // seed para geração de números aleatórios do generator de requisições
    srand(time(NULL));
    Request* requests = generate_requests();

    for (int i = 0; i < NUM_REQUESTS; i++) {
        if (requests[i].type == 'A') {
            alloc_mem(requests[i].pid, requests[i].size);
        } else if (requests[i].type == 'D') {
            dealloc_mem(requests[i].pid);
        }
    }

    free(requests);

    printf("\n> Contagem final de fragmentação interna: %d\n", frag_count());
    print_memory_list();

    return 0;
}