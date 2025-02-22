
#ifndef __PROCESS_TEST_H__
#define __PROCESS_TEST_H__

class CProcTest
{
public:
	CProcTest();
	~CProcTest();
	
	// Fundamental
	void 	GetPid			();
	int		Exec			();
	int		Fork			();
	
	int		Exit			();
	int		Wait			();
	int		WaitPid			();
	
	int		MySystem		( const char* aCmd );
	int		SessionID		();
	int		Demon			();
	
	// Advanced
	int		Nice			();
	int		Priority		();
	int		CpuAffinity		();
	int		Scheduling		();
	int		RetrieveResourceLimit();
	int		ChangeResourceLimit();
	
	// Etc
	int		FdRedirecting();
	
	static void ExitFunc1	();
	static void ExitFunc2	();
	static void OnExitFunc1	( int status, void* arg );
};

#endif
