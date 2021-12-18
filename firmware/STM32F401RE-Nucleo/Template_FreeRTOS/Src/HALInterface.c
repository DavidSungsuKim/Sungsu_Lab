/**************************************************************************
 * @file    HALInterface.c
 * @author  Sungsu Kim
 *
 * @date	July, 2021
 * @brief
 */

/********************************* Include *******************************/
#include "common.h"
#include "stm32f4xx_hal.h"
#include "HALInterface.h"
#include "configSTM32.h"

#include "FreeRTOS.h"
#include "task.h"

/********************************* Const *********************************/
const uint32_t cFLAG_UART_ERROR = (uint32_t)(USART_SR_PE | USART_SR_FE | USART_SR_ORE | USART_SR_NE);

#define UART_RX_BUFF_SIZE			(1u)
#define	SPI_TIMEOUT_MS				(1000u)

/********************************* Types *********************************/

/********************************* Macro *********************************/

/***************************** Local Variable ****************************/
static UART_HandleTypeDef 		g_hUart1;
static UART_HandleTypeDef 		g_hUart2;
static TIM_HandleTypeDef   		g_hTimerPWM;
static SPI_HandleTypeDef		g_hSpi;

static uint8_t             		g_bUartRxBuf	[UART_RX_BUFF_SIZE];
static uint8_t             		g_bUart2RxBuf	[UART_RX_BUFF_SIZE];

/**************************** Global Variable ****************************/

/************************* Function Declaration **************************/
static void SystemClock_Config  (void);
static void Error_Handler       (void);

static void ConfigIRQ			(void);

static void	InitLED				(void);
static void UARTMspInit			(void);
static void UART2MspInit		(void);
static void PWMMspInit			(void);
static void InitSPIModeMaster	(void);
static void InitSPIModeSlave	(void);
static void SPIMspInit			(void);
static void GpioOutMspInit		(void);

/************************* Function Definition **************************/
void HALIF_Init(void)
{
	HAL_Init();

	SystemClock_Config();

	InitLED();

	// For Serial Comm.
	struct stUartConfig uart1;
	uart1.baudRate 		= 115200;
	uart1.dataLength	= UART_WORDLENGTH_8B;
	uart1.stopBits		= UART_STOPBITS_1;
	uart1.parity		= UART_PARITY_NONE;
	HALIF_InitUART1(&uart1);

	// For printf()
	struct stUartConfig uart2;
	uart2.baudRate 		= 115200;
	uart2.dataLength	= UART_WORDLENGTH_8B;
	uart2.stopBits		= UART_STOPBITS_1;
	uart2.parity		= UART_PARITY_NONE;
	HALIF_InitUART2(&uart2);

	HALIF_InitSPI(eSPI_MODE_MASTER);

	GpioOutMspInit();

	ConfigIRQ();
}

uint32_t HALIF_GetSysTick(void)
{
	volatile TickType_t tick = xTaskGetTickCount();
	return (uint32_t)tick;
}

void HALIF_TurnOnLED1(void)
{
	HAL_GPIO_WritePin(LED_GREEN_GPIO_PORT, LED_GREEN_PIN, GPIO_PIN_RESET);
}

void HALIF_TurnOffLED1(void)
{
	HAL_GPIO_WritePin(LED_GREEN_GPIO_PORT, LED_GREEN_PIN, GPIO_PIN_SET);
}

void HALIF_ToggleLED1(void)
{
	HAL_GPIO_TogglePin(LED_GREEN_GPIO_PORT, LED_GREEN_PIN);
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
	RCC_ClkInitStruct.ClockType 			= (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource 			= RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider 		= RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider 		= RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider 		= RCC_HCLK_DIV1;

	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		Error_Handler();
	}
}

static void InitLED(void)
{
	// For LED1
	LED_GREEN_GPIO_CLK_ENABLE();

	GPIO_InitTypeDef  GPIO_InitStruct;

	GPIO_InitStruct.Mode  	= GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  	= GPIO_PULLUP;
	GPIO_InitStruct.Speed 	= GPIO_SPEED_FREQ_LOW;

	GPIO_InitStruct.Pin 	= LED_GREEN_PIN;

	HAL_GPIO_Init(LED_GREEN_GPIO_PORT, &GPIO_InitStruct);
}

eStatus HALIF_InitUART1(const struct stUartConfig *apUart)
{
	eStatus ret = eOK;

	UARTMspInit();

	UART_HandleTypeDef* pHandle = &g_hUart1;

	/*##-1- Configure the UART peripheral ######################################*/
	pHandle->Instance        = UART1_INST;
	pHandle->Init.BaudRate   = apUart->baudRate;
	pHandle->Init.WordLength = apUart->dataLength;
	pHandle->Init.StopBits   = apUart->stopBits;
	pHandle->Init.Parity     = apUart->parity;
	pHandle->Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	pHandle->Init.Mode       = UART_MODE_TX_RX;

	if (HAL_UART_Init(pHandle) != HAL_OK)
		return -1;

	if (HAL_UART_Receive_IT(pHandle, g_bUart2RxBuf, UART_RX_BUFF_SIZE) != HAL_OK)
		return -1;

	pHandle->Instance->CR3 |= USART_CR3_EIE;
	pHandle->Instance->CR1 |= USART_CR1_PEIE;
	pHandle->State = HAL_UART_STATE_READY;

	return ret;
}

eStatus HALIF_InitUART2(const struct stUartConfig *apUart)
{
	eStatus ret = eOK;

	UART2MspInit();

	UART_HandleTypeDef* pHandle = &g_hUart2;

	/*##-1- Configure the UART peripheral ######################################*/
	pHandle->Instance        = UART2_INST;
	pHandle->Init.BaudRate   = apUart->baudRate;
	pHandle->Init.WordLength = apUart->dataLength;
	pHandle->Init.StopBits   = apUart->stopBits;
	pHandle->Init.Parity     = apUart->parity;
	pHandle->Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	pHandle->Init.Mode       = UART_MODE_TX_RX;

	if (HAL_UART_Init(pHandle) != HAL_OK)
		return -1;

	if (HAL_UART_Receive_IT(pHandle, g_bUart2RxBuf, UART_RX_BUFF_SIZE) != HAL_OK)
		return -1;

	pHandle->Instance->CR3 |= USART_CR3_EIE;
	pHandle->Instance->CR1 |= USART_CR1_PEIE;
	pHandle->State = HAL_UART_STATE_READY;

	return ret;
}

eStatus	HALIF_UART1SendSync(const char *aStr)
{
	eStatus ret = eOK;

	uint32_t size 	 = strlen(aStr);
	uint32_t timeOut = 1; 	// This delay should be as small as possible.
	HAL_UART_Transmit(&g_hUart1, (uint8_t *)aStr, size, timeOut);

	return ret;
}

void HALIF_UART1RecvCallback(int8_t aData)
{
	TBD;
}

eStatus	HALIF_UART1SendByteSync(int8_t aData, uint32_t timeoutMs)
{
	eStatus ret = eOK;

	uint32_t timeOut = (uint32_t)timeoutMs;
	HAL_UART_Transmit(&g_hUart1, (uint8_t *)&aData, 1, timeOut);

	return ret;
}

eStatus	HALIF_UART2SendSync(const char *aStr)
{
	eStatus ret = eOK;

	uint32_t size 	 = strlen(aStr);
	uint32_t timeOut = 1; 	// This delay should be as small as possible.
	HAL_UART_Transmit(&g_hUart2, (uint8_t *)aStr, size, timeOut);

	return ret;
}

void HALIF_UART2RecvCallback(int8_t aData)
{
	TBD;
}

eStatus HALIF_InitPWM(double aPeriodSec)
{
	eStatus ret = eOK;

	PWMMspInit();

	TIM_HandleTypeDef*     pHdl   = &g_hTimerPWM;

	uint32_t      clkFreqHz       = HAL_RCC_GetSysClockFreq();
	uint32_t      prescaler       = 25;
	uint32_t      clkFreqHzScaled = clkFreqHz / (prescaler + 1);
	double        pwmPeriodSec    = aPeriodSec;
	double        pwmPeriodHz     = ( 1. / pwmPeriodSec );
	uint32_t      timPeriod       = ( clkFreqHzScaled / (uint32_t)pwmPeriodHz ) - 1;

/*	_printf("CPU clock=%d[Hz]\r\n",        clkFreqHz);
	_printf("pwmPeriodSec=%.3f[Sec]\r\n",  pwmPeriodSec);
	_printf("PwmPeriod=%.3f[Hz]\r\n",      pwmPeriodHz);
	_printf("TimPeriod=%d[Hz]\r\n",        timPeriod);
*/
	pHdl->Instance                = PWM_TIMER_INST;
	pHdl->Init.Period             = timPeriod;                                    	// 0x0000~0xFFFF
	pHdl->Init.Prescaler          = prescaler;                                    	// 0x0000~0xFFFF
	pHdl->Init.ClockDivision      = TIM_CLOCKDIVISION_DIV1;
	pHdl->Init.CounterMode        = TIM_COUNTERMODE_UP;

	//pHdl->Init.RepetitionCounter  = 0x80;                                         // 0x00~0xFF // Valid only for TIM1 and TIM8

	if ( HAL_TIM_PWM_Init(pHdl) != HAL_OK )
		return eERR_STM32_DRIVER;

	return ret;
}

eStatus	HALIF_ControlPWM(ePwmChan aChannel, float aDuty)
{
	eStatus ret = eOK;

	TIM_HandleTypeDef*    pHdl          = &g_hTimerPWM;
	uint32_t              timPeriod     = pHdl->Init.Period;
	uint32_t              pwmChan;

	switch( aChannel )
	{
		case ePWM_CH1 : pwmChan = TIM_CHANNEL_1; break;
		case ePWM_CH2 : pwmChan = TIM_CHANNEL_2; break;
		case ePWM_CH3 : pwmChan = TIM_CHANNEL_3; break;
		case ePWM_CH4 : pwmChan = TIM_CHANNEL_4; break;
		default:
			return eERR_PWM_INVALID_CHAN;
	}

	TIM_OC_InitTypeDef    ocConfig;
	uint32_t              pulse   = (uint32_t)((((float)timPeriod + 1.) * aDuty) / 100. - 1.);

	if ( pulse > timPeriod )
		pulse = timPeriod;

	ocConfig.OCMode			= TIM_OCMODE_PWM2;
	ocConfig.Pulse          = pulse;
	ocConfig.OCPolarity		= TIM_OCPOLARITY_LOW;
	ocConfig.OCNPolarity    = TIM_OCNPOLARITY_LOW;

	if ( HAL_TIM_PWM_ConfigChannel(pHdl, &ocConfig, pwmChan) != HAL_OK )
		return eERR_STM32_DRIVER;

	if ( HAL_TIM_PWM_Start(pHdl, pwmChan) != HAL_OK )
		return eERR_STM32_DRIVER;

	return ret;
}

eStatus HALIF_InitSPI(eSpiMode aMode)
{
	eStatus ret = eOK;

	switch ( aMode )
	{
	case eSPI_MODE_MASTER:
		InitSPIModeMaster();
		break;
	case eSPI_MODE_SLAVE:
		InitSPIModeSlave();
		break;
	default:
		ret = eERR_SPI_INVALID_MODE;
		break;
	}

	return ret;
}

eStatus	HALIF_WriteReadByteSPI(uint8_t tx, uint8_t *rx)
{
	eStatus ret = eOK;
	HAL_StatusTypeDef retHAL = HAL_OK;

	retHAL = HAL_SPI_TransmitReceive(&g_hSpi, &tx, rx, 1, SPI_TIMEOUT_MS);
	if( retHAL != HAL_OK )
		ret = eERR_STM32_DRIVER;

	return ret;
}

eStatus HALIF_ReadSPI(uint8_t *buf, uint16_t size, uint32_t timeOutMs)
{
	eStatus ret = eOK;
	HAL_StatusTypeDef retHAL = HAL_OK;

	retHAL = HAL_SPI_Receive(&g_hSpi, buf, size, timeOutMs);
	if( retHAL != HAL_OK)
		ret = eERR_STM32_DRIVER;

	return ret;
}

eStatus	HALIF_TestSPI(void)
{
	eStatus ret = eOK;

	uint8_t txByte = 0xaa;
	uint8_t rxByte = 0x00;

	HALIF_WriteReadByteSPI( txByte, &rxByte );

	uint8_t bSame = 0;
	if ( txByte == rxByte )
		bSame = 1;

	return ret;
}

void HALIF_setGPIO1(bool bOn)
{
	HAL_GPIO_WritePin( GPIO_OUT1_PORT, GPIO_OUT1_PIN, (bOn ? GPIO_PIN_SET : GPIO_PIN_RESET));
}

void HALIF_setGPIO2(bool bOn)
{
	HAL_GPIO_WritePin( GPIO_OUT2_PORT, GPIO_OUT2_PIN, (bOn ? GPIO_PIN_SET : GPIO_PIN_RESET));
}

void HALIF_setGPIO3(bool bOn)
{
	HAL_GPIO_WritePin( GPIO_OUT3_PORT, GPIO_OUT3_PIN, (bOn ? GPIO_PIN_SET : GPIO_PIN_RESET));
}

void HALIF_setGPIO4(bool bOn)
{
	HAL_GPIO_WritePin( GPIO_OUT4_PORT, GPIO_OUT4_PIN, (bOn ? GPIO_PIN_SET : GPIO_PIN_RESET));
}

static void ConfigIRQ(void)
{
	HAL_NVIC_SetPriority(UART1_IRQ, INT_PRIORITY_HIGH, INT_PRIORITY_HIGH);
	HAL_NVIC_SetPriority(UART2_IRQ, INT_PRIORITY_HIGH, INT_PRIORITY_HIGH);

	HAL_NVIC_EnableIRQ(UART1_IRQ);
	HAL_NVIC_EnableIRQ(UART2_IRQ);
}

static void UARTMspInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	UART1_GPIO_CLK_ENABLE();
	UART1_CLK_ENABLE();

	/*##-2- Configure peripheral GPIO ##########################################*/
	/* UART TX GPIO pin configuration  */
	GPIO_InitStruct.Pin       	= UART1_PIN_TX;
	GPIO_InitStruct.Mode      	= GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      	= GPIO_PULLUP;
	GPIO_InitStruct.Speed     	= GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate 	= UART1_AF;
	HAL_GPIO_Init(UART1_GPIO_PORT_TX, &GPIO_InitStruct);

	/* UART RX GPIO pin configuration  */
	GPIO_InitStruct.Pin 		= UART1_PIN_RX;
	GPIO_InitStruct.Mode      	= GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      	= GPIO_PULLUP;
	GPIO_InitStruct.Speed     	= GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate 	= UART1_AF;
	HAL_GPIO_Init(UART1_GPIO_PORT_RX, &GPIO_InitStruct);
}

static void UART2MspInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	UART2_GPIO_CLK_ENABLE();
	UART2_CLK_ENABLE();

	/*##-2- Configure peripheral GPIO ##########################################*/
	/* UART TX GPIO pin configuration  */
	GPIO_InitStruct.Pin       	= UART2_PIN_TX;
	GPIO_InitStruct.Mode      	= GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      	= GPIO_PULLUP;
	GPIO_InitStruct.Speed     	= GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate 	= UART2_AF;
	HAL_GPIO_Init(UART2_GPIO_PORT_TX, &GPIO_InitStruct);

	/* UART RX GPIO pin configuration  */
	GPIO_InitStruct.Pin 		= UART2_PIN_RX;
	GPIO_InitStruct.Mode      	= GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      	= GPIO_PULLUP;
	GPIO_InitStruct.Speed     	= GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate 	= UART2_AF;
	HAL_GPIO_Init(UART2_GPIO_PORT_RX, &GPIO_InitStruct);
}

static void PWMMspInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	PWM_TIMER_GPIO_CLK_ENABLE();
	PWM_TIMER_CLOCK_ENABLE();

	// Pin config for ch1
	GPIO_InitStruct.Pin           = PWM_TIMER_PIN_CHAN1;
	GPIO_InitStruct.Mode          = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull          = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed         = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate     = PWM_TIMER_AF;

	HAL_GPIO_Init(PWM_TIMER_GPIO_PORT, &GPIO_InitStruct);

	// Pin config for ch2
	GPIO_InitStruct.Pin           = PWM_TIMER_PIN_CHAN2;
	HAL_GPIO_Init(PWM_TIMER_GPIO_PORT, &GPIO_InitStruct);

	// Pin config for ch3
	GPIO_InitStruct.Pin           = PWM_TIMER_PIN_CHAN3;
	HAL_GPIO_Init(PWM_TIMER_GPIO_PORT, &GPIO_InitStruct);

	// Pin config for ch4
	GPIO_InitStruct.Pin           = PWM_TIMER_PIN_CHAN4;
	HAL_GPIO_Init(PWM_TIMER_GPIO_PORT, &GPIO_InitStruct);
}

static void InitSPIModeMaster(void)
{
	if( HAL_SPI_GetState(&g_hSpi) != HAL_SPI_STATE_RESET )
		return;

	SPIMspInit();

	g_hSpi.Instance = SPI_INST;

	SPI_InitTypeDef *init	= &(g_hSpi.Init);

	init->BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	init->Direction 		= SPI_DIRECTION_2LINES;
	init->CLKPhase 			= SPI_PHASE_2EDGE;
	init->CLKPolarity 		= SPI_POLARITY_HIGH;
	init->CRCCalculation 	= SPI_CRCCALCULATION_DISABLED;
	init->CRCPolynomial 	= 7;
	init->DataSize 			= SPI_DATASIZE_8BIT;
	init->FirstBit 			= SPI_FIRSTBIT_MSB;
	init->NSS 				= SPI_NSS_SOFT;
	init->TIMode 			= SPI_TIMODE_DISABLED;
	init->Mode 				= SPI_MODE_MASTER;

	HAL_SPI_Init(&g_hSpi);
}

static void InitSPIModeSlave(void)
{
	if( HAL_SPI_GetState(&g_hSpi) != HAL_SPI_STATE_RESET )
		return;

	SPIMspInit();

	g_hSpi.Instance = SPI_INST;

	SPI_InitTypeDef *init	= &(g_hSpi.Init);

	init->BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8; 	// effective on master mode only
	init->Direction 		= SPI_DIRECTION_2LINES;		// SPI_DIRECTION_2LINES;
	init->CLKPhase 			= SPI_PHASE_1EDGE;			// SPI_PHASE_2EDGE;
	init->CLKPolarity 		= SPI_POLARITY_LOW;			// SPI_POLARITY_HIGH;
	init->CRCCalculation 	= SPI_CRCCALCULATION_DISABLED;
	init->CRCPolynomial 	= 7;
	init->DataSize 			= SPI_DATASIZE_8BIT;
	init->FirstBit 			= SPI_FIRSTBIT_MSB;			// effective on master mode only
	init->NSS 				= SPI_NSS_SOFT;				// SPI_NSS_HARD_INPUT;//SPI_NSS_SOFT; // this SPI module in slave mode is internally selected
	init->TIMode 			= SPI_TIMODE_DISABLED;
	init->Mode 				= SPI_MODE_SLAVE;			// SPI_MODE_MASTER;

	HAL_SPI_Init(&g_hSpi);
}

static void SPIMspInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	SPI_CLK_ENABLE();

	SPI_GPIO_CLK_CLK_ENABLE();
	SPI_GPIO_MISO_CLK_ENABLE();
	SPI_GPIO_MOSI_CLK_ENABLE();

	/* Configure SPI SCK */
	GPIO_InitStruct.Pin 		= SPI_PIN_SCK;
	GPIO_InitStruct.Mode 		= GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull  		= GPIO_PULLUP;
	GPIO_InitStruct.Speed 		= GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate 	= SPI_AF;
	HAL_GPIO_Init(SPI_GPIO_PORT_SCK, &GPIO_InitStruct);

	/* Configure SPI MISO and MOSI */
	GPIO_InitStruct.Pin 		= SPI_PIN_MOSI;
	GPIO_InitStruct.Alternate 	= SPI_AF;
	GPIO_InitStruct.Pull  		= GPIO_PULLDOWN;
	HAL_GPIO_Init(SPI_GPIO_PORT_MOSI, &GPIO_InitStruct);

	GPIO_InitStruct.Pin 		= SPI_PIN_MISO;
	HAL_GPIO_Init(SPI_GPIO_PORT_MISO, &GPIO_InitStruct);
}

static void GpioOutMspInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	GPIO_OUT1_CLK_ENABLE();
	GPIO_OUT2_CLK_ENABLE();
	GPIO_OUT3_CLK_ENABLE();
	GPIO_OUT4_CLK_ENABLE();

	GPIO_InitStruct.Pin 		= GPIO_OUT1_PIN;
	GPIO_InitStruct.Mode 		= GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  		= GPIO_NOPULL;//GPIO_PULLUP;
	GPIO_InitStruct.Speed 		= GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIO_OUT1_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin 		= GPIO_OUT2_PIN;
	HAL_GPIO_Init(GPIO_OUT2_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin 		= GPIO_OUT3_PIN;
	HAL_GPIO_Init(GPIO_OUT3_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin 		= GPIO_OUT4_PIN;
	HAL_GPIO_Init(GPIO_OUT4_PORT, &GPIO_InitStruct);
}

void ISR_UART(void)
{
	UART_HandleTypeDef *huart = &g_hUart1;

	uint32_t tmp1 = 0, tmp2 = 0;

	tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_PE);
	tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_PE);

	/* UART parity error interrupt occurred ------------------------------------*/
	if((tmp1 != RESET) && (tmp2 != RESET))
	{
		__HAL_UART_CLEAR_PEFLAG(huart);
		huart->ErrorCode |= HAL_UART_ERROR_PE;
	}

	tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_FE);
	tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_ERR);

	/* UART frame error interrupt occurred -------------------------------------*/
	if((tmp1 != RESET) && (tmp2 != RESET))
	{
		__HAL_UART_CLEAR_FEFLAG(huart);
		huart->ErrorCode |= HAL_UART_ERROR_FE;
	}

	tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_NE);
	tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_ERR);

	/* UART noise error interrupt occurred -------------------------------------*/
	if((tmp1 != RESET) && (tmp2 != RESET))
	{
		__HAL_UART_CLEAR_NEFLAG(huart);
		huart->ErrorCode |= HAL_UART_ERROR_NE;
	}

	tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_ORE);
	tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_ERR);

	/* UART Over-Run interrupt occurred ----------------------------------------*/
	if((tmp1 != RESET) && (tmp2 != RESET))
	{
		__HAL_UART_CLEAR_OREFLAG(huart);
		huart->ErrorCode |= HAL_UART_ERROR_ORE;
	}

	tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE);
	tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_RXNE);

	/* UART in mode Receiver ---------------------------------------------------*/
	if((tmp1 != RESET) && (tmp2 != RESET))
	{
		HALIF_UART1RecvCallback( huart->Instance->DR );
	}

	tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_TXE);
	tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_TXE);

	/* UART in mode Transmitter ------------------------------------------------*/
	if((tmp1 != RESET) && (tmp2 != RESET))
	{
		;
	}

	tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_TC);
	tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_TC);

	/* UART in mode Transmitter end --------------------------------------------*/
	if((tmp1 != RESET) && (tmp2 != RESET))
	{
		;
	}

	if(huart->ErrorCode != HAL_UART_ERROR_NONE)
	{
		/* Set the UART state ready to be able to start again the process */
		huart->State = HAL_UART_STATE_READY;
	}
}

void ISR_UART2(void)
{
	UART_HandleTypeDef *huart = &g_hUart2;

	uint32_t tmp1 = 0, tmp2 = 0;

	tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_PE);
	tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_PE);

	/* UART parity error interrupt occurred ------------------------------------*/
	if((tmp1 != RESET) && (tmp2 != RESET))
	{
		__HAL_UART_CLEAR_PEFLAG(huart);
		huart->ErrorCode |= HAL_UART_ERROR_PE;
	}

	tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_FE);
	tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_ERR);

	/* UART frame error interrupt occurred -------------------------------------*/
	if((tmp1 != RESET) && (tmp2 != RESET))
	{
		__HAL_UART_CLEAR_FEFLAG(huart);
		huart->ErrorCode |= HAL_UART_ERROR_FE;
	}

	tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_NE);
	tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_ERR);

	/* UART noise error interrupt occurred -------------------------------------*/
	if((tmp1 != RESET) && (tmp2 != RESET))
	{
		__HAL_UART_CLEAR_NEFLAG(huart);
		huart->ErrorCode |= HAL_UART_ERROR_NE;
	}

	tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_ORE);
	tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_ERR);

	/* UART Over-Run interrupt occurred ----------------------------------------*/
	if((tmp1 != RESET) && (tmp2 != RESET))
	{
		__HAL_UART_CLEAR_OREFLAG(huart);
		huart->ErrorCode |= HAL_UART_ERROR_ORE;
	}

	tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE);
	tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_RXNE);

	/* UART in mode Receiver ---------------------------------------------------*/
	if((tmp1 != RESET) && (tmp2 != RESET))
	{
		HALIF_UART2RecvCallback( huart->Instance->DR );
	}

	tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_TXE);
	tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_TXE);

	/* UART in mode Transmitter ------------------------------------------------*/
	if((tmp1 != RESET) && (tmp2 != RESET))
	{
		;
	}

	tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_TC);
	tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_TC);

	/* UART in mode Transmitter end --------------------------------------------*/
	if((tmp1 != RESET) && (tmp2 != RESET))
	{
		;
	}

	if(huart->ErrorCode != HAL_UART_ERROR_NONE)
	{
		/* Set the UART state ready to be able to start again the process */
		huart->State = HAL_UART_STATE_READY;
	}
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
