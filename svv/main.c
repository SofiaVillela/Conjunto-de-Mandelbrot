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
    FILE *file = fopen("mandelbrot_svv_serial.pgm", "w");
    if(file == NULL){
        fprintf(stderr, "erro: não foi possivel abrir o arquivo.\n");
        exit(1);
    }
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    for (int py = 0; py < array_numeros[ALTURA]; py++) {
        for (int px = 0; px < array_numeros[LARGURA]; px++) {
            double c_real = -2.0 + ((double)px / (array_numeros[LARGURA] - 1)) * (1.0 - (-2.0));
            double c_imag = -1.5 + ((double)py / (array_numeros[ALTURA] - 1)) * (1.5 - (-1.5));
            int resultado = mandelbrot_ponto(c_real, c_imag, array_numeros[MAX_ITERACOES]);
            int intensidade = (int)(((double)resultado / array_numeros[MAX_ITERACOES] ) * 255);
            fprintf(file, "%d " ,intensidade);
        }
        fprintf(file, "\n");
    }
    clock_gettime(CLOCK_MONOTONIC, &fim);
    fclose(file);

    double tempo_gasto = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    
    FILE *file_time = fopen("times.txt", "a");
    if(file_time == NULL){
        fprintf(stderr, "erro: nao foi possivel abrir o arquivo");
        exit(1);
    }
    fprintf(file_time, "%lf ", tempo_gasto);
    fclose(file_time);
    return 0;
}
