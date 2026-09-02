#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "funcoes.h"
#include <time.h>
#include <pthread.h>

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
    FILE *file_time = fopen("times.txt", "w");
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
    mandelbrot_openmp(array_numeros[ALTURA], array_numeros[LARGURA],array_numeros[MAX_ITERACOES], array_numeros[NUM_THREADS], array_instensidades);
    clock_gettime(CLOCK_MONOTONIC, &fim);
    escreve_pgm(array_numeros[LARGURA], array_numeros[ALTURA], array_instensidades, file_openmp);
    fclose(file_openmp);
    double tempo_openmp = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    fprintf(file_time, "%lf ", tempo_openmp);
    
     
    
    pthread_t threads[array_numeros[NUM_THREADS]];
    DadosThread dados_threads[array_numeros[NUM_THREADS]];
    
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    for (int t = 0; t < array_numeros[NUM_THREADS]; t++) {
        dados_threads[t].py_inicio = t * (array_numeros[ALTURA] / array_numeros[NUM_THREADS]);
        
        if (t == array_numeros[NUM_THREADS] - 1) {
            dados_threads[t].py_fim = array_numeros[ALTURA];  
        } else {
            dados_threads[t].py_fim = (t + 1) * (array_numeros[ALTURA] / array_numeros[NUM_THREADS]);
        }
        
        dados_threads[t].largura = array_numeros[LARGURA];
        dados_threads[t].altura = array_numeros[ALTURA];
        dados_threads[t].max_iteracao = array_numeros[MAX_ITERACOES];
        dados_threads[t].array_intensidades = array_instensidades;
        
        pthread_create(&threads[t], NULL, mandelbrot_pthead, &dados_threads[t]);
    }

    for(int i = 0; i < array_numeros[NUM_THREADS]; i++){  
        pthread_join(threads[i], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &fim);

    FILE *file_pthreads1 = fopen("mandelbrot_svv_pthreads1.pgm", "w");
    if(file_pthreads1 == NULL){
        fprintf(stderr, "erro: não foi possivel abrir o arquivo.\n");
        exit(1);
    }
    
    escreve_pgm(array_numeros[LARGURA], array_numeros[ALTURA], array_instensidades, file_pthreads1);
    fclose(file_pthreads1);
    double tempo_pthreads1 = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    fprintf(file_time, "%lf ", tempo_pthreads1);
    
    int *array_resultado = malloc(array_numeros[LARGURA] * array_numeros[ALTURA] * sizeof(int));
    if(array_resultado == NULL){
        fprintf(stderr, "erro: nao foi possivel alocar memoria para o array\n");
        exit(1);
    }

    FILE *file_pthreads2 = fopen("mandelbrot_svv_pthreads2.pgm", "w");
    if(file_pthreads2 == NULL){
        fprintf(stderr, "erro: nao foi possivel abrir o arquivo.\n");
        exit(1);
    }

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    mandelbrot_calcula_bruto(array_numeros[ALTURA], array_numeros[LARGURA], array_numeros[MAX_ITERACOES], array_resultado);

    int total_pixels = array_numeros[LARGURA] * array_numeros[ALTURA];
    pthread_t threads2[array_numeros[NUM_THREADS]];
    DadosNormalizacao dados_norm[array_numeros[NUM_THREADS]];

    for (int t = 0; t < array_numeros[NUM_THREADS]; t++) {
        dados_norm[t].inicio = t * (total_pixels / array_numeros[NUM_THREADS]);
        if (t == array_numeros[NUM_THREADS] - 1) {
            dados_norm[t].fim = total_pixels;
        } else {
            dados_norm[t].fim = (t + 1) * (total_pixels / array_numeros[NUM_THREADS]);
        }
        dados_norm[t].max_iter = array_numeros[MAX_ITERACOES];
        dados_norm[t].array_resultado = array_resultado;
        dados_norm[t].array_intensidade = array_instensidades;

        pthread_create(&threads2[t], NULL, mandelbrot_pthead2, &dados_norm[t]);
    }

    for(int i = 0; i < array_numeros[NUM_THREADS]; i++){
        pthread_join(threads2[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    escreve_pgm(array_numeros[LARGURA], array_numeros[ALTURA], array_instensidades, file_pthreads2);
    fclose(file_pthreads2);

    double tempo_pthreads2 = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    fprintf(file_time, "%lf ", tempo_pthreads2);
    

    fclose(file_time);  
    
    return 0;
}
