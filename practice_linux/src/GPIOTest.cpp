
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
		
	m_bInit = false;
}

void
CGpioPGpio::TestGpio1()
{
	TestPGpio( 2, eMODE_OUTPUT, 1 );
}

void
CGpioPGpio::TestGpio2()
{
	TestPGpio( 2, eMODE_OUTPUT, 0 );
}

void
CGpioPGpio::TestPGpio(int aLine, int aMode, int aValue)
{
	if ( !m_bInit )
	{
		printf_error("not init");
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
		printf_error("undefined mode");
	}
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
		printf_error("no chip");
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
			printf_error("request output failed");
			goto _EXIT;
		}
	
		ret = gpiod_line_set_value( pLine, value );		
		if ( ret )
		{
			printf_error("set value failed");
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
		printf_error("unknown mode");
		goto _EXIT;
	}

_EXIT:
	gpiod_line_release(pLine);
	return;
}
