#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include  <string.h>
#include <unistd.h>
#include  <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>

void  ChildProcess(void *ptr,int n);                /* child process prototype  */

int main()
{
	printf("Parent Process Begins!\n");
	/* the size (in bytes) of shared memory object */
	const int SIZE = 409600;
	/* name of the shared memory object */
	const char *name = "OS";
	/* shared memory file descriptor */
	int shm_fd;
	/* pointer to shared memory obect */
	void *ptr;
	/* create the shared memory object */
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	/* configure the size of the shared memory object */
	ftruncate(shm_fd, SIZE);
	/* memory map the shared memory object */
	ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

	int n;
	scanf("%d",&n);

	pid_t  pid;

	pid = fork();
	
	if(pid==0)
	{
		printf("Child Process Begins!\n");     // actually the child process begins just after the fork call , but this line get printed twice if kept above.
		ChildProcess(ptr,n);
		printf("Child Process Ends!\n");
	}
	else 
	{
		wait(NULL);     // wait for the child process to complete  
		shm_fd = shm_open(name, O_RDONLY, 0666);            
		ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
		printf("%s",(char *)ptr);
		/* remove the shared memory object */
		shm_unlink(name);
		printf("Parent Process Ends!\n");
	}
	return 0;
}

void  ChildProcess(void* ptr,int n)
{
	char result[50];

	while(n!=0)
	{
	    sprintf(result, "%d ", n);     // converting the result to string and writing it in result buffer
		sprintf(ptr,"%s",result);     // writing the result as a string
		ptr += strlen(result);       // incrementing the pointer
		if(n==1) break;              // break if n is 1
		if(n%2) n=3*n + 1;
		else n/=2;
	}
	sprintf(ptr,"\n");
	ptr+= 1;      // incrementing the pointer by the length of '\n'
} 


