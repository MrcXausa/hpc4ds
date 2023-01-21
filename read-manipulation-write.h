

// *************************************************Writting*******************************************************
void printValues(int my_rank,float* values){

    int i,j,k; //looping indexes
    
    for(j=0;j<NTIME;j++){ //for each timestamp,
        for(i=0;i<NNODE;i++){ //for each node in that timestamp
            for(k=0;k<NNZ1;k++){ //take all the values in the different mashes and sum them
                printf("process %d, values[%d][%d][%d] = %f \n",my_rank,j,k,i,*(values+j+k+i));
            }
        }
    }

}


void printAverages(int my_rank,float* averages){

    int i,j;//looping indexes
    int count =0, totcount=0;
    for(j=0;j<NTIME;j++){ //for each timestamp
        for(i=0;i<NNODE;i++){ //for each node 
            // printf("process %d, values[%d][%d] = %f \n",my_rank,j,i,*(averages+j+i));
        count++;
        }
        printf("process %d, have worked on %d nodes and timestamp %d \n",my_rank,count,j);
        totcount++;
        count = 0;
    }
    printf("Total runs from all the processes %d \n",totcount);
    printf("Total processes %d \n",numprocs);
    
}

// *************************************************Manipulation*******************************************************
void computeAverages(float* values, float* averages){

    int i,j,k;
    float sum; //partial sum to calculate the averages
   
    for(j=0;j<NTIME;j++){ //for each timestamp,
        for(i=0;i<NNODE;i++){ //for each node in that timestamp
            sum=0;
            for(k=0;k<NNZ1;k++){ //take all the values in the different mashes and sum them
                sum+= *(values+j+k+i);
            }
            *(averages+j+i)=sum/NNZ1; //and then set the result
        }
    }

}

// *************************************************reading*******************************************************
int readVar(const size_t start[NDIMS], const size_t count[NDIMS], float* values){

    /* .nc file */
    int ncid;

    /* var id */
    int unod_id;

    /* Error handling. */
    int retval;
    /* Open the file. */
    if ((retval = nc_open(FILE_NAME_UNOD, NC_NOWRITE, &ncid)))
        return(retval);

        /* Retrieve variable id */
    if ((retval = nc_inq_varid(ncid, VNAME, &unod_id))){
        return(retval);
    }

    /* Reading values */
    if ((retval = nc_get_vars_float(ncid,unod_id,start,count,NULL,values))){
        return(retval);
    }

    /* Close the file. */
    if ((retval = nc_close(ncid)))
        return(retval);

    return 0;
}