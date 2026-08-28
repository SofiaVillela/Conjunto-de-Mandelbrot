#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum { LARGURA, ALTURA, MAX_INTERACOES ,NUM_THREADS};

int main(int argc, char **argv){
    if(argc != 5){
        fprintf(stderr, "erro, quantidade insuficiente de argumentos.\n");
        exit(1);
    }
    
    int array_numeros[4];
    for(int i = 0, j = 1; i < 4; i++, j++){
        array_numeros[i] = atoi(argv[j]);
        if(array_numeros[i] <= 0){
            printf("teste menor/igual a 0: %d", array_numeros[i]);
            exit(1);
        }
    }

    for(int i = 0; i < 4; i++){
        printf("%d ", array_numeros[i]);
    }
    return 0;
}
