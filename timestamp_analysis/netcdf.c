#include "definitions.h"
#include "benchmarking.h"
#include "read-manipulaiton-write.h"


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

    


    /* Timestamps per process */
    int tpp=getTpp(comm_sz); 
    
    //starting time
    start_time();

    /* Splitting arrays */
    size_t start[NDIMS], count[NDIMS];

    /* Reading boundaries */
    start[0]=my_rank*tpp;
    start[1]=0;
    start[2]=0; 

    // how many nodes from the index
    count[0]=tpp;
    count[1]=NNZ1;
    count[2]=NNOD2;

    /* Dinamically allocated matrix to read data from the nc file */
    float* values=(float *)malloc(tpp*NNZ1*NNOD2*sizeof(float));
       
    /* Output matrix with the averages for each node at each timestamp */
    float* averages=(float *)malloc(tpp*NNOD2*sizeof(float));

    /* Read data from .nc file */
    printf("before readvar, process %d \n",my_rank);
    if ((retval = readVar(start,count,values))!=0){
        free(values); 
        free(averages);
        ERR(retval);
    }
    
    /* Computing averages */
    printf("before computeavg, process %d \n",my_rank);
    computeAverages(values,averages,tpp);
    
    /* Ending time and calculation the total time taken by a program */
    end_time();
    statistics();
    double avg = getavgtime();

    if(my_rank == 0) {

        /* Write the output file */
        if ((retval = writeFile(comm_sz,averages,tpp))){
            free(values); 
            free(averages);
            ERR(retval);    
        }

        /* Print performance measures */
        printf("Min: %lf  Max: %lf  Avg:  %lf\n", getmintime(), getmaxtime(),avg/= comm_sz);
        
    }
    else{
        /* Send data to process 0 */
        printf("before message sent, process %d \n",my_rank);
        MPI_Send(averages,tpp*NNOD2,MPI_FLOAT,0,0,MPI_COMM_WORLD);
        printf("after message sent process %d \n",my_rank);
    }


    /* Deallocate matrices */
    free(values); 
    free(averages);
    

    MPI_Finalize();

    return 0;
}





