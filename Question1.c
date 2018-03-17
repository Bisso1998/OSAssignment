
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define SIZE_BUFF 10		/* Size of shared buffer */

int buffer[SIZE_BUFF];  	/* shared buffer */
int rear = 0;  			/* place to add next element */
int front = -1; 			/* place to remove next element */
int count = 0;  		/* number elements in buffer */

int resourceAccess = 0;
int readerPriorityFlag = 0;     /* This flag is set to 1 when readers access the resources so that writers do not access it*/
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER; 	/* mutex lock for buffer */
pthread_cond_t cons_cond = PTHREAD_COND_INITIALIZER; /* consumer waits on this cond var */
pthread_cond_t prod_cond = PTHREAD_COND_INITIALIZER; /* producer waits on this cond var */

pthread_mutex_t mtxData = PTHREAD_MUTEX_INITIALIZER; /* mutex lock for data buffer */

void *writer(void* param);
void *reader(void* param);

int main(int argc, char *argv[]) 
{
	srand(time(NULL));
	pthread_t thId[SIZE_BUFF];
	int idx;
	for (idx = 0; idx < SIZE_BUFF / 2; ++idx)
	{
	  if(pthread_create(&thId[idx], NULL, writer, NULL) != 0)
	  {
		fprintf(stderr, "Unable to create writer thread\n");
		exit(1);
	  }
	  if(pthread_create(&thId[idx + SIZE_BUFF/2], NULL, reader, NULL) != 0)
	  {
		fprintf(stderr, "Unable to create reader thread\n");
		exit(1);
	  }
	}
	for (idx = 0; idx < SIZE_BUFF; ++idx)
	{
		pthread_join(thId[idx], NULL);	
	}
	fprintf(stdout, "Parent thread quitting\n");
	return 0;
}
