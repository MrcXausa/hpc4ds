/* 
Define how many nodes each process has to take care about.
The last process manages the remaining nodes from the division.
The first process keeps track of the rest to be able to read the averages
created by the last process. 
*/
int getTpp(int comm_sz){
    return NTIME/comm_sz;
}


void computeAverages(float* values, float* averages,int tpp){

    int i,j,k;
    float sum; //partial sum to calculate the averages
    

    #pragma omp parallel for private(j,i,k,sum) num_threads(NTIME)
    for(j=0;j<tpp;j++){ 
        //printf("threadId = %d \n", omp_get_thread_num());
        int o_timestamp=j*NNZ1*NNOD2; //offset to access the various timestamps 

        for(i=0;i<NNOD2;i++){ //for each node in that timestamp

            sum=0;

            for(k=0;k<NNZ1;k++){ 
                int o_level=k*NNOD2; //offset to access the various levels
                sum += *(values+o_timestamp+o_level+i);
            }

            *(averages+(j*NNOD2)+i)=sum/NNZ1; //and then set the result
        }
    }

}



int readVar(const size_t start[NDIMS], const size_t count[NDIMS], float* values){

    /* .nc file */
    int ncid;

    /* var id */
    int unod_id;

    /* Error handling. */
    int retval;


    /* Open the file. */
    if ((retval = nc_open(FILE_NAME, NC_NOWRITE, &ncid)))
        return(retval);

        /* Retrieve variable id */
    if ((retval = nc_inq_varid(ncid, VNAME, &unod_id))){
        return(retval);
    }

    /* Reading values */
    if ((retval = nc_get_vara_float(ncid,unod_id,start,count,values))){
        return(retval);
    }

    /* Close the file. */
    if ((retval = nc_close(ncid)))
        return(retval);

    return 0;
}




int writeFile(int comm_sz, float* beginning,int tpp){

    /* Error handling. */
    int retval;

    /* .nc output file */
    int oncid; 

    /* Dimensions' ids */
    int time_dimid,nod2_dimid;

    /* Variable id */
    int avg_vel_id; 


    size_t startp[2],countp[2]; //indexes for writing variable

    /* Creating output file */
    if ((retval = nc_create(OFILE, NC_CLOBBER, &oncid)))
        return retval;

    /* Creating dimensions */
    if ((retval = nc_def_dim(oncid, TIME_NAME, NTIME, &time_dimid)))
        return retval;
    if ((retval = nc_def_dim(oncid, NOD2_NAME, NNOD2, &nod2_dimid)))
        return retval;


    /* Creating variable */
    int dimensionsids[2];

    dimensionsids[0]=time_dimid;
    dimensionsids[1]=nod2_dimid;

    if ((retval = nc_def_var(oncid, AVG_VEL_NAME, NC_FLOAT, 2, dimensionsids, &avg_vel_id)))
        return retval;


    /* End define mode */
    if ((retval = nc_enddef(oncid)))
        return retval;


    /* 
        store the beginning of the file, then gather the data produced 
        by other processes and write them in the file recalling that the last
        process might have worked on more nodes than the others
    */
    float* message=(float *)malloc(tpp*NNOD2*sizeof(float));
    int p;
    for(p=0;p<comm_sz;p++){
        if(p==0){ // at the first iteration, process with rank 0 writes its result
            startp[0]=0;
            startp[1]=0;

            countp[0]=tpp;
            countp[1]=NNOD2;

            
            if ((retval = nc_put_vara_float(oncid,avg_vel_id,startp,countp,beginning))){
                return retval;
            }
        }
        else { 
            MPI_Recv(message,tpp*NNOD2,MPI_FLOAT,p,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

            
            //printAverages(p,message);
            startp[0]=p*tpp;
            startp[1]=0;

            countp[0]=tpp;
            countp[1]=NNOD2;

            
            if ((retval = nc_put_vara_float(oncid,avg_vel_id,startp,countp,message))){
                free(message);
                return retval;
            }
        }
    }

    free(message);

    if ((retval = nc_close(oncid)))
        return retval;

    return retval;
        
}