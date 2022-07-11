/* 
A parallel calculation for Matrix-Matrix multiplication

Compile: gcc a.c -o a
Run: ./a <size_of_matrix>

Code was tested using AMD Ryzen7 3700x 8-Core Processor and Ubuntu 20.04 as OS  
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char * argv[]){
    
    struct timespec ts_start, ts_end; //start and end time local variables
    double time_total; //This is for total calculation time
    
    double **A; // Declare double pointer for matrix A
    double **B; // Declare double pointer for matrix B
    double **C; // Declare double pointer for matrix C
    int dim= atoi(argv[1]); // Declare and set the dimension
            
    int i,j,k; // This is the number for "for" loop
    
    A = ( double ** ) malloc ( dim * sizeof( double * ) ) ; // Allocate the memory for the matrix A
    B = ( double ** ) malloc ( dim * sizeof( double * ) ) ; // Allocate the memory for the matrix B
    C = ( double ** ) malloc ( dim * sizeof( double * ) ) ; // Allocate the memory for the matrix C
    
    for (i=0; i<dim; i++){
    	A[i] = ( double * ) malloc ( dim * sizeof( double ) );
    	B[i] = ( double * ) malloc ( dim * sizeof( double ) );
    	C[i] = ( double * ) malloc ( dim * sizeof( double ) );    
    }

    srand(time(NULL)); // Generate different seed
    
// Fill the matrices 
    for (i=0; i<dim; i++){
            double sum1=0;
            double sum2=0;
            for (j=0; j<dim; j++){
                    A[i][j] = 0;
                    B[i][j] = 0;
                    C[i][j] = 0;
                    if (i<j){
                        A[i][j] = -(rand()%9+1);
                        B[i][j] = -(rand()%9+1);
                        }
                    if (i>j){
                        A[i][j] = A[j][i];
                        B[i][j] = B[j][i];
                        }
                    sum1 += A[i][j];
                    sum2 += B[i][j];                   
            }
            A[i][i] = abs(sum1);//Setting the matrix A
            B[i][i] = abs(sum2);//Setting the matrix B
    } 
    
/*    printf(" \n The matrix,A, is \n");
    for (i=0; i<dim; i++){
            for (j=0; j<dim; j++){
            printf(" %f ",A[i][j]);                    
            }
    printf("\n"); 
    } // Show the matrix
    
        printf(" \n The matrix,B, is \n");
    for (i=0; i<dim; i++){
            for (j=0; j<dim; j++){
            printf(" %f ",B[i][j]);                    
            }
    printf("\n"); 
    } // Show the matrix */ 
    
    clock_gettime(CLOCK_MONOTONIC, &ts_start); // Get start time
    
// Calculation of the matrix-matrix multiplication
    for (i=0; i<dim; i++){
	    for (j=0; j<dim; j++){
	    	for (k=0; k<dim; k++){
	    		C[i][j] = C[i][j] + A[i][k] * B[k][j];
	    	}
	    }
    }
    
/*    printf(" \n The matrix,C, is \n");
    for (i=0; i<dim; i++){
            for (j=0; j<dim; j++){
            printf(" %f ",C[i][j]);                    
            }
    printf("\n"); 
    } // Show the matrix */   

    clock_gettime(CLOCK_MONOTONIC, &ts_end); // Get end time
    
    // Time calculation
    time_total = (ts_end.tv_sec - ts_start.tv_sec) * 1e9 + (ts_end.tv_nsec - ts_start.tv_nsec);
    printf("Total time is %f ms\n", time_total / 1e6);
}
