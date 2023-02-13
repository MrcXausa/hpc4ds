#!/bin/bash

#PBS -l select=1:ncpus=3:mem=64gb 
#execution time
#PBS -l walltime=00:20:00

#set execution queue
#PBS -q short_cpuQ


#load modules and run the program
module load mpich-3.2
module load netcdf-4.7.0--gcc-9.1.0 hdf5-1.10.5--gcc-9.1.0
mpirun.actual -n 3 ./climate_data_analysis_files/timestap_analysis/netcdf