#include "memory.h"
#include "allocated_processes.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * TRECHO DO LIVRO
 * Uma pequena variação do first fit é o next fit. Ele funciona da mesma maneira que
 * o first fit, exceto por memorizar a posição que se encontra um espaço livre
 * adequado sempre que o encontra. Da vez seguinte que for chamado para encontrar um
 * espaço livre, ele começa procurando na lista do ponto onde havia parado, em vez
 * de sempre do princípio, como faz o first fit. Simulações realizadas por Bays
 * (1977) mostram que o next fit tem um desempenho ligeiramente pior do que o do
 * first fit.
 */

// início da lista
static MemorySegment* memory_list_head = NULL;
// nó onde a próxima busca deve começar
static MemorySegment* next_fit_pointer = NULL;

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

    next_fit_pointer = memory_list_head;
}

int alloc_mem(int PID, int mem_units) {
    printf("\n> Nova requisiçao: alocar %d unidades de memória para o processo %d.\n", mem_units, PID);

    // Verificar se ja tem memória alocada para o processo
    if (is_process_allocated(PID) == TRUE) {
        printf("  FALHA AO ALOCAR MEMÓRIA - Já existe memória alocada para o processo PID %d.\n", PID);
        return -1;
    }

    int nodes_traversed = 0;
    MemorySegment* suitable_segment = NULL;
    MemorySegment* current = next_fit_pointer;

    // É uma lista circular. Começa a percorrer no nó após o último alocado,
    // e vai até encontrar um segmento adequado ou voltar ao início
    while (current != NULL) {
        nodes_traversed++;
        
        if (current->PID == -1 && current->size >= mem_units) {
            suitable_segment = current;
            break;
        }
        
        current = current->next;
    }

    if (suitable_segment == NULL)
    {
        current = memory_list_head;
        while (current != next_fit_pointer) {
            nodes_traversed++;
            
            if (current->PID == -1 && current->size >= mem_units) {
                suitable_segment = current;
                break;
            }
            
            current = current->next;
        }
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
     *    - Redefinir o tamanho do segmento
     *    - Criar um novo segmento livre com o tamanho restante, que será o "next" do alocado
     *  - Salvar o nó onde parou a busca para a próxima alocação
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

    // Se o segmento alocado tiver um próximo, começamos a próxima busca por ele
    // Se não, voltamos ao início da lista.
    next_fit_pointer = suitable_segment->next != NULL ? suitable_segment->next : memory_list_head;

    return nodes_traversed;
}

int dealloc_mem(int PID) {
    printf("\n> Nova requisiçao: desalocar processo %d.\n", PID);

    MemorySegment* current = memory_list_head;

    // Percorre a lista para encontrar o segmento alocado ao PID
    while (current != NULL) {
        if (current->PID == PID) {
            current->PID = -1;

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