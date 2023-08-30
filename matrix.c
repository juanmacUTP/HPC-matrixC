#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

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

void multiplyMatrixes(int *n, int *verbose){
    int i, j, k;

    srand(time(0));
    clock_t t;
    t = clock();
    
    int **a = (int **)malloc(*n * sizeof(int *));
    for (i = 0; i < *n; i++) {
        a[i] = (int *)malloc(*n * sizeof(int));
    }
   
    int **b = (int **)malloc(*n * sizeof(int *));
    for (i = 0; i < *n; i++) {
        b[i] = (int *)malloc(*n * sizeof(int));
	}
   
    int **axb = (int **)malloc(*n * sizeof(int *));
    for (i = 0; i < *n; i++) {
        axb[i] = (int *)malloc(*n * sizeof(int));
   }
    
		//filling the matrix  
    fillMatrix(a, n);
    fillMatrix(b, n);
    
   

    for (i = 0; i < *n; i++) {
        for (j = 0; j < *n; j++) {
            axb[i][j] = 0;
            for (k = 0; k < *n; k++) {
                axb[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds

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
	
}


int main(int argc, char *argv[]){
int n = atoi(argv[1]);

int verbose = 0;

if (argc == 3 && strcmp(argv[2], "verbose") == 0) {
        verbose = 1;
    }

multiplyMatrixes(&n, &verbose);


}

