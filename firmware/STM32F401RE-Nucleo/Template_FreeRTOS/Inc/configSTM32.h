/**************************************************************************
 * @file    configSTM32.h
 * @author  Sungsu Kim
 *
 * @date	July, 2021
 * @brief
 */

#ifndef __CONFIG_STM32_H__
#define __CONFIG_STM32_H__

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
#define SPI_GPIO_CLK_CLK_ENABLE()   __GPIOA_CLK_ENABLE()
#define SPI_GPIO_MISO_CLK_ENABLE()  __GPIOA_CLK_ENABLE()
#define SPI_GPIO_MOSI_CLK_ENABLE()  __GPIOA_CLK_ENABLE()
#define SPI_GPIO_PORT_SCK           GPIOA
#define SPI_GPIO_PORT_MISO          GPIOA
#define SPI_GPIO_PORT_MOSI          GPIOA
#define SPI_PIN_SCK                 GPIO_PIN_5
#define SPI_PIN_MISO                GPIO_PIN_6
#define SPI_PIN_MOSI                GPIO_PIN_7
#define SPI_AF                  	GPIO_AF5_SPI1

/* GPIO OUT PORTS **********************************/
#define GPIO_OUT1_CLK_ENABLE() 		__GPIOA_CLK_ENABLE()
#define GPIO_OUT2_CLK_ENABLE() 		__GPIOB_CLK_ENABLE()
#define GPIO_OUT3_CLK_ENABLE()		__GPIOC_CLK_ENABLE()
#define GPIO_OUT4_CLK_ENABLE() 		__GPIOC_CLK_ENABLE()
#define GPIO_OUT1_PORT      		GPIOA
#define GPIO_OUT2_PORT      		GPIOB
#define GPIO_OUT3_PORT      		GPIOC
#define GPIO_OUT4_PORT      		GPIOC
#define GPIO_OUT1_PIN       		GPIO_PIN_4
#define GPIO_OUT2_PIN       		GPIO_PIN_0
#define GPIO_OUT3_PIN       		GPIO_PIN_1
#define GPIO_OUT4_PIN       		GPIO_PIN_0

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
#endif /* __CONFIG_STM32_H__ */
