double mytime,   /*variables used for gathering timing statistics*/
           maxtime,
           mintime,
           avgtime;
void start_time(){
    mytime = MPI_Wtime();  /*get time just before work section */

}
void end_time(){
    mytime = MPI_Wtime() - mytime;  /*get time just after work section*/
}

void statistics(){
/*compute max, min, and average timing statistics*/
    MPI_Reduce(&mytime, &maxtime, 1, MPI_DOUBLE,MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&mytime, &mintime, 1, MPI_DOUBLE, MPI_MIN, 0,MPI_COMM_WORLD);
    MPI_Reduce(&mytime, &avgtime, 1, MPI_DOUBLE, MPI_SUM, 0,MPI_COMM_WORLD);

}

double getmaxtime() {return maxtime;}
double getmintime() {return mintime;}
double getavgtime() {return avgtime;}