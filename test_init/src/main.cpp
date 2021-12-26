#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/times.h>
#include <sys/time.h>
#include <pthread.h>

#include "pigpio.h"

static int lineGpio = -1;

void* 	ThreadProc1			(void *arg);
int		GetLEDTogglePeriod	(int arg, char* args[]);
void	ConfigureLED		(void);
void	DeconfigureLED		(void);
bool	ControlLED			(bool on);

int main (int arg, char* args[])
{
	ConfigureLED();

	int ret = 0;
	pthread_t	thread1;

	int period_ms = GetLEDTogglePeriod( arg, args );
	ret = pthread_create( &thread1, NULL, ThreadProc1, &period_ms );
	ret = pthread_join( thread1, NULL );
	DeconfigureLED();

	printf("task done...ret=%d\r\n", ret);

	return 0;
}
	
void* ThreadProc1(void *arg)
{
	int sleep_us = *(int*)arg * 1000;
	printf("ThreadProc1 has started...period=%dus\r\n",sleep_us);
	while(1)
	{
		ControlLED( true );
		usleep(sleep_us);
		ControlLED( false );
	}

	return NULL;
}

int GetLEDTogglePeriod(int arg, char* args[])
{
	int period_ms = 0;
	if ( arg >= 2 )
	{
		period_ms = atol( args[1] );
		printf("period=%d from arg=%s\r\n", period_ms, args[1]);
	}

	printf("%s arg=%d period=%dms\r\n", __FUNCTION__, arg, period_ms );

	return period_ms;
}

void ConfigureLED(void)
{
	gpioInitialise();

	lineGpio = 2;
	gpioSetMode(lineGpio, PI_OUTPUT);
}

void DeconfigureLED(void)
{
	ControlLED( false );
	gpioTerminate();
}

bool ControlLED (bool on)
{
	bool success = true;

	if ( lineGpio < 0 )
		return false;

//	printf("%s, on=%d\r\n", __FUNCTION__, on );
	gpioWrite( lineGpio, ( on == true) ? 1 : 0 );
	return success;
}