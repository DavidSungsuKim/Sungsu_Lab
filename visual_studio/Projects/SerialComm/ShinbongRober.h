#pragma once

#include "SerialInterface.h"
#include "ShinbongRoverCommmand.h"

class CShinbongRober
{
public:
	CShinbongRober();
	~CShinbongRober();

	long		Initialize	();
	long		Uninitialize();

	long		ChangeSpeed		( int aPercent );
	long		ChangeDirection	( int aSpeed );


protected:
	bool				m_bInit;

	int					m_motorPowerPercent;

	CSerialInterface	m_Serial;
};

