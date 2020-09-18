
#include "Common.h"

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
#include "DriverTest.h"
#include "GPIOTest.h"

int main(int argc, char* argv[]) 
{
	// Show the current directory.
	char currentdir[256] = {0,};
	getcwd( currentdir, 256 );
	printf( "currentdir=%s\n", currentdir );	

	// Get classFlag and funcFlag from input arguments.
	if ( argc < 3 )
	{
		printf("Arguments must be more than three.\n");
		return 1;
	}
	
	char* pFuncFlag = NULL;
	int	classFlag 	= atoi(argv[1]);
	int funcFlag 	= (int)strtol(argv[2], &pFuncFlag, 16);

	printf("arg[1]=%d (classFlag)\n", classFlag);
	printf("arg[2]=0x%x (funcFlag)\n", funcFlag);
		
//	const char* fileLogging = "/home/pi/git_repository/Sungsu_Lab/practice_linux/bin/testLogging.txt";		
//	g_Logger.SetLogFilePath( fileLogging );
//	g_Logger.Telemetry("TEST_FILE is enabled...");
	
	CFileTest 		testFile;
	CProcTest		testProc;
	CPThreadTest 	testPThread;
	CSignalTest		testSignal;
	CTimeTest		testTime;
	CFileTestAdv	testFileAdv;
	CDirectoryTest	testDir;
	CMemoryTest		testMemory;
	CDriverTest		testDriver;
	CGpioInterface	*pGpioIF = NULL;

	const char* fileRead			= "/home/pi/git_repository/Sungsu_Lab/practice_linux/bin/test.txt";
	const char* fileWrite 			= "/home/pi/git_repository/Sungsu_Lab/practice_linux/bin/testWrite.txt";
	const char* fileReadWriteBinary = "/home/pi/git_repository/Sungsu_Lab/practice_linux/bin/testReadWriteBinary.txt";

	switch ( classFlag )
	{
		case 1: 
			printf("CFileTest..\n");
			if ( funcFlag & 0x0001 ) 	testFile.Read		( fileRead );
			if ( funcFlag & 0x0002 ) 	testFile.Write		( fileWrite, "test....write\r\n");
			if ( funcFlag & 0x0004 ) 	testFile.Select		();	
			if ( funcFlag & 0x0008 ) 	testFile.Poll		();
			if ( funcFlag & 0x0010 ) 	testFile.ReadStd	( fileRead );
			if ( funcFlag & 0x0020 ) 	testFile.WriteStd	( fileWrite, "test....writeStd\r\n");
			if ( funcFlag & 0x0040 ) 	testFile.ReadWriteBinaryStd( fileReadWriteBinary );
			break;
		case 2:
			printf("CProcTest..\n");
			if ( funcFlag & 0x0001 ) 	testProc.GetPid		();
			if ( funcFlag & 0x0002 ) 	testProc.Exec		();
			if ( funcFlag & 0x0004 ) 	testProc.Fork		();
			if ( funcFlag & 0x0008 ) 	testProc.Exit		();
			if ( funcFlag & 0x0010 ) 	testProc.Wait		();
			if ( funcFlag & 0x0020 ) 	testProc.WaitPid	();
			if ( funcFlag & 0x0040 ) 	testProc.MySystem	("vim");
			if ( funcFlag & 0x0080 ) 	testProc.SessionID	();
			if ( funcFlag & 0x0100 ) 	testProc.Demon			();
			if ( funcFlag & 0x0200 ) 	testProc.FdRedirecting	();
			if ( funcFlag & 0x0400 )	testProc.Nice		();
			if ( funcFlag & 0x0800 ) 	testProc.Priority	();
			if ( funcFlag & 0x1000 ) 	testProc.CpuAffinity();
			if ( funcFlag & 0x2000 ) 	testProc.Scheduling	();
			if ( funcFlag & 0x4000 ) 	testProc.RetrieveResourceLimit	();
			if ( funcFlag & 0x8000 ) 	testProc.ChangeResourceLimit	();
			break;
		case 3:
			printf("CPThreadTest..\n");
			if ( funcFlag & 0x0001 ) 	testPThread.CreateJoin			();
			if ( funcFlag & 0x0002 ) 	testPThread.SyncMutex			( false /*abMakedeadlock*/ );
			if ( funcFlag & 0x0004 ) 	testPThread.ReaderWriterLocks	();	/*TBD*/
			if ( funcFlag & 0x0008 ) 	testPThread.ConditionVariable	();
			if ( funcFlag & 0x0010 ) 	testPThread.RecursiveMutex		();
			if ( funcFlag & 0x0020 ) 	testPThread.ThreadNSignal		();
			break;
		case 4:
			printf("CSignalTest..\n");
			if ( funcFlag & 0x0001 ) 	testSignal.SignalFundamental	();
			if ( funcFlag & 0x0002 ) 	testSignal.SignalFundamental2	();
			if ( funcFlag & 0x0004 ) 	testSignal.SignalDeadLock		();
			if ( funcFlag & 0x0008 ) 	testSignal.SignalFundamental3	();
			if ( funcFlag & 0x0010 ) 	testSignal.SignalAdvanced		();
			break;
		case 5:
			printf("CTimeTest..\n");
			if ( funcFlag & 0x0001 ) 	testTime.SysConf				();
			if ( funcFlag & 0x0002 ) 	testTime.ClockResolution		();
			if ( funcFlag & 0x0004 ) 	testTime.GetCurrentTime			();
			if ( funcFlag & 0x0008 ) 	testTime.GetProcessTime			();
			if ( funcFlag & 0x0010 ) 	testTime.ConversionTime			();
			if ( funcFlag & 0x0020 ) 	testTime.CalculateTimeBasic		();
			if ( funcFlag & 0x0040 ) 	testTime.Sleep					();
			if ( funcFlag & 0x0080 ) 	testTime.AdvancedSleep			();
			if ( funcFlag & 0x0100 ) 	testTime.TimerWithAlarm			();
			if ( funcFlag & 0x0200 )	testTime.IntervalTimer			();
			if ( funcFlag & 0x0400 )	testTime.AdvancedTimerSignal	();
			if ( funcFlag & 0x0800 ) 	testTime.AdvancedTimerThread	();
			break;
		case 6:
			printf("CFileTestAdv..\n");
			if ( funcFlag & 0x0001 ) 	testFileAdv.WriteVector			();
			if ( funcFlag & 0x0002 ) 	testFileAdv.ReadVector			();	
			if ( funcFlag & 0x0004 ) 	testFileAdv.Epoll				();
			if ( funcFlag & 0x0008 ) 	testFileAdv.MemoryMappedRead	();
			if ( funcFlag & 0x0010 ) 	testFileAdv.MemoryMappedWrite	();
			if ( funcFlag & 0x0020 ) 	testFileAdv.GetFileINode			( argc, (const char**)argv );
			if ( funcFlag & 0x0040 ) 	testFileAdv.SortFilebyPhysicalBlock	( argc, (const char**)argv );
			break;
		case 7:
			printf("CDirectoryTest..\n");
			if ( funcFlag & 0x0001 )	testDir.Stat					(argc, (const char**)argv );
			if ( funcFlag & 0x0002 ) 	testDir.ChangePermission		(argc, (const char**)argv );
			if ( funcFlag & 0x0004 ) 	testDir.ChangeOwnership			(argc, (const char**)argv );
			if ( funcFlag & 0x0008 )	testDir.ChangeOwnershipRoot		(argc, (const char**)argv );
			if ( funcFlag & 0x0010 ) 	testDir.GetCurrentDir			();
			if ( funcFlag & 0x0020 ) 	testDir.ChangeCurrentDir		(argc, (const char**)argv );
			if ( funcFlag & 0x0040 ) 	testDir.ChangeCurrentDirResotore(argc, (const char**)argv );
			if ( funcFlag & 0x0080 ) 	testDir.MakeDir					(argc, (const char**)argv );
			if ( funcFlag & 0x0100 ) 	testDir.RemoveDir				(argc, (const char**)argv );
			if ( funcFlag & 0x0200 ) 	testDir.DirStream				(argc, (const char**)argv );
			if ( funcFlag & 0x0400 ) 	testDir.MakeHardLink			(argc, (const char**)argv );
			if ( funcFlag & 0x0800 ) 	testDir.MakeSymbolicLink		(argc, (const char**)argv );
			if ( funcFlag & 0x1000 ) 	testDir.DoUnLink				(argc, (const char**)argv );
			if ( funcFlag & 0x2000 ) 	testDir.MoveFile				(argc, (const char**)argv );
			if ( funcFlag & 0x4000 ) 	testDir.TestInotify				(argc, (const char**)argv );
			break;
		case 8:
			printf("CMemoryTest..\n");
			if ( funcFlag & 0x0001 ) 	testMemory.AllocDynamicMemory	();
			if ( funcFlag & 0x0002 ) 	testMemory.AlignMem				();
			if ( funcFlag & 0x0004 ) 	testMemory.DataSegment			();
			if ( funcFlag & 0x0008 ) 	testMemory.AnonymousMapping		();
			if ( funcFlag & 0x0010 ) 	testMemory.UseDevZeroObsolete	();
			if ( funcFlag & 0x0020 ) 	testMemory.AdvancedMemoryAlloc	();
			if ( funcFlag & 0x0040 ) 	testMemory.AllocBasedOnStack	(1);
			if ( funcFlag & 0x0080 )	testMemory.ManipulateMem		();
			break;
		case 9:
			printf("CDriverTest..\n");
			if ( funcFlag & 0x0001 )	testDriver.FirstDriver			();
			break;
			
		case 10:
			printf("CGpioPGpio..\n");
			pGpioIF = new CGpioPGpio;
			
			if ( funcFlag & 0x0001 )	pGpioIF->TestGpio1();
			if ( funcFlag & 0x0002 )	pGpioIF->TestGpio2();
		
			delete pGpioIF;
			pGpioIF = NULL;
			break;
			
		case 11:
			printf("CGpioLGpiod..\n");
			pGpioIF = new CGpioLGpiod;
			
			if ( funcFlag & 0x0001 )	pGpioIF->TestGpio1();
			if ( funcFlag & 0x0002 )	pGpioIF->TestGpio2();
		
			pGpioIF->TestGpio3();

			delete pGpioIF;
			pGpioIF = NULL;
			break;
			
		case 12:
			printf("CGpioDirectReg..\n");
			pGpioIF = new CGpioDirectReg;
			
			if ( funcFlag & 0x0001 )	pGpioIF->TestGpio1();
			if ( funcFlag & 0x0002 )	pGpioIF->TestGpio2();
		
			delete pGpioIF;
			pGpioIF = NULL;
			break;
		default:
			break;
	}


	return 0;
}
