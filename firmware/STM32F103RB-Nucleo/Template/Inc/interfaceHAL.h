
#ifndef _INTERFACE_HAL_H_
#define _INTERFACE_HAL_H_

// Interrupts.
#define INT_PRIORITY_HIGHEST	0
#define INT_PRIORITY_HIGH		1
#define INT_PRIORITY_NORMAL		2
#define INT_PRIORITY_LOW		3
#define INT_PRIORITY_LOWEST		15

// UART
#define	BAUD_RATE_UART			1250000

// UART2
#define	BAUD_RATE_UART2			1250000

// SYS TIMER
#define	SYS_TIMER_PERIOD_MS		1//10

// TICK TIMER
#define	TICK_TIMER_UNIT_US		0
#define	TICK_TIMER_UNIT_MS		1

void			HALIF_InitializeHW		();
unsigned int	HALIF_GetSysTick		();	// Returns the current tick in 1ms unit.

void			HALIF_TickTimerCallback	();

static inline int	HALIF_IsTimerExpired	();
static inline void	HALIF_ResetTickTimer	();
static unsigned int	HALIF_GetTimerTick		();

void			HALIF_TestTickTimer		();

int				HALIF_UARTSendSync		(const char* aStr);
int				HALIF_UARTSendAsync		(const char* aStr);
int				HALIF_UARTRecv			(unsigned char* apPacket);
void			HALIF_UARTRecvCallback	(void);

int				HALIF_UART2SendSync		(const char* aStr);

#include "common.h"
#include "stm32f1xx_hal.h"

extern TIM_HandleTypeDef 	g_TimHandle;

// Inline funciton definitions
inline int HALIF_IsTimerExpired()
{
	if (__HAL_TIM_GET_FLAG(&g_TimHandle, TIM_FLAG_UPDATE) != RESET)
	{
		if (__HAL_TIM_GET_IT_SOURCE(&g_TimHandle, TIM_IT_UPDATE) != RESET)
			return 1;
	}
	return 0;
}

inline void HALIF_ResetTickTimer()
{
//	g_TimHandle.Instance->CNT = 0;
	__HAL_TIM_CLEAR_IT(&g_TimHandle, TIM_IT_UPDATE);
}

inline unsigned int HALIF_GetTimerTick()
{
	return (unsigned int)g_TimHandle.Instance->CNT;
}

#endif /* _INTERFACE_HAL_H_ */
