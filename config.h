#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

/*
 *
 * Especificaçoes de memória
 * 
 *  - Tamanho: 1 MB
 *  - Unidade de alocação/Página: 4 KB
 * 
 * Especificaçoes de processo
 * 
 *  - Tamanho: de 3 a 15 unidades de memória
 *  - Unidades de memória: 1 KB
 *  - PID: de 0 a 9999
 */

/**
 * Quantidade de páginas
 * 1 MB / 4 KB = 2^20 / 2^12 = 2^8 = 256
 */
#define TOTAL_PAGES 256
#define PAGE_SIZE 4 // KB

/**
 * Tamanhos mínimo e máximo de um processo/requisição
 */
#define MIN_REQUEST_SIZE 3
#define MAX_REQUEST_SIZE 15
#define MEM_UNIT_SIZE 1 // KB

/**
 * Valor máximo para o PID
 */
#define MAX_PID 9999

/**
 * Número de requisições a serem geradas
 */
#define NUM_REQUESTS 1000

#endif