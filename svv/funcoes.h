#ifndef FUNCOES_H
#define FUNCOES_H

int mandelbrot_ponto(double c_real, double c_imag, int max_iter);
void mandelbrot_serial(int altura, int largura, int max_interacao ,int *array_intensidades);
void mandelbrot_openmp(int altura, int largura, int max_interacao ,int *array_instensidades);
void escreve_pgm(int largura, int altura, int *array_intensidades, FILE *file);
#endif