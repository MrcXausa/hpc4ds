CC = mpicc

CFLAGS = -Wall -g -fopenmp

COMPFLAGS = -std=c99 -I /apps/netCDF4.7.0--gcc-9.1.0/include

LINKFLAGS = -L /apps/netCDF4.7.0--gcc-9.1.0/lib -lnetcdf

all: netcdf

netcdf.o: netcdf.c
	$(CC) $(COMPFLAGS) $(CFLAGS) -c $<

netcdf: netcdf.o
	$(CC) $(LINKFLAGS) $(CFLAGS) -o $@ $^