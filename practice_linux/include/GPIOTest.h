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

class CGpioDirectReg : public CGpioInterface
{
public:
	CGpioDirectReg();
	~CGpioDirectReg();

protected:
	int	m_memFd;
	void	*m_pGpioMap;
	volatile unsigned *m_pGpio;

public:
	void TestGpio1();
	void TestGpio2();

	void TestRegGpio(int aLine, int aMode, int aValue);

protected:
	void SetupIo();

	inline void INP_GPIO	(int aLine) { *(m_pGpio+(aLine/10)) &= ~(7<<((aLine%10)*3)); }
	inline void OUT_GPIO	(int aLine) { *(m_pGpio+(aLine/10)) |=  (1<<((aLine%10)*3)); }

	inline void GPIO_SET	(int aLine) { *(m_pGpio+7)  = 1 << aLine; }
	inline void GPIO_CLR	(int aLine) { *(m_pGpio+10) = 1 << aLine; }

};

#endif
