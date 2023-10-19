#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>
#include <ctype.h>
#include "matrixMultiplication/matrixMultiplication.h"
#include "matrixMultiplication.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>



void multiplyMatrixesSecuencial(const int *size,const int *verbose);
void multiplyMatrixesOmp(const int *size,const int *verbose);
int review_omp_threads (void);

int main(int argc, char *argv[]){
    const int size = strtol(argv[1], NULL, 10);
    int verbose = 0;

    if (argc == 3 && strcmp(argv[3], "verbose") == 0) {
        verbose = 1;
    }

//    printf("pthreads: %d", num_threads);
    multiplyMatrixesSecuencial(&size, &verbose);
//    multiplyMatrixesOmp(&size, &verbose);
//    review_omp_threads();

    return 0;
}




int randomNumber(){
    int upper = 100, lower = 1;
    int num = (rand() % (upper - lower + 1)) + lower;
    return num;
}

void multiplyMatrixesSecuencial(const int *size,const int *verbose){
    clock_t t;

    //Dynamic memory allocation
    int n, i, j, k;
    int(*a)[*size][*size] = malloc(sizeof *a);
    int(*b)[*size][*size] = malloc(sizeof *b);
    int(*c)[*size][*size] = malloc(sizeof *c);


    // Fill the matrix with random numbers
    for (i = 0; i < *size; i++) {
        for (j = 0; j < *size; j++) {
            (*a)[i][j] = randomNumber();
            (*b)[i][j] = randomNumber();
        }
    }
    // Multiply th matrixes
    t = clock();
    for (i = 0; i < *size; i++) {
        for (j = 0; j < *size; j++) {
            (*c)[i][j] = 0;
            for (k = 0; k < *size; k++) {
                (*c)[i][j] += (*a)[i][k] * (*b)[k][j];
            }
        }
    }
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

    // Showing information if verbose
    if(*verbose){
        printf("The matrix a is: \n");
        for (i = 0; i < *size; i++) {
            for (j = 0; j < *size; j++) {
                printf("%d\t", (*a)[i][j]);
            }
            printf("\n");
        }
        printf("The matrix b is: \n");
        for (i = 0; i < *size; i++) {
            for (j = 0; j < *size; j++) {
                printf("%d\t", (*b)[i][j]);
            }
            printf("\n");
        }
        printf("The matrix c is: \n");
        for (i = 0; i < *size; i++) {
            for (j = 0; j < *size; j++) {
                printf("%d\t", (*c)[i][j]);
            }
            printf("\n");
        }
    }

    printf("%d:%f\n",
           *size,time_taken);
    free(a);
    free(b);
    free(c);
}

void multiplyMatrixesOmp(const int *size,const int *verbose){
    clock_t t;

    //Dynamic memory allocation
    int n, i, j, k;
    int(*a)[*size][*size] = malloc(sizeof *a);
    int(*b)[*size][*size] = malloc(sizeof *b);
    int(*c)[*size][*size] = malloc(sizeof *c);


    // Fill the matrix with random numbers
    for (int i = 0; i < *size; i++) {
        for (int j = 0; j < *size; j++) {
            (*a)[i][j] = randomNumber();
            (*b)[i][j] = randomNumber();
        }
    }
    // Multiply th matrixes
    t = clock();
    #pragma omp parallel for default(none) shared(size, a, b, c)
    for  (int i = 0; i < *size; i++) {
        for (int j = 0; j < *size; j++) {
            (*c)[i][j] = 0;
            for (int k = 0; k < *size; k++) {
                (*c)[i][j] += (*a)[i][k] * (*b)[k][j];
            }
        }
    }
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

    // Showing information if verbose
    if(*verbose){
        printf("The matrix a is: \n");
        for (i = 0; i < *size; i++) {
            for (j = 0; j < *size; j++) {
                printf("%d\t", (*a)[i][j]);
            }
            printf("\n");
        }
        printf("The matrix b is: \n");
        for (i = 0; i < *size; i++) {
            for (j = 0; j < *size; j++) {
                printf("%d\t", (*b)[i][j]);
            }
            printf("\n");
        }
        printf("The matrix c is: \n");
        for (i = 0; i < *size; i++) {
            for (j = 0; j < *size; j++) {
                printf("%d\t", (*c)[i][j]);
            }
            printf("\n");
        }
    }

    printf("%d:%f\n",
           *size,time_taken);
    free(a);
    free(b);
    free(c);
}



int review_omp_threads (void)
{
    omp_set_nested(1);
    omp_set_dynamic(0);
#pragma omp parallel
    {
#pragma omp parallel
        {
#pragma omp single
            {
                /*
                * If OMP_NUM_THREADS=2,3 was set, the following should print:
                * Inner: num_thds=3
                * Inner: num_thds=3
                *
                * If nesting is not supported, the following should print:
                * Inner: num_thds=1
                * Inner: num_thds=1
                */
                printf ("Inner: num_thds=%d\n", omp_get_num_threads());
            }
        }
#pragma omp barrier
        omp_set_nested(0);
#pragma omp parallel
        {
#pragma omp single
            {
                /*
                * Even if OMP_NUM_THREADS=2,3 was set, the following should
                * print, because nesting is disabled:
                * Inner: num_thds=1
                * Inner: num_thds=1
                */
                printf ("Inner: num_thds=%d\n", omp_get_num_threads());
            }
        }
#pragma omp barrier
#pragma omp single
        {
            /*
            * If OMP_NUM_THREADS=2,3 was set, the following should print:
            * Outer: num_thds=2
            */
            printf ("Outer: num_thds=%d\n", omp_get_num_threads());
        }
    }
    return 0;
}

