#include <stdio.h>
#include <unistd.h>
#include "class.h"
#include "FileTest.h"
#include "Logger.h"
#include "ProcessTest.h"
#include "PThreadTest.h"
#include "SignalTest.h"
#include "TimeTest.h"
#include "FileTestAdvanced.h"
#include "DirectoryTest.h"
#include "MemoryTest.h"

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

//	CTimeTest	testTime;
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
//	testTime.AdvancedTimerThread();
	
//	CFileTestAdv	testFileAdv;
//	testFileAdv.WriteVector();
//	testFileAdv.ReadVector();	
//	testFileAdv.Epoll();
//	testFileAdv.MemoryMappedRead();
//	testFileAdv.MemoryMappedWrite();
//	testFileAdv.GetFileINode			( argc, (const char**)argv );
//	testFileAdv.SortFilebyPhysicalBlock	( argc, (const char**)argv );

//	CDirectoryTest	testDir;
//	testDir.Stat				(argc, (const char**)argv );
//	testDir.ChangePermission	(argc, (const char**)argv );
//	testDir.ChangeOwnership		(argc, (const char**)argv );
//	testDir.ChangeOwnershipRoot	(argc, (const char**)argv );
//	testDir.GetCurrentDir();
//	testDir.ChangeCurrentDir		(argc, (const char**)argv );
//	testDir.ChangeCurrentDirResotore(argc, (const char**)argv );
//	testDir.MakeDir					(argc, (const char**)argv );
//	testDir.RemoveDir				(argc, (const char**)argv );
//	testDir.DirStream				(argc, (const char**)argv );
//	testDir.MakeHardLink			(argc, (const char**)argv );
//	testDir.MakeSymbolicLink		(argc, (const char**)argv );
//	testDir.DoUnLink				(argc, (const char**)argv );
//	testDir.MoveFile				(argc, (const char**)argv );
//	testDir.TestInotify				(argc, (const char**)argv );
	
	CMemoryTest	testMemory;
//	testMemory.AllocDynamicMemory();
//	testMemory.AlignMem();
//	testMemory.DataSegment();
//	testMemory.AnonymousMapping();
//	testMemory.UseDevZeroObsolete();
//	testMemory.AdvancedMemoryAlloc();
//	testMemory.AllocBasedOnStack( 1 );
	testMemory.ManipulateMem();
	
	return 0;
}
