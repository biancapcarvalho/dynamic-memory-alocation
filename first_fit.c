#include "memory.h"
#include "allocated_processes.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Esse arquivo implementa o algoritmo de alocação de memória First Fit
 * 
 * Implementações pendentes:
 *  - Função de desalocação de memória
 *  - Função para contar fragmentação externa
 */

/**
 * TRECHO DO LIVRO 
 * O algoritmo mais simples é first fit (primeiro encaixe). O gerenciador de memória
 * examina a lista de segmentos até encontrar um espaço livre que seja grande o
 * suficiente. O espaço livre é então dividido em duas partes, uma para o processo e
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
    memory_list_head->size = TOTAL_MEMORY_UNITS;
    memory_list_head->next = NULL;
    memory_list_head->prev = NULL;

    printf("Memória inicializada.\n");
    print_memory_list();
}

int alloc_mem(int PID, int mem_units) {
    printf("\n> Nova requisiçao: processo %d requer %d unidades de memória.\n", PID, mem_units);

    // Verificar se ja tem memória alocada para o processo
    if (is_process_allocated(PID) == TRUE) {
        printf("  FALHA AO ALOCAR MEMÓRIA - Já existe memória alocada para o processo PID %d.\n", PID);
        return -1;
    }

    int nodes_traversed = 0;
    MemorySegment* suitable_segment = NULL;
    MemorySegment* current = memory_list_head;

    // Percorre a lista e para no primeiro segmento livre com tamanho suficiente
    while (current != NULL) {
        nodes_traversed++;
        
        if (current->PID == -1 && current->size >= mem_units) {
            suitable_segment = current;
            break;
        }
        
        current = current->next;
    }

    if (suitable_segment == NULL) {
        printf("  FALHA AO ALOCAR MEMÓRIA - Sem espaço suficiente.\n");
        return -1; 
    }

    /**
     * Para alocar a memória devemos:
     *  - Atualizar o segmento encontrado com o PID
     *  - Se o segmento for maior que o requisitado:
     *    - Redefinir o tamanho do segmento
     *    - Criar um novo segmento livre com o tamanho restante, que será o "next" do alocado
     */
    int remainder_size = suitable_segment->size - mem_units; // tamanho do segmento encontrado - tamanho requisitado

    if (remainder_size == 0)
    {
        suitable_segment->PID = PID;
    } else {
        MemorySegment* new_free_segment = (MemorySegment*)malloc(sizeof(MemorySegment));
        new_free_segment->PID = -1;
        new_free_segment->start_unit = suitable_segment->start_unit + mem_units;
        new_free_segment->size = remainder_size;
        new_free_segment->prev = suitable_segment;
        new_free_segment->next = suitable_segment->next;

        suitable_segment->PID = PID;
        suitable_segment->size = mem_units;
        suitable_segment->next = new_free_segment;
    }

    printf("  Memória alocada com sucesso para o processo %d (%d unidades).\n", PID, mem_units);
    add_allocated_process(PID);

    return nodes_traversed;
}

int dealloc_mem(int PID) {
    printf("\n> Requisição de desalocação: processo %d.\n", PID);

    MemorySegment* current = memory_list_head;

    // Percorre a lista para encontrar o segmento alocado ao PID
    while (current != NULL) {
        if (current->PID == PID) {
            // Desalocar o segmento
            current->PID = -1;

            /**
             * Sao quatro cenários possiveis apos a desalocação:
             * 1. O segmento anterior e o próximo estão alocados -> não faz nada
             * 2. O segmento anterior está livre -> funde com o anterior
             * 3. O próximo segmento está livre -> funde com o próximo
             * 4. Ambos os segmentos estão livres -> funde com ambos
             */

            // Verifica se pode fundir com o segmento anterior
            if (current->prev != NULL && current->prev->PID == -1) {
                current->prev->size += current->size;
                current->prev->next = current->next;
                if (current->next != NULL) {
                    current->next->prev = current->prev;
                }
                current = current->prev;
            }

            // Verifica se pode fundir com o próximo segmento
            if (current->next != NULL && current->next->PID == -1) {
                MemorySegment* next_segment = current->next;
                current->size += next_segment->size;
                current->next = next_segment->next;
                if (next_segment->next != NULL) {
                    next_segment->next->prev = current;
                }
            }

            printf("  Memória desalocada com sucesso para o processo %d.\n", PID);
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

    printf("\n> Contagem de fragmentação externa: %d.\n", frag_count);
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
        printf("Início: %d, Tamanho: %d unidades\n", current->start_unit, current->size);
        current = current->next;
    }
    printf("\n\n");
}