/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct
{ /* Mail object structure */
  uint32_t var1; /* var1 is a uint32_t */
  uint32_t var2; /* var2 is a uint32_t */
  uint8_t var3; /* var3 is a uint8_t */
} Amail_TypeDef;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define blckqSTACK_SIZE   configMINIMAL_STACK_SIZE
#define MAIL_SIZE        (uint32_t) 1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
osThreadId MailQueueProducHandle;
osThreadId MailQueueConsumHandle;

/* USER CODE BEGIN PV */
osMailQId mailId;

uint32_t ProducerValue1 = 0, ProducerValue2 = 0;
uint8_t ProducerValue3 = 0;
__IO uint32_t ProducerErrors = 0;
uint32_t ConsumerValue1 = 0, ConsumerValue2 = 0;
uint8_t ConsumerValue3 = 0;
__IO uint32_t ConsumerErrors = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
void MailQueueProducer(void const * argument);
void MailQueueConsumer(void const * argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  /* Create the mail queue used by the two tasks to pass the struct Amail_TypeDef */
  osMailQDef(mail, MAIL_SIZE, Amail_TypeDef); /* Define mail queue */

  mailId = osMailCreate(osMailQ(mail), NULL); /* create mail queue */
  /* USER CODE END 2 */

  /* Create the thread(s) */
  /* definition and creation of MailQueueProduc */
  osThreadDef(MailQueueProduc, MailQueueProducer, osPriorityBelowNormal, 0, 128);
  MailQueueProducHandle = osThreadCreate(osThread(MailQueueProduc), NULL);

  /* definition and creation of MailQueueConsum */
  osThreadDef(MailQueueConsum, MailQueueConsumer, osPriorityBelowNormal, 0, 128);
  MailQueueConsumHandle = osThreadCreate(osThread(MailQueueConsum), NULL);
  /* Start scheduler */
  osKernelStart();
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LD3_Pin */
  GPIO_InitStruct.Pin = LD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD3_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN Header_MailQueueProducer */
/**
  * @brief  Function implementing the MailQueueProduc thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_MailQueueProducer */
void MailQueueProducer(void const * argument)
{
  /* USER CODE BEGIN 5 */
  Amail_TypeDef *pTMail;

  for(;;)
  {

    pTMail = reinterpret_cast<Amail_TypeDef*>(osMailAlloc(mailId, osWaitForever)); /* Allocate memory */
    pTMail->var1 = ProducerValue1; /* Set the mail content */
    pTMail->var2 = ProducerValue2;
    pTMail->var3 = ProducerValue3;

    if(osMailPut(mailId, pTMail) != osOK) /* Send Mail */
    {
      ++ProducerErrors;

      /* Turn on LED3 to indicate error */
      //BSP_LED_On(LED3);
    }
    else
    {
      /* Increment the variables we are going to post next time round.  The
      consumer will expect the numbers to follow in numerical order. */
      ++ProducerValue1;
      ProducerValue2 += 2;
      ProducerValue3 += 3;

      /* Toggle LED3 to indicate a correct number received  */
      if( (ProducerErrors == 0) && (ConsumerErrors == 0) )
        //BSP_LED_Toggle(LED3);

      osDelay(250);
    }
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_MailQueueConsumer */
/**
* @brief Function implementing the MailQueueConsum thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_MailQueueConsumer */
void MailQueueConsumer(void const * argument)
{
  /* USER CODE BEGIN MailQueueConsumer */
  osEvent event;
  Amail_TypeDef *pRMail;

  for(;;)
  {
    /* Get the message from the queue */
    event = osMailGet(mailId, osWaitForever); /* wait for mail */

    if(event.status == osEventMail)
    {
      pRMail = reinterpret_cast<Amail_TypeDef*>(event.value.p);

      if((pRMail->var1 != ConsumerValue1) || (pRMail->var2 != ConsumerValue2) || (pRMail->var3 != ConsumerValue3))
      {
        /* Catch-up. */
        ConsumerValue1 = pRMail->var1;
        ConsumerValue2 = pRMail->var2;
        ConsumerValue3 = pRMail->var3;

        ++ConsumerErrors;

        /* Turn on LED3 to indicate error */
        //BSP_LED_On(LED3);
      }
      else
      {
        /* Calculate values we expect to remove from the mail queue next time
        round. */
        ++ConsumerValue1;
        ConsumerValue2 += 2;
        ConsumerValue3 += 3;
      }

      osMailFree(mailId, pRMail); /* free memory allocated for mail */
    }
  }
  /* USER CODE END MailQueueConsumer */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
