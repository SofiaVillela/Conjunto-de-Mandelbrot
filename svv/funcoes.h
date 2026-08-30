#ifndef FUNCOES_H
#define FUNCOES_H

int mandelbrot_ponto(double c_real, double c_imag, int max_iter);
void mandelbrot_serial(int altura, int largura, int max_interacao ,FILE *file);
#endif