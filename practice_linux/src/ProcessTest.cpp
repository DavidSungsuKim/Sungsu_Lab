
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "ProcessTest.h"
#include "Logger.h"

CProcTest::CProcTest()
{
	
}

CProcTest::~CProcTest()
{
	
}

void
CProcTest::GetPid()
{
	pid_t	pid;
	pid_t	ppid;
	pid 	= getpid();
	ppid	= getppid();
	
	g_Logger.Telemetry2( __FILE__, __LINE__, "pid=%d, ppid=%d", pid, ppid );
}

int
CProcTest::Exec()
{
	int	ret = 0;

/*	I don't know why it doesn't work below.
	const char* pathBin = "usr/bin/vim";
	const char* arg		= "vim";
	const char* pathFile= "/home/pi/git_repository/Sungsu_Lab/practice_linux/bin/test.txt";
	
	ret = execlp( pathBin, arg, NULL );	// The last argument MUST be "NULL" in execl().	
//	ret = execvp( "vim", "vim", pathFile, NULL);
	*/
	ret = execl( "/home/pi/git_repository/Sungsu_Lab/practice_linux/bin/helloworld", "helloworld", NULL );
	if ( ret == -1 )
	{
		g_Logger.Telemetry2( __FILE__, __LINE__, "ret=-1");
		perror("execl");
	}
	
//	g_Logger.Telemetry2( __FILE__, __LINE__, "Is this code reachable?");	
	
	return ret;
}

int 
CProcTest::Fork()
{
	int ret = 0;
	
	pid_t pid;
	g_Logger.Telemetry2( __FILE__, __LINE__, "before fork....." );
		
	pid = fork();
	
	if ( pid > 0 )
		g_Logger.Telemetry2( __FILE__, __LINE__, "child pid=%d", pid);
	else if ( pid == 0 )
	{
		g_Logger.Telemetry2( __FILE__, __LINE__, "pid(@child)=%d", pid);	

		ret = execl( "/home/pi/git_repository/Sungsu_Lab/practice_linux/bin/helloworld", "helloworld", NULL );
		if ( ret == -1 )
		{
			g_Logger.Telemetry2( __FILE__, __LINE__, "ret=-1");
			perror("execl");
		}

/*		// Can I do fork() again? Of course
		pid = fork();
		g_Logger.Telemetry2( __FILE__, __LINE__, "fork again.....pid=%d", pid );
		if ( pid == -1 )
			perror("fork");	
		GetPid();	
		*/
	}
	else
	{
		g_Logger.Telemetry2( __FILE__, __LINE__, "fork error..", pid);	
		perror("fork");
	}
	
	return ret;
}

int g_x = 1;

int		
CProcTest::Exit()
{
	int ret = 0;
	long atexit_max;
	
	atexit_max = sysconf (_SC_ATEXIT_MAX);
	g_Logger.Telemetry2( __FILE__, __LINE__, "atexit_max=%ld", atexit_max);		
	
	ret = atexit(ExitFunc1);
	if ( ret )
		perror("atexit");
	ret = atexit(ExitFunc2);
	if ( ret )
		perror("atexit");
	
	ret = on_exit( OnExitFunc1, &g_x );

	return ret;
}

int
CProcTest::Wait()
{
	int ret = 0;

	int status;
	pid_t	pid;
	
	pid = fork();
	if ( !pid )
	{
	//	abort();
		return -1;
	}
		
	g_Logger.Telemetry2( __FILE__, __LINE__, "Wait():forked... pid(child)=%d", pid);	
		
	pid = wait( &status );
	if ( pid == -1 )
		perror("wait");
	
	printf( "pid=%d\n", pid );	
	
	if ( WIFEXITED(status))
		printf( "Normal termination with exit status=%d\n", WEXITSTATUS( status ));

	// If child process finishes by abort(), it comes in this if.
	if ( WIFSIGNALED(status))
		printf( "Killed by signal=%d%s\n", WTERMSIG(status), WCOREDUMP(status) ? " (dumped core" : "" );
		
	if ( WIFSTOPPED(status))
		printf( "Stopped by signal=%d\n", WSTOPSIG(status) );
		
	if ( WIFCONTINUED(status))
		printf( "Continued\n" );
	
	return ret;
}

int
CProcTest::WaitPid()
{
	int ret = 0;

	int status;
	pid_t	pid;
	
	pid = fork();
	if ( !pid )
		return 0;
		
	sleep(1);	// It seems that I need to give some time for the child process to work before calling waitpid(),
				// or the returned pi from waitpid() becomes '0'.
		
	g_Logger.Telemetry2( __FILE__, __LINE__, "WaitPid():forked... pid(child)=%d", pid);	

#if (0)		
	pid = waitpid( pid, &status, WNOHANG );
	if ( pid == -1 )
		perror("wait");	
#else
	idtype_t 	idtype 	= P_PID;	// P_PID, P_GID, or P_ALL
	id_t		id		= pid;
	siginfo_t	siginfo;

	pid = waitid( idtype, id, &siginfo, WNOHANG );
	
/*	printf( "	siginfo.si_signo=%d\n", 	siginfo.si_signo );	
	printf( "	siginfo.si_code=%d\n", 		siginfo.si_code );
	printf( "	siginfo.si_pid=%d\n", 		siginfo.si_pid );
	printf( "	siginfo.si_uid=%d\n", 		siginfo.si_uid );
	printf( "	siginfo.*si_addr=0x%x\n", 	siginfo.si_addr);
	printf( "	siginfo.si_status=%d\n", 	siginfo.si_status );
	printf( "	siginfo.si_band=%d\n", 		siginfo.si_band );
	*/ 
#endif

	printf( "pid=%d\n", pid );	

	if ( WIFEXITED(status))
	printf( "Normal termination with exit status=%d\n", WEXITSTATUS( status ));

	if ( WIFSIGNALED(status))
		printf( "Killed by signal=%d%s\n", WTERMSIG(status), WCOREDUMP(status) ? " (dumped core)" : "" );
		
	return ret;
}

int
CProcTest::MySystem( const char* aCmd )
{	
	int ret;
		
#define MY_SYSTEM
#ifndef MY_SYSTEM
	int ret;
	
	ret = system("vim");
	printf("%d=system()\r\n", ret);
	
	return ret;
#else
	int status;
	pid_t	pid;
	
	pid = fork();
	printf("MySystem:fork...pid=%d\r\n", pid);
	
	if ( pid == -1 )
	{
		printf("MySystem:pid=%d, L=%d\r\n", pid, __LINE__);
		return -1;
	}
	else if ( pid == 0 )
	{
	//	case1)
	//	char* const argv[] = {"sh", "-c", "vim", NULL};
	
	//	case2) So it can have command from input arguments.
		char* argv[4];
		argv[0] = (char*)"sh";
		argv[1] = (char*)"-c";
		argv[2] = (char*)aCmd;
		argv[3] = NULL;
		
	/*	case3) This doesn't work since the type of 'argv' is char const*, 
	 	which means all values in the container must be fixed in compile time and are not changeable.
	 
		char const* argv[4];
		argv[0] = "sh";
		argv[1] = "-c";
		argv[2] = "vim"//aCmd;
		argv[3] = NULL;
		*/
	
		printf("MySystem:pid=%d, L=%d\r\n", pid, __LINE__);
	
		ret = execv( "/bin/sh", argv );
		if ( ret == -1 )
			perror("execv");
		
		exit(-1);
	}
	
	printf("MySystem:wait..\r\n");
	
	sleep(1);
	
	if ( waitpid( pid, &status, 0 ) == -1 )
	{
		printf("MySystem:wait...(-1)\r\n");
		return -1;
	}
	else if ( WIFEXITED( status ) )
	{
		printf("MySystem:wait...exited\r\n");
		return WEXITSTATUS( status );
	}
	
	printf("MySystem:wait...done\r\n");
	
	return -1;
#endif
}

void 
CProcTest::ExitFunc1()
{
	g_Logger.Telemetry2( __FILE__, __LINE__, "Exitfunc1...");	
}

void
CProcTest::ExitFunc2()
{
	g_Logger.Telemetry2( __FILE__, __LINE__, "Exitfunc2...");	
}

void 
CProcTest::OnExitFunc1( int status, void* arg )
{
	// 'status' is the value from return or exit().
	g_Logger.Telemetry2( __FILE__, __LINE__, "OnExitFunc1... status=%d, g_x=%d", status, *(int*)arg );
}
