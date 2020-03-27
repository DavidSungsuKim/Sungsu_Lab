#pragma once

////////////////////////////////////////////////////////////////////////
// CWindowsSys class is for testing Windows sytem programming interface.
////////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>

class CWindowsSys
{
public:
	CWindowsSys();
	~CWindowsSys();

	// Console
	HANDLE	m_hConsoleRead;
	HANDLE	m_hConsoleOut;
	long	m_countWrite;

	void	OpenConsole		();
	void	ReadConsole		( std::string& arStrIn );
	void	WriteConsole	( std::string& arStrOut );
	void	CloseConsole	();

	// Exception
	void	HandleException	();
	
	// Memory Mapped File
	void	TestMemoryMappedFile();
	
	// Process
	void	TestProcessCall	();

	// Thread
	DWORD	m_threadParam;
	void	TestThread		();
	void	Worker			(DWORD aWorker);
	static DWORD WINAPI ThreadProc(LPVOID* pParam);

	// Mutex
	LPCTSTR	m_strMutexName;
	DWORD	m_threadMutexParam;
	void	TestMutex		();
	void	WorkerMutex		(DWORD aWorker);
	static DWORD WINAPI	ThreadMutexProc(LPVOID* pParam);

	// Semaphore
	LPCTSTR	m_strSemaphoreName;
	DWORD	m_threadSemParam;
	void	TestSemaphore	();
	void	WorkerSemaphore	(DWORD aWorker);
	static DWORD WINAPI ThreadSemaphoreProc(LPVOID* pParam);

	// Event

	// Thread Pool
	DWORD	m_threadPoolParam;
	void	TestThreadPool	();
	static void	CALLBACK	WorkerThreadPool(PTP_CALLBACK_INSTANCE pInst, PVOID vp, PTP_WORK pWork);

	// Wait Instance
	typedef struct QUEUE_OBJECT_TAG
	{
		HANDLE	qGuard;
		HANDLE	qNe;
		HANDLE	qNf;
		DWORD	qSize;
		DWORD	qFirst;
		DWORD	qLast;
		char*	msgArray;
	}QUEUE_OBJECT;

	DWORD	QueueInitialize	(QUEUE_OBJECT*, DWORD, DWORD);
	DWORD	QueueDestroy	(QUEUE_OBJECT*);
	DWORD	QueueDestroyed	(QUEUE_OBJECT*);
	DWORD	QueueEmpty		(QUEUE_OBJECT*);
	DWORD	QueueFull		(QUEUE_OBJECT*);
	DWORD	QueueGet		(QUEUE_OBJECT*, PVOID, DWORD, DWORD);
	DWORD	QueuePut		(QUEUE_OBJECT*, PVOID, DWORD, DWORD);
	DWORD	QueueRemove		(QUEUE_OBJECT*, DWORD, DWORD);
	DWORD	QueueInsert		(QUEUE_OBJECT*, DWORD, DWORD);

	// Etc
	void	TestVariable(const char* str, ...);
};

