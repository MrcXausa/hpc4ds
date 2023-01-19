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

/* amount of nodes each process reads */
#define NNODE 1000000


//error function
#define ERR(e) {printf("Error: %s\n", nc_strerror(e)); return 2;}

void printAverages(int my_rank,float averages[NTIME][NNODE]);
void computeAverages(float values[NTIME][NNZ1][NNODE], float averages[NTIME][NNODE]);
void printValues(int my_rank,float values[NTIME][NNZ1][NNODE]);

#endif
