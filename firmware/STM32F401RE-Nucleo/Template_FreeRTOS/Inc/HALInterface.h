
#ifndef _HAL_INTERFACE_H_
#define _HAL_INTERFACE_H_

#include <stdint.h>
#include <stdbool.h>
#include "errorCode.h"

#ifdef __cplusplus
extern "C" {
#endif

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

typedef enum
{
	ePWM_CH1	= 1,
	ePWM_CH2,
	ePWM_CH3,
	ePWM_CH4
}ePwmChan;

struct stUartConfig
{
	unsigned int BaudRate;		// bps,  same as UART_InitTypeDef's
	unsigned int DataLength;	// bits, same as UART_InitTypeDef's
	unsigned int StopBits;		// same as UART_InitTypeDef's
	unsigned int Parity;		// same as UART_InitTypeDef's
};

void 			HALIF_Initialize 	 	(void);

eStatus			HALIF_InitializeUART1	(const struct stUartConfig *apUart);
eStatus			HALIF_UART1SendByteSync	(int8_t aData, uint32_t timeoutMs);
eStatus			HALIF_UART1SendSync		(const char *aStr);

eStatus			HALIF_InitializeUART2	(const struct stUartConfig *apUart);
eStatus			HALIF_UART2SendSync		(const char *aStr);

eStatus			HALIF_InitPWM			(void);
eStatus			HALIF_ControlPWM		(ePwmChan aChannel, uint32_t aDuty);

void			HALIF_TurnOnLED1		(void);
void			HALIF_TurnOffLED1		(void);
void			HALIF_ToggleLED1		(void);

void			HALIF_InterruptEnable	(void);
void			HALIF_InterruptDisable	(void);

#ifdef __cplusplus
}
#endif

#endif /* _HAL_INTERFACE_H_ */
