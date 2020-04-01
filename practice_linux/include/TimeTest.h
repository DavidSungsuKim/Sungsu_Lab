
#ifndef __TIMETEST_H__
#define __TIMETEST_H__
#endif

class CTimeTest
{
public:
	CTimeTest();
	~CTimeTest();
	
	void 	SysConf			();
	void	ClockResolution	();
	
	void	GetCurrentTime	();
	void	GetProcessTime	();
	
	void	ConversionTime	();
	void	CalculateTimeBasic	();	
};
