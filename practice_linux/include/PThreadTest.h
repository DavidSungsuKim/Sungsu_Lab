
#ifdef __PTHREAD_TEST_H__
#define __PTHREAD_TEST_H__
#endif

#include <pthread.h>

class CPThreadTest
{
public:
	CPThreadTest();
	~CPThreadTest();
	
	int CreateJoin			();
	int SyncMutex			( bool abMakeDeadLock = false );
	
	int	ReaderWriterLocks	();
	int ConditionVariable	();
		
	// Callback functions for threads
	
protected:
	bool	m_bMakeDeadLock;

	struct 
	{
		int data;
		pthread_mutex_t mutex;
		
	}m_SharedResource;

	typedef struct stCondition
	{
	/*	struct message
		{
			struct message* next;
		}*msg;
		*/
			
		char 			msg[256];	// for simple test
		pthread_mutex_t	mutex;
		pthread_cond_t	cond;
		
	}StCondition;

	StCondition		m_Cond;

	pthread_mutex_t	m_mutex1;
	pthread_mutex_t	m_mutex2;
	
	static int		m_retThread1;
	static int		m_retThread2;
	
	void		InitConditionVariable	();
	void		UninitConditionVariable	();
	
	static void* ThreadProc1		(void* arg);
	static void* ThreadProc2		(void* arg);
	
	static void* ThreadProcMutex1	(void* arg);	
	static void* ThreadProcMutex2	(void* arg);
	
	static void* ThreadProcRWLocks1	(void* arg);
	static void* ThreadProcRWLocks2	(void* arg);
	
	// For ConditionVariable()
	static void* ThreadProducer		(void* arg);
	static void* ThreadConsumer		(void* arg);
};
