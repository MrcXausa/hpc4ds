#!/bin/bash

<<<<<<< HEAD:timestap_analysis/script_t.sh
#PBS -l select=1:ncpus=3:mem=64gb 
=======
#PBS -l select=1:ncpus=1:mem=2gb 
>>>>>>> ceafaec11f66169aa6d78001699987ec7999f660:timestamp_analysis/script_t.sh
#execution time
#PBS -l walltime=1:00:00

#set execution queue
#PBS -q short_cpuQ


#load modules and run the program
module load mpich-3.2
module load netcdf-4.7.0--gcc-9.1.0 hdf5-1.10.5--gcc-9.1.0
<<<<<<< HEAD:timestap_analysis/script_t.sh
mpirun.actual -n 3 ./climate_data_analysis_files/timestap_analysis/netcdf
=======
mpirun.actual -n 1 ./timestamp_analysis/netcdf
>>>>>>> ceafaec11f66169aa6d78001699987ec7999f660:timestamp_analysis/script_t.sh
