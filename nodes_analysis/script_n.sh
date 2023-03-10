#!/bin/bash
#PBS -l select=4:ncpus=2:mem=2gb 
#execution time
#PBS -l walltime=0:20:00

#set execution queue
#PBS -q short_cpuQ


#load modules and run the program
module load mpich-3.2
module load netcdf-4.7.0--gcc-9.1.0 hdf5-1.10.5--gcc-9.1.0
mpirun.actual -n 8 ./nodes_analysis/netcdf
