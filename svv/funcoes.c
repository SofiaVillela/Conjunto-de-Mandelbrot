#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "funcoes.h"



int mandelbrot_ponto(double c_real, double c_imag, int max_iter){
    double z_real = 0.0;
    double z_imag = 0.0;

    int i;
    for(i= 0; i < max_iter; i++){
        z_real = ((pow(z_real, 2) - pow(z_imag, 2)) + c_real);
        z_imag = (2 * z_real * z_imag) * c_imag; 
        
    }
    return i;
}