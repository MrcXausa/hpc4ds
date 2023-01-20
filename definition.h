#ifndef __DEFINITION__
#define __DEFINITION__

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
#define VNAME "unod"


/* amount of nodes each process reads */
#define number_of_nodes 8852366
#define numprocs 8
int temp = (int)(number_of_nodes / numprocs);
#define NNODE temp

//error function
#define ERR(e) {printf("Error: %s\n", nc_strerror(e)); return 2;}

void printAverages(int my_rank,float* averages);
void computeAverages(float* values, float* averages);
void printValues(int my_rank,float* values);
int readVar(const size_t start[NDIMS], const size_t count[NDIMS], float* values);
void start_time();
void end_time();
void statistics();
double getmaxtime();
double getmintime();
double getavgtime();

#endif
