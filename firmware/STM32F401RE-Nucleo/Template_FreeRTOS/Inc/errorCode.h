/**************************************************************************
 * @file    errorCode.h
 * @author  Sungsu Kim
 *
 * @date	July, 2021
 * @brief
 */

#ifndef __ERROR_CODE_H__
#define __ERROR_CODE_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	eOK	= 0,
	eERR_STM32_DRIVER,
	eERR_PWM_INVALID_CHAN,
	eERR_SPI_INVALID_MODE

}eStatus;

#ifdef __cplusplus
}
#endif

#endif /* __ERROR_CODE_H__ */
