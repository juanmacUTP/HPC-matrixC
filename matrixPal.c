#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

#define MAX_THREADS 4 // Ajusta el número de hilos según tus necesidades

// Estructura para pasar argumentos a los hilos
typedef struct {
    int id;           // ID del hilo
    int **a;          // Matriz A
    int **b;          // Matriz B
    int **result;     // Matriz resultado
    int n;            // Tamaño de las matrices
    int num_threads;  // Número total de hilos
} ThreadArgs;

//random number generation
int randomNumber(){
    int upper = 100, lower = 1;
    int num = (rand() % (upper - lower + 1)) + lower;
    return num;
}

void fillMatrix(int **matrixPointer, int *n){
   
    int i, j;
    for (i = 0; i < *n; i++) {
        for (j = 0; j < *n; j++) {
            matrixPointer[i][j] = randomNumber();
        }
    }
}



void printMatrix(int **matrixPointer, int *n){
   
    int i, j;
   
   
    for (i = 0; i < *n; i++) {
        for (j = 0; j < *n; j++) {
            printf("%d\t", matrixPointer[i][j]);
        }
        printf("\n");
    }

}

// Función para multiplicar una porción de las matrices y medir el tiempo
void *multiplyMatrixSegment(void *args) {
    ThreadArgs *threadArgs = (ThreadArgs *)args;
    int id = threadArgs->id;
    int n = threadArgs->n;
    int num_threads = threadArgs->num_threads;

    int start_row = id * (n / num_threads);
    int end_row = (id == num_threads - 1) ? n : (id + 1) * (n / num_threads);

    clock_t t;
    t = clock();

    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < n; j++) {
            threadArgs->result[i][j] = 0;
            for (int k = 0; k < n; k++) {
                threadArgs->result[i][j] += threadArgs->a[i][k] * threadArgs->b[k][j];
            }
        }
    }

    t = clock() - t;
    double time_taken = ((double)t) / CLOCKS_PER_SEC; // Tiempo en segundos

    printf("Hilo %d: El tiempo de ejecución fue de %f segundos\n", id, time_taken);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    
    int verbose = 0;

	if (argc == 3 && strcmp(argv[2], "verbose") == 0) {
        verbose = 1;
    }

    // Crear matrices a, b y resultado
    int **a = (int **)malloc(n * sizeof(int *));
    int **b = (int **)malloc(n * sizeof(int *));
    int **axb = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        a[i] = (int *)malloc(n * sizeof(int));
        b[i] = (int *)malloc(n * sizeof(int));
        axb[i] = (int *)malloc(n * sizeof(int));
    }

    //filling the matrix  
    fillMatrix(a, n);
    fillMatrix(b, n);

    // Crear hilos
    pthread_t threads[MAX_THREADS];
    ThreadArgs threadArgs[MAX_THREADS];

    for (int i = 0; i < MAX_THREADS; i++) {
        threadArgs[i].id = i;
        threadArgs[i].a = a;
        threadArgs[i].b = b;
        threadArgs[i].result = axb;
        threadArgs[i].n = n;
        threadArgs[i].num_threads = MAX_THREADS;

        pthread_create(&threads[i], NULL, multiplyMatrixSegment, &threadArgs[i]);
    }

    // Esperar a que los hilos terminen
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Imprimir resultados (sin cambios)
    if(*verbose){
	printf("The matrix a is: \n");
  	printMatrix(a, n);
	printf("The matrix b is: \n");
	printMatrix(b, n);			
    	printf("The product of the two matrices is: \n");
    	printMatrix(axb, n);
    }
    printf("(%d) the multiplication took %f seconds to execute \n", *n,time_taken);
    free(a);
    free(b);
    free(axb);
	

    return 0;
}

