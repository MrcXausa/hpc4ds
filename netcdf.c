#include <stdio.h>
#include <string.h>
#include <netcdf.h>

//#define FILE_NAME "tos_Omon_AWI-ESM-1-1-LR_historical_r1i1p1f1_gn_185001-185012.nc"


/* files names */
#define FILE_NAME_UNOD "/shares/HPC4DataScience/FESOM2/unod.fesom.2010.nc"
#define FILE_NAME_VNOD "/shares/HPC4DataScience/FESOM2/vnod.fesom.2010.nc"
#define FILE_NAME_MESH "/shares/HPC4DataScience/FESOM2/fesom.mesh.diag.nc"
//a file for the output shall be written and created also


/*
how to split data into portions? 
i.e accessing data only in a single timeframe or a single node at all timestamps and levels
*/


//error function
#define ERR(e) {printf("Error: %s\n", nc_strerror(e)); return 2;}


/* main */
int main (int argc, char** argv){

    /* .nc file */
    int ncid;

    /* Variables for reading file info */
    int ndims_in, nvars_in, ngatts_in, unlimdimid_in;

    /* var ids */
    int nz1_varid;

    /* Error handling. */
    int retval;

    /* Open the file. */
    if ((retval = nc_open(FILE_NAME, NC_NOWRITE, &ncid)))
        ERR(retval);


    /* Reading file info */
    if ((retval = nc_inq(ncid, &ndims_in, &nvars_in, &ngatts_in, &unlimdimid_in)))
        ERR(retval);

    printf("dimensions: %d",ndims_in);
    printf("\n");
    printf("variables: %d",nvars_in);
    printf("\n");
    printf("global attributes: %d",ngatts_in);
    printf("\n");
    printf("unlimdimid: %d",unlimdimid_in);



    /* read the nz1 variable */

    float nz1[69]; //already know there are 69 values

    if ((retval = nc_inq_varid(ncid, "nz1", &nz1_varid)))
        ERR(retval);

    if ((retval = nc_get_var1_float(ncid, nz1_varid, nz1)))
        ERR(retval);
    
    
    /* print nz1 */
    printf("\n printing nz1: \n");

    int i;
    for ( i = 0; i < 69; i++){
        printf("%f \n",nz1[i]);

    }
    

    /* Close the file. */
    if ((retval = nc_close(ncid)))
        ERR(retval);




    return 0;
}