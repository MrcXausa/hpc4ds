# hpc4ds
High performance computing for data science project

Instructions:
to run the program, load the modules:

module load mpich-3.2
module load netcdf-4.7.0--gcc-9.1.0 hdf5-1.10.5--gcc-9.1.0

and then compile using make.
Then run the program using:

qsub script.sh

To check the execution status use:

qstat <id>

where id is the job id returned by the qsub command


