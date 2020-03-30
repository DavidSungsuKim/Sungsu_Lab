
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
//#include <sys/time.h>

#include "PThreadTest.h"

int	CPThreadTest::m_retThread1 	= 0;
int	CPThreadTest::m_retThread2	= 0;

struct to_info
{
	void	(*to_fn)(void*);
	void*	to_arg;
	struct 	timespec to_wait;
};

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
	InitRecursiveMutex();
	InitThreadSignal();
}

CPThreadTest::~CPThreadTest()
{
	pthread_mutex_destroy( &m_SharedResource.mutex );
	
	UninitConditionVariable();
	UninitRecursiveMutex();
	UninitThreadSignal();
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

int		
CPThreadTest::RecursiveMutex()
{	
	int condition;
	
	struct timeval	tv;
	struct timespec when;
	gettimeofday( &tv, NULL );
	
	time_t 	sec 	= 5;
	long	nsec	= 0;
	
	when.tv_sec 	= tv.tv_sec + sec;
	when.tv_nsec	= tv.tv_usec * 1000 + nsec;
	condition		= 1;
	
	printf("RecursiveMutex: timeout %ld.%ld[sec]\n", sec, nsec/1000 );
		
	pthread_mutex_lock( &m_mutexRecur );
	printf("RecursiveMutex: mutex locked\n");
		
	if ( condition )
	{
		Timeout( &when, Retry, (void*)this );
	}
	
	pthread_mutex_unlock( &m_mutexRecur );
	printf("RecursiveMutex: mutex unlocked\n");
	
	sleep( sec + 3 );
		
	return 0;
}

void	
CPThreadTest::InitRecursiveMutex()
{
	pthread_mutexattr_t mutexAttr;
	pthread_mutexattr_init( &mutexAttr);

	pthread_mutexattr_settype( &mutexAttr, PTHREAD_MUTEX_RECURSIVE );
	pthread_mutex_init( &m_mutexRecur, &mutexAttr );
	
	pthread_mutexattr_destroy( &mutexAttr );
}

void	
CPThreadTest::UninitRecursiveMutex()
{
	pthread_mutex_destroy( &m_mutexRecur );	
}

void	
CPThreadTest::Timeout( /*const*/ struct timespec* when, void (*func)(void*), void* arg )
{
#define SECTONSEC 	1000000000
#define USECTONSEC	1000

	struct timespec now;
	struct timeval	tv;
	struct to_info*	tip;
	
	gettimeofday( &tv, NULL );
	now.tv_sec		= tv.tv_sec;
	now.tv_nsec		= tv.tv_usec * USECTONSEC;
		
	if (( when->tv_sec > now.tv_sec ) ||
		(( when->tv_sec == now.tv_sec ) && ( when->tv_nsec > now.tv_nsec )))
	{	
		tip = (struct to_info*)malloc( sizeof(struct to_info) );	// This must be deleted
		if ( tip != NULL )
		{
			tip->to_fn				= func;
			tip->to_arg				= arg;
			tip->to_wait.tv_sec		= when->tv_sec	- now.tv_sec;
			
			if ( when->tv_nsec >= now.tv_nsec )
			{
				tip->to_wait.tv_nsec = when->tv_nsec - now.tv_nsec;
			}
			else
			{
				tip->to_wait.tv_sec--;
				tip->to_wait.tv_nsec = SECTONSEC - now.tv_nsec + when->tv_nsec;
			}
			
			printf("Timeout: wait time=%ld.%ld[sec]\n", tip->to_wait.tv_sec, tip->to_wait.tv_nsec/1000 );
						
			pthread_t	thread, threadCheck;
			int 		err;
							
			err = pthread_create( &thread, NULL, Timeout_helper, (void*)tip  );
			if ( err == 0 )
			{
				pthread_create( &threadCheck, NULL, TimeStamp, (void*)tip  );
				return;
			}
		}
	}
		
	(*func)(arg);	
}

void* 
CPThreadTest::Timeout_helper(void* arg)
{
	struct to_info*	tip;
	
	tip = (struct to_info*)arg;
	
	struct timeval	tStart;
	struct timeval	tEnd;
	
	printf("Timeout_helper: nanosleep start...%ld.%ld[sec]\n", tip->to_wait.tv_sec, tip->to_wait.tv_nsec/1000 );
	
	gettimeofday( &tStart, NULL );
	
	nanosleep( &tip->to_wait, NULL );

	gettimeofday( &tEnd, NULL );
	
	time_t 	tElapsedSec 	= tEnd.tv_sec - tStart.tv_sec;
	long	tElapsedUsec;
	
	if ( tEnd.tv_usec < tStart.tv_usec )
	{
		tElapsedSec--;
		tElapsedUsec = 1000000000 + tEnd.tv_usec - tStart.tv_usec;
	}
	else
		tElapsedUsec = tEnd.tv_usec - tStart.tv_usec;
	
	printf("Timeout_helper: nanosleep end...elapsed=%ld.%ld[sec]\n", (long)tElapsedSec, tElapsedUsec );
	
	(*tip->to_fn)( tip->to_arg );
	
	// Important
	free( tip );
	
	return (0);
}

void
CPThreadTest::Retry(void* arg)
{
	CPThreadTest* pThis = (CPThreadTest*)arg;
	
	pthread_mutex_lock( &pThis->m_mutexRecur );
	printf("Retry: mutex is locked...\n");
	
	pthread_mutex_unlock( &pThis->m_mutexRecur );
	printf("Retry: mutex is unlocked...\n");
}

void*	
CPThreadTest::TimeStamp(void* arg)
{
	struct to_info*	tip;
	
	tip = (struct to_info*)arg;

	int timeSec = tip->to_wait.tv_sec + 1;
	int i = 0;
	
	while( timeSec-- )
	{
		sleep(1);
		printf("TimePrint:	%d[sec]\n", ++i );
	}
	
	return NULL;
}

int	
CPThreadTest::ThreadNSignal()
{
	int err;
	sigset_t	oldmask;
	pthread_t	tid;
	
	printf("ThreadNSignal: start..\n");
	
	sigemptyset( &m_mask );
	sigaddset( &m_mask, SIGINT );
	sigaddset( &m_mask, SIGQUIT );
	
	err = pthread_sigmask( SIG_BLOCK, &m_mask, &oldmask);
	if ( err != 0 )
	{
		printf("ThreadNSignal: SIG_BLOCK error\n");
		return -1;
	}
		
	err = pthread_create( &tid, NULL, ThreadSignal, this );
	if ( err != 0 )
	{
		printf("ThreadNSignal: pthread_create\n");
		return -1;		
	}
	
	pthread_mutex_lock( &m_mutexSig );
	while( m_quitFlag < 3 )
	{
		// Would it be okay if I wait for the condition repeatedly like this?
		pthread_cond_wait( &m_condSig, &m_mutexSig );
		printf("ThreadNSignal: condition signaled...m_quitFlag=%d\n", m_quitFlag );
		
		// Once pthread_cond_wait() is returned, the mutex can remain unlocked. Is that okay?
		// 	: I think we should have taken mutex before evaluating 'm_quitFlag' again.		
		int ret;
	//	sleep(1);	
	//	pthread_mutex_unlock( &m_mutexSig );
		
		ret = pthread_mutex_trylock( &m_mutexSig );	// But it doesn't seem to lock again since it appear to remain locked.
		if ( ret != 0 )								// Interesting...hm...
			printf("ThreadNSignal: trylock fail, EBUSY?, which means it's alread locked?\n");
	}
		
	pthread_mutex_unlock( &m_mutexSig );
	
	m_quitFlag = 0;
	
	if ( sigprocmask( SIG_SETMASK, &oldmask, NULL ) < 0 )
	{
		printf("ThreadNSignal: SIG_SETMASK error\n");
		return -1;
	}
	
	printf("ThreadNSignal: end...\n");
		
	return 0;		
}

void 	
CPThreadTest::InitThreadSignal()
{
	m_mutexSig 	= PTHREAD_MUTEX_INITIALIZER;
	m_condSig	= PTHREAD_COND_INITIALIZER;
	
	m_quitFlag	= 0;
}

void	
CPThreadTest::UninitThreadSignal()
{
	
}

void*	
CPThreadTest::ThreadSignal(void* arg)
{
	CPThreadTest*	pThis = (CPThreadTest*)arg;
	
	int err, signo;	

	printf("	ThreadSignal: start...\n" );
	
	for(;;)
	{
		err = sigwait( &pThis->m_mask, &signo );
		if ( err != 0 )
		{
			printf("	ThreadSignal: sigwait err=%d\n", err );
			break;
		}
		
		switch( signo )
		{
		case SIGINT:
			printf("	ThreadSignal: SIGINT, signo=%d\n", signo );				
			break;
			
		case SIGQUIT:
			printf("	ThreadSignal: SIGQUIT, signo=%d\n", signo );			
			
			int quit;
			pthread_mutex_lock( &pThis->m_mutexSig );
		
			quit = ++(pThis->m_quitFlag);
						
			pthread_mutex_unlock( &pThis->m_mutexSig );
			
			printf("	ThreadSignal: m_quitFlag=%d\n", quit );
						
			pthread_cond_signal( &pThis->m_condSig );
	
			if ( pThis->m_quitFlag > 3 )
			{
				printf("	ThreadSignal: end...by SIGQUIT\n" );
				return(0);
			}
			break;
			
		default:		
			printf("	ThreadSignal: Unexpected signal, signo=%d\n", signo );
			break;
		}
	}
	
	printf("	ThreadSignal: end...\n" );	
	return NULL;
}
