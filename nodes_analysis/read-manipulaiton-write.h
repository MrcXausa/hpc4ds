/* 
Define how many nodes each process has to take care about.
The last process manages the remaining nodes from the division.
The first process keeps track of the rest to be able to read the averages
created by the last process. 
*/
int getNnode(int my_rank,int comm_sz,int* rest){
    int nnode  = (int)(NNOD2 / comm_sz);
    if(my_rank==comm_sz-1 || my_rank==0){ //last process has a differend nnode value, and the first stores the rest value
        *rest=NNOD2%comm_sz;
        if(my_rank==comm_sz-1)
            nnode+=*rest;
    }
    return nnode;
}


void computeAverages(float* values, float* averages,int nnode){

    int i,j,k;
    float sum; //partial sum to calculate the averages
    

    #pragma omp parallel for private(j,i,k,sum) num_threads(NTIME)
    for(j=0;j<NTIME;j++){ 
        //printf("threadId = %d \n", omp_get_thread_num());
        int o_timestamp=j*NNZ1*nnode; //offset to access the various timestamps 

        for(i=0;i<nnode;i++){ //for each node in that timestamp

            sum=0;

            for(k=0;k<NNZ1;k++){ 
                int o_level=k*nnode; //offset to access the various levels
                sum += *(values+o_timestamp+o_level+i);
            }

            *(averages+(j*nnode)+i)=sum/NNZ1; //and then set the result
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
    if ((retval = nc_get_vars_float(ncid,unod_id,start,count,NULL,values))){
        return(retval);
    }

    /* Close the file. */
    if ((retval = nc_close(ncid)))
        return(retval);

    return 0;
}




int writeFile(int comm_sz, float* beginning,int nnode,int rest){

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
    float* message=(float *)malloc(NTIME*nnode*sizeof(float));
    int p;
    for(p=0;p<comm_sz;p++){
        if(p==0){ // at the first iteration, process with rank 0 writes its result
            startp[0]=0;
            startp[1]=0;

            countp[0]=NTIME;
            countp[1]=nnode;

            
            if ((retval = nc_put_vara_float(oncid,avg_vel_id,startp,countp,beginning))){
                return retval;
            }
        }
        else if(p==comm_sz-1){ //consider that the last process works on (NNOD2/comm_sz)+rest nodes
            int lastnode=nnode+rest;
            MPI_Recv(message,NTIME*lastnode,MPI_FLOAT,p,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

            
            //printAverages(p,message);
            startp[0]=0;
            startp[1]=p*nnode;

            countp[0]=NTIME;
            countp[1]=lastnode;

            
            if ((retval = nc_put_vara_float(oncid,avg_vel_id,startp,countp,message))){
                free(message);
                return retval;
            }
        }
        else{ // for all other iterations, simply gather data and write it in the file
            MPI_Recv(message,NTIME*nnode,MPI_FLOAT,p,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

            startp[0]=0;
            startp[1]=p*nnode;

            countp[0]=NTIME;
            countp[1]=nnode;
            
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