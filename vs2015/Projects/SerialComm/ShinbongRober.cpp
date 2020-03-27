#include "stdafx.h"
#include "ShinbongRober.h"

#include "SerialInterface.h"

CShinbongRober::CShinbongRober()
{
	m_bInit = false;
	Uninitialize();
}

CShinbongRober::~CShinbongRober()
{
	Uninitialize();
}

long		
CShinbongRober::Initialize()
{
	if ( m_Serial.OpenPort(_T("COM6")) != 0 )
		return -1;


	m_bInit = true;
	return 0;
}

long		
CShinbongRober::Uninitialize()
{
	if (m_bInit)
	{
		m_Serial.ClosePort();
	}

	m_motorPowerPercent	= 0;
	m_bInit	= false;
	return 0;
}

long		
CShinbongRober::ChangeSpeed( int aPercent )
{
	if ( !m_bInit )
		return -1;

	int	value = aPercent;
	if (value < 1)	value = 1;
	if (value > 99)	value = 99;

	StMsg	message;
	int		ret;
	message.data[eCmd]	 = eCmdChangeSpeed;
	message.data[eData1] = _ASCII(_NUM10TH(value));
	message.data[eData2] = _ASCII(_NUM1TH(value));
	ret = m_Serial.SendPackets(message.data, eSize);

	return ret;
}

long		
CShinbongRober::ChangeDirection( int aPercent )
{
	if (!m_bInit)
		return -1;

	int	value = aPercent;
	if (value < -99)value = -99;
	if (value > 99)	value = 99;

	StMsg	message;
	int		ret;
	message.data[eCmd] = eCmdChangeDir;
	message.data[eData1] = _ASCII(_NUM10TH(value));
	message.data[eData2] = _ASCII(_NUM1TH(value));
	ret = m_Serial.SendPackets(message.data, eSize);

	return ret;
}