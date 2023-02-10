#include <stdio.h>
#include <string.h>
#include <netcdf.h>
#include <stdlib.h>
#include <mpi.h>



#define ERR(e) {printf("Error: %s\n", nc_strerror(e)); return 2;}
#define NNZ1 69
#define NDIMS 3
#define NNOD2 8852366 

#define FILE_NAME "/shares/HPC4DataScience/FESOM2/unod.fesom.2010.nc"
#define OFILE "unod_averages.nc"
#define VNAME "unod"
#define AVG_VEL_NAME "avg_unod"



int getNnode(int comm_sz){

    return (int)(NNOD2 / comm_sz);
}

int main (int argc, char** argv){
    MPI_Init(NULL,NULL);
    
    int retval,ncidavg,ncidcomp;
    int unod_id,unodavg_id;
    int chosenNode=NNOD2-1;

    //--------------------------------------- Open both files --------------------------------
    if ((retval = nc_open("/shares/HPC4DataScience/FESOM2/unod.fesom.2010.nc", NC_NOWRITE, &ncidcomp)))
        return(retval);
    if ((retval = nc_open("unod_averages.nc", NC_NOWRITE, &ncidavg)))
        return(retval);
   

    //---------------------------------------- Get variables ids -----------------------------
    if ((retval = nc_inq_varid(ncidcomp, "unod", &unod_id))){
        return(retval);
    }
    if ((retval = nc_inq_varid(ncidavg, "avg_unod", &unodavg_id))){
        return(retval);
    }


    /*
    Read data from the main file. Take a specific timestamp and a specific node and
    all the levels. Then do the averages over all value
    */
    size_t start[NDIMS], count[NDIMS];
    
    start[0]=11;
    start[1]=0; 
    start[2]=chosenNode; 

    count[0]=1;
    count[1]=NNZ1; 
    count[2]=1; 

    float values[1][NNZ1][1];

    /* Reading values */
    if ((retval = nc_get_vars_float(ncidcomp,unod_id,start,count,NULL,&values[0][0][0]))){
        return(retval);
    }
    printf("hello \n");
    int i;
    float temp=0;
    float value;
    for(i=0;i<NNZ1;i++){
        temp+=values[0][i][0];
    }
    value=temp/NNZ1;





    size_t start1[2], count1[2];

    start1[0]=11; 
    start1[1]=chosenNode; 

    count1[0]=1; 
    count1[1]=1; 
    float value1;
    if ((retval =nc_get_vars_float(ncidavg,unodavg_id,start1,count1,NULL,&value1))){
        return(retval);
    }
    
    /* Close the file. */
    if ((retval = nc_close(ncidcomp)))
        return(retval);
    if ((retval = nc_close(ncidavg)))
        return(retval);

    printf("value= %f\n",value);
    printf("value1= %f\n",value1);

    MPI_Finalize();
    return 0;
}
