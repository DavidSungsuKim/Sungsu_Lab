
#ifndef _CONFIG_STM32_H_
#define _CONFIG_STM32_H_

/* UART#1 *****************************************/
#ifdef MODE_NUCLEO_F401RE
	#define 	UART_INST				USART1
	#define 	UART_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOA_CLK_ENABLE();
	#define 	UART_CLK_ENABLE()		__HAL_RCC_USART1_CLK_ENABLE();
	#define		GPIO_PORT_UART_TX		GPIOA
	#define		GPIO_PORT_UART_RX		GPIOA
	#define 	PIN_UART_TX				GPIO_PIN_9
	#define 	PIN_UART_RX				GPIO_PIN_10
#endif /* MODE_NUCLEO_F401RE */

/* UART#2 *****************************************/
#ifdef MODE_NUCLEO_F401RE
	#define 	UART2_INST				USART1
	#define 	UART2_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOA_CLK_ENABLE();
	#define 	UART2_CLK_ENABLE()		__HAL_RCC_USART1_CLK_ENABLE();
	#define 	UART2_TX_AF				GPIO_AF7_USART1	// F401RE only
	#define 	UART2_RX_AF				GPIO_AF7_USART1	// F401RE only
	#define		GPIO_PORT_UART2_TX		GPIOA
	#define		GPIO_PORT_UART2_RX		GPIOA
	#define		PIN_UART2_TX			GPIO_PIN_9
	#define		PIN_UART2_RX			GPIO_PIN_10

#endif /* MODE_NUCLEO_F401RE */

/* OTHERS ******************************************/
#ifdef MODE_NUCLEO_F401RE
	#define 	LED1_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOA_CLK_ENABLE();
	#define 	LED1_GPIO_PORT			GPIOA
	#define 	LED1_PIN            	GPIO_PIN_5
#endif /* MODE_NUCLEO_F401RE */

#endif /* _CONFIG_STM32_H_ */
