CC = mpicc

CFLAGS = -Wall -g 

COMPFLAGS = -std=c99 -I /apps/netCDF4.7.0--gcc-9.1.0/include

LINKFLAGS = -L /apps/netCDF4.7.0--gcc-9.1.0/lib -lnetcdf

all: validity

validity.o: validity.c
	$(CC) $(COMPFLAGS) $(CFLAGS) -c $<

validity: validity.o
	$(CC) $(LINKFLAGS) $(CFLAGS) -o $@ $^