#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/times.h>
#include <sys/time.h>
#include <pthread.h>

void* ThreadProc1(void *arg);

int main (void)
{
	int ret = 0;
	pthread_t	thread1;

	ret = pthread_create( &thread1, NULL, ThreadProc1, NULL );
	
	ret = pthread_join( thread1, NULL );
	printf("task done...ret=%d\r\n", ret);

	return 0;
}
	
void* ThreadProc1(void *arg)
{
	int sleep_us = 100000;
	int i = 0;
	printf("ThreadProc1 has started...\r\n");
	while(1)
	{
	//	printf("Hello, i=%d\r\n",i++);
		usleep(sleep_us);
	}

	return NULL;
}