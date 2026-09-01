#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "funcoes.h"
#include <time.h>


int mandelbrot_ponto(double c_real, double c_imag, int max_iter){
    double z_real = 0.0;
    double z_imag = 0.0;

    int i;
    for(i= 0; i < max_iter; i++){
        double temp = z_real;
        
        z_real = ((pow(temp, 2) - pow(z_imag, 2)) + c_real);
        z_imag = (2 * temp * z_imag) + c_imag; 
        if((pow(z_real, 2) + pow(z_imag, 2)) > 4){
            return i;
        } 
    }
    return i;
}

void mandelbrot_serial(int altura, int largura, int max_iteracao , int *array_intensidades){
    for (int py = 0; py < altura; py++) {
        for (int px = 0; px < largura; px++) {
            double c_real = -2.0 + ((double)px / (largura - 1)) * (1.0 - (-2.0));
            double c_imag = -1.5 + ((double)py / (altura - 1)) * (1.5 - (-1.5));
            int resultado = mandelbrot_ponto(c_real, c_imag, max_iteracao);
            int intensidade = (int)(((double)resultado / max_iteracao ) * 255);
            array_intensidades[largura * py + px] = intensidade;
        }
    }
}

void mandelbrot_openmp(int altura, int largura, int max_iteracao , int num_threads,int *array_intensidades){
    #pragma omp parallel for num_threads(num_threads)
    for (int py = 0; py < altura; py++) {
        for (int px = 0; px < largura; px++) {
            double c_real = -2.0 + ((double)px / (largura - 1)) * (1.0 - (-2.0));
            double c_imag = -1.5 + ((double)py / (altura - 1)) * (1.5 - (-1.5));
            int resultado = mandelbrot_ponto(c_real, c_imag, max_iteracao);
            int intensidade = (int)(((double)resultado / max_iteracao ) * 255);
            array_intensidades[largura * py + px] = intensidade;
        }
        
    }
}

void *mandelbrot_pthead(void *arg){
    DadosThread *dados = (DadosThread *)arg;
    if(dados == NULL){
        fprintf(stderr,"dados invalidos (NULL)\n");
        exit(1);
    }
    for(int py = dados->py_inicio; py < dados->py_fim; py++){
        for(int px = 0; px < dados->largura; px++){
            double c_real = -2.0 + ((double)px / (dados->largura - 1)) * (1.0 - (-2.0));
            double c_imag = -1.5 + ((double)py / (dados->altura - 1)) * (1.5 - (-1.5));
            int resultado = mandelbrot_ponto(c_real, c_imag, dados->max_iteracao);
            int intensidade = (int)(((double)resultado / dados->max_iteracao ) * 255);
            dados->array_intensidades[dados->largura * py + px] = intensidade;
        }
    }
    return NULL;
}

void *mandelbrot_pthead2(void *arg){
    DadosThread *dados = (DadosThread *)arg;
    if(dados == NULL){
        fprintf(stderr, "erro: dados invalidos (NULL)\n");
        exit(1);
    }
    for(int py = dados->id_thread; py < dados->altura; py += dados->num_threads){
        for(int px = 0; px < dados->largura; px++){
            double c_real = -2.0 + ((double)px / (dados->largura - 1)) * (1.0 - (-2.0));
            double c_imag = -1.5 + ((double)py / (dados->altura - 1)) * (1.5 - (-1.5));
            int resultado = mandelbrot_ponto(c_real, c_imag, dados->max_iteracao);
            int intensidade = (int)(((double)resultado / dados->max_iteracao) * 255);
            dados->array_intensidades[dados->largura * py + px] = intensidade;
        }
    }
    return NULL;
}

void escreve_pgm(int largura, int altura, int *array_intensidades, FILE *file){
    for(int i = 0; i < largura * altura; i++){
        fprintf(file, "%d ", array_intensidades[i]);
        if((i + 1) % largura == 0){
            fprintf(file, "\n");
        }
    }
}