
#ifndef __SIGNAL_TEST_H__
#define __SIGNAL_TEST_H__
#endif

#include <signal.h>
#include <pthread.h>

class CSignalTest
{
public:
	CSignalTest();
	~CSignalTest();
	
	void 	SignalFundamental	();
	void	SignalFundamental2	();
	void	SignalFundamental3	();
	void	SignalDeadLock		();
	void	SignalAdvanced		();
	
protected:

	/* For 	SignalFundamental() & 
			SignalFundamental2() &
			SignalFundamental3() &
			SignalAdvanced()
		*/ 
	static 	void SignalHandler1	(int signo);
	
	// For SignalDeadLock()
	static 	pthread_mutex_t	m_mutex1;
	
	static 	void SignalHandlerDeadLock	(int signo);
	
	// For SignalAdvanced()
	static	void SignalHandlerAdvanced(int signo, siginfo_t* pSi, void* pContext);
};
