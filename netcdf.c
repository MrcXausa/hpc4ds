#include <stdio.h>
#include <string.h>
#include <netcdf.h>
#include <mpi.h>


/* files names */
#define FILE_NAME_UNOD "/shares/HPC4DataScience/FESOM2/unod.fesom.2010.nc"
#define FILE_NAME_VNOD "/shares/HPC4DataScience/FESOM2/vnod.fesom.2010.nc"
#define FILE_NAME_MESH "/shares/HPC4DataScience/FESOM2/fesom.mesh.diag.nc"

/* dimensions and amout of values for each one */
#define NDIMS 3
#define NTIME 12
#define NNZ1 69
#define NNOD2 8852366 

/* amount of nodes each process reads */
#define NNODE 80



//error function
#define ERR(e) {printf("Error: %s\n", nc_strerror(e)); return 2;}


/* main */
int main (int argc, char** argv){

    /* .nc file */
    int ncid;


    /* var ids */
    int unod_id;

    /* Error handling. */
    int retval;

    /* parallel programming variables */
    int comm_sz; //number of processes
    int my_rank; //rank of each process

    //splitting arrays
    size_t start[NDIMS], count[NDIMS];

    /* Open the file. */
    
    if ((retval = nc_open(FILE_NAME_UNOD, NC_NOWRITE, &ncid)))
        ERR(retval);


    //PARALLELIZATION BEGINS   --------------------------------------------
    MPI_Init(NULL,NULL);

    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

    //var where the dummy read value
    double values[NTIME][NNZ1][NNODE]; 


    //read only one value
    start[0]=0;
    start[1]=60;
    start[2]=my_rank*100;

    count[0]=NTIME;
    count[1]=9;
    count[2]=NNODE;

    if ((retval = nc_inq_varid(ncid, "unod", &unod_id))){
        ERR(retval);
    }

    if ((retval = nc_get_vars_double(ncid,unod_id,start,count,NULL,&values[0][0][0]))){
        ERR(retval);
    }	


    int i,j,k; //looping indexes
    printf("process %d, values[11][77][7] = %lf \n",my_rank,values[11][77][7]);

    /*
    for(j=0;j<NTIME;j++){ //for each timestamp,
        for(i=0;i<NNODE;i++){ //for each node in that timestamp
            for(k=0;k<9;k++){ //take all the values in the different mashes and sum them
                printf("process %d, values[%d][%d][%d] = %lf \n",my_rank,j,i,k,values[j][i][k]);
            }
        }
    }
    */


    /*
    this matrix will contain the averages for each node at each timestamp.
    this will be written on a two-dimensional file

    float averages[NTIME][NNODE]; 

    float sum; //partial sum to calculate the averages
    */
    
    
    /*
    //algorith to calculate the averages
    for(j=0;j<NTIME;j++){ //for each timestamp,
        for(i=0;i<NNODE;i++){ //for each node in that timestamp
            sum=0;
            for(k=0;k<NNZ1;k++){ //take all the values in the different mashes and sum them
                sum+=values[j][i][k];
            }
            averages[j][i]=sum/NNZ1; //and then set the result
        }
    }

    //printing averages
    for(j=0;j<NTIME;j++){ //for each timestamp
        for(i=0;i<NNODE;i++){ //for each node in that timestamp
            printf("process %d, values[%d][%d] = %f \n",my_rank,j,i,averages[j][i]);
        }
    }
    */
    
    

    /* Close the file. */
    if ((retval = nc_close(ncid)))
        ERR(retval);

    MPI_Finalize();



    return 0;
}

/*
open questions (still not handled):
 - work also on vnod
 - write the output files (every process write its part or not)

problems:
 - weird behaviour reading same data for 80 nodes
*/