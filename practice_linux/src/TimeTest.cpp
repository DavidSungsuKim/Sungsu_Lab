
#include "Common.h"
	
#include <unistd.h>
#include <sys/time.h>
#include <sys/times.h>

#include <stdio.h>
#include <stdlib.h>
#include "TimeTest.h"

extern const char* const sys_siglist[];

CTimeTest::CTimeTest()
{
	m_timerID 		= 0;
	
	m_timerIDThread	= 0;
	m_testVal		= 0;
}

CTimeTest::~CTimeTest()
{
	if ( m_timerID )
		timer_delete( m_timerID );

	if ( m_timerIDThread )
		timer_delete( m_timerIDThread );	
}

void 
CTimeTest::SysConf()
{
	long hz;
	
/*	Aside from _SC_CLK_TCK, many POSIX variables can be retrieved by the function.
  * http://man7.org/linux/man-pages/man3/sysconf.3.html
	*/ 
	
	hz = sysconf(_SC_CLK_TCK);
	if ( hz == -1 )
		perror("sysconf");
		
	printf("SysConf: hz=%ld\n", hz );
}

void	
CTimeTest::ClockResolution()
{
	clockid_t	clocks[] =
	{
		CLOCK_REALTIME,
		CLOCK_MONOTONIC,
		CLOCK_PROCESS_CPUTIME_ID,
		CLOCK_THREAD_CPUTIME_ID,
		CLOCK_MONOTONIC_RAW,
		(clockid_t)-1
	};
	
	int i;
	
	for( i = 0; clocks[i] != (clockid_t)-1; i++ )
	{
		struct timespec res;
		int ret;
		
		ret = clock_getres( clocks[i], &res );
		if ( ret )
			perror("clock_getres");
		else
			printf("clock=%10d sec=%10ld nsec=%10ld\n", clocks[i], res.tv_sec, res.tv_nsec );
	}
}

void	
CTimeTest::GetCurrentTime()
{
	int	ret;
	time_t tErr;
	
	// 1. Using time_t (sec)
	time_t t;
/*	typedef long time_t	// it represents time in seconds from the origin, so it is usually a huge number.
 */ 	
	tErr = time( &t );
	if ( tErr == -1 )
	{
		perror("time");
		return;
	}
	
	printf("GetCurrentTime: time, t=%ld\n", t);
	
	// 2. Using timeval (usec)
	struct timeval tVal;
/*	{
 *		time_t		tv_sec 
 * 		suseconds_t	tv_usec
 * 	}
 **/ 
	
	ret = gettimeofday( &tVal, NULL );
	if ( ret == -1 )
	{
		perror("gettimeofday");
		return;
	}
	
	printf("GetCurrentTime: gettimeofday, tv_sec=%ld tv_usec=%ld\n", tVal.tv_sec, tVal.tv_usec );
	
	// 3. Using timespec (nsec)
	struct timespec tSpec;
/*	{
 * 		time_t	tv_sec
 * 		long	tv_nsec
 * 	}
 */ 
	
	clockid_t clockid = CLOCK_REALTIME;
/*	clockid can be :
	CLOCK_REALTIME,
	CLOCK_MONOTONIC,
	CLOCK_PROCESS_CPUTIME_ID,
	CLOCK_THREAD_CPUTIME_ID,
	CLOCK_MONOTONIC_RAW,
	 */
	
	ret = clock_gettime( clockid, &tSpec );
	if ( ret == -1 )
	{
		perror("clock_gettime");
		return;
	}
	
	printf("GetCurrentTime: clock_gettime, clockid=%d tSpec.tv_sec=%ld tSpec.tv_nsec=%ld\n", 
			clockid, tSpec.tv_sec, tSpec.tv_nsec );	
}

void	
CTimeTest::GetProcessTime()
{
	clock_t	ret;
	
	struct tms tProc;
/*	{
 * 		clock_t	tms_utime	// time consumed in user space
 * 		clock_t	tms_stime	// time consumed in kernel space
 * 		clock_t	tms_cutime	// time consumed in user space by child process
 * 		clock_t	tms_cstime	// time consumed in kernel space child process
 * 	}
 **/	
	
	ret = times( &tProc );
	if ( ret == -1 )
	{
		perror("times");
		return;
	}
	
	printf("GetProcessTime: tms_utime=%ld tms_stime=%ld tms_cutime=%ld tms_cstime=%ld\n",
			tProc.tms_utime, tProc.tms_stime, tProc.tms_cutime, tProc.tms_cstime );
}

void	
CTimeTest::ConversionTime()
{
	time_t 	t;
	time_t	tErr;
	struct tm tmTest;
	
	tErr = time( &t );
	if ( tErr == -1 )
	{
		perror("time");
		return;
	}
	
	gmtime_r( &t, &tmTest );	// This function represent the time in UTC time.
	printf("ConversionTime: tm(ASCII)= %s\n", asctime( &tmTest ));	
	
	time_t tFromTm;
	tFromTm = mktime( &tmTest );
	
	char buf[256] ={0,};
	ctime_r( &tFromTm, buf ); 	// ctime and ctime_r add '\n' at the end of the string
	printf("ConversionTime: time_t(ASCII)= %s", buf );		
}

void	
CTimeTest::CalculateTimeBasic()
{
	time_t	t1, t2;
	double	tDiff;
	
	int second = 3;
	printf("CalculateTimeBasic: sleep(%d)...\n", second );
		
	time( &t1 );
	
	sleep(second);

	time( &t2 );
	
	tDiff = difftime( t2, t1 );
	
	printf("CalculateTimeBasic: t1=%ld, t2=%ld, t2-t1=%.1f[sec]\n", t1, t2, tDiff );
}

void	
CTimeTest::Sleep()
{
	int sec = 5;
	int remained;
	
	printf("Sleep: sleep(%d)\n", sec );
	remained = sleep( sec );
	if ( remained )
	{
		printf("Sleep: remained=%d\n", remained);
	}
	
	unsigned int usec = 100;
	
	printf("Sleep: usleep(%d)\n", usec );
	usleep( usec );
	
	struct timespec tSpec = { .tv_sec=1, .tv_nsec=1000 };
	struct timespec tRem;
		
	int ret;
	
	printf("Sleep: nanosleep(%ld.xxx)\n", tSpec.tv_sec );
	
	ret = nanosleep( &tSpec, &tRem );
	if ( ret )
		perror("nanosleep");	
}

void
CTimeTest::AdvancedSleep()
{
	int ret;
	clockid_t	clockId = CLOCK_MONOTONIC;
/*	CLOCK_MONOTONIC	: suitable for relative sleep
 * 	CLOCK_REALTIME	: suitable for absolute sleep
 * 	...
 */
	
	// 1. Relative Sleep
	int				flags;
	struct timespec tSpec;
	struct timespec tRem;
	
	flags	= 0;
	tSpec 	= { .tv_sec=1, .tv_nsec=100 };
	printf("AdvancedSleep: clock_nanosleep(%ld.xxx) (relative)\n", tSpec.tv_sec );
	
	ret = clock_nanosleep( clockId, flags, &tSpec, &tRem /*or NULL */ );
	if ( ret )
		perror("clock_nanosleep");
	
	// 2. Absolute Sleep
	flags 	= TIMER_ABSTIME;
	tSpec 	= { .tv_sec=1, .tv_nsec=100 };
		
	struct timespec tNow;
	ret = clock_gettime( clockId, &tNow );
	if ( ret )
		perror("clock_gettime");
		
	tNow.tv_sec 	+= tSpec.tv_sec;
	tNow.tv_nsec 	+= tSpec.tv_nsec;
	
	printf("AdvancedSleep: clock_nanosleep(%ld.xxx) (absolute)\n", tSpec.tv_sec );	
	
	ret = clock_nanosleep( clockId, flags, &tSpec, &tRem /*or NULL */ );
	if ( ret )
		perror("clock_nanosleep");	
	
	// 3. Using 'select()'
	
/*	int select( int n,
 * 		fd_set *freadfds,
 * 		fd_set *writefds,
 * 		fd_set *exceptfds,
 * 		struct tiimeval *timeout );
 */
	struct timeval tVal;
	tVal.tv_sec		= 0;
	tVal.tv_usec	= 1000000;

	printf("AdvancedSleep: select\n" );		
	select( 0, NULL, NULL, NULL, &tVal );
	
}

void	
CTimeTest::TimerWithAlarm()
{
	int sec = 3;
	int prev;
	
	signal( SIGALRM, AlarmHandler );
	
	printf("TimerWithAlarm: alarm(%d)\n", sec );
	
	prev = alarm( sec );
	if ( prev )
		printf("TimerWithAlarm: Previous alarm is cancelled. Remaining time was %d[sec]\n", prev );		
	
	for(;;)
		pause();
}

void
CTimeTest::AlarmHandler(int signo)
{
	printf("AlarmHandler, signo=%d(%s)\n", signo, sys_siglist[ signo ]);
	
//	alarm( 1 );	// can be called inside
	exit( EXIT_SUCCESS );
}

void	
CTimeTest::IntervalTimer()
{
	int ret;
	struct itimerval delay;
	
	signal( SIGALRM, IntervalAlarmHandler );
	
	delay.it_value.tv_sec		= 3;
	delay.it_value.tv_usec		= 0;
	delay.it_interval.tv_sec	= 0;
	delay.it_interval.tv_usec	= 500000;	// 500ms
	
	printf("IntervalTimer: setitimer()\n");
	
	ret = setitimer( ITIMER_REAL, &delay, NULL );
	if ( ret )
	{
		perror("settimer");	
		return;
	}
	
	for(;;)
		pause();	
}

void
CTimeTest::IntervalAlarmHandler(int signo)
{
	printf("IntervalAlarmHandler, signo=%d(%s)\n", signo, sys_siglist[ signo ]);
}

void	
CTimeTest::AdvancedTimerSignal()
{
	int ret;
	
	if ( m_timerID )
		return;
	
	struct sigevent evp;
	evp.sigev_value.sival_ptr	= &m_timerID;
	evp.sigev_notify 			= SIGEV_SIGNAL;
	evp.sigev_signo				= SIGUSR1;	

/*	Note that I MUST add '-lrt' option to compile option 
 * 	to be able to use functions like timer_create().
 */
	ret = timer_create( CLOCK_REALTIME, &evp, &m_timerID );
	if ( ret )
		perror("timer_create");
		
	int flags = 0;
	struct itimerspec	tSpec;
	struct itimerspec	tOld;
	
	tSpec.it_value.tv_sec		= 1;
	tSpec.it_value.tv_nsec		= 0;	
	tSpec.it_interval.tv_sec	= 0;
	tSpec.it_interval.tv_nsec	= 500000000;	// 500ms
		
	ret = timer_settime( m_timerID, flags, &tSpec, &tOld );
	if ( ret )
		perror("timer_settime");
	
	signal( SIGUSR1, AdvancedTimerHandlerSignal );	
	
	for(;;)
		pause();
}

void 
CTimeTest::AdvancedTimerHandlerSignal(int signo)
{
	printf("AdvancedTimerHandlerSignal, signo=%d(%s)\n", signo, sys_siglist[ signo ]);	
}

void	
CTimeTest::AdvancedTimerThread()
{
	int ret;
	
	if ( m_timerIDThread )
		return;
	
	struct sigevent evp;
	evp.sigev_value.sival_ptr	= this;
	evp.sigev_notify 			= SIGEV_THREAD;
	evp.sigev_notify_function	= AdvancedTimerHandlerThread;

/*	Note that I MUST add '-lrt' option to compile option 
 * 	to be able to use functions like timer_create().
 */
 
	m_testVal = 7;
	
	ret = timer_create( CLOCK_REALTIME, &evp, &m_timerIDThread );
	if ( ret )
		perror("timer_create");
		
	int flags = 0;
	struct itimerspec	tSpec;
	struct itimerspec	tOld;
	
	tSpec.it_value.tv_sec		= 1;
	tSpec.it_value.tv_nsec		= 0;	
	tSpec.it_interval.tv_sec	= 0;
	tSpec.it_interval.tv_nsec	= 500000000;	// 500ms
		
	ret = timer_settime( m_timerIDThread, flags, &tSpec, &tOld );
	if ( ret )
		perror("timer_settime");
		
	for(;;)
		pause();
}

void 
CTimeTest::AdvancedTimerHandlerThread(union sigval arg)
{ 
/* union sigval
 * {
 * 	int sival_int;
 * 	void *sival_ptr;
}*/
	CTimeTest* pThis = (CTimeTest*)arg.sival_ptr;
	
	printf("AdvancedTimerHandlerThread, testVal=%d\n", pThis->m_testVal );	
}