
#include "Common.h"
#include "GPIOTest.h"

#include <unistd.h>
#include <sys/time.h>
#include <sys/times.h>

#include <pthread.h>

#include <pigpio.h>

// Direct register access
#include <fcntl.h>
#include <sys/mman.h>

#define BCM2711_PERI_BASE 	0xFE000000
#define BLOCK_SIZE 		(4 * 1024)
#define PAGE_SIZE		(4 * 1024)
#define GPIO_BASE 	(	BCM2711_PERI_BASE + 0x200000)

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
	m_pLine = NULL;
	m_line  = -1;

	m_monInput = 0;
	m_bTaskRun = false;

	int ret;
	ret = pthread_create( &m_Task, NULL, TaskProc, (void*)this );
	if ( ret )
		_printf_error("pthread create fail");
}

CGpioLGpiod::~CGpioLGpiod()
{
	if ( m_pChip )
		gpiod_chip_close( m_pChip );
}

void
CGpioLGpiod::TestGpio1()
{
//	TestLGpio( 2, eMODE_OUTPUT, 1 );
	StartMonitorInput();	
}

void
CGpioLGpiod::TestGpio2()
{
//	TestLGpio( 2, eMODE_OUTPUT, 0 );
	StopMonitorInput();
}

void 
CGpioLGpiod::TestGpio3()
{
	printf("Input numbers:\n 1: Start monitoring\n 2: End monitoring\n 3: Terminate\n");

	int num;
	while(1)
	{
		scanf("%d",&num);

		if ( num == 1 )
		{
			StartMonitorInput();
		}
		else if ( num == 2 )
		{
			StopMonitorInput();
		}
		else if ( num == 3 )
		{
			TerminateMonitorInput();
			break;
		}
		else
		{
			printf("You entered wrong character...\n");
		}

	}

/*	int ret;
	int *pVal;
	ret = pthread_join( m_Task, (void**)&pVal );
	if ( ret )
		_printf_error("pthread_join");
	*/
	return;
}

void
CGpioLGpiod::StartMonitorInput()
{
	printf("StartMonitorInput\n");

	if ( m_pLine == NULL )
	{
		m_line = 2;
		TestLGpio( m_line, eMODE_INPUT, 0 );
	}
	
	m_monInput = 1;
}

void
CGpioLGpiod::StopMonitorInput()
{
	printf("StopMonitorInput\n");
	m_monInput = 0;
}

void
CGpioLGpiod::TerminateMonitorInput()
{
	printf("TerminateMonitorInput\n");
	m_monInput = -1;
}

void
CGpioLGpiod::Task()
{
	int count = 0;

	printf("Wait...\n");
	sleep(3);
	printf("Task..Started\n");

	for(;;)
	{
		if ( m_monInput == 1 )
		{
			if ( m_pLine == NULL )
			{
				_printf_error("m_pLine NULL");
				continue;
			}

			int value = gpiod_line_get_value( m_pLine );
			printf("Line%d status=%d count=%d\r", m_line, value, count++);
		}
		else if ( m_monInput == -1 )
			break;

		usleep(10);
	//	sleep(1);
	}
}

void*
CGpioLGpiod::TaskProc(void* apVoid)
{
	CGpioLGpiod* p = (CGpioLGpiod*)apVoid;

	p->Task();

	return NULL;
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
	m_pLine = pLine;
	
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
		ret = gpiod_line_request_input( pLine, "null" );
		if ( ret )
		{
			_printf_error("request input");
		}
		return;
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

//////////////////////////////////////////////////////////

CGpioDirectReg::CGpioDirectReg()
{
	m_memFd = -1;
	m_pGpioMap = NULL;
	m_pGpio	= NULL;

	SetupIo();
}

CGpioDirectReg::~CGpioDirectReg()
{
	if ( m_pGpioMap != NULL )
		munmap( m_pGpioMap, BLOCK_SIZE );
}

void
CGpioDirectReg::TestGpio1()
{
	TestRegGpio( 2, eMODE_OUTPUT, 1 );
}

void
CGpioDirectReg::TestGpio2()
{
	TestRegGpio( 2, eMODE_OUTPUT, 0 );
}

void 
CGpioDirectReg::TestRegGpio(int aLine, int aMode, int aValue)
{
	if ( !m_pGpio )
	{
		_printf_error("m_pGpio=NULL");
		return;
	}

	int line = aLine;
	int mode = aMode;

	if ( mode == eMODE_OUTPUT )
	{

		INP_GPIO( line );
		OUT_GPIO( line );

		if ( aValue )
			GPIO_SET( line );
		else
			GPIO_CLR( line );
	}
	else if ( mode == eMODE_INPUT )
	{
		;// TBD
	}
	else
		_printf_error("unknown mode");

	return;
}

void
CGpioDirectReg::SetupIo()
{
	m_memFd = open("/dev/mem", O_RDWR | O_SYNC);
	if ( m_memFd < 0 )
	{
		_printf_error("memFd fail");
		return;
	}

	m_pGpioMap = mmap( NULL,
			BLOCK_SIZE,
			PROT_READ | PROT_WRITE,
			MAP_SHARED,
			m_memFd,
			GPIO_BASE);

	close( m_memFd );

	printf("SetupIO, memFd=%d, pGpioMap=0x%lx\n", m_memFd, (long int)m_pGpioMap);

	if ( m_pGpioMap == MAP_FAILED )
	{
		_printf_error("map failed");
		return;
	}

	m_pGpio = (volatile unsigned*)m_pGpioMap;
}
