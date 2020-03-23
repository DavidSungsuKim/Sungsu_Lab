
#ifndef __PROCESS_TEST_H__
#define __PROCESS_TEST_H__
#endif

class CProcTest
{
public:
	CProcTest();
	~CProcTest();
	
	void 	GetPid	();
	int		Exec	();
	int		Fork	();
	
	int		Exit	();
	int		Wait	();
	int		WaitPid	();
	
	int		MySystem( const char* aCmd );
	int		SessionID();
	int		Demon	();
	
	static void ExitFunc1	();
	static void ExitFunc2	();
	static void OnExitFunc1	( int status, void* arg );
};
