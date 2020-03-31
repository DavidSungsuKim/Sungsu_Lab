
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include "SignalTest.h"

extern const char* const sys_siglist[];

pthread_mutex_t	CSignalTest::m_mutex1;

CSignalTest::CSignalTest()
{
	
}

CSignalTest::~CSignalTest()
{
	
}

void 
CSignalTest::SignalFundamental()
{
	sighandler_t sigRet;
	
	sigRet = signal( SIGINT, SignalHandler1 );
	if ( sigRet )
	{
	//	perror("signal");
		fprintf( stderr, "Cannot handle SIGINT\n");
		exit( EXIT_FAILURE );
	}
	
	for(;;)
		pause();
		
	exit( EXIT_SUCCESS );
}
	
void 
CSignalTest::SignalFundamental2()
{
	sighandler_t sigRet;
	
	sigRet = signal( SIGINT, SignalHandler1 );
	if ( sigRet )
	{
		fprintf( stderr, "Cannot handle SIGINT\n");
		exit( EXIT_FAILURE );
	}
	
	sleep(3);
	
	// To make a self signal
	raise(2 /*SIGINT*/);
		
	exit( EXIT_SUCCESS );
}

void 
CSignalTest::SignalHandler1(int signo)
{
	printf("SignalHandler1, sig=%d(%s)\n", signo, sys_siglist[signo] );
	exit( EXIT_SUCCESS );
}

void
CSignalTest::SignalDeadLock()
{
	m_mutex1 	= PTHREAD_MUTEX_INITIALIZER;	// No recursive by default

	sighandler_t sigRet;
	sigRet = signal( SIGINT, SignalHandlerDeadLock );
	if ( sigRet )
	{
		fprintf( stderr, "Cannot handle SIGINT\n");
		exit( EXIT_FAILURE );
	}
	
	pthread_mutex_lock( &m_mutex1 );
	
	printf("SignalDeadLock: mutex lock...\n");
	
	for(;;)
		pause();
		
	pthread_mutex_unlock( &m_mutex1 );
	
	exit( EXIT_SUCCESS );
}

void 
CSignalTest::SignalHandlerDeadLock(int signo)
{
/* 	By the time this signal handler is called, 
	the mutex has already been taken in SignalDeadLock(), 
	which makes deadlock unless the mutex is set recursive.
	*/
	 
	pthread_mutex_lock( &m_mutex1 );
	
	printf("SignalHandlerDeadLock, sig=%d(%s)\n", signo, sys_siglist[signo] );
	
	pthread_mutex_unlock( &m_mutex1 );
}
