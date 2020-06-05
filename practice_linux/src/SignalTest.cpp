
#include "Common.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "SignalTest.h"

extern const char* const sys_siglist[];

pthread_mutex_t	CSignalTest::m_mutex1;

/* Signal Reference
	 1) SIGHUP       2) SIGINT       3) SIGQUIT      4) SIGILL
	 5) SIGTRAP      6) SIGABRT      7) SIGBUS       8) SIGFPE
	 9) SIGKILL     10) SIGUSR1     11) SIGSEGV     12) SIGUSR2
	13) SIGPIPE     14) SIGALRM     15) SIGTERM     16) SIGSTKFLT
	17) SIGCHLD     18) SIGCONT     19) SIGSTOP     20) SIGTSTP
	21) SIGTTIN     22) SIGTTOU     23) SIGURG      24) SIGXCPU
	25) SIGXFSZ     26) SIGVTALRM   27) SIGPROF     28) SIGWINCH
	29) SIGIO       30) SIGPWR      31) SIGSYS      34) SIGRTMIN
	35) SIGRTMIN+1  36) SIGRTMIN+2  37) SIGRTMIN+3  38) SIGRTMIN+4
	39) SIGRTMIN+5  40) SIGRTMIN+6  41) SIGRTMIN+7  42) SIGRTMIN+8
	43) SIGRTMIN+9  44) SIGRTMIN+10 45) SIGRTMIN+11 46) SIGRTMIN+12
	47) SIGRTMIN+13 48) SIGRTMIN+14 49) SIGRTMIN+15 50) SIGRTMAX-14
	51) SIGRTMAX-13 52) SIGRTMAX-12 53) SIGRTMAX-11 54) SIGRTMAX-10
	55) SIGRTMAX-9  56) SIGRTMAX-8  57) SIGRTMAX-7  58) SIGRTMAX-6
	59) SIGRTMAX-5  60) SIGRTMAX-4  61) SIGRTMAX-3  62) SIGRTMAX-2
	63) SIGRTMAX-1  64) SIGRTMAX
	
	Signal Name	Signal Number	Description 
	SIGHUP	1	Hang up detected on controlling terminal or death of controlling process
	SIGINT	2	Issued if the user sends an interrupt signal (Ctrl + C)
	SIGQUIT	3	Issued if the user sends a quit signal (Ctrl + D)
	SIGFPE	8	Issued if an illegal mathematical operation is attempted
	SIGKILL	9	If a process gets this signal it must quit immediately and will not perform any clean-up operations
	SIGALRM	14	Alarm clock signal (used for timers)
	SIGTERM	15	Software termination signal (sent by kill by default)
*/

CSignalTest::CSignalTest()
{
	
}

CSignalTest::~CSignalTest()
{
	
}

void 
CSignalTest::SignalFundamental()
{
	sighandler_t sigRet;
	
	sigRet = signal( SIGINT, SignalHandler1 );
	if ( sigRet )
	{
	//	perror("signal");
		fprintf( stderr, "Cannot handle SIGINT\n");
		exit( EXIT_FAILURE );
	}
	
	for(;;)
		pause();	// Make the process sleep and be ready to wake up on signals
		
	exit( EXIT_SUCCESS );
}
	
void 
CSignalTest::SignalFundamental2()
{
	sighandler_t sigRet;
	
	sigRet = signal( SIGINT, SignalHandler1 );
	if ( sigRet )
	{
		fprintf( stderr, "Cannot handle SIGINT\n");
		exit( EXIT_FAILURE );
	}
	
	sleep(3);
	
	// To make a self signal
	raise(2 /*SIGINT*/);
		
	exit( EXIT_SUCCESS );
}

void	
CSignalTest::SignalFundamental3()
{	
	sigset_t	sigSet;
	sigset_t	sigSetOld;
	
	// Here sigSet is used to block signals from handling.
	// For example, if SIGINT is added, the program won't respond to user's Ctrl+C key.
	// These blocked signals can be retrieved by sigpending() below.
	sigemptyset( &sigSet );
//	sigfillset( &sigSet );

	printf("SignalFundamental3: signal %d(%s) is blocked.\n", SIGINT, sys_siglist[SIGINT] );
	sigaddset( &sigSet, SIGINT );	// This signal will be blocked.
	
	sigprocmask( SIG_BLOCK, &sigSet, &sigSetOld );	// SIG_SETMASK, SIG_BLOCK, SIG_UNBLOCK
	
	sighandler_t sigRet;
	sigRet = signal( SIGINT, SignalHandler1 );
	if ( sigRet )
	{
		fprintf( stderr, "Cannot handle SIGINT\n");
		exit( EXIT_FAILURE );
	}

	printf("SignalFundamental3: Wait for signal %d...(Ctrl+C)\n", SIGINT );
		
	for(;;)
	{	
		sigset_t	sigPending;
		int 		ret;

		sigemptyset( &sigPending );
		ret = sigpending( &sigPending );
		if ( ret == 0 )
		{
			int i;
			for ( i = 1; i <= 64; i++ )
			{
				if ( sigismember( &sigPending, i ) )
				{								
					printf("SignalFundamental3: signal %d(%s) is unblocked.\n", i, sys_siglist[i] );
					// Unblock the signal
					sigemptyset( &sigSet );
					sigaddset( &sigSet, i );	
					sigprocmask( SIG_UNBLOCK, &sigSet, &sigSetOld );
					
					break;
				}
			}
		}

		sleep(1);
	}
		
	printf("SignalFundamental3: end...");	
	exit( EXIT_SUCCESS );	
}

void 
CSignalTest::SignalHandler1(int signo)
{
	printf("SignalHandler1, sig=%d(%s)\n", signo, sys_siglist[signo] );
	exit( EXIT_SUCCESS );
}

void
CSignalTest::SignalDeadLock()
{
	m_mutex1 	= PTHREAD_MUTEX_INITIALIZER;	// No recursive by default

	sighandler_t sigRet;
	sigRet = signal( SIGINT, SignalHandlerDeadLock );
	if ( sigRet )
	{
		fprintf( stderr, "Cannot handle SIGINT\n");
		exit( EXIT_FAILURE );
	}
	
	pthread_mutex_lock( &m_mutex1 );
	
	printf("SignalDeadLock: mutex lock...\n");
	
	for(;;)
		pause();
		
	pthread_mutex_unlock( &m_mutex1 );
	
	exit( EXIT_SUCCESS );
}

void 
CSignalTest::SignalHandlerDeadLock(int signo)
{
/* 	By the time this signal handler is called, 
	the mutex has already been taken in SignalDeadLock(), 
	which makes deadlock unless the mutex is set recursive.
	*/
	 
	pthread_mutex_lock( &m_mutex1 );
	
	printf("SignalHandlerDeadLock, sig=%d(%s)\n", signo, sys_siglist[signo] );
	
	pthread_mutex_unlock( &m_mutex1 );
}

void	
CSignalTest::SignalAdvanced()
{
/* struct sigaction
 * {
 * 	void (*sa_handler)(int)'
 * 	void (*sa_sigaction)(int, siginfo_t*, void*);
 * 	sigset_t sa_mask;			// Signals to be blocked
 * 	int sa_flags;
 * 	void (*sa_restorer)(void);	// Not POSIX standard
 */	
	
	struct sigaction act, actOld;

	act.sa_handler 		= SignalHandler1;
	act.sa_sigaction	= SignalHandlerAdvanced;
	act.sa_flags		= 0;
	act.sa_flags		|= SA_SIGINFO;	// To choose SignalHandlerAdvanced instead of SignalHandler1
	
	int ret;
	ret = sigaction( SIGINT, &act, &actOld );
	if ( ret != 0 )
	{
		fprintf( stderr, "Cannot handle SIGINT\n");
		exit( EXIT_FAILURE );
	}
	
	for(;;)
		pause();
		
	exit( EXIT_SUCCESS );	
}

void 
CSignalTest::SignalHandlerAdvanced(int signo, siginfo_t* pSi, void* pContext )
{
	printf("SignalHandlerAdvanced, sig=%d(%s)\n", signo, sys_siglist[signo] );
	
/*	struct ucontext_t
	{
		uc_stack.ss_sp
		uc_stack.ss_size
		uc_stack.ss_flags
	}*/

	ucontext_t*	pUcontext = (ucontext_t*)pContext;
	    
	printf("	uc_stack.ss_sp    = 0x%ld\n", 	(long)pUcontext->uc_stack.ss_sp );
	printf("	uc_stack.ss_size  = 0x%ld\n", 	(long)pUcontext->uc_stack.ss_size );
	printf("	uc_stack.ss_flags = 0x%d\n", 	pUcontext->uc_stack.ss_flags );
	printf("\n");
	printf("	si_signo = %d (%s)\n", pSi->si_signo, sys_siglist[signo] );
	printf("	si_errno = %d\n", pSi->si_errno );
	printf("	si_code  = %d", pSi->si_code );
	
	switch( pSi->si_code )
	{
	case SI_ASYNCIO: 	printf(" (SI_ASYNCIO)\n");	break;
	case SI_KERNEL: 	printf(" (SI_KERNEL)\n");	break;
	case SI_MESGQ: 		printf(" (SI_MESGQ)\n");	break;
	case SI_QUEUE: 		printf(" (SI_QUEUE)\n");	break;
	case SI_TIMER: 		printf(" (SI_TIMER)\n");	break;
	case SI_TKILL: 		printf(" (SI_TKILL)\n");	break;
	case SI_SIGIO: 		printf(" (SI_SIGIO)\n");	break;
	case SI_USER: 		printf(" (SI_USER)\n");		break;
	// effective only if signal is 'SIGBUS'.
	case BUS_ADRALN: 	printf(" (BUS_ADRALN)\n");	break;
	case BUS_ADRERR: 	printf(" (BUS_ADRERR)\n");	break;
	case BUS_OBJERR: 	printf(" (BUS_OBJERR)\n");	break;
	default:
		printf(" (unknown)\n");	
		break;
	}	
	
/*	printf("	si_pid=%d\n", pSi->si_pid );
	printf("	si_uid=%d\n", pSi->si_uid );
	printf("	si_status=%d\n", pSi->si_status );
	printf("	si_utime=%d\n", pSi->si_utime );
	printf("	si_stime=%d\n", pSi->si_stime );
	printf("	si_value=%d\n", pSi->si_value );	// signal payload value
	printf("	si_int=%d\n", pSi->si_int );
	printf("	si_ptr=%d\n", pSi->si_ptr );
	printf("	*si_addr=%d\n", pSi->si_addr );
	printf("	*si_band=%d\n", pSi->si_band );
	printf("	si_fd=%d\n", pSi->si_fd );*/
	printf("SignalHandlerAdvanced, end...\n");
	
	exit( EXIT_SUCCESS );	
}
