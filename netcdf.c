#include <stdio.h>
#include <string.h>
#include <netcdf.h>
#include <mpi.h>

//#define FILE_NAME "tos_Omon_AWI-ESM-1-1-LR_historical_r1i1p1f1_gn_185001-185012.nc"


/* files names */
#define FILE_NAME_UNOD "/shares/HPC4DataScience/FESOM2/unod.fesom.2010.nc"
#define FILE_NAME_VNOD "/shares/HPC4DataScience/FESOM2/vnod.fesom.2010.nc"
#define FILE_NAME_MESH "/shares/HPC4DataScience/FESOM2/fesom.mesh.diag.nc"

#define NDIMS 3
#define NTIME 12
#define NNZ1 69
#define NNOD2 8852366 



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

    /*
    parallel programming variables
    */
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
    float values[NNZ1][NTIME][100]; 




    start[0]=0;
    start[1]=0;
    start[2]=0;

    count[0]=12;
    count[1]=69;
    count[2]=100;

    if ((retval = nc_inq_varid(ncid, "unod", &unod_id))){
        ERR(retval);
    }

    printf("ciao da %d",my_rank);

    if ((retval = nc_get_vara_float(ncid,unod_id,start,count,&values[0][0][0]))){
        ERR(retval);
    }	

    printf("process %d, values[0][0][0]=%f \n",my_rank,values[0][0][0]);

    /* Close the file. */
    if ((retval = nc_close(ncid)))
        ERR(retval);

    MPI_Finalize();



    return 0;
}