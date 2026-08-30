#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "funcoes.h"
#include <time.h>

enum { LARGURA, ALTURA, MAX_ITERACOES ,NUM_THREADS};
struct timespec inicio, fim;


int main(int argc, char **argv){
    if(argc != 5){
        fprintf(stderr, "erro: quantidade insuficiente de argumentos.\n");
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

    int *array_instensidades = malloc(array_numeros[LARGURA] * array_numeros[ALTURA] * sizeof(int));
    if(array_instensidades == NULL){
        fprintf(stderr, "erro: nao foi possivel alocar memoria para o array\n");
        exit(1);
    }
    FILE *file_time = fopen("times.txt", "a");
    if(file_time == NULL){
        fprintf(stderr, "erro: nao foi possivel abrir o arquivo\n");
        exit(1);
    }

    FILE *file_serial = fopen("mandelbrot_svv_serial.pgm", "w");
    if(file_serial == NULL){
        fprintf(stderr, "erro: não foi possivel abrir o arquivo.\n");
        exit(1);
    }

    clock_gettime(CLOCK_MONOTONIC, &inicio);
    mandelbrot_serial(array_numeros[ALTURA], array_numeros[LARGURA], array_numeros[MAX_ITERACOES], array_instensidades);
    clock_gettime(CLOCK_MONOTONIC, &fim);
    escreve_pgm(array_numeros[LARGURA], array_numeros[ALTURA], array_instensidades, file_serial);
    fclose(file_serial);
    double tempo_serial = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    fprintf(file_time, "%lf ", tempo_serial);


    FILE *file_openmp = fopen("mandelbrot_svv_openmp.pgm", "w");
    if(file_openmp == NULL){
        fprintf(stderr, "erro: não foi possivel abrir o arquivo.\n");
        exit(1);
    }
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    mandelbrot_openmp(array_numeros[ALTURA], array_numeros[LARGURA], array_numeros[MAX_ITERACOES], array_instensidades);
    clock_gettime(CLOCK_MONOTONIC, &fim);
    escreve_pgm(array_numeros[LARGURA], array_numeros[ALTURA], array_instensidades, file_openmp);
    fclose(file_openmp);
    double tempo_openmp = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    fprintf(file_time, "%lf ", tempo_openmp);
    
    fclose(file_time);  

    return 0;
}
