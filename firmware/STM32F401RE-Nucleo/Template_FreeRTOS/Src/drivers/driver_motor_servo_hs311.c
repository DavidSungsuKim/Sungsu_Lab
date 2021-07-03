/**************************************************************************
 * @file    driver_motor_servo_hs311.c
 * @author  Sungsu Kim
 *
 * @date
 * @brief
 */

/********************************* Include *******************************/
#include "driver_motor_servo_hs311.h"
#include "HALInterface.h"

/********************************* Const *********************************/
#define	PWM_PERIOD_SEC				(0.01)
#define HS311_NL_DEG				(-45.0)
#define HS311_PL_DEG        		( 45.0)
#define HS311_DEG_OFFSET			(45.0)
#define HS311_RANGE_DEG     		(90.0)
#define HS311_TIME_PWM_ON_NL		(1.0)
#define HS311_DUTY_AT_NL_DEG		(10.0)
#define	HS311_DUTY_DEGREE_RATIO		(5.0 / 90.0)

/********************************* Types *********************************/

/********************************* Macro *********************************/

/***************************** Local Variable ****************************/

/**************************** Global Variable ****************************/

/************************* Function Declaration **************************/
static uint32_t	calculatePWMduty	(int32_t aDeg);

/************************* Function Definition ***************************/
eStatus HS311_Init(void)
{
	eStatus ret = eOK;
	double periodSec = PWM_PERIOD_SEC;

	ret = HALIF_InitPWM( periodSec );
	return ret;
}

eStatus HS311_Move(int32_t aDeg)
{
	eStatus ret = eOK;

	static uint32_t dir	 = 0;
	static uint32_t duty = 10;//calculatePWMduty( aDeg );

	ret = HALIF_ControlPWM( ePWM_CH1, duty );

	if ( !dir )
	{
		duty += 1;
		if ( duty > 20 )
			dir = 1;
	}

	if ( dir )
	{
		duty -= 1;
		if ( duty < 10 )
		{
			duty = 10;
			dir = 0;
		}
	}

	return ret;
}

eStatus HS311_Stop(void)
{
	eStatus ret = eOK;
	TBD;
	return ret;
}

static uint32_t	calculatePWMduty(int32_t aDeg)
{
	if ( aDeg < HS311_NL_DEG )
		aDeg = HS311_NL_DEG;
	else if ( aDeg > HS311_PL_DEG)
		aDeg = HS311_RANGE_DEG;

	uint32_t pulseDuty 	= (( aDeg + HS311_DEG_OFFSET ) * HS311_DUTY_DEGREE_RATIO + HS311_TIME_PWM_ON_NL ) / PWM_PERIOD_SEC / 10;
	return pulseDuty;
}
