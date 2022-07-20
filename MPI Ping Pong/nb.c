/* 
MPI Ping Pong using non-blocking communication
Compile: mpicc nb.c -o nb
Run: mpirun -n 2 ./nb 

Code was tested using AMD Ryzen7 3700x 8-Core Processor and Ubuntu 20.04 as OS 

Information regarding lscpu:
Architecture:                    x86_64
CPU op-mode(s):                  32-bit, 64-bit
Byte Order:                      Little Endian
Address sizes:                   43 bits physical, 48 bits virtual
CPU(s):                          16
On-line CPU(s) list:             0-15
Thread(s) per core:              2
Core(s) per socket:              8
Socket(s):                       1
NUMA node(s):                    1
Vendor ID:                       AuthenticAMD
CPU family:                      23
Model:                           8
Model name:                      AMD Ryzen 7 2700X Eight-Core Processor
Stepping:                        2
Frequency boost:                 enabled
CPU MHz:                         2200.000
CPU max MHz:                     3700.0000
CPU min MHz:                     2200.0000
BogoMIPS:                        7399.04
Virtualization:                  AMD-V
L1d cache:                       256 KiB
L1i cache:                       512 KiB
L2 cache:                        4 MiB
L3 cache:                        16 MiB

*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
	
	// MPI Initialization
	MPI_Init(&argc, &argv);
	
	int exchange = 100000; // Number of exchange (send and receive)
	int tag0=0, tag1=1; // Set the tag
	int i, ii, j, k, N, *A; // Declare the vector and number for loop
	double start_time, stop_time, elapsed_time; // Declare for calculation of time
	int size, rank;
	
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Status stat;
	MPI_Request send_handle, recv_handle;

	// Check for the MPI size
	if(size != 2){
		if(rank == 0){
			printf("MPI Ping Pong requires exactly 2 ranks, the program cannot run with %d \n", size);
		}
		MPI_Finalize();
		exit(0);
	}

	// Loop for 8 separate measurements
	for(j=0; j<8; j++){
	if(rank == 0) printf("Mesurement number %i \n", j+1);
	
	// Loop to measure 4 to 32768 beytes 
	for(i=0; i<=13; i++){

		 N = 1 << i;
	
		 A = (int*)malloc(N*sizeof(int));// Allocate the memory for the vector A

		// Fill the vector with a random integer 
		for(k=0; k<N; k++){
			A[k] = rand()%9+1;
		}
		
		// Get start time
		start_time = MPI_Wtime();

		// Transfer data with size of 4xN bytes for exchange iterations using non-blocking communication 
		for(int ii=1; ii<=exchange; ii++){
			
			if(rank == 0){
				MPI_Isend(A, N, MPI_DOUBLE, 1, tag0, MPI_COMM_WORLD, &send_handle);
				MPI_Irecv(A, N, MPI_DOUBLE, 1, tag1, MPI_COMM_WORLD, &recv_handle);
				MPI_Wait(&send_handle, &stat);
				MPI_Wait(&recv_handle, &stat);
			}
			else if(rank == 1){
				MPI_Irecv(A, N, MPI_DOUBLE, 0, tag0, MPI_COMM_WORLD, &recv_handle);
				MPI_Isend(A, N, MPI_DOUBLE, 0, tag1, MPI_COMM_WORLD, &send_handle);
				MPI_Wait(&send_handle, &stat);
				MPI_Wait(&recv_handle, &stat);
			}
		}

		// Get end time and time calculation
		stop_time = MPI_Wtime();
		elapsed_time = stop_time - start_time;

		if(rank == 0) printf("Transfer size (bytes): %10i, Total Time (ms): %16f\n", 4*N, elapsed_time*1000); //Print the results

		free(A);
	}}

	MPI_Finalize();

	return 0;
}
