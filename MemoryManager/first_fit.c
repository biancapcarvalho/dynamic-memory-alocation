#include "memory.h"
#include "allocated_processes.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * TRECHO DO LIVRO 
 * O algoritmo mais simples é first fit (primeiro encaixe). O gerenciador de memória
 * examina a lista de segmentos até encontrar um espaço livre que seja grande o su-
 * ficiente. O espaço livre é então dividido em duas partes, uma para o processo e
 * outra para a memória não utilizada, exceto no caso estatisticamente improvável de
 * um encaixe exato. First fit é um algoritmo rápido, pois ele procura fazer a menor
 * busca possível. 
 */

// início da lista
static MemorySegment* memory_list_head = NULL;

void init_memory(void) {
    memory_list_head = (MemorySegment*)malloc(sizeof(MemorySegment));
    if (memory_list_head == NULL) {
        fprintf(stderr, "Erro: Falha ao inicializar a memória\n");
        exit(1);
    }

    memory_list_head->PID = -1;
    memory_list_head->start_unit = 0;
    memory_list_head->size = TOTAL_PAGES;
    memory_list_head->frag_size = 0;
    memory_list_head->next = NULL;
    memory_list_head->prev = NULL;

    printf("Memória inicializada.\n");
    print_memory_list();
}

int alloc_mem(int PID, int mem_units) {
    printf("\n> Nova requisiçao: alocar %d unidades de memória para o processo %d.\n", mem_units, PID);

    // Verificar se ja tem memória alocada para o processo
    if (is_process_allocated(PID) == true) {
        printf("  FALHA AO ALOCAR MEMÓRIA - Já existe memória alocada para o processo PID %d.\n", PID);
        return -1;
    }

    int required_mem_kb = mem_units * MEM_UNIT_SIZE;
    int pages_needed = (required_mem_kb + PAGE_SIZE - 1) / PAGE_SIZE; // força o arredondamento para cima

    printf("\n  Procurando por um segmento de %d paginas livres para alocar...\n", pages_needed);

    int nodes_traversed = 0;
    MemorySegment* suitable_segment = NULL;
    MemorySegment* current = memory_list_head;

    // Percorre a lista e para no primeiro segmento livre com tamanho suficiente
    while (current != NULL) {
        nodes_traversed++;
        
        if (current->PID == -1 && current->size >= pages_needed) {
            suitable_segment = current;
            break;
        }
        
        current = current->next;
    }

    if (suitable_segment == NULL) {
        // Aqui devo desalocar algum processo ou retornar falha?
        printf("  FALHA AO ALOCAR MEMÓRIA - Sem espaço suficiente.\n");
        return -1;
    }

    /**
     * Para alocar a memória devemos:
     *  - Atualizar o segmento encontrado com o PID
     *  - Se o segmento for maior que o requisitado:
     *    - Criar um novo segmento livre com o tamanho restante, que será o "next" do alocado
     */
    int remainder_size = suitable_segment->size - pages_needed; // tamanho do segmento encontrado - tamanho requisitado
    int frag_size_kb = (pages_needed * PAGE_SIZE) - required_mem_kb;

    if (remainder_size == 0)
    {
        suitable_segment->PID = PID;
        suitable_segment->frag_size = frag_size_kb;
    } else {
        MemorySegment* new_free_segment = (MemorySegment*)malloc(sizeof(MemorySegment));
        new_free_segment->PID = -1;
        new_free_segment->start_unit = suitable_segment->start_unit + pages_needed;
        new_free_segment->size = remainder_size;
        new_free_segment->frag_size = 0;
        new_free_segment->prev = suitable_segment;
        new_free_segment->next = suitable_segment->next;

        suitable_segment->PID = PID;
        suitable_segment->size = pages_needed;
        suitable_segment->frag_size = frag_size_kb;
        suitable_segment->next = new_free_segment;

        if (new_free_segment->next != NULL) {
            new_free_segment->next->prev = new_free_segment;
        }
    }

    printf("  Memória alocada com sucesso para o processo %d (%d unidades em %d paginas).\n", PID, mem_units, pages_needed);
    add_allocated_process(PID);

    return nodes_traversed;
}

int dealloc_mem(int PID) {
    printf("\n> Nova requisiçao: desalocar processo %d.\n", PID);

    MemorySegment* current = memory_list_head;

    // Percorre a lista para encontrar o segmento alocado ao PID
    while (current != NULL) {
        if (current->PID == PID) {
            current->PID = -1;
            current->frag_size = 0;

            /**
             * Sobre os vizinhos, sao quatro cenários possiveis apos a desalocação:
             * 1. O segmento anterior e o próximo estão alocados -> não faz nada
             * 2. O segmento anterior está livre -> funde com o anterior
             * 3. O próximo segmento está livre -> funde com o próximo
             * 4. Ambos os segmentos estão livres -> funde com ambos
             */

            // Verifica se pode fundir com o próximo segmento
            if (current->next != NULL && current->next->PID == -1) {
                MemorySegment* next_segment = current->next;
                current->size += next_segment->size;
                current->next = next_segment->next;
                if (next_segment->next != NULL) {
                    next_segment->next->prev = current;
                }
                free(next_segment);
            }

            // Verifica se pode fundir com o segmento anterior
            if (current->prev != NULL && current->prev->PID == -1) {
                MemorySegment* current_segment_to_free = current;
                current->prev->size += current->size;
                current->prev->next = current->next;
                if (current->next != NULL) {
                    current->next->prev = current->prev;
                }
                current = current->prev;
                free(current_segment_to_free);
            }

            printf("  Memória desalocada com sucesso para o processo %d.\n", PID);
            remove_allocated_process(PID);
            return 1;
        }
        current = current->next;
    }

    printf("  FALHA AO DESALOCAR MEMÓRIA - Processo %d não encontrado.\n", PID);
    return -1;
}

// CORRIGIR - DEVE CONTAR A FRAGMENTAÇÃO EXTERNA OU INTERNA?
int frag_count() {
    int frag_count = 0;
    MemorySegment* current = memory_list_head;

    while (current != NULL) {
        if (current->frag_size == 1 || current->frag_size == 2) {
            frag_count++;
        }
        current = current->next;
    }

    return frag_count;
}

void print_memory_list(void) {
    printf("\n> Status da Memória\n");
    MemorySegment* current = memory_list_head;
    int i = 0;
    while (current != NULL) {
        printf("  Segmento %d: ", i++);
        if (current->PID == -1) {
            printf("[ LIVRE ] ");
        } else {
            printf("[ PID %d ] ", current->PID);
        }
        printf("Início: %d, Tamanho: %d paginas, Fragmentação interna: %d KB\n", current->start_unit, current->size, current->frag_size);
        current = current->next;
    }
    printf("\n\n");
}