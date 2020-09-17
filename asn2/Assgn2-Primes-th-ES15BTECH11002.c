/*
Abhinav Gupta
ES15BTECH11002
Assignment 2 - Multi-Threading
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


int *primes;  // shared memory between threads

// declaring n and k variables
long n;
int k;

int isPrime(long n)
{
    // Corner cases
    if (n <= 1)  return 0;
    if (n <= 3)  return 1;

    // This is checked so that we can skip 
    // middle five numbers in below loop
    if (n%2 == 0 || n%3 == 0) return 0;

    for (long long i=5; i*i<=n; i=i+6)
        if (n%i == 0 || n%(i+2) == 0)
           return 0;
    return 1;
}

void *primes_thread(void *X)
{
	int *addr = (int*)X;
	int id = *addr;
	// int id =(int)X;
	// printf("%d",id);
	long len1=id;
	if(len1==0) len1 = k;         // check from k, check k,2k,etc
	while(len1 < n)
	{
		if(isPrime(len1)) primes[len1] = 1;
		else primes[len1]=0;
		len1 += k;
	}
 	pthread_exit(NULL);

}

int main(int argc, char *argv[])
{
	int ret;
	clock_t start,end;                                      //variables for start and end time
	time_t start_t,end_t;
	double net_time;

	if(argc != 3)
	{ 
		printf("Enter Correct Run Command - ( ./a.out n k) where 1 < n < 1000000 and 2 < k < 32 "); 
		return 0;
	}

	n = atoi(argv[1]);
	k = atoi(argv[2]);
		
	const  long SIZE = n*sizeof(int);  
	
	primes = (int *)malloc(SIZE);

	start = clock();  // Start Time


	pthread_t my_threads[k]; 

	int *a;
	a = (int*)malloc(k*sizeof(int));        
	for(int i=0;i<k;++i) a[i]=i;
	for (int i=0;i<k;++i)
	{
		ret = pthread_create(&my_threads[i], NULL, primes_thread, (void*)&a[i]);  // typecast the last argument to void*
		if(ret != 0)
	    {
	       printf("Error: pthread_create() failed\n");
	       exit(EXIT_FAILURE); 
	    }
	}

    for(int i=0 ; i < k ; ++i)
    {
        pthread_join(my_threads[i], 0);
    }	

    end = clock();

    
    net_time = ((double)end-start)/CLOCKS_PER_SEC;   // calculating time taken
	
	FILE *fptr;

   	fptr = fopen("primes-th.txt", "w");
	for (long i=1;i<n;++i)
	{
	 	if(primes[i]==1) fprintf(fptr,"%ld ",i);
	}
	fclose(fptr);
	// printf("** CPU Time used:- %f **\n", net_time); // Printing Net time taken


	free(primes);          // freeing memory
    return 0;
}