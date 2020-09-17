/*
Abhinav Gupta
ES15BTECH11002
Assignment 2 - Multi-Processing
*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>


int shm_fd;
int *primes;   /* pointer to shared memory obect */


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


int main(int argc, char *argv[])
{
	clock_t start,end;
  	double net_time;

	if(argc != 3)
	{ 
		printf("Enter Correct Run Command - ( ./a.out n k) where 1 < n < 1000000 and 2 < k < 32 "); 
		return 0;
	}
	
	// declaring n and k variables
	long n;
	int k;
	
	n = atoi(argv[1]);
	k = atoi(argv[2]);

	const  long SIZE = n*sizeof(int);          /* the size (in bytes) of shared memory object */
	pid_t  pid[k];                         /* shared memory file descriptor */                     
	shm_fd = shm_open("prime", O_CREAT | O_RDWR, 0666);         /* create the shared memory object */
	ftruncate(shm_fd, SIZE);                                     /* configure the size of the shared memory object */
	
	primes = (int *)mmap(0, SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);        /* memory map the shared memory object */


	start = clock();


	for(int i=0;i<k;++i)  // parent creates k child processes
	{
		pid[i] = fork();
		if(pid[i]==0) 
		{	
			int processid = i;
			if(processid == 0) processid = k; 
			while(processid < n)
			{
				if(isPrime(processid)) primes[processid] = 1;
				else primes[processid]=0;
				processid += k;
			}
			exit(0);
		}

		else if (pid[i] < 0) // if error in creating child process
        { 	
        	printf("Error Creating Process ! \n");
          	exit(0);                                       
        }
	}

	for(int i=0;i<k;++i) if(pid[i]>0) wait(NULL);

	end = clock();

	net_time = ((double)end-start)/CLOCKS_PER_SEC;

	FILE *fptr;

   	fptr = fopen("primes-proc.txt", "w");
	for (long i=1;i<n;++i)
	{
	 	if(primes[i]==1) fprintf(fptr,"%ld ",i);
	}
	fclose(fptr);
	
	printf("** Time used:- %f **\n", net_time);                   // printing out the time used


	shm_unlink("prime");                                     // deallocating shared memory
	return 0;
}

