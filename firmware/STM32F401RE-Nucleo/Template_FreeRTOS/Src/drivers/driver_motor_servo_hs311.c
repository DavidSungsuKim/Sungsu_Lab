/**************************************************************************
 * @file    driver_motor_servo_hs311.c
 * @author  Sungsu Kim
 *
 * @date
 * @brief
 */

/********************************* Include *******************************/
#include <stdbool.h>
#include "common.h"
#include "driver_motor_servo_hs311.h"
#include "HALInterface.h"

/********************************* Const *********************************/
#define	PWM_PERIOD_SEC					(0.01f)
#define HS311_PWM_DUTY_OFFSET_PERCENT	(10.f)
#define	HS311_DUTY_DEGREE_RATIO			(5.f / 45.f)
#define HS311_RANGE_DEG     			(90.f)

/********************************* Types *********************************/

/********************************* Macro *********************************/

/***************************** Local Variable ****************************/

/**************************** Global Variable ****************************/

/************************* Function Declaration **************************/
static float	calculatePWMduty	(float aDeg);

/************************* Function Definition ***************************/
eStatus HS311_Init(void)
{
	eStatus ret = eOK;
	double periodSec = PWM_PERIOD_SEC;

	ret = HALIF_InitPWM( periodSec );
	return ret;
}

eStatus HS311_Move(float aDeg)
{
	eStatus ret = eOK;
	float duty = calculatePWMduty( aDeg );

	ret = HALIF_ControlPWM( ePWM_CH1, duty );
	return ret;
}

eStatus HS311_Stop(void)
{
	eStatus ret = eOK;
	TBD;
	return ret;
}

void HS311_Test(void)
{
#define MOVE_AMOUNT_DEGREE	(1.0)

	static float 	deg 	= 0.0;
	static bool		bInc 	= true;

	_printf("HS311_Test, deg=%.3f\r\n", deg);

	HS311_Move( deg );
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

static float calculatePWMduty(float aDeg)
{
	if ( aDeg < 0 )
		aDeg = 0;
	else if ( aDeg > HS311_RANGE_DEG)
		aDeg = HS311_RANGE_DEG;

	float pulseDuty = (( aDeg * HS311_DUTY_DEGREE_RATIO ) + HS311_PWM_DUTY_OFFSET_PERCENT);
	return pulseDuty;
}
