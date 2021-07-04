/**************************************************************************
 * @file    servo.cpp
 * @author  Sungsu Kim
 *
 * @date
 * @brief
 */

/********************************* Include *******************************/
#include <stdbool.h>
#include "common.h"
#include "servo.h"
#include "HALInterface.h"

/********************************* Const *********************************/

/********************************* Types *********************************/

/********************************* Macro *********************************/

/***************************** Local Variable ****************************/

/**************************** Global Variable ****************************/

/************************* Function Declaration **************************/

/************************* Function Definition ***************************/
CServo::CServo(float aPwmPeriodMs, float aDutyCycleMinMs, float aDutyCycleMaxMs, float aMoveRangeDeg)
{
	m_pwmPeriodMs 		= aPwmPeriodMs;
	m_dutyCycleMinMs 	= aDutyCycleMinMs;
	m_dutyCycleMaxMs	= aDutyCycleMaxMs;
	m_moveRangeDeg		= aMoveRangeDeg;
}

CServo::~CServo()
{

}

eStatus CServo::Init(void)
{
	m_dutyMoveRangeRatio = ( m_dutyCycleMaxMs - m_dutyCycleMinMs) * m_pwmPeriodMs / m_moveRangeDeg;
	m_dutyOffset		 = m_dutyCycleMinMs / m_pwmPeriodMs * 100.;

	eStatus ret = eOK;
	double periodSec = (double)m_pwmPeriodMs / 1000.;

	ret = HALIF_InitPWM( periodSec );
	return ret;
}

eStatus CServo::Move(float aDeg)
{
	eStatus ret = eOK;
	float duty = CalculatePWMDuty( aDeg );
	ret = HALIF_ControlPWM( ePWM_CH1, duty );
	return ret;
}

eStatus	CServo::Stop(void)
{
	eStatus ret = eOK;
	TBD;
	return ret;
}

void CServo::Test(void)
{
#define MOVE_AMOUNT_DEGREE	(1.0)

	static float 	deg 	= 0.0;
	static bool		bInc 	= true;

	_printf("HS311_Test, deg=%.3f\r\n", deg);

	Move( deg );
	if ( bInc )
	{
		deg += MOVE_AMOUNT_DEGREE;
		if ( deg >= 90.0 )
			bInc = false;
	}
	else
	{
		deg -= MOVE_AMOUNT_DEGREE;
		if ( deg <= 0.0 )
			bInc = true;
	}
}

float CServo::CalculatePWMDuty(float aDeg)
{
	if ( aDeg < 0 )
		aDeg = 0;
	else if ( aDeg > m_moveRangeDeg )
		aDeg = m_moveRangeDeg;

	float pulseDuty = ( aDeg * m_dutyMoveRangeRatio ) + m_dutyOffset;
	return pulseDuty;
}

