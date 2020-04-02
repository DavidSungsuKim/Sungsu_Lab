
#ifndef __TIMETEST_H__
#define __TIMETEST_H__
#endif

#include <time.h>
#include <signal.h>

class CTimeTest
{
public:
	CTimeTest();
	~CTimeTest();
	
	void 	SysConf				();
	void	ClockResolution		();
	
	void	GetCurrentTime		();
	void	GetProcessTime		();
	
	void	ConversionTime		();
	void	CalculateTimeBasic	();	
	
	void	Sleep				();
	void	AdvancedSleep		();
	
	void	TimerWithAlarm		();
	void	IntervalTimer		();
	
	void	AdvancedTimerSignal	();
	void	AdvancedTimerThread	();
	
	static void AlarmHandler			(int signo);
	static void IntervalAlarmHandler	(int signo);
	
	// For AdvancedTimerSignal()
	timer_t	m_timerID;
	static void AdvancedTimerHandlerSignal	(int signo);
	
	// For AdvancedTimerThread()
	timer_t	m_timerIDThread;
	int		m_testVal;
	
	static void AdvancedTimerHandlerThread	(union sigval arg);
};
