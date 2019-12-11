/**********************************************************************

                   Example 5_pi_pt_to_pt.c 

  Objective           : To compute the value of PI by numerical integration.
                        MPI Point-to-Point communication library calls are used.
                        This example demonstrates the use of 
                        MPI_Init
                        MPI_Comm_rank
                        MPI_Comm_size
                        MPI_Send
                        MPI_Recv
                        MPI_Finalize

  Input               : The number of intervals.

  Output              : The calculated value of PI.

**********************************************************************/

#include <stdio.h>
#include <math.h>
#include "mpi.h"

/*....User defined function....*/
double func(double x)
{
    return (4.0 / (1.0 + x*x));
};

int main(int argc,char *argv[])
{
    int        NoIntervals, interval;
    int	       MyRank, Numprocs;
    int        iproc, Root = 0;
    int        Source, Source_tag;
    int        Destination, Destination_tag;
    double     PI25DT = 3.141592653589793238462643;
    double     mypi, pi, h, sum, x;
    MPI_Status status;

	 /*....MPI initialization....*/
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&Numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&MyRank);

    if(MyRank == Root)
    {
       puts("Enter the NoIntervals of intervals: ");
       scanf("%d",&NoIntervals);

       /*....Send the number of subintervals to each processor....*/ 
       for(iproc = 1 ; iproc < Numprocs ; iproc++)
       {
	   Destination     = iproc;
	   Destination_tag = 0;
           MPI_Send(&NoIntervals, 1, MPI_INT, Destination, Destination_tag, MPI_COMM_WORLD);
       }
     }

    else
    {
	 Source = Root;
	 Source_tag = 0;
         MPI_Recv(&NoIntervals, 1, MPI_INT, Source, Source_tag, MPI_COMM_WORLD, &status);
    }


    h   = 1.0 / (double) NoIntervals;
    sum = 0.0;
    for(interval = MyRank + 1; interval <= NoIntervals; interval += Numprocs)
    {
        x    = h * ((double)interval - 0.5);
        sum += func(x);
    }
    mypi = h * sum;

   /*....Collect the areas calculated by other processes in P0....*/ 
	 pi = 0.0;
    if(MyRank == Root){
       pi = pi + mypi;
       for(iproc = 1 ; iproc < Numprocs ; iproc++) {
           Source     = iproc;
	   Source_tag = 1;
           MPI_Recv(&mypi, 1, MPI_DOUBLE, Source, Source_tag, MPI_COMM_WORLD, &status);
           pi = pi + mypi;
       }
    }

    else
    {
	 Destination = Root;
	 Destination_tag = 1;
         MPI_Send(&mypi, 1, MPI_DOUBLE, Destination, Destination_tag, MPI_COMM_WORLD);
    }

    if(MyRank == Root)
       printf("pi is approximately %.16f, Error is %.16f\n",pi, fabs(pi - PI25DT));

    MPI_Finalize();

}

            



