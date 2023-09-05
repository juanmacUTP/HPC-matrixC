#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

#define MAX_THREADS 4 //number of threads

// structur to send args to the threads
typedef struct {
    int id;           // thread ID
    int **a;          // Matrix a
    int **b;          // Matrix b
    int **result;     // Matrix result
    int n;            // matrix size
    int num_threads;  // total number of threads
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

// Function to multiply a portion of matrices and measure time
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
    double time_taken = ((double)t) / CLOCKS_PER_SEC; // Time in seconds
    printf("(%d)Thread# %d: the multiplication took %f seconds to execute\n",n, id, time_taken);
    

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
   
    int verbose = 0;

if (argc == 3 && strcmp(argv[2], "verbose") == 0) {
        verbose = 1;
    }

    // Create matrixes a, b & result
    int **a = (int **)malloc(n * sizeof(int *));
    int **b = (int **)malloc(n * sizeof(int *));
    int **axb = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        a[i] = (int *)malloc(n * sizeof(int));
        b[i] = (int *)malloc(n * sizeof(int));
        axb[i] = (int *)malloc(n * sizeof(int));
    }

    //filling the matrix  
    fillMatrix(a, &n);
    fillMatrix(b, &n);

    // Create threads
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

    // wait for the threads finish
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // print results
    if(verbose){
		printf("The matrix a is: \n");
  		printMatrix(a, &n);
		printf("The matrix b is: \n");
		printMatrix(b, &n);
    	printf("The product of the two matrices is: \n");
    	printMatrix(axb, &n);
    }
    free(a);
    free(b);
    free(axb);

    return 0;
}

