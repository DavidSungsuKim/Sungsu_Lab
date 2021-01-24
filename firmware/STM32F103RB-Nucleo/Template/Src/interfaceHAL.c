
#include <string.h>

#include "interfaceHAL.h"
#include "common.h"

#include "stm32f1xx_hal.h"
#include "stm32f1xx_nucleo.h"

// UART
#define  BSP_UART_RX_BUFF_SIZE		1

static UART_HandleTypeDef 	g_UartHandle;
static uint8_t             	g_bUartRxBuf[BSP_UART_RX_BUFF_SIZE];
const  uint32_t				cFLAG_UART_ERROR = (uint32_t)(USART_SR_PE | USART_SR_FE | USART_SR_ORE | USART_SR_NE);

GPIO_TypeDef*				GPIO_PORT_UART_TX	= GPIOA;
GPIO_TypeDef*				GPIO_PORT_UART_RX	= GPIOA;
const uint16_t				PIN_UART_TX			= GPIO_PIN_9;
const uint16_t				PIN_UART_RX			= GPIO_PIN_10;

// GPIO
static GPIO_InitTypeDef  	GPIO_InitStruct;

// Tick Timer
TIM_HandleTypeDef 	g_TimHandle;

void 	SystemClock_Config	(void);
int 	InitializeSysTimer	(int aPeriodMs, int aIntPriority);

int		InitializeTickTimer	(int aPeriod, int aUnit, int aIntPriority);
void	TickTimerMspInit	(int aIntPriority);

void 	InitializeLED		(void);
int		InitializeUART		(void);
void	UARTMspInit			(void);

void HALIF_InitializeHW()
{
	HAL_Init();

	/* Configure the system clock to 64 MHz */
	SystemClock_Config();

#ifdef CONFIG_USE_SYSTICK_TIMER
	InitializeSysTimer(SYS_TIMER_PERIOD_MS, INT_PRIORITY_HIGHEST);
#endif
	InitializeLED();
	InitializeUART();

#ifdef CONFIG_USE_TICK_TIMER
	InitializeTickTimer(100, TICK_TIMER_UNIT_US, INT_PRIORITY_HIGH);
#endif
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 64000000
  *            HCLK(Hz)                       = 64000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            PLLMUL                         = 16
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef clkinitstruct = {0};
	RCC_OscInitTypeDef oscinitstruct = {0};

	/* Configure PLL ------------------------------------------------------*/
	/* PLL configuration: PLLCLK = (HSI / 2) * PLLMUL = (8 / 2) * 16 = 64 MHz */
	/* PREDIV1 configuration: PREDIV1CLK = PLLCLK / HSEPredivValue = 64 / 1 = 64 MHz */
	/* Enable HSI and activate PLL with HSi_DIV2 as source */
	oscinitstruct.OscillatorType  		= RCC_OSCILLATORTYPE_HSI;
	oscinitstruct.HSEState        		= RCC_HSE_OFF;
	oscinitstruct.LSEState        		= RCC_LSE_OFF;
	oscinitstruct.HSIState        		= RCC_HSI_ON;
	oscinitstruct.HSICalibrationValue 	= RCC_HSICALIBRATION_DEFAULT;
	oscinitstruct.HSEPredivValue    	= RCC_HSE_PREDIV_DIV1;
	oscinitstruct.PLL.PLLState    		= RCC_PLL_ON;
	oscinitstruct.PLL.PLLSource   		= RCC_PLLSOURCE_HSI_DIV2;
	oscinitstruct.PLL.PLLMUL      		= RCC_PLL_MUL16;
	if (HAL_RCC_OscConfig(&oscinitstruct)!= HAL_OK)
	{
		/* Initialization Error */
		while(1);
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
	 clocks dividers */
	clkinitstruct.ClockType 			= (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	clkinitstruct.SYSCLKSource 			= RCC_SYSCLKSOURCE_PLLCLK;
	clkinitstruct.AHBCLKDivider 		= RCC_SYSCLK_DIV1;
	clkinitstruct.APB2CLKDivider 		= RCC_HCLK_DIV1;
	clkinitstruct.APB1CLKDivider 		= RCC_HCLK_DIV2;
	if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2)!= HAL_OK)
	{
		/* Initialization Error */
		while(1);
	}
}

#ifdef CONFIG_USE_SYSTICK_TIMER
int InitializeSysTimer(int aPeriodMs, int aIntPriority)
{
	int ret = 0;
	if ( aIntPriority != INT_PRIORITY_HIGHEST )
		return -1;

	if ( aPeriodMs <= 0 )
		return -1;

	uint32_t tickDiv = (1000 / aPeriodMs);
	HAL_SYSTICK_Config(SystemCoreClock / tickDiv);

	/* Note :
	 * This function must be called after HAL_SYSTICK_Config() for all interrupts to be nested by SysTick interrupt.
	 * Otherwise, the priority setting won't be effective, leading to abnormal behavior.
	 */
	HAL_NVIC_SetPriority(SysTick_IRQn, INT_PRIORITY_HIGHEST , INT_PRIORITY_HIGHEST );

	return ret;
}

unsigned int HALIF_GetSysTick()
{
	return HAL_GetTick();
}

// Override (stm32f1xx_hal.c)
uint32_t HAL_GetTick(void)
{
  return (uwTick * SYS_TIMER_PERIOD_MS);
}
#endif

int	InitializeTickTimer(int aPeriod, int aUnit, int aIntPriority)
{
	TickTimerMspInit(aIntPriority);

	uint32_t freqDesiredHz;
	uint32_t period;

	/* NOTE :
	 * the maximum value that can be set to Init.Period is 0xFFFF.
	 * So, as I set T=1us for the this timer clock, the maximum timer period is 65535us(=65.53ms).
	 */
	if (aPeriod < 0xFFFF)
		period = aPeriod;
	else
		return -1;

	if (aUnit == TICK_TIMER_UNIT_US)
		freqDesiredHz 	= 1000000;
	else if (aUnit == TICK_TIMER_UNIT_MS)
		freqDesiredHz	= 1000;
	else
		return -1;

	uint32_t uwPrescalerValue 		= (uint32_t)(SystemCoreClock / freqDesiredHz) - 1;
	if (uwPrescalerValue > 0xFFFF)
		return -1;

	TIM_HandleTypeDef	*pHandle = &g_TimHandle;

	/* Set TIMx instance */
	pHandle->Instance 				= TIM2;
	pHandle->Init.Period            = period - 1;
	pHandle->Init.Prescaler         = uwPrescalerValue;
	pHandle->Init.ClockDivision     = 0;
	pHandle->Init.CounterMode       = TIM_COUNTERMODE_UP;
	pHandle->Init.RepetitionCounter = 0;
	pHandle->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	if (HAL_TIM_Base_Init(pHandle) != HAL_OK)
		return -1;

	/*##-2- Start the TIM Base generation in interrupt mode ####################*/
	/* Start Channel1 */
	if (HAL_TIM_Base_Start_IT(pHandle) != HAL_OK)
		return -1;

	return 0;
}

void TickTimerMspInit(int aIntPriority)
{
	/*##-1- Enable peripheral clock #################################*/
	/* TIMx Peripheral clock enable */
	__HAL_RCC_TIM2_CLK_ENABLE();

#ifdef CONFIG_TICK_TIMER_IRQ
	/*##-2- Configure the NVIC for TIMx ########################################*/
	/* Set the TIMx priority */
	HAL_NVIC_SetPriority(TIM2_IRQn, aIntPriority, INT_PRIORITY_HIGHEST);

	/* Enable the TIMx global Interrupt */
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
#endif
}

#ifdef CONFIG_TICK_TIMER_IRQ
void HALIF_TickTimerCallback()
{
	HAL_TIM_IRQHandler(&g_TimHandle);
}

// Override
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	BSP_LED_Toggle(LED2);
}
#endif

void HALIF_TestTickTimer()
{
	if (HALIF_IsTimerExpired())
	{
		_printf("tick=%d\r\n", HALIF_GetTimerTick());
		HALIF_ResetTickTimer();
	}
}

void InitializeLED(void)
{
	/* -1- Enable GPIO Clock (to be able to program the configuration registers) */
	LED2_GPIO_CLK_ENABLE();

	/* -2- Configure IO in output push-pull mode to drive external LEDs */
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

	GPIO_InitStruct.Pin = LED2_PIN;
	HAL_GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStruct);
}

int InitializeUART(void)
{
	UARTMspInit();

	UART_HandleTypeDef* pHandle = &g_UartHandle;

	/*##-1- Configure the UART peripheral ######################################*/
	pHandle->Instance        = USART1;
	pHandle->Init.BaudRate   = BAUD_RATE_UART;
	pHandle->Init.WordLength = UART_WORDLENGTH_9B;
	pHandle->Init.StopBits   = UART_STOPBITS_1;
	pHandle->Init.Parity     = UART_PARITY_ODD;
	pHandle->Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	pHandle->Init.Mode       = UART_MODE_TX_RX;

	if (HAL_UART_Init(pHandle) != HAL_OK)
		return -1;//	_printfEx("UART init failed.\r\n");

	if (HAL_UART_Receive_IT(pHandle, g_bUartRxBuf, BSP_UART_RX_BUFF_SIZE) != HAL_OK)
		return -1;//

	pHandle->RxState = HAL_UART_STATE_READY;	// Change the state to READY.

	return 0;
}

int	HALIF_UARTSendSync(const char* aStr)
{
#ifdef CONFIG_USE_UART

	uint32_t size 	 = strlen(aStr);
	uint32_t timeOut = 1; 	// This delay should be as small as possible.
	HAL_UART_Transmit(&g_UartHandle, (uint8_t *)aStr, size, timeOut);

#endif /* CONFIG_USE_UART */
	return 0;
}

int	HALIF_UARTSendAsync(const char* aStr)
{
	return 0;
}

int	HALIF_UARTRecv(unsigned char* apPacket)
{
	if (HAL_UART_Receive(&g_UartHandle, apPacket, 1, 10) != HAL_OK)
		return -1;

	return 0;
}

void UARTMspInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* Enable GPIO TX/RX clock */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/* Enable USARTx clock */
	__HAL_RCC_USART1_CLK_ENABLE();

	/*##-2- Configure peripheral GPIO ##########################################*/
	/* UART TX GPIO pin configuration  */
	GPIO_InitStruct.Pin       	= PIN_UART_TX;
	GPIO_InitStruct.Mode      	= GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      	= GPIO_PULLUP;
	GPIO_InitStruct.Speed     	= GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIO_PORT_UART_TX, &GPIO_InitStruct);

	/* UART RX GPIO pin configuration  */
	GPIO_InitStruct.Pin 		= PIN_UART_RX;
	GPIO_InitStruct.Mode      	= GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull      	= GPIO_PULLUP;
	GPIO_InitStruct.Speed     	= GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIO_PORT_UART_RX, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(USART1_IRQn, INT_PRIORITY_HIGH, INT_PRIORITY_HIGHEST);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
}

void HALIF_UARTRecvCallback(void)
{
	UART_HandleTypeDef *pHandle = &g_UartHandle;

	uint32_t isrflags   = READ_REG(pHandle->Instance->SR);
	uint32_t cr1its     = READ_REG(pHandle->Instance->CR1);
	uint32_t errorflags = 0x00U;

	/* If no error occurs */
	errorflags = (isrflags & cFLAG_UART_ERROR);
	if (errorflags == RESET)
	{
		/* UART in mode Receiver -------------------------------------------------*/
		if (((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
		{
			uint8_t packet = 0;
			if (HAL_UART_Receive(pHandle, &packet, 1, 10) != HAL_OK)
				return;
#if 1 /* test */
			HAL_UART_Transmit(&g_UartHandle, &packet, 1, 10);
#endif
		}
	}
}
