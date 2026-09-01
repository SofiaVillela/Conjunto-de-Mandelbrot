Conjunto de Mandelbrot 


Programa em C que gera uma imagem do conjunto de Mandelbrot em quatro versões: serial, OpenMP e duas implementações com Pthreads usando estratégias de divisão de trabalho diferentes, medindo e comparando o tempo de execução de cada uma.


## Compilar:
make

## Ou manualmente
gcc main.c funcoes.c -o mandelbrot -lm -fopenmp -lpthread

## Para limpar o binário e os arquivos gerados:
make clean

## Executar
./mandelbrot [largura] [altura] [max_iteracoes] [num_threads]

## Exemplo:
./mandelbrot 200 200 500 4


## Saídas geradas
Arquivo	                    Descrição
mandelbrot_svv_serial.pgm	    Imagem gerada pela versão serial
mandelbrot_svv_openmp.pgm	    Imagem gerada pela versão OpenMP
mandelbrot_svv_pthreads1.pgm	Imagem gerada pela versão Pthreads 1 
mandelbrot_svv_pthreads2.pgm	Imagem gerada pela versão Pthreads 2
times.txt	                    Tempo de execução de cada uma das quatro versões em ordem

### As quatro imagens .pgm devem ser idênticas entre si — pode ser conferido com:

diff mandelbrot_svv_serial.pgm mandelbrot_svv_openmp.pgm
diff mandelbrot_svv_serial.pgm mandelbrot_svv_pthreads1.pgm
diff mandelbrot_svv_serial.pgm mandelbrot_svv_pthreads2.pgm

(sem saída = arquivos idênticos)

## Estrutura do projeto
svv/
├── main.c
# valida argumentos, executa as 4 versões, mede tempo, escreve saídas
├── funcoes.c
# cálculo do Mandelbrot e as 4 implementações + escrita do .pgm
├── funcoes.h   
# declarações de funções e struct DadosThread (usada pelas Pthreads)
├── Makefile    
# regras de compilação (all) e limpeza (clean)
├── evidencias.log  
# log de testes gerado com `script -a evidencias.log`
└── svv.pdf       
# relatório da implementação

## Validação de erros
O programa rejeita e informa erro (via stderr) nos seguintes casos:

Quantidade incorreta de argumentos
Argumento não numérico (ex.: texto no lugar de número)
Argumento numérico menor ou igual a zero
Falha ao abrir/criar arquivo de saída
Falha na alocação de memória
