
#ifndef _CONFIG_STM32_H_
#define _CONFIG_STM32_H_

#ifdef MODE_NUCLEO_F401RE

/* UART#1 *****************************************/
#define UART1_INST					USART1
#define UART1_CLK_ENABLE()			__HAL_RCC_USART1_CLK_ENABLE();
#define UART1_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE();
#define	UART1_GPIO_PORT_TX			GPIOA
#define	UART1_GPIO_PORT_RX			GPIOA
#define UART1_PIN_TX				GPIO_PIN_9
#define UART1_PIN_RX				GPIO_PIN_10
#define UART1_AF					GPIO_AF7_USART1

/* UART#2 *****************************************/
#define UART2_INST					USART6
#define UART2_CLK_ENABLE()			__HAL_RCC_USART6_CLK_ENABLE();
#define UART2_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOC_CLK_ENABLE();
#define	UART2_GPIO_PORT_TX			GPIOC
#define	UART2_GPIO_PORT_RX			GPIOC
#define	UART2_PIN_TX				GPIO_PIN_6
#define	UART2_PIN_RX				GPIO_PIN_7
#define UART2_AF					GPIO_AF8_USART6

/* PWM TIMER **************************************/
#define PWM_PERIOD_SEC				(0.005)
#define	PWM_TIMER_INST				TIM4
#define	PWM_TIMER_CLOCK_ENABLE()	__HAL_RCC_TIM4_CLK_ENABLE()
#define PWM_TIMER_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define PWM_TIMER_GPIO_PORT         GPIOB
#define PWM_TIMER_PIN_CHAN1         GPIO_PIN_6
#define PWM_TIMER_PIN_CHAN2         GPIO_PIN_7
#define PWM_TIMER_PIN_CHAN3       	GPIO_PIN_8
#define PWM_TIMER_PIN_CHAN4      	GPIO_PIN_9
#define PWM_TIMER_AF       			GPIO_AF2_TIM4

/* SPI *********************************************/
#define SPI_INST					SPI1
#define SPI_CLK_ENABLE()			__SPI1_CLK_ENABLE()
#define SPI_GPIO_CLK_CLK_ENABLE()   __GPIOB_CLK_ENABLE()
#define SPI_GPIO_MISO_CLK_ENABLE()  __GPIOB_CLK_ENABLE()
#define SPI_GPIO_MOSI_CLK_ENABLE()  __GPIOB_CLK_ENABLE()
#define SPI_GPIO_PORT_SCK           GPIOB
#define SPI_GPIO_PORT_MISO          GPIOB
#define SPI_GPIO_PORT_MOSI          GPIOB
#define SPI_PIN_SCK                 GPIO_PIN_3
#define SPI_PIN_MISO                GPIO_PIN_4
#define SPI_PIN_MOSI                GPIO_PIN_5
#define SPI_AF                  	GPIO_AF5_SPI1

/* IRS & INTERRUPT *********************************/
#define ISR_UART					USART1_IRQHandler
#define ISR_UART2					USART6_IRQHandler
#define UART1_IRQ					USART1_IRQn
#define UART2_IRQ					USART6_IRQn

/* OTHERS ******************************************/
#define LED_GREEN_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOA_CLK_ENABLE();
#define LED_GREEN_GPIO_PORT			GPIOA
#define LED_GREEN_PIN            	GPIO_PIN_5

#endif /* MODE_NUCLEO_F401RE */
#endif /* _CONFIG_STM32_H_ */
