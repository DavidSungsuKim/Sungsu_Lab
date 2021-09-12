/**************************************************************************
 * @file    XXX.c
 * @author  Sungsu Kim
 *
 * @date
 * @brief
 */

/********************************* Include *******************************/
#include <stdbool.h>
#include "common.h"
#include "driver_motor_step_unipolar.h"
#include "HALInterface.h"

/********************************* Const *********************************/

/********************************* Types *********************************/

/********************************* Macro *********************************/

/***************************** Local Variable ****************************/
static uint32_t g_stepCount = 0;
static int32_t 	g_currentStepCount = 0;

/**************************** Global Variable ****************************/

/************************* Function Declaration **************************/
static void moveStep	(uint32_t aStep);

/************************* Function Definition ***************************/
eStatus	STEP_UNI_Init(uint32_t aStepCount)
{
	eStatus ret = eOK;

	g_stepCount = aStepCount;
	return ret;
}

eStatus	STEP_UNI_moveOneStep(void)
{
	eStatus ret = eOK;


	return ret;
}

eStatus	STEP_UNI_move(uint32_t aStep)
{
	eStatus ret = eOK;
	if ( !g_stepCount )
		return eERR_STEP_COUNT_NOT_DEFINED;



	return ret;
}

static void moveStep(uint32_t aStep)
{

}
