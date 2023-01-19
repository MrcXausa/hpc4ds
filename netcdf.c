#include "definition.h"


int main (int argc, char** argv){

    /* .nc file */
    int ncid;

    /* var id */
    int unod_id;

    /* Error handling. */
    int retval;

    /* parallel programming variables */
    int comm_sz;
    int my_rank;

    /* splitting arrays */
    size_t start[NDIMS], count[NDIMS];

    /* Open the file. */
    
    if ((retval = nc_open(FILE_NAME_UNOD, NC_NOWRITE, &ncid)))
        ERR(retval);


    //PARALLELIZATION BEGINS --------------------------------------------
    MPI_Init(NULL,NULL);

    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    MPI_Barrier(MPI_COMM_WORLD);
    /* Array for reading values from file */
    float values[NTIME][NNZ1][NNODE]; 

    //starting time
    start_time();
    /* Reading boundaries */
    start[0]=0;
    start[1]=0;
    start[2]=my_rank*NNODE;

    // how many nodes from the index
    count[0]=NTIME;
    count[1]=NNZ1;
    count[2]=NNODE;

    /* Reading variable id */
    if ((retval = nc_inq_varid(ncid, "unod", &unod_id))){
        ERR(retval);
    }

    /* Reading values */
    if ((retval = nc_get_vars_float(ncid,unod_id,start,count,NULL,&values[0][0][0]))){
        ERR(retval);
    }	


    /* Output matrix with the averages for each node at each timestamp */
    float averages[NTIME][NNODE];

    //printValues(my_rank,values);
    
    /* Computing averages */
    computeAverages(values,averages);

    /* Printing on stdout */
    printAverages(my_rank,averages);
   

    /* Close the file. */
    if ((retval = nc_close(ncid)))
        ERR(retval);

    
    //ending time and calculation the total time taken by a program
    end_time();
    statistics();
    double avg = getavgtime();
    if(my_rank == 0) {
      printf("Min: %lf  Max: %lf  Avg:  %lf Total_time: %lf \n", getmintime(), getmaxtime(),avg/= numprocs,gettottime());
    }

    MPI_Finalize();

    return 0;
}


void printValues(int my_rank,float values[NTIME][NNZ1][NNODE]){

    int i,j,k; //looping indexes

    for(j=0;j<NTIME;j++){ //for each timestamp,
        for(i=0;i<NNODE;i++){ //for each node in that timestamp
            for(k=0;k<NNZ1;k++){ //take all the values in the different mashes and sum them
                printf("process %d, values[%d][%d][%d] = %lf \n",my_rank,j,i,k,values[j][k][i]);
            }
        }
    }
}


void printAverages(int my_rank,float averages[NTIME][NNODE]){

    int i,j,totcount=0,count;//looping indexes
    
    for(j=0;j<NTIME;j++){ //for each timestamp
    count=0;
        for(i=0;i<NNODE;i++){ //for each node 
            // printf("process %d, values[%d][%d] = %f \n",my_rank,j,i,averages[j][i]);
            count++;
            totcount++;
        }
    printf("process %d  took %d nodes of mesh = \n",my_rank,count);
    }
    printf("Total nodes processed  %d = \n",totcount);
    printf("Total Processes = %d\n",numprocs);
}


void computeAverages(float values[NTIME][NNZ1][NNODE], float averages[NTIME][NNODE]){

    int i,j,k;
    float sum; //partial sum to calculate the averages

    for(j=0;j<NTIME;j++){ //for each timestamp,
        for(i=0;i<NNODE;i++){ //for each node in that timestamp
            sum=0;
            for(k=0;k<NNZ1;k++){ //take all the values in the different mashes and sum them
                sum+=values[j][k][i];
            }
            averages[j][i]=sum/NNZ1; //and then set the result
        }
    }
}

