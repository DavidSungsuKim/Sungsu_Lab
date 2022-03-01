#include "stdafx.h"

#include <windows.h>
#include <process.h>

#include "WindowsSys.h"

BOOL WINAPI HandlerRoutine(DWORD dwctrlType );
BOOL WINAPI HandlerRoutine(DWORD dwctrlType)
{
	int test = 0;
	switch (dwctrlType)
	{
	case CTRL_C_EVENT:
		test = 1;
		break;
	case CTRL_CLOSE_EVENT:
		test = 2;
		break;
	case CTRL_BREAK_EVENT:
		test = 3;
		break;
	case CTRL_LOGOFF_EVENT:
		test = 4;
		break;
	case CTRL_SHUTDOWN_EVENT:
		test = 5;
		break;
	}

	return true;
}

CWindowsSys::CWindowsSys()
{
	m_hConsoleOut	= NULL;
	m_countWrite	= 0;

}

CWindowsSys::~CWindowsSys()
{
	if ( m_hConsoleOut )
		CloseHandle( m_hConsoleOut );
}

void	
CWindowsSys::OpenConsole()
{
	if ( m_hConsoleOut )
		return;

	AllocConsole();

	m_hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleMode( m_hConsoleOut, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT );

	m_hConsoleRead = GetStdHandle(STD_INPUT_HANDLE);
	SetConsoleMode( m_hConsoleRead, ENABLE_PROCESSED_INPUT | ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT );

	SetConsoleCtrlHandler( HandlerRoutine, true );
}

void	
CWindowsSys::ReadConsole( std::string& arStrIn )
{
	char	buff[32]		= {0};
	DWORD	maxChar			= 32;
	DWORD	charIn;

	::ReadConsoleA( m_hConsoleRead, buff, maxChar-2, &charIn, NULL );

	buff[charIn-2] = '\0';	// in order to replace '\r\n' with and to prevent buffer overrun
	arStrIn = buff;
//	int value = atoi(buff);	// can be used to convert character stream to an integer.
}

void
CWindowsSys::WriteConsole( std::string& arStrOut )
{
	if ( !m_hConsoleOut )
		return;

	DWORD		count;

	// Here WriteConsoleA is for ASCII, 
	// and WriteConsoleW is for UNICODE, so if I want to print out normal character string on the console screen
	// I need to call WriteConsoleA.
	::WriteConsoleA( m_hConsoleOut, arStrOut.c_str(), arStrOut.length(), &count, NULL);
}

void
CWindowsSys::CloseConsole()
{
	FreeConsole();
}

void	
CWindowsSys::HandleException()
{	
	int x = 1;
	__try 
	{
		{
			{
				{
					int xxx = x;
					return;
				}

				int xx = x;
			}
		}
	}
	__finally
	{
		x++;
	}

	int y = x;
}

void	
CWindowsSys::TestMemoryMappedFile()
{
	LPCSTR	fOut = "D:\\Sungsu_LAB\\App\\Windows\\WindowsSystem\\Debug\\TestMemoryMapped.txt";
	HANDLE	hOut;
	HANDLE	hOutMap;

	LPTSTR	pOutFile, pOut;
	LARGE_INTEGER	fileSize;

	fileSize.QuadPart = 1000;	// arbitrary for testing...

	hOut		= CreateFileA( fOut, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	hOutMap		= CreateFileMapping( hOut, NULL, PAGE_READWRITE, fileSize.HighPart, fileSize.LowPart, NULL );
	pOutFile	= (LPTSTR)MapViewOfFile( hOutMap, FILE_MAP_WRITE, 0, 0, (SIZE_T)fileSize.QuadPart );
	pOut		= pOutFile;

	for ( int i = 0; i < (fileSize.QuadPart / 4); i++ )
	{
// 		char buffer[4];
// 		sprintf_s(buffer, "%d", i);
// 
// 		std::string strOut = buffer;
// 		WriteConsole(strOut);
// 
// 		memcpy_s(pOut, 4, buffer, 4);

		*pOut = 'CBA';
		pOut += 2;
	}

	pOutFile[fileSize.QuadPart -1] = '\0';

	UnmapViewOfFile(pOutFile);
	CloseHandle(hOutMap);
	CloseHandle(hOut);
}

void	
CWindowsSys::TestProcessCall()
{
	TCHAR		commandLine[256]	= _T("WindowsSystem");
	DWORD		dwCreationFlags		= CREATE_UNICODE_ENVIRONMENT;
	HANDLE		hProcess;
	STARTUPINFO	startupSearch;
	PROCESS_INFORMATION		procInfo;

	GetStartupInfo( &startupSearch );

	bool bProcOpened		= false;
	bool bProcTerminated	= false;
	
	bProcOpened = CreateProcess( NULL, commandLine, NULL, NULL, TRUE, dwCreationFlags, NULL, NULL, &startupSearch, &procInfo );
	if ( bProcOpened )
	{
		hProcess = procInfo.hProcess;

		// Wait some time......for the process to finish tasks...
		if ( WaitForSingleObject( hProcess, 60000 ) != WAIT_OBJECT_0 )
		{
			// Timeout..
		}
		
		bProcTerminated = true;
	}

	return;
}

void	
CWindowsSys::TestThread()
{
	enum { eNumThread = 5 };
	char buffer[128];
	std::string strThread;

	DWORD	exitCode;
	HANDLE	hThread[eNumThread];

	for (int i = 0; i < eNumThread; i++)
	{
		m_threadParam = (DWORD)i;
		hThread[i] = (HANDLE)_beginthreadex(NULL,
											0,										// 0 : use default stack size (1024KB)
											(_beginthreadex_proc_type)ThreadProc,	// I need to use type-casting on VS2015
											this,									// To designate the address of this class instance
											0,										// 0 : Execute right away, 1 : CREATE_SUSPENDED
											NULL);									// Thread ID
		// To have an interval between each thread..
		Sleep(1);
		if ( hThread[i] == NULL )
			return;
	}

	bool	bJobDone		= false;
	int		numThreadCnt	= eNumThread;
#if 1
	if ( WaitForMultipleObjects(numThreadCnt, hThread, true, 10000) != WAIT_OBJECT_0 )
		bJobDone = false;
	else
		bJobDone = true;

	for(int i = 0; i < eNumThread; i++)
	{
		GetExitCodeThread(hThread[i], &exitCode);

		sprintf_s(buffer, "ExitCode=%d\r\n", exitCode);
		strThread = buffer;
		WriteConsole(strThread);

		CloseHandle(hThread[i]);
	}

#else
	DWORD	nHandle;
	int		nThread;

	while( numThreadCnt )
	{
		nHandle = WaitForMultipleObjects(numThreadCnt, hThread, FALSE, 10000 );	
		nThread	= (int)nHandle - (int)WAIT_OBJECT_0;

		GetExitCodeThread(hThread[nThread], &exitCode);
		CloseHandle(hThread[nThread]);

		sprintf_s(buffer, "nHandle=%d, nThread=%d, ExitCode=%d\r\n", nHandle, nThread, exitCode);
		strThread = buffer;
		WriteConsole(strThread);

		hThread[nThread] = hThread[numThreadCnt-1];	// This is mandatory
		numThreadCnt--;
	}
#endif
	bool bCheck = bJobDone;

	TestVariable("%d",1,2,3,4,5);
}

DWORD WINAPI
CWindowsSys::ThreadProc(LPVOID* pParam)
{
	DWORD			exitCode;
	CWindowsSys*	pThis	 = reinterpret_cast<CWindowsSys*>(pParam);

	exitCode = pThis->m_threadParam;
	pThis->Worker(pThis->m_threadParam);

	return exitCode;
}

void	
CWindowsSys::Worker(DWORD aWorker)
{
	long tickStart = ::GetTickCount();

	char buffer[128];
	std::string strWorker;

	sprintf_s(buffer, "Worker#%d:Start..\r\n", aWorker);
	strWorker = buffer;

	WriteConsole(strWorker);

	while ((::GetTickCount() - tickStart) < 3000)
	{
		sprintf_s(buffer, "Worker#%d:Working.. tick=%d\r\n", aWorker, (::GetTickCount() - tickStart) );
		strWorker = buffer;

		WriteConsole(strWorker);
		Sleep(500);
	}

	sprintf_s(buffer, "Worker#%d:End.. tick=%d\r\n", aWorker, (::GetTickCount() - tickStart));
	strWorker = buffer;

	WriteConsole(strWorker);

	return;
}

//#define _USE_MUTEX_	1
void	
CWindowsSys::TestMutex()
{
	enum { eNumThread = 5 };
	char buffer[128];
	std::string strThread;

#ifdef _USE_MUTEX_
	sprintf_s(buffer, "TestMutex\r\n");
#else
	sprintf_s(buffer, "TestWithoutMutex\r\n");
#endif
	strThread = buffer;
	WriteConsole(strThread);

	DWORD	exitCode;
	HANDLE	hThread[eNumThread];

	m_strMutexName	= _T("MyMutex");

	for (int i = 0; i < eNumThread; i++)
	{
		m_threadMutexParam = (DWORD)i;
		hThread[i] = (HANDLE)_beginthreadex(NULL,
											0,											// 0 : use default stack size (1024KB)
											(_beginthreadex_proc_type)ThreadMutexProc,	// I need to use type-casting on VS2015
											this,										// To designate the address of this class instance
											0,											// 0 : Execute right away, 1 : CREATE_SUSPENDED
											NULL);										// Thread ID
		// To give an interval between each thread..																		
		Sleep(1);	
		if (hThread[i] == NULL)
			return;
	}

	bool	bJobDone = false;
	int		numThreadCnt = eNumThread;
	if (WaitForMultipleObjects(numThreadCnt, hThread, true, INFINITE) != WAIT_OBJECT_0)
		bJobDone = false;
	else
		bJobDone = true;

	for (int i = 0; i < eNumThread; i++)
	{
		GetExitCodeThread(hThread[i], &exitCode);

		sprintf_s(buffer, "ExitCode=%d\r\n", exitCode);
		strThread = buffer;
		WriteConsole(strThread);

		CloseHandle(hThread[i]);
	}

#ifdef _USE_MUTEX_
	sprintf_s(buffer, "TestMutex...Done\r\n", exitCode);
#else
	sprintf_s(buffer, "TestWithoutMutex...Done\r\n", exitCode);
#endif
	strThread = buffer;
	WriteConsole(strThread);
}

void
CWindowsSys::WorkerMutex(DWORD aWorker)
{
	char buffer[128];
	std::string strWorker;

#ifdef _USE_MUTEX_
	HANDLE hMutex = CreateMutex(NULL, true, m_strMutexName);
	if (!hMutex)
		return;

	DWORD waitResult;
	waitResult = WaitForSingleObject(hMutex, INFINITE);
	if ((waitResult != WAIT_OBJECT_0) && (waitResult != WAIT_ABANDONED_0))
		return;

	sprintf_s(buffer, "Worker#%d:Got Mutex!\r\n", aWorker);
	strWorker = buffer;
	WriteConsole(strWorker);

#else
	sprintf_s(buffer, "Worker#%d:Without Mutex!\r\n", aWorker);
	strWorker = buffer;
	WriteConsole(strWorker);
#endif

	long tickStart = ::GetTickCount();
	for( int i = 0; i < 5; i++)
	{
		sprintf_s(buffer, "Worker#%d:Working%d/5.. tick=%d\r\n", aWorker, i+1, (::GetTickCount() - tickStart));
		strWorker = buffer;
		WriteConsole(strWorker);

		Sleep(200);
	}

#ifdef _USE_MUTEX_
	ReleaseMutex(hMutex);
#endif

	return;
}

DWORD WINAPI
CWindowsSys::ThreadMutexProc(LPVOID* pParam)
{
	DWORD			exitCode;
	CWindowsSys*	pThis = reinterpret_cast<CWindowsSys*>(pParam);

	exitCode = pThis->m_threadMutexParam;
	pThis->WorkerMutex(pThis->m_threadMutexParam);

	return exitCode;
}

void	
CWindowsSys::TestSemaphore()
{
	enum { eNumThread = 5 };						// Thread count = 5

	m_strSemaphoreName = _T("MySemaphore");

	// Create semaphore.
	HANDLE	hSemaphore;
	long	countSemaphore = eNumThread - 2;		// Semaphore count = 3 (=5-2)
	hSemaphore = CreateSemaphore(NULL, countSemaphore, countSemaphore, m_strSemaphoreName);

	char buffer[128];
	std::string strThread;

	sprintf_s(buffer, "TestSemaphore, ThreadCnt=%d, SemCountMax=%d\r\n", eNumThread, countSemaphore );
	strThread = buffer;
	WriteConsole(strThread);

	DWORD	exitCode;
	HANDLE	hThread[eNumThread];

	for (int i = 0; i < eNumThread; i++)
	{
		m_threadSemParam = (DWORD)i;
		hThread[i] = (HANDLE)_beginthreadex(NULL,
											0,											// 0 : use default stack size (1024KB)
											(_beginthreadex_proc_type)ThreadSemaphoreProc,	// I need to use type-casting on VS2015
											this,										// To designate the address of this class instance
											0,											// 0 : Execute right away, 1 : CREATE_SUSPENDED
											NULL);										// Thread ID
																						// To give an interval between each thread..																		
		Sleep(1);
		if (hThread[i] == NULL)
			return;
	}

	bool	bJobDone = false;
	int		numThreadCnt = eNumThread;
	if (WaitForMultipleObjects(numThreadCnt, hThread, true, INFINITE) != WAIT_OBJECT_0)
		bJobDone = false;
	else
		bJobDone = true;

	for (int i = 0; i < eNumThread; i++)
	{
		GetExitCodeThread(hThread[i], &exitCode);

		sprintf_s(buffer, "ExitCode=%d\r\n", exitCode);
		strThread = buffer;
		WriteConsole(strThread);

		CloseHandle(hThread[i]);
	}

	sprintf_s(buffer, "TestSemaphore...Done\r\n", exitCode);

	strThread = buffer;
	WriteConsole(strThread);
}

void	
CWindowsSys::WorkerSemaphore(DWORD aWorker)
{
	char buffer[128];
	std::string strWorker;

	// Open the semaphore handle.
	HANDLE hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS/*SYNCHRONIZE*/, false, m_strSemaphoreName);
	if (!hSemaphore)		// Here the access flag should be at least SEMAPHORE_MODIFY_STATE to be able to modify count 
		return;				// by using ReleaseSemaphore. Otherwise, the count doesn't change.

	sprintf_s(buffer, "Worker#%d:Wait to start..\r\n", aWorker);
	strWorker = buffer;
	WriteConsole(strWorker);

	// Wait for a semaphore to begin working.
	DWORD waitResult;
	waitResult = WaitForSingleObject(hSemaphore, INFINITE);
	if ((waitResult != WAIT_OBJECT_0) && (waitResult != WAIT_ABANDONED_0))
		return;

	sprintf_s(buffer, "Worker#%d:Got Semaphore!\r\n", aWorker);
	strWorker = buffer;
	WriteConsole(strWorker);

	long tickStart = ::GetTickCount();
	for (int i = 0; i < 5; i++)
	{
		sprintf_s(buffer, "Worker#%d:Working%d/5.. tick=%d\r\n", aWorker, i + 1, (::GetTickCount() - tickStart));
		strWorker = buffer;
		WriteConsole(strWorker);

		Sleep(200);
	}

	// Release semaphore so that another thread waiting for the semaphore can work.
	long prevCount;
	ReleaseSemaphore( hSemaphore, 1, &prevCount);	// increase by just 1, and don't refer the last count.

	sprintf_s(buffer, "Worker#%d:Finished..SemCount(Prev=%d,Now=%d)\r\n", aWorker, prevCount, prevCount+1 );
	strWorker = buffer;
	WriteConsole(strWorker);

	return;
}

DWORD WINAPI 
CWindowsSys::ThreadSemaphoreProc(LPVOID* pParam)
{
	DWORD			exitCode;
	CWindowsSys*	pThis = reinterpret_cast<CWindowsSys*>(pParam);

	exitCode = pThis->m_threadSemParam;
	pThis->WorkerSemaphore(pThis->m_threadSemParam);

	return exitCode;
}

void	
CWindowsSys::TestThreadPool()
{
	enum 
	{
		eNumWoker = 20
	};

	TP_CALLBACK_ENVIRON	cbe;
	PTP_WORK			ptp[eNumWoker];

	InitializeThreadpoolEnvironment( &cbe );

	m_threadPoolParam = 3000;

	for ( int i = 0; i < eNumWoker; i++ )
	{
		ptp[i] = CreateThreadpoolWork( WorkerThreadPool, this, &cbe );
		if ( ptp[i] == NULL )
			return;
	}

	for (int i = 0; i < eNumWoker; i++)
		SubmitThreadpoolWork( ptp[i] );

	for (int i = 0; i < eNumWoker; i++)
	{
		WaitForThreadpoolWorkCallbacks( ptp[i], FALSE );
		CloseThreadpoolWork( ptp[i] );
	}
}

void CALLBACK	
CWindowsSys::WorkerThreadPool(PTP_CALLBACK_INSTANCE pInst, PVOID vp, PTP_WORK pWork)
{
	// Note : I should be careful in using C library since this callback isn't by "_beginthreadex".
	char buffer[128];
	std::string strWorker;

	CWindowsSys* pThis = reinterpret_cast<CWindowsSys*>(vp);

	long tickStart = ::GetTickCount();

	sprintf_s(buffer, "Worker:Start, ThreadID=%d\n\r", GetCurrentThreadId() );
	strWorker = buffer;
	pThis->WriteConsole(strWorker);

	long tickEnd= tickStart + pThis->m_threadPoolParam;
	while (::GetTickCount() < tickEnd )
		Sleep(0);
	
	sprintf_s(buffer, "Worker:Ended, tick=%d\n\r", ::GetTickCount() - tickStart);
	strWorker = buffer;
	pThis->WriteConsole(strWorker);
}

DWORD	
CWindowsSys::QueueInitialize(QUEUE_OBJECT* q, DWORD mSize, DWORD nMsgs)
{
	if ((q->msgArray = (char*)calloc(nMsgs, mSize)) == NULL)
		return 1;

	q->qFirst	= q->qLast = 0;
	q->qSize	= nMsgs;

	q->qGuard	= CreateMutex( NULL, FALSE, NULL );
	q->qNe		= CreateEvent( NULL, TRUE, FALSE, NULL );
	q->qNf		= CreateEvent(NULL, TRUE, FALSE, NULL);

	return 0;
}

DWORD	
CWindowsSys::QueueDestroy(QUEUE_OBJECT* q)
{
	WaitForSingleObject( q->qGuard, INFINITE );
	
	free( q->msgArray );
	q->msgArray = NULL;
	CloseHandle( q->qNe );
	CloseHandle( q->qNf );
	ReleaseMutex( q->qGuard );
	CloseHandle( q->qGuard );

	return 0;
}

DWORD	
CWindowsSys::QueueDestroyed(QUEUE_OBJECT* q)
{
	return q->msgArray == NULL;
}

DWORD	
CWindowsSys::QueueEmpty(QUEUE_OBJECT* q)
{
	return (q->qFirst == q->qLast);
}

DWORD	
CWindowsSys::QueueFull(QUEUE_OBJECT* q)
{
	return (q->qFirst - q->qLast) == 1 ||
			((q->qLast == (q->qSize - 1)) && (q->qFirst == 0));
}

DWORD	
CWindowsSys::QueueGet(QUEUE_OBJECT* q, PVOID msg, DWORD mSize, DWORD maxWait)
{
	DWORD	TotalWaitTime	= 0;
	BOOL	TimedOut		= FALSE;

	WaitForSingleObject( q->qGuard, INFINITE );
	if ( q->msgArray == NULL )
		return 1;

	//....

	return 0;
}

DWORD	
CWindowsSys::QueuePut(QUEUE_OBJECT* q, PVOID msg, DWORD mSize, DWORD maxWait)
{
	return 0;
}

DWORD	
CWindowsSys::QueueRemove(QUEUE_OBJECT*, DWORD, DWORD)
{
	return 0;
}

DWORD	
CWindowsSys::QueueInsert(QUEUE_OBJECT*, DWORD, DWORD)
{
	return 0;
}


void	
CWindowsSys::TestVariable(const char* str, ...)
{
	;
}