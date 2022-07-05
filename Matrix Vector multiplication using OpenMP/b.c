/* 
A parallel calculation for Matrix Vector multiplication using OpenMP

Compile: gcc b.c -o b -fopenmp
Run: ./b

Code was tested using AMD Ryzen7 3700x 8-Core Processor and Ubuntu 20.04 as OS  
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main(int argc, char ** argv){
    
    double tim;
    double st;
    
    int **mat; // Declare double pointer for matrix
    int *vec; // Declare pointer for vector
    int *result; // Local variable for result
    int dim=100000; // Declare and set the dimension
        
    int i,j; // This is the number for "for" loop
    
    mat = ( int ** ) malloc ( dim * sizeof( int * ) ) ; // Allocate the memory for the mat
    for (i=0; i<dim; i++)
        mat[i] = ( int * ) malloc ( dim * sizeof( int ) );
    vec = ( int * ) malloc ( dim * sizeof( int ) ); // Allocate the memory for the vec
    result = (int *) malloc (dim * sizeof (int) ); // Memory allocate for the result
    srand(time(NULL)); // Generate different seed
    
    //printf(" The vector,v, is \n");
    for (i=0; i<dim; i++){
            vec[i] = rand()%9+1; // Setting the vector
            //printf(" %d \n",vec[i]);
            int sum=0;
            for (j=0; j<dim; j++){
                    mat[i][j] = 0;
                    if (i<j){
                        mat[i][j] = -(rand()%9+1);
                        }
                    if (i>j){
                        mat[i][j] = mat[j][i];
                        }
                    sum += mat[i][j];                   
            }
            mat[i][i] = abs(sum);//Setting the matrix
    } 
    
   /* printf(" \n The matrix,M, is \n");
    for (i=0; i<dim; i++){
            for (j=0; j<dim; j++){
            printf(" %d ",mat[i][j]);                    
            }
    printf("\n"); 
    } // Show the matrix */
    st = omp_get_wtime(); // Get start time
#pragma omp parallel for num_threads(8) default(none) schedule(static, 64) private(i,j) shared(mat, vec, result) firstprivate(dim) 
    for (i=0; i<dim; i++){
	    result[i] = 0;
	    for (j=0; j<dim; j++){
		result[i] += mat[i][j] * vec[j]; // Calculate the multiplication of each rows in the matrix and vector
	    }
	//printf(" The result %dth is %d \n", i, result[i]);
    }     

    //Time calculation
    tim = omp_get_wtime() - st; 
    printf("Time for parallel calculation is %f ms\n", tim *1000);
}
