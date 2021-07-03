/**************************************************************************
 * @file    driver_motor_servo_hs311.h
 * @author  Sungsu Kim
 *
 * @date
 * @brief
 */

#ifndef __DRV_MOTOR_SERVO_HS311_H__
#define __DRV_MOTOR_SERVO_HS311_H__

/********************************* Include *******************************/
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
eStatus HS311_Init	(void);
eStatus HS311_Move	(int32_t aDeg);
eStatus HS311_Stop	(void);

/************************* Function Definition ***************************/

#ifdef __cplusplus
}
#endif

#endif /* __DRV_MOTOR_SERVO_HS311_H__ */
