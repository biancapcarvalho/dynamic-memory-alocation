#ifndef CONFIG_H
#define CONFIG_H

/*
!!!! VERIFICAR COM O PROF SE A UNIDADE DE MEMÓRIA É DE 1 OU 4 KB !!!!
 *
 * Especificaçoes de memória
 * 
 *  - Tamanho: 1 MB
 *  - Unidade de memória: 4 KB
 * 
 * Especificaçoes de processo
 * 
 *  - Tamanho: de 3 a 15 unidades de memória
 *  - PID: de 0 a 9999
 */

/**
 * Quantidade de unidades de memória
 * 1 MB / 4 KB = 2^20 / 2^12 = 2^8 = 256
 */
#define TOTAL_MEMORY_UNITS 256

/**
 * Tamanho mínimo de um processo/requisição
 */
#define MIN_REQUEST_SIZE 3

/**
 * Tamanho máximo de um processo/requisição
 */
#define MAX_REQUEST_SIZE 15

/**
 * Valor máximo para o PID
 */
#define MAX_PID 9999

#define TRUE 1
#define FALSE 0
typedef int bool;

#endif