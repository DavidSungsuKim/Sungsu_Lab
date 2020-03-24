
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fcntl.h>
#include <errno.h>
#include <sched.h>
#include <linux/fs.h>

#include "ProcessTest.h"
#include "Logger.h"

extern int errno;

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
	pid_t	sid;
	
	sid = getsid(0);
	
	pid = fork();
	printf("MySystem:fork...pid=%d sid=%d\r\n", pid, sid);
	
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
		
		if ( setsid() == -1 )
		{
			perror("setsid");
			return -1;
		}
	
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

int
CProcTest::SessionID()
{
	int ret = 0;
	
	pid_t pid;
	pid_t sid;
		
	pid = fork();
	if ( pid == -1 )
	{
		perror("fork");
		return -1;
	}
	else if ( pid != 0 )
	{
		sid = getsid(0);
		printf("SessionID:getsid=%d\r\n", sid);
		exit(EXIT_SUCCESS);
	}
	
	// Make new session by calling 'setsid()'
	/* This makes these happen :
	 * 	1. the process calling setsid() becomes the leader and the only member of a new session.
	 * 	2. the process doesn't have control tty.
	 * 	3. the process creates a new process group having ifself the leader and the only member in it.
	 * 	4. the process's pid becomes the process group's id.
	 */
	
	sid = setsid();	// new session id is returned.
	if ( sid == -1 )
	{
		perror("setsid");
		return -1;
	}
	else
	{
		pid_t sid2 = getsid(0);
		printf("SessionID:getsid=%d(%d)\r\n", sid, sid2);		
	}
	
	return ret;
}

int		
CProcTest::Demon()
{
	pid_t 	pid;
	int		i;
	
	// Fork to create a new process to be a daemon process.
	pid = fork();
	if ( pid == -1 )
	{
		return -1;
	}
	else if ( pid != 0 )
	{
		// Call exit not to make daemon process the leader of any process group.
		exit(EXIT_SUCCESS);
	}
	
	// Call setsid() to make this process the leader of a new process group and of a new session.
	if ( setsid() == -1 )
	{
		return -1;
	}
	
#ifdef AUTO_POST_PROCESSING	
	if ( deamon(0 /*nochdir*/, 0 /*noclose*/) == -1 )
		return -1;
#else	
	// Change the current directory root to avoid situation making unable to unmount file systems associated.
	if ( chdir("/") == -1 )
	{
		return -1;
	}
	
	// Close all file descriptors that might be opened.
	for ( i = 0; i < 1024/*NR_OPEN*/; i++ )
		close(i);
		
	// Redirect all standard i/o file descriptors to /dev/null
	FdRedirecting();
//	open( "/dev/null", O_RDWR );
//	dup (0);
//	dup (0);
	
	/*
	 * Do whatever you want as a DEAMON
	 * 
	 * Let's test laster with some threads and signals....
	 * 
	 */	 
	 
#endif

	return 0;
}

int		
CProcTest::Nice()
{
	int ret = 0;
	
	// Get the current NICE
	errno 	= 0;
	ret 	= nice( 0 );

	printf( "Nice:nice=%d\r\n", ret );
	
	// Try to update NICE below 0.
	errno = 0;			// Clear errno first.	
	int incNice = -10; 	// NICE can be between -29~10. 
						// But this process must have 'CAP_SYS_NICE' function to go below 0 for NICE.
	ret	= nice( incNice );
	if ( errno )
	{
		perror("nice");
	}
	else
		printf( "Nice:nice=%d(updated)\r\n", ret );
	
	return ret;
}

int
CProcTest::Priority()
{
	int ret = 0;
	
	// Get the priority of the current process
	int which 	= PRIO_PROCESS;	// or PRIO_PGRP, PRIO_USER
	int who		= 0; 			// 0 means id of the current process, of the process group, or of user.
	
	errno = 0;
	ret = getpriority( which, who );
	if ( ret == -1 )
		perror("getpriority");
	else
		printf("Priority: nice(current)=%d\r\n", ret );
	
	// Try to update the priority
	which 	= PRIO_PGRP;
	who		= 0;
	
	int priority = 10;
	
	errno = 0;
	ret = setpriority( which, who, priority );
	if ( ret == -1 )
		perror("setpriority");
	else
	{
		errno = 0;
		ret = getpriority( which, who );
		if ( ret == -1 ) 
			perror("setpriority");
		else
			printf("Priority: nice(updated)=%d\r\n", ret );	
	}
	
	return ret;
}

int		
CProcTest::CpuAffinity()
{
	int ret = 0;
		
	cpu_set_t 	set;
	pid_t		pid = 0; // current process	

	// Check the current status.
	CPU_ZERO( &set );
	ret = sched_getaffinity( pid, sizeof(cpu_set_t), &set );
	if ( ret == -1 )
		perror("sched_getaffinity");
	
	int i;	
	int cpuCheck = 8; // CPU_SETSIZE
	for ( i = 0; i < cpuCheck; i++ )
	{
		int cpu;
		
		cpu = CPU_ISSET ( i, &set );
		printf("cpu=%i, is %s\n", i, cpu ? "set" : "unset" );
	}	
	printf("\n");
	
	// Try to modify
	CPU_ZERO( &set );
	CPU_SET( 0, &set );
	CPU_CLR( 1, &set );
	CPU_CLR( 2, &set );
	CPU_CLR( 3, &set );

	for ( i = 0; i < cpuCheck; i++ )
	{
		int cpu;
		
		cpu = CPU_ISSET ( i, &set );
		printf("cpu=%i, is %s\n", i, cpu ? "set" : "unset" );
	}
		
	return ret;
}

int		
CProcTest::Scheduling()
{
	int ret = 0;
	
	pid_t 	pid = 0; // current process
	int 	policy;
	
	// Check the current scheduling policy
	policy = sched_getscheduler( pid );
	
	std::string policyName = "Policy: ";
	
	switch ( policy )
	{
		case SCHED_OTHER:	policyName += "SCHED_OTHER( default policy for non real-time processes in Linux)"; 	break;
		case SCHED_RR:		policyName += "SCHED_RR"; 		break;
		case SCHED_FIFO:	policyName += "SCHED_FIFO"; 	break;
		case SCHED_BATCH:	policyName += "SCHED_BATCH";	break;
		case -1:			policyName += "-1"; break;
		default: break;
	}
	
	printf("%s\n", policyName.c_str() );
	
	// Try to update scheduling policy to a different one.
	// It doesn't seem to work because of....limitations in users...?
	struct sched_param sp = { .sched_priority = 1 };
	
	if ( sched_setscheduler( pid, SCHED_RR, &sp ) == -1 )
	{
		perror("sched_setscheduler");
		return -1;
	}
	
	policyName = "Policy(Updated): ";
	
	switch ( policy )
	{
		case SCHED_OTHER:	policyName += "SCHED_OTHER( default policy for non real-time processes in Linux)"; 	break;
		case SCHED_RR:		policyName += "SCHED_RR"; 		break;
		case SCHED_FIFO:	policyName += "SCHED_FIFO"; 	break;
		case SCHED_BATCH:	policyName += "SCHED_BATCH";	break;
		case -1:			policyName += "-1"; break;
		default: break;
	}
	
	printf("%s\n", policyName.c_str() );
	
	return ret;
}

int 
CProcTest::RetrieveResourceLimit()
{
	int ret = 0;
	
	struct rlimit limit;
	
	enum
	{
		eNumResource = 16
	};
	
	int typesResource[ eNumResource ] =
	{
		RLIMIT_AS,			// The maximum number of bytes of the address space for the processes
		RLIMIT_CORE,		// The size of the maximum core file
		RLIMIT_CPU,			// The maximum CPU time that this process can consume (second unit)
		RLIMIT_DATA,		// The maximum size of data segment and heap all together for the processes
		RLIMIT_FSIZE,		// The maximum size of a file that this process can produce
		RLIMIT_LOCKS,		// The maximum number of file locks that processes can use
		RLIMIT_MEMLOCK,		// The maximum number of bytes that can be locked
		RLIMIT_MSGQUEUE,	// The maximum number of bytes that the user can allocate for POSIX message queues.
		RLIMIT_NICE,		// The maximum value for nice to be lowered
		RLIMIT_NOFILE,		// One number bigger than the maximum number of files that processes can open
		RLIMIT_NPROC,		// The maximum number of processes that can be executed at a certain time
		RLIMIT_RSS,			// The maximum nmber of pages on which processes reside
		RLIMIT_RTTIME,		// The CPU time that FIFO processes can consume without blocking system calls
		RLIMIT_RTPRIO,		// The maximum value for priority level that processes without 'CAP_SYS_NICE' can request
		RLIMIT_SIGPENDING,	// The maximum number of signals that can be inserted to queues.
		RLIMIT_STACK		// The maximum size of process stack in byte units
	};
	
	const char* namesResource[eNumResource] =
	{
		"RLIMIT_AS",
		"RLIMIT_CORE",
		"RLIMIT_CPU",
		"RLIMIT_DATA",
		"RLIMIT_FSIZE",
		"RLIMIT_LOCKS",
		"RLIMIT_MEMLOCK",
		"RLIMIT_MSGQUEUE",
		"RLIMIT_NICE",
		"RLIMIT_NOFILE",
		"RLIMIT_NPROC",
		"RLIMIT_RSS",
		"RLIMIT_RTTIME",
		"RLIMIT_RTPRIO",
		"RLIMIT_SIGPENDING",
		"RLIMIT_STACK"
	};
	
	
	printf("Retrieve resource limit information for the current process:\n");
	printf("rlim_cur:soft limit, rlim_max:hard limit, (-): infinite\n");
		
	int i;
	for ( i = 0; i < (int)eNumResource; i++ )
	{
		ret = getrlimit( typesResource[i], &limit );
		
		if ( ret == -1 )
		{
			perror("getrlimit");
			break;
		}
		
		printf("%18s	rlim_cur=%10ld, rlim_max=%10ld\n", namesResource[i], limit.rlim_cur, limit.rlim_max );
	}
	
	return ret;
}

int 
CProcTest::ChangeResourceLimit()
{
	int ret = 0;
	
	struct rlimit rlim;
	
	RetrieveResourceLimit();
	
	rlim.rlim_cur 	= 32*1024*1024;
	rlim.rlim_max	= RLIM_INFINITY;

	printf("ChangeResourceLimit: RLIMIT_CORE.rlim_cur=%ld\n",  rlim.rlim_cur);
		
	ret = setrlimit( RLIMIT_CORE, &rlim );
	if ( ret == -1 )
		perror("setrlimit");
	
	RetrieveResourceLimit();
	
	return ret;
}

int	
CProcTest::FdRedirecting()
{
	const char*	path = "/dev/null";	// default
//	const char* path = "/home/pi/git_repository/Sungsu_Lab/practice_linux/bin/testRedirecting.txt";
	
	int	fd0, fd1, fd2;

	// I don't exactly understand how these works.
	fd0	= open( path, O_RDWR );
	fd1	= dup (0);
	fd2	= dup (0);
	
	// At this time, fd0, fd1, and fd2 should be 0, 1, and 2 respectably.
	// If not, this is dealt with an error according to Advanced Programming in Unix.... p.428.
		
	printf("test..fdStdIn=%d\r\n", 	fd0);
	printf("test..fdStdOut=%d\r\n", fd1);
	printf("test..fdStdErr=%d\r\n",	fd2);	
	
	return 0;
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
