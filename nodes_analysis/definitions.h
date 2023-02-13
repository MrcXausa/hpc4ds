#include <stdio.h>
#include <string.h>
#include <netcdf.h>
#include <mpi.h>
#include <omp.h>
#include <stdlib.h>


/* dimensions and amount of values for each one */
#define NDIMS 3
#define NTIME 12
#define NNZ1 69
#define NNOD2 4426183
// #define NNOD2 8852366 

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



/* Error function */
#define ERR(e) {printf("Error: %s\n", nc_strerror(e)); return 2;}


/* Functions signatures */
int getNnode(int my_rank,int comm_sz,int* rest);
void computeAverages(float* values, float* averages,int nnode);
int readVar(const size_t start[NDIMS], const size_t count[NDIMS], float* values);
int writeFile(int comm_sz, float* beginning,int nnode,int rest);
void start_time();
void end_time();
void statistics();
double getmaxtime();
double getmintime();
double getavgtime();


