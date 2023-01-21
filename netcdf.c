#include "definition.h"
#include "benchmarking.h"
#include "read-manipulation-write.h"
#include <stdlib.h>


int main (int argc, char** argv){

    /* parallel programming variables */
    int comm_sz;
    int my_rank;

    /* Error handling */
    int retval;

    //PARALLELIZATION BEGINS --------------------------------------------
    MPI_Init(NULL,NULL);

    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz); /* Number of processes */
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank); /* Process rank */
    MPI_Barrier(MPI_COMM_WORLD);



    //starting time
    start_time();

    /* Splitting arrays */
    size_t start[NDIMS], count[NDIMS];

 


    /* Reading boundaries */
    start[0]=0;
    start[1]=0;
    start[2]=my_rank*NNODE;

    // how many nodes from the index
    count[0]=NTIME;
    count[1]=NNZ1;
    count[2]=NNODE;

    /* Dinamically allocated matrix to read data from the nc file */
    float* values=(float *)malloc(NTIME*NNZ1*NNODE*sizeof(float));
       
    /* Output matrix with the averages for each node at each timestamp */
    float* averages=(float *)malloc(NTIME*NNODE*sizeof(float));

    /* Read data from .nc file */
    if ((retval = readVar(start,count,values))!=0){
        ERR(retval);
    }
    
    /* Computing averages */
    computeAverages(values,averages);

    /* Printing on stdout */
    printAverages(my_rank,averages);


    
    //ending time and calculation the total time taken by a program
    end_time();
    statistics();
    double avg = getavgtime();

    if(my_rank == 0) {
      printf("Min: %lf  Max: %lf  Avg:  %lf\n", getmintime(), getmaxtime(),avg/= comm_sz);
    }


    /* Deallocate matrix */
    free(values); 

    MPI_Finalize();

    return 0;
}
