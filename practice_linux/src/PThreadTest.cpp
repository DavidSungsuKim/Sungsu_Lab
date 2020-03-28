
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
//#include <sys/time.h>

#include "PThreadTest.h"

int	CPThreadTest::m_retThread1 	= 0;
int	CPThreadTest::m_retThread2	= 0;

CPThreadTest::CPThreadTest()
{
	m_bMakeDeadLock = false;
	
	// Initialize mutexes by Option 1 or 2.
	// Note that if mutexes are created on dynamic memory,
	// users need to call pthread_mutex_destroy() before freeing memory.
	
	// Option 1.
//	m_SharedResource.mutex = PTHREAD_MUTEX_INITIALIZER;
	
	// Option 2.
	pthread_mutex_init( &m_SharedResource.mutex, NULL );
	
	m_SharedResource.data = 0;
	
	InitConditionVariable();
}

CPThreadTest::~CPThreadTest()
{
	pthread_mutex_destroy( &m_SharedResource.mutex );
	
	UninitConditionVariable();
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

int 
CPThreadTest::SyncMutex( bool abMakeDeadLock )
{
	int ret = 0;
	pthread_t	thread1, thread2;
	
	m_bMakeDeadLock = abMakeDeadLock;
	
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
		
	m_bMakeDeadLock = false;
	
	return ret;
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
		printf("ThreadProcMutex1:wait for mutex\n");
		pthread_mutex_lock( &pThis->m_SharedResource.mutex );
		
		pThis->m_SharedResource.data++;
		
		if ( pThis->m_bMakeDeadLock )
		{
			printf("ThreadProcMutex1:Deadlock will happen...\n");	
			// Try to lock the mutex twice and get stuck.
			pthread_mutex_lock( &pThis->m_SharedResource.mutex );
		}
		
		printf("ThreadProcMutex1:mutex locked, %d/%d, data=%d\n", 5-i, 5, pThis->m_SharedResource.data );
		
		sleep(1);
		
		pthread_mutex_unlock( &pThis->m_SharedResource.mutex );
		
	//	sleep(0);
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
		printf("ThreadProcMutex2:wait for mutex\n");
		pthread_mutex_lock( &pThis->m_SharedResource.mutex );
		printf("ThreadProcMutex2:mutex locked, %d/%d\n", 5-i, 5);
		
		pThis->m_SharedResource.data++;
		
		printf("ThreadProcMutex2:mutex locked, %d/%d, data=%d\n", 5-i, 5, pThis->m_SharedResource.data );
				
		sleep(1);
		
		pthread_mutex_unlock( &pThis->m_SharedResource.mutex );
		
	//	sleep(0);
	}
	
	return NULL;
}

int	
CPThreadTest::ReaderWriterLocks()
{
	int ret = 0;
	pthread_t	thread1, thread2;
	
	// Create threads.
	ret = pthread_create( &thread1, NULL, ThreadProcRWLocks1, this );
	if ( ret )
	{
		perror("pthread_create");
		return ret;
	}
	
	printf("pthread_create, TID=%ld\n", thread1 );
		
	ret = pthread_create( &thread2, NULL, ThreadProcRWLocks2, this );
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
CPThreadTest::ThreadProcRWLocks1(void* arg)
{
	CPThreadTest* pThis = (CPThreadTest*)arg;
	
	/* TBD */
	pThis = pThis;
	
	return NULL;
}

void* 
CPThreadTest::ThreadProcRWLocks2(void* arg)
{
	CPThreadTest* pThis = (CPThreadTest*)arg;

	/* TBD */		
	pThis = pThis;
	
	return NULL;
}

int
CPThreadTest::ConditionVariable()
{
	int ret = 0;
	pthread_t	thread1, thread2;
	
	// Create threads.
	ret = pthread_create( &thread1, NULL, ThreadProducer, this );
	if ( ret )
	{
		perror("pthread_create");
		return ret;
	}
	
	printf("pthread_create, TID=%ld\n", thread1 );
		
	ret = pthread_create( &thread2, NULL, ThreadConsumer, this );
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

void
CPThreadTest::InitConditionVariable()
{
	// Refer these when attributes should be changed from default values.
/*	pthread_mutexattr_t mutexAttr;
	pthread_mutexattr_init( &mutexAttr);

//	pthread_mutexattr_gettype( &mutexAttr );
	pthread_mutexattr_settype( &mutexAttr, PTHREAD_MUTEX_RECURSIVE );		
	// PTHREAD_MUTEX_NORMAL		: default type on Linux
	// PTHREAD_MUTEX_DEFAULT	: 
	// PTHREAD_MUTEX_RECURSIVE	: multiple locking is allowed. 
	
	pthread_mutexattr_setpshared( &mutexAttr, PTHREAD_PROCESS_PRIVATE ); 	
	// PTHREAD_PROCESS_SHARED	: the mutex can be shared between processes, but it requires more resources.
	// PTHREAD_PROCESS_PRIVATE	: the default and it's more efficient
	
	pthread_mutexattr_
	pthread_mutex_init( &m_Cond.mutex, &mutexAttr );
	*/
		
	// Codes used
	pthread_mutex_init( &m_Cond.mutex, NULL );
	pthread_cond_init( &m_Cond.cond, NULL );
	memset( m_Cond.msg, 0, sizeof(m_Cond.msg));
}

void		
CPThreadTest::UninitConditionVariable()
{
	pthread_mutex_destroy( &m_Cond.mutex );
	pthread_cond_destroy( &m_Cond.cond );	
}
	
void* 
CPThreadTest::ThreadProducer(void* arg)
{
	CPThreadTest* 	pThis = (CPThreadTest*)arg;
	StCondition*	pCond = &pThis->m_Cond;
	if ( !pCond )
		return NULL;
	
	printf("ThreadProducer: Start...\n");
	printf("ThreadProducer: Put any message.\n(Put 'quit' if you want to finish.)\n");
	
	for(;;)
	{
		char msg[256] = {0,};
		scanf("%s", msg);
			
		pthread_mutex_lock( &pCond->mutex );
		
		memcpy( pCond->msg, msg, sizeof(msg));		
		
		pthread_mutex_unlock( &pCond->mutex );
		pthread_cond_signal( &pCond->cond );
		
		if( !strcmp( msg, "quit" ) )
		{
			printf("Quit okay...\n");
			break;
		}
		
		sleep(0);
	}	
	
	printf("ThreadProducer: End...\n");
	return NULL;
}

void* 
CPThreadTest::ThreadConsumer(void* arg)
{
	CPThreadTest* 	pThis = (CPThreadTest*)arg;
	StCondition*	pCond = &pThis->m_Cond;
	if ( !pCond )
		return NULL;
	
	printf("	ThreadConsumer: Start...\n");
		
	for(;;)
	{
		pthread_mutex_lock( &pCond->mutex );	// I think the reason we lock the mutex here is for situations 
												// we deal with some data protected by the mutex.
		while( pCond->msg[0] == '\0' )
		{
			int ret;
			ret = pthread_cond_wait( &pCond->cond, &pCond->mutex );
			if ( ret )
			{
				pthread_mutex_unlock( &pCond->mutex );				
				printf("	ThreadConsumer: %d=pthread_mutex_unlock'n", ret );
				break;
			}
		}
	
		printf("	ThreadConsumer: Msg=%s\n", pCond->msg );
		
		if ( !strcmp( pCond->msg, "quit" ) )
		{
			pthread_mutex_unlock( &pCond->mutex );
			break;
		}
		
		memset( pCond->msg, 0, sizeof(pCond->msg));
		
		pthread_mutex_unlock( &pCond->mutex );
	}	
	
	printf("	ThreadConsumer: End...\n");
	return NULL;
}
