#include<omp.h>
#include<stdio.h>
void main()
{
	#pragma omp parallel
    	{
	
        	int ID= omp_get_thread_num();
        	printf("charan hello= %d \n", ID);
        	printf("Hello charan= %d \n", ID);
    	}
}
