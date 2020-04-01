
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>

#include <stdio.h>

#include "TimeTest.h"

CTimeTest::CTimeTest()
{
	
}

CTimeTest::~CTimeTest()
{
	
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
