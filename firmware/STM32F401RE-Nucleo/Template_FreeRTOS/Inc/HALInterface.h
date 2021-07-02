/**************************************************************************
 * @file    HALInterface.h
 * @author  Sungsu Kim
 *
 * @date	July, 2021
 * @brief
 */

#ifndef __HAL_INTERFACE_H__
#define __HAL_INTERFACE_H__

/********************************* Include *******************************/
#include <stdint.h>
#include <stdbool.h>
#include "errorCode.h"

#ifdef __cplusplus
extern "C" {
#endif

/********************************* Const *********************************/
// Interrupts.
#define INT_PRIORITY_HIGHEST	0
#define INT_PRIORITY_HIGH		1
#define INT_PRIORITY_NORMAL		2
#define INT_PRIORITY_LOW		3
#define INT_PRIORITY_LOWEST		15

// SYS TIMER
#define	SYS_TIMER_PERIOD_MS		1//10

// TICK TIMER
#define	TICK_TIMER_UNIT_US		0
#define	TICK_TIMER_UNIT_MS		1

/********************************* Types *********************************/
typedef enum
{
	ePWM_CH1	= 1,
	ePWM_CH2,
	ePWM_CH3,
	ePWM_CH4
}ePwmChan;

typedef enum
{
	eSPI_MODE_MASTER = 0,
	eSPI_MODE_SLAVE
}eSpiMode;

struct stUartConfig
{
	uint32_t baudRate;		// bps,  same as UART_InitTypeDef's
	uint32_t dataLength;	// bits, same as UART_InitTypeDef's
	uint32_t stopBits;		// same as UART_InitTypeDef's
	uint32_t parity;		// same as UART_InitTypeDef's
};

/********************************* Macro *********************************/
#define HALIF_EnableInterrupt()			__enable_irq()
#define HALIF_DisableInterrupt()		__disable_irq()

/***************************** Local Variable ****************************/

/**************************** Global Variable ****************************/

/************************* Function Declaration **************************/
void 			HALIF_Init 	 			(void);

eStatus			HALIF_InitUART1			(const struct stUartConfig *apUart);
eStatus			HALIF_UART1SendByteSync	(int8_t aData, uint32_t timeoutMs);
eStatus			HALIF_UART1SendSync		(const char *aStr);
void			HALIF_UART1RecvCallback	(int8_t aData);

eStatus			HALIF_InitUART2			(const struct stUartConfig *apUart);
eStatus			HALIF_UART2SendSync		(const char *aStr);
void			HALIF_UART2RecvCallback	(int8_t aData);

eStatus			HALIF_InitPWM			(double aPeriodSec);
eStatus			HALIF_ControlPWM		(ePwmChan aChannel, uint32_t aDuty);

eStatus			HALIF_InitSPI			(eSpiMode aMode);
eStatus			HALIF_WriteReadByteSPI	(uint8_t tx, uint8_t *rx);
eStatus			HALIF_ReadSPI			(uint8_t *buf, uint16_t size, uint32_t timeOutMs);
eStatus			HALIF_TestSPI			(void);

void			HALIF_TurnOnLED1		(void);
void			HALIF_TurnOffLED1		(void);
void			HALIF_ToggleLED1		(void);

/************************* Function Definition ***************************/
#ifdef __cplusplus
}
#endif

#endif /* __HAL_INTERFACE_H__ */
