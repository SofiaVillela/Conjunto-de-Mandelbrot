CC = gcc

CFLAGS = -Wall -lm -fopenmp -lpthread

LOGIN = svv

all: mandelbrot

mandelbrot: main.c funcoes.c funcoes.h

	$(CC) main.c funcoes.c -o mandelbrot $(CFLAGS)

clean:

	rm -f mandelbrot mandelbrot_$(LOGIN)_*.pgm times.txt
