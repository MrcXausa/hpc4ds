#!/bin/bash

#PBS -l select=1:ncpus=1:mem=2gb 
#execution time
#PBS -l walltime=1:00:00

#set execution queue
#PBS -q short_cpuQ


#load modules and run the program
module load mpich-3.2
module load netcdf-4.7.0--gcc-9.1.0 hdf5-1.10.5--gcc-9.1.0
mpirun.actual -n 1 ./timestamp_analysis/netcdf