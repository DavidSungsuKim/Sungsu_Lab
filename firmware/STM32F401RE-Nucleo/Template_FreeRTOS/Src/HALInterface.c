
#include <string.h>

#include "main.h"
#include "common.h"
#include "HALInterface.h"

#include "configSTM32.h"

const  uint32_t				cFLAG_UART_ERROR = (uint32_t)(USART_SR_PE | USART_SR_FE | USART_SR_ORE | USART_SR_NE);

/* UART#1 ******************************************/
#define  BSP_UART_RX_BUFF_SIZE		1
UART_HandleTypeDef 			g_UartHandle;
static uint8_t             	g_bUartRxBuf[BSP_UART_RX_BUFF_SIZE];

/* UART#2 (For Debugging) **************************/
static UART_HandleTypeDef 	g_Uart2Handle;
static uint8_t             	g_bUart2RxBuf[BSP_UART_RX_BUFF_SIZE];

/* OTHERS ******************************************/
static GPIO_InitTypeDef  	GPIO_InitStruct;


static void SystemClock_Config  (void);
static void Error_Handler       (void);

static void	InitializeLED		(void);

static void UARTMspInit			(void);
static void UART2MspInit		(void);

void HALIF_Initialize(void)
{
	HAL_Init();

	SystemClock_Config();

	InitializeLED();

	struct stUartConfig uart1;
	uart1.BaudRate 		= 1250000;
	uart1.DataLength	= UART_WORDLENGTH_9B;
	uart1.StopBits		= UART_STOPBITS_1;
	uart1.Parity		= UART_PARITY_ODD;
	HALIF_InitializeUART1(&uart1);

	struct stUartConfig uart2;
	uart2.BaudRate 		= 1250000;
	uart2.DataLength	= UART_WORDLENGTH_9B;
	uart2.StopBits		= UART_STOPBITS_1;
	uart2.Parity		= UART_PARITY_ODD;
	HALIF_InitializeUART2(&uart2);
}

unsigned int HALIF_GetSysTick()
{
	return HAL_GetTick();
}

void HALIF_TurnOnLED1()
{
	HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, GPIO_PIN_RESET);
}

void HALIF_TurnOffLED1()
{
	HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, GPIO_PIN_SET);
}

void HALIF_ToggleLED1()
{
	HAL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN);
}

void HALIF_InterruptEnable()
{
	__enable_irq();
}

void HALIF_InterruptDisable()
{
	__disable_irq();
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 84000000
  *            HCLK(Hz)                       = 84000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 16000000
  *            PLL_M                          = 16
  *            PLL_N                          = 336
  *            PLL_P                          = 4
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale2 mode
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	__HAL_RCC_PWR_CLK_ENABLE();

	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

	RCC_OscInitStruct.OscillatorType 		= RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState 				= RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue 	= 0x10;
	RCC_OscInitStruct.PLL.PLLState 			= RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource 		= RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM 				= 16;
	RCC_OscInitStruct.PLL.PLLN 				= 336;
	RCC_OscInitStruct.PLL.PLLP 				= RCC_PLLP_DIV4;
	RCC_OscInitStruct.PLL.PLLQ 				= 7;

	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
  
	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
	clocks dividers */
	RCC_ClkInitStruct.ClockType 		= (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource 		= RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider 	= RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider 	= RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider 	= RCC_HCLK_DIV1;

	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		Error_Handler();
	}
}

static void InitializeLED(void)
{
	// For LED1
	LED1_GPIO_CLK_ENABLE();

	GPIO_InitStruct.Mode  	= GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  	= GPIO_PULLUP;
	GPIO_InitStruct.Speed 	= GPIO_SPEED_FREQ_LOW;

	GPIO_InitStruct.Pin 	= LED1_PIN;

	HAL_GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStruct);
}

int HALIF_InitializeUART1(const struct stUartConfig *apUart)
{
	UARTMspInit();

	UART_HandleTypeDef* pHandle = &g_UartHandle;

	/*##-1- Configure the UART peripheral ######################################*/
	pHandle->Instance        = UART1_INST;
	pHandle->Init.BaudRate   = apUart->BaudRate;
	pHandle->Init.WordLength = apUart->DataLength;
	pHandle->Init.StopBits   = apUart->StopBits;
	pHandle->Init.Parity     = apUart->Parity;
	pHandle->Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	pHandle->Init.Mode       = UART_MODE_TX_RX;

	if (HAL_UART_Init(pHandle) != HAL_OK)
		return -1;

/*	if (HAL_UART_Receive_IT(pHandle, g_bUart2RxBuf, BSP_UART_RX_BUFF_SIZE) != HAL_OK)
		return -1;

	pHandle->Instance->CR3 |= USART_CR3_EIE;
	pHandle->Instance->CR1 |= USART_CR1_PEIE;
*/
	pHandle->State = HAL_UART_STATE_READY;	// Change the state to READY.

	return 0;
}

static void UARTMspInit(void)
{
	/* NOTE :
	 * Alternate setting is mandatory in F401RE.
	 */

	GPIO_InitTypeDef  GPIO_InitStruct;

	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* Enable GPIO TX/RX clock */
	UART1_GPIO_CLK_ENABLE();

	/* Enable USARTx clock */
	UART1_CLK_ENABLE();

	/*##-2- Configure peripheral GPIO ##########################################*/
	/* UART TX GPIO pin configuration  */
	GPIO_InitStruct.Pin       	= PIN_UART1_TX;
	GPIO_InitStruct.Mode      	= GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      	= GPIO_PULLUP;
	GPIO_InitStruct.Speed     	= GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate 	= UART1_TX_AF;
	HAL_GPIO_Init(GPIO_PORT_UART1_TX, &GPIO_InitStruct);

	/* UART RX GPIO pin configuration  */
	GPIO_InitStruct.Pin 		= PIN_UART1_RX;
	GPIO_InitStruct.Mode      	= GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull      	= GPIO_PULLUP;
	GPIO_InitStruct.Speed     	= GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate 	= UART1_RX_AF;
	HAL_GPIO_Init(GPIO_PORT_UART1_RX, &GPIO_InitStruct);

//	HAL_NVIC_SetPriority(USART3_IRQn, INT_PRIORITY_HIGH, INT_PRIORITY_HIGH);
//	HAL_NVIC_EnableIRQ(USART3_IRQn);
}

int	HALIF_UART1SendSync(const char *aStr)
{
	uint32_t size 	 = strlen(aStr);
	uint32_t timeOut = 1; 	// This delay should be as small as possible.
	HAL_UART_Transmit(&g_UartHandle, (uint8_t *)aStr, size, timeOut);

	return 0;
}

int	HALIF_UART1SendByteSync(char aData, int timeoutMs)
{
	uint32_t timeOut = (uint32_t)timeoutMs;
	HAL_UART_Transmit(&g_UartHandle, (uint8_t *)&aData, 1, timeOut);

	return 0;
}

int HALIF_InitializeUART2(const struct stUartConfig *apUart)
{
	UART2MspInit();

	UART_HandleTypeDef* pHandle = &g_Uart2Handle;

	/*##-1- Configure the UART peripheral ######################################*/
	pHandle->Instance        = UART2_INST;
	pHandle->Init.BaudRate   = apUart->BaudRate;
	pHandle->Init.WordLength = apUart->DataLength;
	pHandle->Init.StopBits   = apUart->StopBits;
	pHandle->Init.Parity     = apUart->Parity;
	pHandle->Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	pHandle->Init.Mode       = UART_MODE_TX_RX;

	if (HAL_UART_Init(pHandle) != HAL_OK)
		return -1;

/*	if (HAL_UART_Receive_IT(pHandle, g_bUart2RxBuf, BSP_UART_RX_BUFF_SIZE) != HAL_OK)
		return -1;

	pHandle->Instance->CR3 |= USART_CR3_EIE;
	pHandle->Instance->CR1 |= USART_CR1_PEIE;
*/
	pHandle->State = HAL_UART_STATE_READY;	// Change the state to READY.

	return 0;
}

static void UART2MspInit(void)
{
	/* NOTE :
	 * Alternate setting is mandatory in F401RE.
	 */

	GPIO_InitTypeDef  GPIO_InitStruct;

	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* Enable GPIO TX/RX clock */
	UART2_GPIO_CLK_ENABLE();

	/* Enable USARTx clock */
	UART2_CLK_ENABLE();

	/*##-2- Configure peripheral GPIO ##########################################*/
	/* UART TX GPIO pin configuration  */
	GPIO_InitStruct.Pin       	= PIN_UART2_TX;
	GPIO_InitStruct.Mode      	= GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      	= GPIO_PULLUP;
	GPIO_InitStruct.Speed     	= GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate 	= UART2_TX_AF;
	HAL_GPIO_Init(GPIO_PORT_UART2_TX, &GPIO_InitStruct);

	/* UART RX GPIO pin configuration  */
	GPIO_InitStruct.Pin 		= PIN_UART2_RX;
	GPIO_InitStruct.Mode      	= GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull      	= GPIO_PULLUP;
	GPIO_InitStruct.Speed     	= GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate 	= UART2_RX_AF;
	HAL_GPIO_Init(GPIO_PORT_UART2_RX, &GPIO_InitStruct);

//	HAL_NVIC_SetPriority(USART3_IRQn, INT_PRIORITY_HIGH, INT_PRIORITY_HIGH);
//	HAL_NVIC_EnableIRQ(USART3_IRQn);
}

int	HALIF_UART2SendSync(const char *aStr)
{
	uint32_t size 	 = strlen(aStr);
	uint32_t timeOut = 1; 	// This delay should be as small as possible.
	HAL_UART_Transmit(&g_Uart2Handle, (uint8_t *)aStr, size, timeOut);

	return 0;
}

static void Error_Handler(void)
{
	while(1)
	{
	}
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}

#endif
