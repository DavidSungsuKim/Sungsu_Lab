#include <stdio.h>
#include <unistd.h>
#include "class.h"
#include "FileTest.h"
#include "Logger.h"
#include "ProcessTest.h"
#include "PThreadTest.h"
#include "SignalTest.h"
#include "TimeTest.h"

int main(int argc, char* argv[]) 
{
	CTestChild testChild;
	CTest* pTest = &testChild;
	pTest->Func();
	
	int i;
	for( i = 0; i < argc; i++ )
		printf("argc[%d]=%s\r\n", i, argv[i]);
	
	char currentdir[256] = {0,};
	getcwd( currentdir, 256 );
	printf( "currentdir=%s\n", currentdir );	
	
//	const char* fileLogging = "/home/pi/git_repository/Sungsu_Lab/practice_linux/bin/testLogging.txt";		
//	g_Logger.SetLogFilePath( fileLogging );

#ifdef TEST_FILE
	g_Logger.Telemetry("TEST_FILE is enabled...");
	
	CFileTest testFile;

	const char* fileRead			= "/home/pi/git_repository/Sungsu_Lab/practice_linux/bin/test.txt";
	const char* fileWrite 			= "/home/pi/git_repository/Sungsu_Lab/practice_linux/bin/testWrite.txt";
	const char* fileReadWriteBinary = "/home/pi/git_repository/Sungsu_Lab/practice_linux/bin/testReadWriteBinary.txt";
			
	testFile.Read( fileRead );
	testFile.Write( fileWrite, "test....write\r\n");
	testFile.Select();	
	testFile.Poll();
	
	testFile.ReadStd( fileRead );
	testFile.WriteStd( fileWrite, "test....writeStd\r\n");
	testFile.ReadWriteBinaryStd( fileReadWriteBinary );
#endif
	
	CProcTest	testProc;
	testProc.GetPid();
//	testProc.Exec();
//	testProc.Fork();
//	testProc.Exit();
//	testProc.Wait();
//	testProc.WaitPid();
//	testProc.MySystem("vim");
//	testProc.SessionID();
//	testProc.Demon();
//	testProc.FdRedirecting();

//	testProc.Nice();
//	testProc.Priority();
//	testProc.CpuAffinity();
//	testProc.Scheduling();
//	testProc.RetrieveResourceLimit();
//	testProc.ChangeResourceLimit();
	
//	CPThreadTest testPThread;
//	testPThread.CreateJoin();
//	testPThread.SyncMutex( false /*abMakedeadlock*/ );
//	testPThread.ReaderWriterLocks();	/*TBD*/
//	testPThread.ConditionVariable();
//	testPThread.RecursiveMutex();
//	testPThread.ThreadNSignal();

//	CSignalTest	testSignal;
//	testSignal.SignalFundamental();
//	testSignal.SignalFundamental2();
//	testSignal.SignalDeadLock();
//	testSignal.SignalFundamental3();
//	testSignal.SignalAdvanced();

	CTimeTest	testTime;
//	testTime.SysConf();
//	testTime.ClockResolution();
//	testTime.GetCurrentTime();
//	testTime.GetProcessTime();
//	testTime.ConversionTime();
//	testTime.CalculateTimeBasic();
//	testTime.Sleep();
//	testTime.AdvancedSleep();
//	testTime.TimerWithAlarm();
//	testTime.IntervalTimer();
//	testTime.AdvancedTimerSignal();
	testTime.AdvancedTimerThread();
	
	return 0;
}
