#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "allocated_processes.h"

static MemorySegment* next_fit_pointer = NULL;

/**
 * TRECHO DO LIVRO 
 * O algoritmo mais simples é first fit (primeiro encaixe). O gerenciador de memória
 * examina a lista de segmentos até encontrar um espaço livre que seja grande o su-
 * ficiente. [...] First fit é um algoritmo rápido, pois ele procura fazer a menor
 * busca possível.
 */
MemorySegment* find_first_fit(MemorySegment* head, int pages_needed, int* nodes_traversed) {
    *nodes_traversed = 0;
    MemorySegment* current = head;

    // Percorre a lista e para no primeiro segmento livre com tamanho suficiente
    while (current != NULL) {
        (*nodes_traversed)++;
        
        if (current->PID == -1 && current->size >= pages_needed) {
            return current;
        }
        
        current = current->next;
    }

    return NULL;
}

// serve para o first fit e para o best fit
void dealloc_common(MemorySegment* current, int PID) {
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
}

/**
 * TRECHO DO LIVRO
 * O best fit faz uma busca em toda a lista do início ao fim, e escolhe o menor
 * espaço livre que seja adequado. Em vez de escolher um espaço livre grande
 * demais que talvez seja necessário mais tarde, o best fit tenta encontrar um
 * que seja de um tamanho próximo do tamanho real necessário, para casar da me-
 * lhor maneira possível a solicitação com os segmentos disponíveis.
 */
MemorySegment* find_best_fit(MemorySegment* head, int pages_needed, int* nodes_traversed) {
    *nodes_traversed = 0;
    MemorySegment* current = head;
    MemorySegment* suitable_segment = NULL;

    // Percorre toda a lista atualizando o segmento livre mais adequado
    int smallest_size_found = TOTAL_PAGES + 1; // inicializa com um valor maior que o total de memória
    while (current != NULL) {
        (*nodes_traversed)++;
        
        if (current->PID == -1 && current->size >= pages_needed && current->size < smallest_size_found) {
            suitable_segment = current;
            smallest_size_found = current->size;
        }
        current = current->next;
    }

    return suitable_segment;
}

/**
 * TRECHO DO LIVRO
 * Uma pequena variação do first fit é o next fit. Ele funciona da mesma maneira que
 * o first fit, exceto por memorizar a posição que se encontra um espaço livre ade-
 * quado sempre que o encontra. Da vez seguinte que for chamado para encontrar um
 * espaço livre, ele começa procurando na lista do ponto onde havia parado, em vez
 * de sempre do princípio, como faz o first fit. Simulações realizadas por Bays
 * (1977) mostram que o next fit tem um desempenho ligeiramente pior do que o do
 * first fit.
 */
void init_next_fit(MemorySegment* head) {
    next_fit_pointer = head;
}

MemorySegment* find_next_fit(MemorySegment* head, int pages_needed, int* nodes_traversed) {
    *nodes_traversed = 0;
    MemorySegment* current = next_fit_pointer;

    /**
     * É uma lista circular. Começa a percorrer a partir do último nó alocado e vai até
     * encontrar um segmento adequado ou voltar ao início e percorrer até o anterior ao último alocado
     */
    while (current != NULL) {
        (*nodes_traversed)++;
        
        if (current->PID == -1 && current->size >= pages_needed) {
            next_fit_pointer = current->next != NULL ? current->next : head;
            return current;
        }
        
        current = current->next;
    }

    current = head;
    while (current != next_fit_pointer) {
        (*nodes_traversed)++;
        
        if (current->PID == -1 && current->size >= pages_needed) {
            next_fit_pointer = current->next != NULL ? current->next : head;
            return current;
        }
        
        current = current->next;
    }

    next_fit_pointer = current->next != NULL ? current->next : head;
    return NULL;
}

void dealloc_next_fit(MemorySegment* current, int PID) {
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

        if (next_fit_pointer == next_segment) {
            next_fit_pointer = current;
        }

        current->size += next_segment->size;
        current->next = next_segment->next;
        if (next_segment->next != NULL) {
            next_segment->next->prev = current;
        }            
        free(next_segment);
    }

    // Verifica se pode fundir com o segmento anterior
    if (current->prev != NULL && current->prev->PID == -1) {
        if (next_fit_pointer == current) {
            next_fit_pointer = current->prev;
        }

        MemorySegment* current_segment_to_free = current;
        current->prev->size += current->size;
        current->prev->next = current->next;
        if (current->next != NULL) {
            current->next->prev = current->prev;
        }
        current = current->prev;
        free(current_segment_to_free);
    }
}