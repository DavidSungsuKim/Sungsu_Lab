
#ifndef _CONFIG_STM32_H_
#define _CONFIG_STM32_H_

#ifdef MODE_NUCLEO_F401RE

/* UART#1 *****************************************/
#define UART1_INST					USART1
#define UART1_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE();
#define UART1_CLK_ENABLE()			__HAL_RCC_USART1_CLK_ENABLE();
#define UART1_TX_AF					GPIO_AF7_USART1
#define UART1_RX_AF					GPIO_AF7_USART1
#define	GPIO_PORT_UART1_TX			GPIOA
#define	GPIO_PORT_UART1_RX			GPIOA
#define PIN_UART1_TX				GPIO_PIN_9
#define PIN_UART1_RX				GPIO_PIN_10

/* UART#2 *****************************************/
#define UART2_INST					USART6
#define UART2_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOC_CLK_ENABLE();
#define UART2_CLK_ENABLE()			__HAL_RCC_USART6_CLK_ENABLE();
#define UART2_TX_AF					GPIO_AF8_USART6
#define UART2_RX_AF					GPIO_AF8_USART6
#define	GPIO_PORT_UART2_TX			GPIOC
#define	GPIO_PORT_UART2_RX			GPIOC
#define	PIN_UART2_TX				GPIO_PIN_6
#define	PIN_UART2_RX				GPIO_PIN_7

/* PWM TIMER **************************************/
#define PWM_PERIOD_SEC				(0.005)
#define	TIMER_PWM_INST				TIM4
#define	TIMER_PWM_CLOCK_ENABLE()	__HAL_RCC_TIM4_CLK_ENABLE()
#define TIMER_PWM_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define TIMER_PWM_GPIO_PORT         GPIOB
#define PWM_PIN_CHANNEL1            GPIO_PIN_6
#define PWM_PIN_CHANNEL2            GPIO_PIN_7
#define PWM_PIN_CHANNEL3            GPIO_PIN_8
#define PWM_PIN_CHANNEL4            GPIO_PIN_9
#define PWM_AF_TIMER_PWM       		GPIO_AF2_TIM4

/* SPI *********************************************/
#define SPI_INST					SPI1
#define SPI_CLK_ENABLE()			__SPI1_CLK_ENABLE()
#define SPI_CLK_GPIO_CLK_ENABLE()   __GPIOB_CLK_ENABLE()
#define SPI_MISO_GPIO_CLK_ENABLE()  __GPIOB_CLK_ENABLE()
#define SPI_MOSI_GPIO_CLK_ENABLE()  __GPIOB_CLK_ENABLE()
#define GPIO_PORT_SPI_SCK           GPIOB
#define GPIO_PORT_SPI_MISO          GPIOB
#define GPIO_PORT_SPI_MOSI          GPIOB
#define PIN_SPI_SCK                 GPIO_PIN_3
#define PIN_SPI_MISO                GPIO_PIN_4
#define PIN_SPI_MOSI                GPIO_PIN_5
#define SPI_AF                  	GPIO_AF5_SPI1

/* INTERRUPTS **************************************/
#define UART1_IRQ					USART1_IRQn
#define UART2_IRQ					USART6_IRQn

/* OTHERS ******************************************/
#define LED_GREEN_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOA_CLK_ENABLE();
#define LED_GREEN_GPIO_PORT			GPIOA
#define LED_GREEN_PIN            	GPIO_PIN_5

#endif /* MODE_NUCLEO_F401RE */
#endif /* _CONFIG_STM32_H_ */
