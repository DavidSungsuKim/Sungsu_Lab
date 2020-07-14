
#include <process.h>
#include <windows.h>

#include "..//include/Common.h"
#include "..//include/ThreadTest.h"

CThreadTest::CThreadTest()
{
#ifdef _OS_WINDOWS_
	m_hThread = (HANDLE)_beginthreadex(NULL,
				0,										// 0 : use default stack size (1024KB)
				(_beginthreadex_proc_type)ThreadProc,	// I need to use type-casting on VS2015
				this,									// To designate the address of this class instance
				0,										// 0 : Execute right away, 1 : CREATE_SUSPENDED
				NULL);
#endif
}

CThreadTest::~CThreadTest()
{

}

void 
CThreadTest::ThreadProc()
{

}

void 
CThreadTest::Task()
{


}