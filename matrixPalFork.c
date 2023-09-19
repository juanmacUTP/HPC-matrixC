#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Estructura para pasar argumentos a los procesos
typedef struct {
    int id;           // process ID
    int **a;          // Matrix A
    int **b;          // Matrix B
    int **result;     // result Matrix 
    int n;            // matrix size
    int num_processes; // total number of process
    int verbose;      // verbose parameter
} ProcessArgs;


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
void multiplyMatrixSegment(ProcessArgs *processArgs) {
    int id = processArgs->id;
    int n = processArgs->n;
    int num_processes = processArgs->num_processes;
    int verbose = processArgs->verbose;

    int start_row = id * (n / num_processes);
    int end_row = (id == num_processes - 1) ? n : (id + 1) * (n / num_processes);

    clock_t t;
    t = clock();

    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < n; j++) {
            processArgs->result[i][j] = 0;
            for (int k = 0; k < n; k++) {
                processArgs->result[i][j] += processArgs->a[i][k] * processArgs->b[k][j];
            }
        }
    }

    t = clock() - t;
    double time_taken = ((double)t) / CLOCKS_PER_SEC; // Tiempo en segundos

    if (verbose) {
        printf("Hilo %d:\n", id);
        printf("El tiempo de ejecución fue de %f segundos\n", time_taken);
        printf("Matriz resultante:\n");
        printMatrix(processArgs->result, &n);
    } else {
        printf("Hilo %d: El tiempo de ejecución fue de %f segundos\n", id, time_taken);
    }
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


    

    // Create processes
    pid_t child_pids[MAX_PROCESSES];
    ProcessArgs processArgs[MAX_PROCESSES];

    for (int i = 0; i < MAX_PROCESSES; i++) {
        // Create processes son
        pid_t child_pid = fork();

        if (child_pid == 0) {
            // process son
            processArgs[i].id = i;
            processArgs[i].a = a;
            processArgs[i].b = b;
            processArgs[i].result = axb;
            processArgs[i].n = n;
            processArgs[i].num_processes = MAX_PROCESSES;

            multiplyMatrixSegment(&processArgs[i]);
            exit(0); // Exit the child process after completing its work
        } else if (child_pid > 0) {
            // Process fahter: to save the PID of process son
            child_pids[i] = child_pid;
        } else {
            // Error to create process son
            fprintf(stderr, "Error al crear el proceso hijo.\n");
            return 1;
        }
    }

	// Wait for child processes to finish
    for (int i = 0; i < MAX_PROCESSES; i++) {
        waitpid(child_pids[i], NULL, 0);
    }

	//matrix print if verbose parameter is present
    if(verbose){
		printf("The matrix a is: \n");
  		printMatrix(a, &n);
		printf("The matrix b is: \n");
		printMatrix(b, &n);
    	printf("The product of the two matrices is: \n");
    	printMatrix(axb, &n);
    }
    
    //free memory
    free(a);
    free(b);
    free(axb);


    return 0;
}

