
#ifndef _THREAD_TEST_H_
#define _THREAD_TEST_H_

#include <process.h>

class CThreadTest
{
public:
	CThreadTest();
	~CThreadTest();

protected:
	HANDLE m_hThread;

	static void ThreadProc();
	void Task();
};

#endif