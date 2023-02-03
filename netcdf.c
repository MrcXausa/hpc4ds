#include "definitions.h"
#include "benchmarking.h"
#include <omp.h>


int main (int argc, char** argv){

    /* parallel programming variables */
    int comm_sz;
    int my_rank;

    /* Error handling */
    int retval;

    /* Handle node/process division rest */
    int rest=0;

    //PARALLELIZATION BEGINS --------------------------------------------
    MPI_Init(NULL,NULL);

    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz); /* Number of processes */
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank); /* Process rank */
    MPI_Barrier(MPI_COMM_WORLD);


    /* Define how many nodes each process has to take care about */
    int nnode  = (int)(NNOD2 / comm_sz);

    if(my_rank==comm_sz-1 || my_rank==0){ //last process has a differend nnode value, and the first store the rest value
        rest=NNOD2%comm_sz;
        if(my_rank==comm_sz-1)
            nnode+=rest;
    }



    //starting time
    start_time();

    /* Splitting arrays */
    size_t start[NDIMS], count[NDIMS];

    /* Reading boundaries */
    start[0]=0;
    start[1]=0;
    start[2]=my_rank*(nnode-rest); 

    // how many nodes from the index
    count[0]=NTIME;
    count[1]=NNZ1;
    count[2]=nnode;

    /* Dinamically allocated matrix to read data from the nc file */
    float* values=(float *)malloc(NTIME*NNZ1*nnode*sizeof(float));
       
    /* Output matrix with the averages for each node at each timestamp */
    float* averages=(float *)malloc(NTIME*nnode*sizeof(float));

    /* Read data from .nc file */
    if ((retval = readVar(start,count,values,nnode))!=0){
        free(values); 
        free(averages);
        ERR(retval);
    }
    
    /* Computing averages */
    computeAverages(values,averages,nnode);
    
    /* Ending time and calculation the total time taken by a program */
    end_time();
    statistics();
    double avg = getavgtime();

    if(my_rank == 0) {

        /* Printing performance measures */
        printf("Min: %lf  Max: %lf  Avg:  %lf\n", getmintime(), getmaxtime(),avg/= comm_sz);

        /* Writing the output file */
        if ((retval = writeFile(comm_sz,averages,nnode,rest))){
            free(values); 
            free(averages);
            ERR(retval);    
        }
    }
    else{
        MPI_Send(averages,NTIME*nnode,MPI_FLOAT,0,0,MPI_COMM_WORLD);
    }


    /* Deallocate matrices */
    free(values); 
    free(averages);
    

    MPI_Finalize();

    return 0;
}





