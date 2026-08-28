#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "funcoes.h"


enum { LARGURA, ALTURA, MAX_ITERACOES ,NUM_THREADS};

int main(int argc, char **argv){
    if(argc != 5){
        fprintf(stderr, "erro, quantidade insuficiente de argumentos.\n");
        exit(1);
    }
    char *endptr;
    int array_numeros[4];
    for(int i = 0, j = 1; i < 4; i++, j++){
        array_numeros[i] =  strtol(argv[j], &endptr, 10);
        if(*endptr != '\0'){
            fprintf(stderr, "teste de erro de caracter diferente de numero\n");
            exit(1);
        }
        if(array_numeros[i] <= 0){
            fprintf(stderr,"menor/igual a 0: %d\n", array_numeros[i]);
            exit(1);
        }
    }
    int resultado = mandelbrot_ponto(0.0, 0.0, 10);
    printf("resultado: %d\n", resultado);
    return 0;
}
