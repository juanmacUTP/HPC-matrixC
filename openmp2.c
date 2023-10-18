#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>

int randomNumber() {
    int upper = 100, lower = 1;
    int num = (rand() % (upper - lower + 1)) + lower;
    return num;
}

void multiplyMatrixesV3(const int *size, const int *verbose) {
    clock_t t;

    // Dynamic memory allocation
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

    // Multiply the matrices using OpenMP parallelization
    t = clock();
#pragma omp parallel for private(j, k)
    for (i = 0; i < *size; i++) {
        for (j = 0; j < *size; j++) {
            (*c)[i][j] = 0;
            for (k = 0; k < *size; k++) {
                (*c)[i][j] += (*a)[i][k] * (*b)[k][j];
            }
        }
    }
    t = clock() - t;
    double time_taken = ((double)t) / CLOCKS_PER_SEC; // in seconds

    // Showing information if verbose
    if (*verbose) {
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

    printf("(%d) The multiplication took %f seconds to execute \n", *size, time_taken);
    free(a);
    free(b);
    free(c);
}

int main(int argc, char *argv[]) {
    const int size = strtol(argv[1], NULL, 10);
    int verbose = 0;

    if (argc == 3 && strcmp(argv[2], "verbose") == 0) {
        verbose = 1;
    }

    multiplyMatrixesV3(&size, &verbose);

    return 0;
}
