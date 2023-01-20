#include "definition.h"
#include "benchmarking.h"
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


void printValues(int my_rank,float* values){

    int i,j,k; //looping indexes

    for(j=0;j<NTIME;j++){ //for each timestamp,
        for(i=0;i<NNODE;i++){ //for each node in that timestamp
            for(k=0;k<NNZ1;k++){ //take all the values in the different mashes and sum them
                printf("process %d, values[%d][%d][%d] = %f \n",my_rank,j,k,i,*(values+j+k+i));
            }
        }
    }

}


void printAverages(int my_rank,float* averages){

    int i,j;//looping indexes

    for(j=0;j<NTIME;j++){ //for each timestamp
        for(i=0;i<NNODE;i++){ //for each node 
            printf("process %d, values[%d][%d] = %f \n",my_rank,j,i,*(averages+j+i));
        }
    }
    
}


void computeAverages(float* values, float* averages){

    int i,j,k;
    float sum; //partial sum to calculate the averages
   
    for(j=0;j<NTIME;j++){ //for each timestamp,
        for(i=0;i<NNODE;i++){ //for each node in that timestamp
            sum=0;
            for(k=0;k<NNZ1;k++){ //take all the values in the different mashes and sum them
                sum+= *(values+j+k+i);
            }
            *(averages+j+i)=sum/NNZ1; //and then set the result
        }
    }

}

int readVar(const size_t start[NDIMS], const size_t count[NDIMS], float* values){

    /* .nc file */
    int ncid;

    /* var id */
    int unod_id;

    /* Error handling. */
    int retval;
    /* Open the file. */
    if ((retval = nc_open(FILE_NAME_UNOD, NC_NOWRITE, &ncid)))
        return(retval);

        /* Retrieve variable id */
    if ((retval = nc_inq_varid(ncid, VNAME, &unod_id))){
        return(retval);
    }

    /* Reading values */
    if ((retval = nc_get_vars_float(ncid,unod_id,start,count,NULL,values))){
        return(retval);
    }

    /* Close the file. */
    if ((retval = nc_close(ncid)))
        return(retval);

    return 0;
}

