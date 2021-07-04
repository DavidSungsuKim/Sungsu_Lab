/**************************************************************************
 * @file    XXX.h
 * @author  Sungsu Kim
 *
 * @date
 * @brief
 */

/********************************* Include *******************************/
#ifndef __SERVO_H__
#define __SERVO_H__

#include "common.h"
#include "errorCode.h"

#ifdef __cplusplus
extern "C" {
#endif
/********************************* Const *********************************/

/********************************* Types *********************************/
class CServo
{
public:
	CServo(float aPwmPeriodMs, float aDutyCycleMinMs, float aDutyCycleMaxMs, float aMoveRangeDeg);
	~CServo();

	eStatus	Init	(void);
	eStatus Move	(float aDeg);
	eStatus	Stop	(void);
	void	Test	(void);

protected:
	float	CalculatePWMDuty	(float aDeg);

private:
	float m_pwmPeriodMs;
	float m_dutyCycleMinMs;
	float m_dutyCycleMaxMs;
	float m_moveRangeDeg;

	float m_dutyMoveRangeRatio;
	float m_dutyOffset;
};

/********************************* Macro *********************************/

/***************************** Local Variable ****************************/

/**************************** Global Variable ****************************/

/************************* Function Declaration **************************/

/************************* Function Definition ***************************/

#ifdef __cplusplus
}
#endif

#endif /* __SERVO_H__ */
