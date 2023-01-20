# hpc4ds
High performance computing for data science project

# Instructions:
to run the program, load the modules:

```text
module load mpich-3.2
module load netcdf-4.7.0--gcc-9.1.0 hdf5-1.10.5--gcc-9.1.0
```

and then compile using make.
Then run the program using:

```text
qsub script.sh
```

To check the execution status use:

```text
qstat <id>
```

where id is the job id returned by the qsub command.


# link to the report
https://www.overleaf.com/project/63c2bd69d79b4323fe90d2e6
