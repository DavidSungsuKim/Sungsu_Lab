/**************************************************************************
 * @file    XXX.h
 * @author  Sungsu Kim
 *
 * @date
 * @brief
 */

/********************************* Include *******************************/
#ifndef __DRV_MOTOR_STEP_UNIPOLAR_H__
#define __DRV_MOTOR_STEP_UNIPOLAR_H__

#include <stdint.h>
#include "common.h"
#include "errorCode.h"

#ifdef __cplusplus
extern "C" {
#endif
/********************************* Const *********************************/

/********************************* Types *********************************/

/********************************* Macro *********************************/

/***************************** Local Variable ****************************/

/**************************** Global Variable ****************************/

/************************* Function Declaration **************************/
eStatus	STEP_UNI_Init			(uint32_t aStepCount);
eStatus	STEP_UNI_moveOneStep	(void);
eStatus	STEP_UNI_move			(uint32_t aStep);

/************************* Function Definition ***************************/

#ifdef __cplusplus
}
#endif

#endif /* __DRV_MOTOR_STEP_UNIPOLAR_H__ */
