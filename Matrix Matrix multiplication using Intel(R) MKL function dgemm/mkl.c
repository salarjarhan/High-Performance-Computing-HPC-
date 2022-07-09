/* 
A parallel calculation for Matrix Matrix multiplication using Intel(R) MKL function dgemm

This example computes real matrix C=alpha*A*B+beta*C

Compile: icc -mkl mkl.c
Run: ./a.out <number of threads> <size_of_matrix>

Code was tested using AMD Ryzen7 3700x 8-Core Processor and Ubuntu 20.04 as OS  
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mkl.h"

int main(int argc, char * argv[]){
    
    double s_elapsed, s_initial;//This is for the time calculation
    double **A, *A1; // Declare double pointer for matrix A
    double **B, *B1; // Declare double pointer for matrix B
    double *C; // Declare double pointer for matrix C
    int thread_count = strtol(argv[1], NULL, 10); // Declare and set the thread number
    int dim= atoi(argv[2]); // Declare and set the dimension
    double alpha = 1.0, beta = 0.0; //Real values used to scale matrices   
    int i,j,k; // This is the number for "for" loop
    
    A = ( double ** ) malloc ( dim * sizeof( double * ) ) ; // Allocate the memory for the matrix A
    B = ( double ** ) malloc ( dim * sizeof( double * ) ) ; // Allocate the memory for the matrix B
    
    A1 = ( double * ) mkl_malloc ( dim * dim * sizeof( double ), 64 ) ; // Allocate the memory for the matrix A1 using MKL
    B1 = ( double * ) mkl_malloc ( dim * dim * sizeof( double ), 64 ) ; // Allocate the memory for the matrix B1 using MKL
    C = ( double * ) mkl_malloc ( dim * dim * sizeof( double ), 64 ) ; // Allocate the memory for the matrix C using MKL

    for (i=0; i<dim; i++){
    	A[i] = ( double * ) malloc ( dim * sizeof( double ) );
    	B[i] = ( double * ) malloc ( dim * sizeof( double ) );   
    }

    srand(time(NULL)); // Generate different seed
    
// Fill the SPD matrices of A and B 
    for (i=0; i<dim; i++){
            double sum1=0;
            double sum2=0;
            for (j=0; j<dim; j++){
                    A[i][j] = 0;
                    B[i][j] = 0;
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
    
/* Activate if you want to see the output (Matrices A and B)
    printf(" \n The matrix,A, is \n");
    for (i=0; i<dim; i++){
            for (j=0; j<dim; j++){
            printf(" %f ",A[i][j]);                    
            }
    printf("\n"); 
    } // Show the matrix A
    
    printf(" \n The matrix,B, is \n");
    for (i=0; i<dim; i++){
            for (j=0; j<dim; j++){
            printf(" %f ",B[i][j]);                    
            }
    printf("\n"); 
    } // Show the matrix B */
  
//Convert the matrices to the BLAS format 
    for (i=0; i<dim; i++) {
      for (j=0; j<dim; j++) {
        A1[j+i*dim]=A[i][j]; //Setting the matrix A1
        B1[j+i*dim]=B[i][j]; //Setting the matrix B1
        C[j+i*dim]=0.0; //Setting the matrix C to 0.0
      }
    }
    
//Number of threads Used for dgemm    
    mkl_set_num_threads(thread_count);

// Get start time
    s_initial = dsecnd();
   
//Calculation of matrix product using Intel(R) MKL dgemm function via CBLAS interface
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                dim, dim, dim, alpha, A1, dim, B1, dim, beta, C, dim);
   
// Get end time
    s_elapsed = (dsecnd() - s_initial);

// Time calculation
    printf("Calculation time is %f ms\n", s_elapsed*1000);
    
/* Activate if you want to see the result (Matrix C)
   printf(" \n The matrix,C, is \n");
    for (i=0; i<dim; i++){
            for (j=0; j<dim; j++){
            printf(" %f ",C[j+i*dim]);                    
            }
    printf("\n"); 
    } // Show the matrix C*/
    
//Deallocating memory
    mkl_free(A1);
    mkl_free(B1);
    mkl_free(C);     
}
