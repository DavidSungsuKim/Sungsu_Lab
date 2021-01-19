
#ifndef _INTERFACE_HAL_H_
#define _INTERFACE_HAL_H_

// Interrupts.
#define 	INT_PRIORITY_HIGHEST	0
#define 	INT_PRIORITY_HIGH		1
#define 	INT_PRIORITY_NORMAL		2
#define 	INT_PRIORITY_LOW		3
#define 	INT_PRIORITY_LOWEST		15

// UART
#define		BAUD_RATE_UART			115200


void		HALIF_InitializeHW		();

int			HALIF_UARTSendSync		(const char* aStr);
int			HALIF_UARTSendAsync		(const char* aStr);
int			HALIF_UARTRecv			(unsigned char* apPacket);
void		HALIF_UARTRecvCallback	(void);

#endif /* _INTERFACE_HAL_H_ */
