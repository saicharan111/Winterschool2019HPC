/*****************************************************************************
About     : Serial program to add N elements
Objective : Use OpenMP to make the program parallel with correct output. 
	    Time the serial program, and parallel program with different no. of threads.
********************************************************************************/


#include<stdio.h>
#include<omp.h>
#define N 1000000000

int main()
{
 long int i,a[N],sum=0, localsum=0;
#pragma omp for
 for(i=0;i<N;i++)
 {
  a[i]=i+1;
 }
#pragma omp parallel firstprivate(localsum){
#pragma omp parallel for 
 for(i=0;i<N;i++)
 {
  localsum=localsum+a[i];
 }
#pragma omp critical
sum=sum+localsum;
}
  printf("Sum=%ld  \n",sum);

 return 0;
}


