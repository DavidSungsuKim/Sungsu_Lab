
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
//#include <sys/time.h>

#include "PThreadTest.h"

int	CPThreadTest::m_retThread1 	= 0;
int	CPThreadTest::m_retThread2	= 0;

CPThreadTest::CPThreadTest()
{
	m_mutex1 = PTHREAD_MUTEX_INITIALIZER;
	m_mutex2 = PTHREAD_MUTEX_INITIALIZER;
}

CPThreadTest::~CPThreadTest()
{
	
}

int 
CPThreadTest::CreateJoin()
{
	int ret = 0;
	pthread_t	thread1, thread2;
//	pthread_attr_t	attr1, attr2;
	
	const char* msg1 = "Thing1";
	const char* msg2 = "Thing2";
	
	// Create threads.
	ret = pthread_create( &thread1, NULL, ThreadProc1, (void*)msg1 );
	if ( ret )
	{
		perror("pthread_create");
		return ret;
	}
	
	printf("pthread_create, TID=%ld\n", thread1 );
		
	ret = pthread_create( &thread2, NULL, ThreadProc2, (void*)msg2 );
	if ( ret )
	{
		perror("pthread_create");
		return ret;
	}

	printf("pthread_create, TID=%ld\n", thread2 );
		
//	sleep(1);		// I don't need to insert any time 
					// since theads had already started working by the time 'pthread_create' returns.
	
	// Wait for threads to be done.
	int* pVal1 = 0;	// Here that we have a pointer for return value from a thread function means 
	int* pVal2 = 0;	// real value for the pointer should exist somewhere else like as a global or static member variable.
					// Q : But, why should we take this way?
					// A : This is in order to enable passing more complex structure when threads finish.
					// Another solution is to allocate a variable from dynamic memory and deleting after refering to it.

	ret = pthread_join( thread1, (void**)&pVal1 );	// This pthread_join() will block until the thread finishes.
	if ( ret )
		perror("pthread_join");
	printf( "Return code Thread1=%d\n", *pVal1 );
	
	ret = pthread_join( thread2, (void**)&pVal2 );
	if ( ret )
		perror("pthread_join");
	printf( "Return code Thread2=%d\n", *pVal2 );
		
	return ret;
}

int 
CPThreadTest::SyncMutex()
{
	int ret = 0;
	pthread_t	thread1, thread2;
	
	// Create threads.
	ret = pthread_create( &thread1, NULL, ThreadProcMutex1, this );
	if ( ret )
	{
		perror("pthread_create");
		return ret;
	}
	
	printf("pthread_create, TID=%ld\n", thread1 );
		
	ret = pthread_create( &thread2, NULL, ThreadProcMutex2, this );
	if ( ret )
	{
		perror("pthread_create");
		return ret;
	}

	printf("pthread_create, TID=%ld\n", thread2 );

	// Wait until threads finish their job.
	ret = pthread_join( thread1, NULL );
	if ( ret )
		perror("pthread_join");
	
	ret = pthread_join( thread2, NULL );
	if ( ret )
		perror("pthread_join");
		
	return ret;
}

void* 
CPThreadTest::ThreadProc1( void *arg )
{
	pthread_t	tid = pthread_self();
	printf("ThreadProc1: TID=%ld\n", tid );	
	
	const char* pMsg = (const char*)arg;
	
	int i = 5;
	while(i--)
	{
		printf("ThreadProc1:%s, %d/%d\n", pMsg, 5-i, 5);
		sleep(1);
	}

	m_retThread1 = 1;	// This value can be refered from retval in pthread_join() function.
#ifdef EXIT_BY_EXIT
	pthread_exit( &m_retThread1 );
	return NULL;
#else	
	return &m_retThread1;
#endif
}

void* 
CPThreadTest::ThreadProc2( void *arg )
{
	pthread_t	tid = pthread_self();
	printf("ThreadProc2: TID=%ld\n", tid );
	
	const char* pMsg = (const char*)arg;
	
	int i = 5;
	while(i--)
	{
		printf("ThreadProc2:%s, %d/%d\n", pMsg, 5-i, 5);
		sleep(1);
	}
	
	m_retThread2 = 2;	// This value can be refered from retval in pthread_join() function.
#ifdef EXIT_BY_EXIT
	pthread_exit( m_retThread2 );
	return NULL;
#else	
	return ( &m_retThread2 );
#endif
}

void* 
CPThreadTest::ThreadProcMutex1( void *arg )
{
	pthread_t	tid = pthread_self();
	printf("ThreadProcMutex1: TID=%ld\n", tid );
	
	CPThreadTest* pThis = (CPThreadTest*)arg;
	
	int i = 5;
	while(i--)
	{
		printf("ThreadProcMutex1:wait for mutex1\n");
		pthread_mutex_lock( &pThis->m_mutex1 );
		printf("ThreadProcMutex1:mutex1 locked, %d/%d\n", 5-i, 5);
		
		sleep(1);
		
		pthread_mutex_unlock( &pThis->m_mutex1 );
		
		sleep(0);
		
		printf("ThreadProcMutex1:mutex1 unlocked, %d/%d\n", 5-i, 5);
	}
	
	return NULL;
}

void* 
CPThreadTest::ThreadProcMutex2( void *arg )
{
	pthread_t	tid = pthread_self();
	printf("ThreadProcMutex2: TID=%ld\n", tid );
	
	CPThreadTest* pThis = (CPThreadTest*)arg;
	
	int i = 5;
	while(i--)
	{
		printf("ThreadProcMutex2:wait for mutex1\n");
		pthread_mutex_lock( &pThis->m_mutex1 );
		printf("ThreadProcMutex2:mutex1 locked, %d/%d\n", 5-i, 5);
		
		sleep(1);
		
		pthread_mutex_unlock( &pThis->m_mutex1 );
		
		sleep(0);
				
		printf("ThreadProcMutex2:mutex1 unlocked, %d/%d\n", 5-i, 5);
	}
	
	return NULL;
}
