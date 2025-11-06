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
}

int alloc_mem(int PID, int mem_units) {
    printf("\n> Nova requisiçao: alocar %d unidades de memória para o processo %d.\n", mem_units, PID);

    if (is_process_allocated(PID) == true) {
        printf("  FALHA AO ALOCAR MEMÓRIA - Já existe memória alocada para o processo PID %d.\n", PID);
        return -1;
    }

    int required_mem_kb = mem_units * MEM_UNIT_SIZE;
    int pages_needed = (required_mem_kb + PAGE_SIZE - 1) / PAGE_SIZE; // força o arredondamento para cima
    int nodes_traversed = 0;
    MemorySegment* suitable_segment = NULL;
    MemorySegment* current = memory_list_head;

    printf("\n  Procurando por um segmento de %d paginas livres para alocar...\n", pages_needed);

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
        printf("  FALHA AO ALOCAR MEMÓRIA - Sem memória livre contígua suficiente.\n");
        return -2;
    }

    /**
     * Para alocar a memória devemos:
     *  - Atualizar o segmento encontrado com o PID
     *  - Se o segmento for maior que o requisitado:
     *    - Criar um novo segmento livre com o tamanho restante, que será o "next" do alocado
     */
    int remainder_size = suitable_segment->size - pages_needed;
    int frag_size_kb = (pages_needed * PAGE_SIZE) - required_mem_kb;

    if (remainder_size == 0) {
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

    if (!is_process_allocated(PID)) {
        printf("  FALHA AO DESALOCAR MEMÓRIA - Processo %d não possui memória alocada.\n", PID);
        return -1;
    }

    MemorySegment* current = memory_list_head;

    // Percorre a lista para encontrar o segmento alocado com o PID
    while (current != NULL) {
        if (current->PID == PID) {
            current->PID = -1;
            current->frag_size = 0;

            /**
             * Sobre os vizinhos, sao quatro cenários possiveis após a desalocação:
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

int frag_count() {
    int frag_count = 0;
    MemorySegment* current = memory_list_head;

    while (current != NULL) {
        if (current->PID == -1 && (current->size == 1 || current->size == 2)) {
            frag_count++;
        }
        current = current->next;
    }

    return frag_count;
}

double avg_ext_frag_size(void) {
    int frag_count = 0;
    int frag_size_sum = 0;
    double avg_frag = 0;
    MemorySegment* current = memory_list_head; 

    while (current != NULL) {
        if (current->PID == -1) {
            frag_count++;
            frag_size_sum += current->size;
        }
        current = current->next;
    }

    avg_frag = (frag_count > 0) ? ((double)frag_size_sum / frag_count) : 0.0;
    return avg_frag;
}

double avg_int_frag_size() {
    int frag_count = 0;
    int frag_size_sum = 0;
    double avg_frag = 0;
    MemorySegment* current = memory_list_head;

    while (current != NULL) {
        if (current->frag_size > 0) {
            frag_count++;
            frag_size_sum += current->frag_size;
        }
        current = current->next;
    }

    avg_frag = (frag_count > 0) ? ((double)frag_size_sum / frag_count) : 0.0;
    return avg_frag;
}

void print_memory_list(FILE* output_file) {
    printf("\n> Status da Memória\n");
    MemorySegment* current = memory_list_head;
    int i = 0;
    while (current != NULL) {
        fprintf(output_file, "  Segmento %d: ", i++);
        if (current->PID == -1) {
            fprintf(output_file, "[ LIVRE ] ");
        } else {
            fprintf(output_file, "[ PID %d ] ", current->PID);
        }
        fprintf(output_file, "Início: %d, Tamanho: %d paginas, Fragmentação interna: %d KB\n", current->start_unit, current->size, current->frag_size);
        current = current->next;
    }
    fprintf(output_file, "\n\n");
}

void print_memory_map(FILE* output_file) {
    if (output_file == NULL) return;

    fprintf(output_file, "\n> Mapa da Memória\n");

    MemorySegment* current = memory_list_head;
    int page_counter = 0;
    char marker;
    
    fprintf(output_file, "  ");

    while (current != NULL) {
        if (current->PID == -1) {
            marker = '_';
        } else {
            marker = '#';
        }

        for (int i = 0; i < current->size; i++) {
            fprintf(output_file, "[%c]", marker);
            page_counter++;
            
            if (page_counter % 32 == 0 && page_counter < TOTAL_PAGES) {
                fprintf(output_file, "\n  ");
            }
        }
        current = current->next;
    }
    
    fprintf(output_file, "\n  Legenda: [_] = Livre, [#] = Alocado\n\n");
}

void cleanup_memory(void) {
    MemorySegment* current = memory_list_head;
    while (current != NULL) {
        MemorySegment* temp = current;
        current = current->next;
        free(temp);
    }
    memory_list_head = NULL;
}