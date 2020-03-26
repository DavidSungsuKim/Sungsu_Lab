
#ifdef __PTHREAD_TEST_H__
#define __PTHREAD_TEST_H__
#endif

#include <pthread.h>

class CPThreadTest
{
public:
	CPThreadTest();
	~CPThreadTest();
	
	int CreateJoin	();
	int SyncMutex	();
	
	// Callback functions for threads
	
protected:
	pthread_mutex_t	m_mutex1;
	pthread_mutex_t	m_mutex2;
	
	static int	m_retThread1;
	static int	m_retThread2;
		
	static void* ThreadProc1		(void* arg);
	static void* ThreadProc2		(void* arg);
	
	static void* ThreadProcMutex1	(void* arg);	
	static void* ThreadProcMutex2	(void* arg);	
};
