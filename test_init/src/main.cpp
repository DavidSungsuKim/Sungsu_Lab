#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/times.h>
#include <sys/time.h>
#include <pthread.h>

#include "pigpio.h"

static int lineGpio = -1;

void* 	ThreadProc1			(void *arg);
bool	GetLEDLevel			(int arg, char* args[]);
void	ConfigureLED		(void);
void	DeconfigureLED		(void);
bool	ControlLED			(bool on);

int main (int arg, char* args[])
{
	ConfigureLED();

	int ret = 0;
	pthread_t	thread1;
	ret = pthread_create( &thread1, NULL, ThreadProc1, NULL );
	
	bool on = GetLEDLevel( arg, args );
	ControlLED( true );

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

bool GetLEDLevel(int arg, char* args[])
{
	bool on = false;
	if ( arg > 2 )
	{
		on = (args[1] == "on") ? true : false;
	}

	printf("%s on=%d\r\n", __FUNCTION__, on );
}

void ConfigureLED(void)
{
	gpioInitialise();

	lineGpio = 2;
	gpioSetMode(lineGpio, PI_OUTPUT);
}

void DeconfigureLED(void)
{
	gpioTerminate();
}

bool ControlLED (bool on)
{
	bool success = true;

	if ( lineGpio < 0 )
		return false;

	gpioWrite( lineGpio, on == true ? 1 : 0 );
	return success;
}