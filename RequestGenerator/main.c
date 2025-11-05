#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "request_generator.h"

int main() {
    srand(time(NULL));
    FILE* low_load_file = init_request_file("RequestGenerator/carga_baixa.txt");
    generation_loop(low_load_file, 25);
    fclose(low_load_file);

    FILE* medium_load_file = init_request_file("RequestGenerator/carga_media.txt");
    generation_loop(medium_load_file, 50);
    fclose(medium_load_file);

    FILE* high_load_file = init_request_file("RequestGenerator/carga_alta.txt");
    generation_loop(high_load_file, 75);
    fclose(high_load_file);

    fprintf(stderr, "\n\n*** Arquivos gerados: carga_baixa.txt, carga_media.txt, carga_alta.txt ***\n");
    return 0;
}