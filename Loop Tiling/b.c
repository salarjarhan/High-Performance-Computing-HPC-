/* 
A parallel calculation for Matrix-Matrix multiplication using block size

Compile: gcc b.c -o b
Run: ./b <size_of_matrix> <block_size>

Best range for block size based on my L1 cache is 20 to 25

Code was tested using AMD Ryzen7 3700x 8-Core Processor and Ubuntu 20.04 as OS 
L1 cache: 512 KB
L2 cache: 4.0 MB 
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
    int s= atoi(argv[2]); // Declare and set the block size
        
    int i,j,k,ii,jj,kk; // This is the number for "for" loop
    
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

// Calculation of the matrix-matrix multiplication with a block size
    for (ii=0; ii<dim; ii+=s){
    	for (jj=0; jj<dim; jj+=s){
    		for (kk=0; kk<dim; kk+=s){
    			for (i=ii; i<ii+s; i++){
    				for (j=jj; j<jj+s; j++){
    					for (k=kk; k<kk+s; k++){
    						C[i][j] = C[i][j] + A[i][k] * B[k][j];
    					}
    				}
    			}
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
