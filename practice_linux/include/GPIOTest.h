#ifndef _GPIOTEST_H_
#define _GPIOTEST_H

#include <gpiod.h>

class CGpioInterface
{
public:
	CGpioInterface() {};
	virtual ~CGpioInterface() {};

	enum
	{
		eMODE_OUTPUT = 0,
		eMODE_INPUT  = 1
	};

	enum 
	{
		eVALUE_HIGH = 1,
		eVALUE_LOW  = 0
	};

	virtual void TestGpio1()=0;
	virtual void TestGpio2()=0;
};

class CGpioLGpiod : public CGpioInterface
{	
public:
	CGpioLGpiod();
	~CGpioLGpiod();

protected:
	struct gpiod_chip *m_pChip;

public:
	void TestGpio1();
	void TestGpio2();
	
	void TestLGpio(int aLine, int aMode, int aValue);
};

class CGpioPGpio : public CGpioInterface
{	
public:
	CGpioPGpio();
	~CGpioPGpio();

protected:
	bool m_bInit;

public:
	void TestGpio1();
	void TestGpio2();

	void TestPGpio(int aLine, int aMode, int aValue);
	void TestPWM(int aLine, int aFreq, int aDuty);
};

#endif
