#!/bin/bash 
 
#PBS -l select=1:ncpus=4:mem=2gb 
 
#execution time 
#PBS -l walltime=0:01:00 
 
#set execution queue 
#PBS -q short_cpuQ 
 
#command to run the program 
/home/huma.rehman/climate_data_analysis_files/makefile