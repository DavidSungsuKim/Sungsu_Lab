
#ifndef __SIGNAL_TEST_H__
#define __SIGNAL_TEST_H__
#endif

#include <pthread.h>

class CSignalTest
{
public:
	CSignalTest();
	~CSignalTest();
	
	void 	SignalFundamental	();
	void	SignalFundamental2	();
	void	SignalDeadLock		();
	
protected:

	// For SignalFundamental() & SignalFundamental()
	static 	void SignalHandler1	(int signo);
	
	// For SignalDeadLock()
	static 	pthread_mutex_t	m_mutex1;
	
	static 	void SignalHandlerDeadLock	(int signo);
};
