#include <stdio.h>
#include <string.h>
#include <netcdf.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>

/* files names */



/* Dimensions names for output file */
#define TIME_NAME "time"
#define NOD2_NAME "nod2"

/* working on unod */
#define FILE_NAME "/shares/HPC4DataScience/FESOM2/unod.fesom.2010.nc"
#define OFILE "unod_averages.nc"
#define VNAME "unod"
#define AVG_VEL_NAME "avg_unod"

/* working on vnod */
//#define FILE_NAME "/shares/HPC4DataScience/FESOM2/vnod.fesom.2010.nc"
//#define OFILE "vnod_averages.nc"
//#define VNAME "vnod"
//#define AVG_VEL_NAME "avg_vnod"


/* dimensions and amout of values for each one */
#define NDIMS 3
#define NTIME 12
#define NNZ1 69
#define NNOD2 8852366 


/* Error function */
#define ERR(e) {printf("Error: %s\n", nc_strerror(e)); return 2;}


/* Functions signatures */
void computeAverages(float* values, float* averages,int nnode);
int readVar(const size_t start[NDIMS], const size_t count[NDIMS], float* values,int nnode);
int writeFile(int comm_sz, float* beginning,int nnode,int rest);
void start_time();
void end_time();
void statistics();
double getmaxtime();
double getmintime();
double getavgtime();


void computeAverages(float* values, float* averages,int nnode){

    int i,j,k;
    float sum; //partial sum to calculate the averages
    
    //#pragma omp parallel for
    for(j=0;j<NTIME;j++){ //for each timestamp,
        for(i=0;i<nnode;i++){ //for each node in that timestamp
            sum=0;
            for(k=0;k<NNZ1;k++){ //take all the values in the different mashes and sum them
                sum+= *(values+j+k+i);
            }
            *(averages+j+i)=sum/NNZ1; //and then set the result
        }
    }

}

int readVar(const size_t start[NDIMS], const size_t count[NDIMS], float* values,int nnode){

    /* .nc file */
    int ncid;

    /* var id */
    int unod_id;

    /* Error handling. */
    int retval;


    /* Open the file. */
    if ((retval = nc_open(FILE_NAME, NC_NOWRITE, &ncid)))
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

int writeFile(int comm_sz, float* beginning,int nnode,int rest){
    int retval=0;
    int oncid; //output file id
    int time_dimid,nod2_dimid; //dimensions ids
    int avg_vel_id; //variable id


    size_t startp[2],countp[2]; //indexes for writing variable

    /* Creating output file */
    if ((retval = nc_create(OFILE, NC_CLOBBER, &oncid)))
        return retval;

    /* Creating dimensions */
    if ((retval = nc_def_dim(oncid, TIME_NAME, NTIME, &time_dimid)))
        return retval;
    if ((retval = nc_def_dim(oncid, NOD2_NAME, NNOD2, &nod2_dimid)))
        return retval;


    /* Creating variables */
    int dimensionsids[2];
    dimensionsids[0]=time_dimid;
    dimensionsids[1]=nod2_dimid;
    if ((retval = nc_def_var(oncid, AVG_VEL_NAME, NC_FLOAT, 2, dimensionsids, &avg_vel_id)))
        return retval;

    /* End define mode. */
    if ((retval = nc_enddef(oncid)))
        return retval;


    float* message=(float *)malloc(NTIME*nnode*sizeof(float));
    int p;
    for(p=0;p<comm_sz;p++){
        if(p==0){
            startp[0]=0;
            startp[1]=0;

            countp[0]=NTIME;
            countp[1]=nnode;

            
            if ((retval = nc_put_vara_float(oncid,avg_vel_id,startp,countp,beginning))){
                return retval;
            }
        }
        else if(p==comm_sz-1){ //consider that the last process works on (NOD2/comm_sz)+rest nodes
            int lastnnode=nnode+rest;
            MPI_Recv(message,NTIME*lastnnode,MPI_FLOAT,p,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

            
            //printAverages(p,message);
            startp[0]=0;
            startp[1]=p*nnode;

            countp[0]=NTIME;
            countp[1]=lastnnode;

            
            if ((retval = nc_put_vara_float(oncid,avg_vel_id,startp,countp,message))){
                free(message);
                return retval;
            }
        }
        else{
            MPI_Recv(message,NTIME*nnode,MPI_FLOAT,p,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

            
            
            startp[0]=0;
            startp[1]=p*nnode;

            countp[0]=NTIME;
            countp[1]=nnode;

            
            if ((retval = nc_put_vara_float(oncid,avg_vel_id,startp,countp,message))){
                free(message);
                return retval;
            }
        }
       
            
    }
    free(message);

    if ((retval = nc_close(oncid)))
      return retval;

    return retval;
        
}


