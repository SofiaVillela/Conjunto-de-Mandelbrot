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

void mandelbrot_serial(int altura, int largura, int max_interacao , int *array_instensidades){
    for (int py = 0; py < altura; py++) {
        for (int px = 0; px < largura; px++) {
            double c_real = -2.0 + ((double)px / (largura - 1)) * (1.0 - (-2.0));
            double c_imag = -1.5 + ((double)py / (altura - 1)) * (1.5 - (-1.5));
            int resultado = mandelbrot_ponto(c_real, c_imag, max_interacao);
            int intensidade = (int)(((double)resultado / max_interacao ) * 255);
            array_instensidades[largura * py + px] = intensidade;
        }
    }
}

void mandelbrot_openmp(int altura, int largura, int max_interacao ,int *array_instensidades){
    #pragma omp parallel for
    for (int py = 0; py < altura; py++) {
        for (int px = 0; px < largura; px++) {
            double c_real = -2.0 + ((double)px / (largura - 1)) * (1.0 - (-2.0));
            double c_imag = -1.5 + ((double)py / (altura - 1)) * (1.5 - (-1.5));
            int resultado = mandelbrot_ponto(c_real, c_imag, max_interacao);
            int intensidade = (int)(((double)resultado / max_interacao ) * 255);
            array_instensidades[largura * py + px] = intensidade;
        }
        
    }
}

void escreve_pgm(int largura, int altura, int *array_intensidades, FILE *file){
    for(int i = 0; i < largura * altura; i++){
        fprintf(file, "%d ", array_intensidades[i]);
        if((i + 1) % largura == 0){
            fprintf(file, "\n");
        }
    }
}