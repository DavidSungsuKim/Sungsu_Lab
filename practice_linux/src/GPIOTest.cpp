
#include "Common.h"
#include "GPIOTest.h"

#include <unistd.h>
#include <sys/time.h>
#include <sys/times.h>
#include <pigpio.h>

CGpioPGpio::CGpioPGpio()
{
	m_bInit = true;

	if( gpioInitialise() < 0 )
		m_bInit = false;
}

CGpioPGpio::~CGpioPGpio()
{
	if ( m_bInit )
		gpioTerminate();
}

void
CGpioPGpio::TestGpio1()
{
	TestPGpio( 2, eMODE_OUTPUT, 1 );
//	TestPWM(2, 10000, 0);
}

void
CGpioPGpio::TestGpio2()
{
	TestPGpio( 2, eMODE_OUTPUT, 0 );

/*	int duty = 100;
	while(duty)
	{
		TestPWM(2, 10000, duty);
		duty -= 20;
		sleep(1);
	}
	*/
}

void
CGpioPGpio::TestPGpio(int aLine, int aMode, int aValue)
{
	if ( !m_bInit )
	{
		_printf_error("not init");
		return;
	}

	int line = aLine;
	int mode = aMode;
	int value = aValue;
	
	if ( mode == eMODE_OUTPUT )
	{
		gpioSetMode(line, PI_OUTPUT);
		gpioWrite(line, value);		
	}
	else if ( mode == eMODE_INPUT )
	{
		
	}
	else
	{
		_printf_error("undefined mode");
	}
}

void
CGpioPGpio::TestPWM(int aLine, int aFreq, int aDuty)
{
	int ret = 0;

	printf("CGpioPGpio::TestPWM, line=%d, freq=%d, duty=%d\n", aLine, aFreq, aDuty);
	if ( !m_bInit )
	{
		_printf_error("not init");
		return;
	}

	gpioSetMode(aLine, PI_OUTPUT);
	
/*	ret = gpioServo(aLine, 1000);
	if ( ret )
		_printf_ret_error(ret, "gpioServo");
*/
/*	ret = gpioSetPWMfrequency(aLine, aFreq);
	if ( ret )
		_printf_ret_error(ret,"gpioSetPWMfrequency");
*/
	int dutyVal = (aDuty * 255) / 100;
	if ( dutyVal < 0 )
		dutyVal = 0;

	printf("dutyVal=%d\n", dutyVal);

	ret = gpioPWM(aLine, dutyVal);
	if ( ret )
		_printf_error("PWM");
}

///////////////////////////////////

CGpioLGpiod::CGpioLGpiod()
{
	m_pChip = NULL;
	m_pChip = gpiod_chip_open("/dev/gpiochip0");
}

CGpioLGpiod::~CGpioLGpiod()
{
	if ( m_pChip )
		gpiod_chip_close( m_pChip );
}

void
CGpioLGpiod::TestGpio1()
{
	TestLGpio( 2, eMODE_OUTPUT, 1 );
}

void
CGpioLGpiod::TestGpio2()
{
	TestLGpio( 2, eMODE_OUTPUT, 0 );
}

void
CGpioLGpiod::TestLGpio(int aLine, int aMode, int aValue)
{
	if ( !m_pChip )
	{
		_printf_error("no chip");
		return;
	}
	
	int ret = 0;
	
	int line = aLine;
	int mode = aMode;
	int value = aValue;
	
	struct gpiod_line *pLine = gpiod_chip_get_line( m_pChip, line );
	
	if ( mode == eMODE_OUTPUT )
	{
		ret = gpiod_line_request_output( pLine, "null", GPIOD_LINE_ACTIVE_STATE_HIGH );
		if ( ret )
		{
			_printf_error("request output failed");
			goto _EXIT;
		}
	
		ret = gpiod_line_set_value( pLine, value );		
		if ( ret )
		{
			_printf_error("set value failed");
			goto _EXIT;
		}		

	/* Comment
	 * I don't know why, but there seems to be current leakage even when I don't turn it on in libgpiod mode.
	 * It's different from when I used pigpio.
	 * I can test with these code.
		printf("wait..\n");
		sleep(1);
		*/
	}
	else if ( mode == eMODE_INPUT )
	{
		; // TBD
	}
	else
	{
		_printf_error("unknown mode");
		goto _EXIT;
	}

_EXIT:
	gpiod_line_release(pLine);
	return;
}
