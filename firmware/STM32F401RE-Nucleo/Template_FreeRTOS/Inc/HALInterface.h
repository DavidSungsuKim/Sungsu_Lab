
#ifndef _HAL_INTERFACE_H_
#define _HAL_INTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

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

struct stUartConfig
{
	unsigned int BaudRate;		// bps
	/* NOTE :
	 * add other UART properties as you wish
	 */
};

void 			HALIF_Initialize 	 	(void);

int				HALIF_UART2SendSync		(const char *aStr);

void			HALIF_TurnOnLED1		();
void			HALIF_TurnOffLED1		();
void			HALIF_ToggleLED1		();

void			HALIF_InterruptEnable	();
void			HALIF_InterruptDisable	();

#ifdef __cplusplus
}
#endif

#endif /* _HAL_INTERFACE_H_ */
