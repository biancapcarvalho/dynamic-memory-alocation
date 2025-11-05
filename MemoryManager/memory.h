#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../config.h"

/**
 * TRECHO DO LIVRO
 * Cada entrada na lista especifica se é um espaço livre (L) ou alocado a um processo
 * (P), o endereço no qual se inicia esse segmento, o comprimento e um ponteiro para o
 * item seguinte
 * [...]
 * Como a vaga da tabela de processos para o que está sendo concluído geralmente aponta
 * para a entrada da lista do próprio processo, talvez seja mais conveniente ter a lista
 * como uma lista duplamente encadeada, em vez daquela com encadeamento simples. Essa
 * estrutura torna mais fácil encontrar a entrada anterior e ver se a fusão é possível.
 */

 /**
  * Estrutura que representa um segmento da memória na lista
  * -> Cada segmento é composto por x páginas consecutivas
  */
typedef struct MemorySegment {
    int PID;                        // PID do processo alocado (-1 se for um espaço livre)
    int start_unit;                 // unidade inicial do segmento
    int size;                       // quantidade de páginas
    int frag_size;                  // tamanho da fragmentação interna (em KB)
    struct MemorySegment* prev;     // ponteiro para o segmento anterior
    struct MemorySegment* next;     // ponteiro para o próximo segmento
} MemorySegment;


/**
 * Declarações para usar o padrão Strategy e reduzir a quantidade de códigos repetidos
 * antes era um arquivo para cada algoritmo de alocação, sendo que frag_cout() era igual
 * em todos, e alloc_mem() e delloc_mem() tinha muitas partes iguais
 */
typedef enum {
    FIRST_FIT,
    BEST_FIT,
    NEXT_FIT
} AllocAlgorithm;

typedef MemorySegment* (*AllocFunc)(MemorySegment* head, int pages_needed, int* nodes_traversed);
typedef int (*DeallocFunc)(MemorySegment* current, int PID);

/**
 * Função para inicializar a memória
 * - Cria o primeiro segmento livre que representa toda a memória disponível (tamanho de 256 páginas)
 */
void init_memory(AllocAlgorithm strategy);

/**
 * Função para alocar um segmento de memória
 * Parametros:
 *  - PID : O identificador do processo que requisita a memória
 *  - mem_units: A quantidade de unidades de memória requisitadas
 * Retorno:
 *  - Em caso de falha por falta de memória contígua suficiente: -2
 *  - Para as demais falhas: -1
 *  - Em caso de sucesso: nº de nós atravessados na lista até encontrar um segmento adequado
 */
int alloc_mem(int PID, int mem_units);

/**
 * Função para desalocar a memória
 * Parâmetros:
 *  - PID: O identificador do processo que terá sua memória desalocada
 * Retorno:
 *  - Em caso de falha: -1
 *  - Em caso de sucesso: 1
 */
int dealloc_mem(int PID);

/**
 * Função para contar fragmentação externa
 * Retorno:
 *  - Quantidade de fragmentos externos de tamanho 1 ou 2 unidades
 * 
 * A fragmentação externa ocorre quando os segmentos de memória livre estão espalhados, deixando
 * lacunas entre os segmentos alocados. Mesmo que a soma total da memória livre seja suficiente para
 * atender a uma requisição de alocação, a falta de segmentos livres consecuitivos impede a alocação
 */
int frag_count();

/**
 * Função para calcular a média de fragmentação externa
 * Retorno:
 *  - Tamanho médio em páginas de segmentos livres (fragmentação externa)
 * Obs.: Considera todos os segmentos livres para calcular a média
 */
double avg_ext_frag_size();

/**
 * Função para calcular a média de fragmentação interna
 * Retorno:
 *  - Tamanho médio em KB de memória não usada em segmentos alocados (fragmentação interna)
 * 
 * TRECHO DO LIVRO
 * Um segmento de código, dados, ou pilha escolhido ao acaso não ocupará um número inteiro de
 * páginas. Na média, metade da página final estará vazia. O espaço extra nessa página é des-
 * perdiçado. Esse desperdício é chamado de fragmentação interna.
 */
double avg_int_frag_size();

/**
 * Função para imprimir o estado atual da lista
 * Lista todos os segmentos, indicando se estão livres ou alocados, seus tamanhos e fragmentação interna
 */
void print_memory_list();

/**
 * Função para gravar o mapa da memória no arquivo de estatísticas gerais
 * Parâmetros:
 *  - output_file: Ponteiro para o arquivo onde o mapa será gravado
 * Obs.: O mapa da memória representa visualmente os segmentos alocados e livres
 */
void print_memory_map(FILE* output_file);

// Demais declarações para o uso de Strategy
MemorySegment* find_first_fit(MemorySegment* head, int pages_needed, int* nodes_traversed);
MemorySegment* find_best_fit(MemorySegment* head, int pages_needed, int* nodes_traversed);
MemorySegment* find_next_fit(MemorySegment* head, int pages_needed, int* nodes_traversed);
void init_next_fit(MemorySegment* head);
void dealloc_common(MemorySegment* current, int PID); // para o first fit e o best fit
void dealloc_next_fit(MemorySegment* current, int PID);

#endif