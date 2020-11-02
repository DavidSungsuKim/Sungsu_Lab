/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                             (c) Copyright 2011; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                        BOARD SUPPORT PACKAGE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                         STM3240G-EVAL
*                                        Evaluation Board
*
* Filename      : bsp.c
* Version       : V1.00
* Programmer(s) : FF
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define   BSP_MODULE

#include  <bsp.h>
#include  <os.h>
#include  <string.h>

#include "UartController.h"

extern OS_SEM  Sem1, Sem2, Sem3, Sem4;

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define  BSP_BIT_RCC_PLLCFGR_PLLM                8u
#define  BSP_BIT_RCC_PLLCFGR_PLLN              400u
#define  BSP_BIT_RCC_PLLCFGR_PLLP                4u
#define  BSP_BIT_RCC_PLLCFGR_PLLQ                7u

#define  BSP_BIT_RCC_PLLI2SCFGR_PLLI2S_N       258u
#define  BSP_BIT_RCC_PLLI2SCFGR_PLLI2S_R         3u


#define  BSP_GPIOG_LED1                        DEF_BIT_06
#define  BSP_GPIOG_LED2                        DEF_BIT_08
#define  BSP_GPIOI_LED3                        DEF_BIT_09
#define  BSP_GPIOC_LED4                        DEF_BIT_07


#define  BSP_UART_RX_BUFF_SIZE                  1


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

unsigned char SemNum = 0;
OS_TICK preClk = 0;

// RS232
static  UART_HandleTypeDef      s_hUart;
static  uint8_t                 g_bUartRxBuf[BSP_UART_RX_BUFF_SIZE];

// TIMER
static  TIM_HandleTypeDef       s_hTimer;

/*
*********************************************************************************************************
*                                             REGISTERS
*********************************************************************************************************
*/

#define  BSP_REG_DEM_CR                       (*(CPU_REG32 *)0xE000EDFC)
#define  BSP_REG_DWT_CR                       (*(CPU_REG32 *)0xE0001000)
#define  BSP_REG_DWT_CYCCNT                   (*(CPU_REG32 *)0xE0001004)
#define  BSP_REG_DBGMCU_CR                    (*(CPU_REG32 *)0xE0042004)

/*
*********************************************************************************************************
*                                            REGISTER BITS
*********************************************************************************************************
*/

#define  BSP_DBGMCU_CR_TRACE_IOEN_MASK                   0x10
#define  BSP_DBGMCU_CR_TRACE_MODE_ASYNC                  0x00
#define  BSP_DBGMCU_CR_TRACE_MODE_SYNC_01                0x40
#define  BSP_DBGMCU_CR_TRACE_MODE_SYNC_02                0x80
#define  BSP_DBGMCU_CR_TRACE_MODE_SYNC_04                0xC0
#define  BSP_DBGMCU_CR_TRACE_MODE_MASK                   0xC0

#define  BSP_BIT_DEM_CR_TRCENA                    DEF_BIT_24

#define  BSP_BIT_DWT_CR_CYCCNTENA                 DEF_BIT_00

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/*      Sungsu
static  void  BSP_LED_Init(void);
*/

void    SendSignal              (void);
int     BSP_Init_UART           (void);
int     BSP_Init_PWM            (void);

// Sungsu : These functions are from HAL_TimeBase a sample project.
static void SystemClock_Config(void);           
static void Error_Handler(void);               

// Interrupt callback functions
void    BSP_User_Button_ISR     (void);
void    BSP_NotifyIntUART1      (void);
void    BSP_NotifyIntTIM4       (void);

void SendSignal(void)
{
    OS_TICK clk;
    OS_ERR err;
    int tmp;
    
    clk = OSTimeGet(&err);
    tmp = clk - preClk;
    preClk = clk; 
    if( tmp < 200 ) return;
    printf("clk=%d\r\n", clk);    
    
    switch(SemNum)
    {
      case 0:
        OSSemPost(&Sem2, OS_OPT_POST_1 + OS_OPT_POST_NO_SCHED, &err);   break;
      case 1:
        OSSemPost(&Sem1, OS_OPT_POST_1 + OS_OPT_POST_NO_SCHED, &err);   break;
      case 2:
        OSSemPost(&Sem3, OS_OPT_POST_1 + OS_OPT_POST_NO_SCHED, &err);   break;
      case 3:
        OSSemPost(&Sem4, OS_OPT_POST_1 + OS_OPT_POST_NO_SCHED, &err);   break;
    }
    if( (++SemNum) > 3 ) SemNum = 0;
}

void BSP_User_Button_ISR(void)
{
    SendSignal();
    
    /* Clear the EXTI line 1 pending bit */
/*      Sungsu    
    EXTI_ClearITPendingBit(EXTI_Line0);
    */
}

int BSP_Init_UART(void)
{
  int nRet = 0;
  
  UART_HandleTypeDef* pHdl;
 
  pHdl = &s_hUart;
  
  pHdl->Instance          = USARTx; 
  pHdl->Init.BaudRate     = 9600;
  pHdl->Init.WordLength   = UART_WORDLENGTH_8B;
  pHdl->Init.StopBits     = UART_STOPBITS_1;
  pHdl->Init.Parity       = UART_PARITY_NONE;
  pHdl->Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  pHdl->Init.Mode         = UART_MODE_TX_RX;
  pHdl->Init.OverSampling = UART_OVERSAMPLING_16;
    
  if( HAL_UART_Init(pHdl) != HAL_OK )
  {
    Error_Handler(); 
    nRet = 1;
  }
  
  // Mapping Rx buffer
  HAL_UART_Receive_IT(&s_hUart, g_bUartRxBuf, BSP_UART_RX_BUFF_SIZE);
    
  return nRet;
}

void BSP_NotifyIntUART1 (void)
{ 
  HAL_UART_IRQHandler(&s_hUart);
  
  UartNotifyRecv(s_hUart.pRxBuffPtr[0]);
}

void BSP_NotifyIntTIM4 (void)
{
//  HAL_TIM_IRQHandler(&s_hTimer); 
  static int xxx = 0;
  xxx++;
}

int BSP_Send_Uart(char const* pData, uint16_t Size, uint32_t Timeout)
{
    int nRet = 0;
    
    HAL_UART_Transmit(&s_hUart, pData, Size, Timeout);
    
    return nRet;
}
    
int BSP_SendMessage(char const* cString)
{
  int nRet = 0;

  unsigned short	usLen;
	
  usLen	= strlen((const char*)cString);
    
  HAL_UART_Transmit(&s_hUart, cString, usLen, 0x00FF);  
  
  return nRet;
}

int BSP_Recv_Uart(uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  HAL_StatusTypeDef   tDef;
    
  tDef = HAL_UART_Receive(&s_hUart, pData, Size, Timeout);
  
  return (int)tDef; 
}

// under testing..
int BSP_Reset_RecvBuff()
{
  HAL_UART_Receive_IT(&s_hUart, g_bUartRxBuf, BSP_UART_RX_BUFF_SIZE);
  
  return 0;
}

int BSP_Init_PWM(void)
{
  // Reference :
  // 1. stm32f4xx_hal_tim.h    
  // 2. https://stm32f4-discovery.net/2014/05/stm32f4-stm32f429-discovery-pwm-tutorial/  
  
  int nRet = 0;
    
  TIM_HandleTypeDef*     pHdl   = &s_hTimer;
      
  uint32_t      clkFreqHz       = BSP_CPU_ClkFreq();
  uint32_t      prescaler       = 25;
  uint32_t      clkFreqHzScaled = clkFreqHz / (prescaler + 1);
  double        pwmPeriodSec    = BSP_PWM1GetPeriodSec();
  double        pwmPeriodHz     = ( 1. / pwmPeriodSec );
  uint32_t      timPeriod       = ( clkFreqHzScaled / (uint32_t)pwmPeriodHz ) - 1;        // 0xFFFF is the maximum period that I can use.

  printf("CPU clock=%d[Hz]\r\n",        BSP_CPU_ClkFreq());
  printf("pwmPeriodSec=%.3f[Sec]\r\n",  pwmPeriodSec);  
  printf("PwmPeriod=%.3f[Hz]\r\n",      pwmPeriodHz);  
  printf("TimPeriod=%d[Hz]\r\n",        timPeriod);

  pHdl->Instance                = TIMx;
  pHdl->Init.Period             = timPeriod;                                    // 0x0000~0xFFFF
  pHdl->Init.Prescaler          = prescaler;                                    // 0x0000~0xFFFF
  pHdl->Init.ClockDivision      = TIM_CLOCKDIVISION_DIV1;
  pHdl->Init.CounterMode        = TIM_COUNTERMODE_UP;
//pHdl->Init.RepetitionCounter  = 0x80;                                         // 0x00~0xFF // Valid only for TIM1 and TIM8
    
  if( HAL_TIM_PWM_Init(pHdl) != HAL_OK )
  {
    Error_Handler(); 
    nRet = 1;
  }
  
  return nRet;
}

double BSP_PWM1GetPeriodSec ( )
{
  // In terms of resolution, 0.0025 is optimal to control HS311,
  // but 0.005 seems to be the best for torque at neutral position.
  return 0.005;//0.0025;//0.01;//0.02
}

int BSP_PWM1( uint32_t channel, uint32_t duty )
{
  TIM_HandleTypeDef*     pHdl           = &s_hTimer;
  uint32_t              pwmChan;
  uint32_t              timPeriod       = pHdl->Init.Period;
  
  switch( channel )
  {
    case PWM1_CHAN1 : pwmChan = TIM_CHANNEL_1; break;
    case PWM1_CHAN2 : pwmChan = TIM_CHANNEL_2; break;
    case PWM1_CHAN3 : pwmChan = TIM_CHANNEL_3; break;
    case PWM1_CHAN4 : pwmChan = TIM_CHANNEL_4; break;
    default:
      return -1;
  }
  
  uint32_t      dutycopied = duty;
  
  printf("Duty=%d%\r\n", dutycopied);
  
  TIM_OC_InitTypeDef    ocConfig;
  uint32_t              pulse   = ((timPeriod + 1) * dutycopied) / 100 - 1;
  
  if ( pulse > timPeriod )
    pulse = timPeriod;
  
  ocConfig.OCMode              = TIM_OCMODE_PWM2;
  ocConfig.Pulse               = pulse;
  ocConfig.OCPolarity          = TIM_OCPOLARITY_LOW;
  ocConfig.OCNPolarity         = TIM_OCNPOLARITY_LOW;
  
  if ( HAL_TIM_PWM_ConfigChannel(pHdl, &ocConfig, pwmChan) != HAL_OK )
    Error_Handler();

  if ( HAL_TIM_PWM_Start(pHdl, pwmChan) != HAL_OK )
    Error_Handler(); 
  
  return 0;
}

static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  
  /* Enable HSI Oscillator and activate PLL with HSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 0x10;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
 
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

static void Error_Handler(void)
{
   printf("Error handler.....\r\n");
}

/*
*********************************************************************************************************
*                                               BSP_Init()
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) This function SHOULD be called before any other BSP function is called.
*
*               (2) CPU instruction / data tracing requires the use of the following pins :
*                   (a) (1) Aysynchronous     :  PB[3]
*                       (2) Synchronous 1-bit :  PE[3:2]
*                       (3) Synchronous 2-bit :  PE[4:2]
*                       (4) Synchronous 4-bit :  PE[6:2]
*
*                   (c) The application may wish to adjust the trace bus width depending on I/O
*                       requirements.
*********************************************************************************************************
*/

void  BSP_Init (void)
{
/*      Sungsu  */
  HAL_Init();    
  
  SystemClock_Config();
        
  HAL_NVIC_EnableIRQ(SysTick_IRQn);
  HAL_ResumeTick();
  
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  
  BSP_Init_UART();
  BSP_Init_PWM();

  // Sungsu : interrupt configuration
  BSP_IntInit();
  BSP_IntVectSet        (BSP_INT_ID_USART1, BSP_NotifyIntUART1);
  BSP_IntPrioSet        (BSP_INT_ID_USART1, 2);
  BSP_IntEn             (BSP_INT_ID_USART1);
  
  // Do I need to use these?
  BSP_IntVectSet        (BSP_INT_ID_TIM4, BSP_NotifyIntTIM4);
  BSP_IntPrioSet        (BSP_INT_ID_TIM4, 1);
  BSP_IntEn             (BSP_INT_ID_TIM4);
  
/*      Sungsu  
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);    // When this is used, stack-related error happens. I guess I may need to clarify interrupt handling in terms of being used in OS. 
  */    
/*      Sungsu
  BSP_IntInit();
  BSP_IntVectSet(BSP_INT_ID_EXTI0, BSP_User_Button_ISR);
*/
  
/*      Sungsu  : This is a set of original codes..
    BSP_IntInit();

    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_ON);                                  // HSE = 8MHz ext. crystal.                            
    RCC_WaitForHSEStartUp();
    
    RCC_HCLKConfig(RCC_SYSCLK_Div1);                            // HCLK    = AHBCLK  = PLL / AHBPRES(1) = 100MHz.     
    RCC_PCLK2Config(RCC_HCLK_Div2);                             // APB2CLK = AHBCLK  / APB2DIV(1)       = 100MHz.       
    RCC_PCLK1Config(RCC_HCLK_Div1);                             // APB1CLK = AHBCLK  / APB1DIV(2)       = 48MHz (max).  
    
                                                                // PLLCLK    = HSE * (PLLN / PLLM)      = 400MHz.       
                                                                // SYSCLK    = PLLCLK / PLLP            = 100MHz.       
                                                                // OTG_FSCLK = PLLCLK / PLLQ            =  48MHz.        
    RCC_PLLConfig(RCC_PLLCFGR_PLLSRC_HSE,
                  BSP_BIT_RCC_PLLCFGR_PLLM,
                  BSP_BIT_RCC_PLLCFGR_PLLN, 
                  BSP_BIT_RCC_PLLCFGR_PLLP, 
                  BSP_BIT_RCC_PLLCFGR_PLLQ);
    
    RCC_PLLCmd(ENABLE);
    
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {       // Wait for PLL to lock.                                
        ;
    }
    
    
    FLASH_SetLatency(FLASH_ACR_PRFTEN |FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_3WS);                          // 3 Flash wait states              
    FLASH_PrefetchBufferCmd(ENABLE);
    
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);                  // HCLK = SYSCLK = PLL = 100MHz.                        
    while (RCC_GetSYSCLKSource() != RCC_CFGR_SWS_PLL) {
        ;
    }
   
    RCC_I2SCLKConfig(RCC_I2S2CLKSource_PLLI2S);                 // I2S clock configuration                             
    RCC_PLLI2SConfig(BSP_BIT_RCC_PLLI2SCFGR_PLLI2S_N, BSP_BIT_RCC_PLLI2SCFGR_PLLI2S_R);
    RCC_PLLI2SCmd(ENABLE);
    while (RCC_GetFlagStatus(RCC_FLAG_PLLI2SRDY) == RESET) {       // Wait for PLLI2S to lock.                          
        ;
    }
 
    BSP_LED_Init();                                             // Init LEDs.                                           

    STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_EXTI);
    
    BSP_IntVectSet(BSP_INT_ID_EXTI0, BSP_User_Button_ISR);
    
#ifdef TRACE_EN                                                 // See project / compiler preprocessor options.         
    BSP_CPU_REG_DBGMCU_CR |=  BSP_DBGMCU_CR_TRACE_IOEN_MASK;    // Enable tracing (see Note #2).                        
    BSP_CPU_REG_DBGMCU_CR &= ~BSP_DBGMCU_CR_TRACE_MODE_MASK;    // Clr trace mode sel bits.                             
    BSP_CPU_REG_DBGMCU_CR |=  BSP_DBGMCU_CR_TRACE_MODE_SYNC_04; // Cfg trace mode to synch 4-bit.                       
#endif
  */
}

/*
*********************************************************************************************************
*                                            BSP_CPU_ClkFreq()
*
* Description : Read CPU registers to determine the CPU clock frequency of the chip.
*
* Argument(s) : none.
*
* Return(s)   : The CPU clock frequency, in Hz.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT32U  BSP_CPU_ClkFreq (void)
{
    return (HAL_RCC_GetHCLKFreq());
  
/*      Sungsu  
    RCC_ClocksTypeDef  rcc_clocks;


    RCC_GetClocksFreq(&rcc_clocks);
    return ((CPU_INT32U)rcc_clocks.HCLK_Frequency);
    */
}

/*
*********************************************************************************************************
*                                           BSP_LED_Init()
*
* Description : Initialize any or all the LEDs on the board.
*
* Argument(s) : led     The ID of the LED to control:
*
*                       0    inialize ALL  LEDs
*                       1    inialize user LED1
*                       2    inialize user LED2
*                       3    inialize user LED3
*                       4    inialize user LED4
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

/*      Sungsu
static void  BSP_LED_Init()
{
    STM_EVAL_LEDInit(LED4);
    STM_EVAL_LEDInit(LED3);
    STM_EVAL_LEDInit(LED5);
    STM_EVAL_LEDInit(LED6);
}
*/

/*
*********************************************************************************************************
*                                             BSP_LED_On()
*
* Description : Turn ON any or all the LEDs on the board.
*
* Argument(s) : led     The ID of the LED to control:
*
*                       0    turns ON ALL  LEDs
*                       1    turns ON user LED1
*                       2    turns ON user LED2
*                       3    turns ON user LED3
*                       4    turns ON user LED4
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

/*
void  BSP_LED_On (CPU_INT08U  led)
{
      Sungsu    
    switch (led) {
        case 0:
            
              STM_EVAL_LEDOn(LED4);
              STM_EVAL_LEDOn(LED3);
              STM_EVAL_LEDOn(LED5);
              STM_EVAL_LEDOn(LED6);
             break;

        case 1:
             STM_EVAL_LEDOn(LED3);
              break;

        case 2:    
          STM_EVAL_LEDOn(LED4);
             break;

        case 3: 
          STM_EVAL_LEDOn(LED5);
             break;
             
        case 4:   
          STM_EVAL_LEDOn(LED6);
             break;             

        default:
             break;
    }
}
*/

/*
*********************************************************************************************************
*                                              BSP_LED_Off()
*
* Description : Turn OFF any or all the LEDs on the board.
*
* Argument(s) : led     The ID of the LED to control:
*
*                       0    turns OFF ALL the LEDs
*                       1    turns OFF user LED1
*                       2    turns OFF user LED2
*                       3    turns OFF user LED3
*                       4    turns OFF user LED4
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

/*      Sungsu
void  BSP_LED_Off (CPU_INT08U led)
{

    switch (led) {
        case 0:       
              STM_EVAL_LEDOff(LED4);
              STM_EVAL_LEDOff(LED3);
              STM_EVAL_LEDOff(LED5);
              STM_EVAL_LEDOff(LED6);
             break;

        case 1:
          STM_EVAL_LEDOff(LED3);
             break;

        case 2:   
          STM_EVAL_LEDOff(LED4);
             break;

        case 3:   
          STM_EVAL_LEDOff(LED5);
             break;
             
        case 4: 
          STM_EVAL_LEDOff(LED6);
             break;             

        default:
             break;
    }
}
*/


/*
*********************************************************************************************************
*                                            BSP_LED_Toggle()
*
* Description : TOGGLE any or all the LEDs on the board.
*
* Argument(s) : led     The ID of the LED to control:
*
*                       0    TOGGLE ALL the LEDs
*                       1    TOGGLE user LED1
*                       2    TOGGLE user LED2
*                       3    TOGGLE user LED3
*                       4    TOGGLE user LED4
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

/*      Sungsu
void  BSP_LED_Toggle (CPU_INT08U  led)
{
    switch (led) {
        case 0:
          STM_EVAL_LEDToggle(LED4);
              STM_EVAL_LEDToggle(LED3);
              STM_EVAL_LEDToggle(LED5);
              STM_EVAL_LEDToggle(LED6);
             break;

        case 1:
              STM_EVAL_LEDToggle(LED3);
             break;
             
        case 2:
              STM_EVAL_LEDToggle(LED4);
             break;
             
        case 3:
              STM_EVAL_LEDToggle(LED5);
             break;
             
        case 4:
              STM_EVAL_LEDToggle(LED6);
             break;

        default:
             break;
    }
}
*/

void BSP_msec_Delay(CPU_INT32U msec)
{
    volatile CPU_INT32U i;
    
    do{
      for(i=0;i<6000u;i++);
    }while(msec--);
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                          CPU_TS_TmrInit()
*
* Description : Initialize & start CPU timestamp timer.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : CPU_TS_Init().
*
*               This function is an INTERNAL CPU module function & MUST be implemented by application/
*               BSP function(s) [see Note #1] but MUST NOT be called by application function(s).
*
* Note(s)     : (1) CPU_TS_TmrInit() is an application/BSP function that MUST be defined by the developer 
*                   if either of the following CPU features is enabled :
*
*                   (a) CPU timestamps
*                   (b) CPU interrupts disabled time measurements
*
*                   See 'cpu_cfg.h  CPU TIMESTAMP CONFIGURATION  Note #1'
*                     & 'cpu_cfg.h  CPU INTERRUPTS DISABLED TIME MEASUREMENT CONFIGURATION  Note #1a'.
*
*               (2) (a) Timer count values MUST be returned via word-size-configurable 'CPU_TS_TMR' 
*                       data type.
*
*                       (1) If timer has more bits, truncate timer values' higher-order bits greater 
*                           than the configured 'CPU_TS_TMR' timestamp timer data type word size.
*
*                       (2) Since the timer MUST NOT have less bits than the configured 'CPU_TS_TMR' 
*                           timestamp timer data type word size; 'CPU_CFG_TS_TMR_SIZE' MUST be 
*                           configured so that ALL bits in 'CPU_TS_TMR' data type are significant.
*
*                           In other words, if timer size is not a binary-multiple of 8-bit octets 
*                           (e.g. 20-bits or even 24-bits), then the next lower, binary-multiple 
*                           octet word size SHOULD be configured (e.g. to 16-bits).  However, the 
*                           minimum supported word size for CPU timestamp timers is 8-bits.
*
*                       See also 'cpu_cfg.h   CPU TIMESTAMP CONFIGURATION  Note #2'
*                              & 'cpu_core.h  CPU TIMESTAMP DATA TYPES     Note #1'.
*
*                   (b) Timer SHOULD be an 'up'  counter whose values increase with each time count.
*
*                   (c) When applicable, timer period SHOULD be less than the typical measured time 
*                       but MUST be less than the maximum measured time; otherwise, timer resolution 
*                       inadequate to measure desired times.
*
*                   See also 'CPU_TS_TmrRd()  Note #2'.
*********************************************************************************************************
*/

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void  CPU_TS_TmrInit (void)
{
    CPU_INT32U  fclk_freq;


    fclk_freq = BSP_CPU_ClkFreq();

    BSP_REG_DEM_CR     |= (CPU_INT32U)BSP_BIT_DEM_CR_TRCENA;    /* Enable Cortex-M4's DWT CYCCNT reg.                   */
    BSP_REG_DWT_CYCCNT  = (CPU_INT32U)0u;
    BSP_REG_DWT_CR     |= (CPU_INT32U)BSP_BIT_DWT_CR_CYCCNTENA;

    CPU_TS_TmrFreqSet((CPU_TS_TMR_FREQ)fclk_freq);
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                           CPU_TS_TmrRd()
*
* Description : Get current CPU timestamp timer count value.
*
* Argument(s) : none.
*
* Return(s)   : Timestamp timer count (see Notes #2a & #2b).
*
* Caller(s)   : CPU_TS_Init(),
*               CPU_TS_Get32(),
*               CPU_TS_Get64(),
*               CPU_IntDisMeasStart(),
*               CPU_IntDisMeasStop().
*
*               This function is an INTERNAL CPU module function & MUST be implemented by application/
*               BSP function(s) [see Note #1] but SHOULD NOT be called by application function(s).
*
* Note(s)     : (1) CPU_TS_TmrRd() is an application/BSP function that MUST be defined by the developer 
*                   if either of the following CPU features is enabled :
*
*                   (a) CPU timestamps
*                   (b) CPU interrupts disabled time measurements
*
*                   See 'cpu_cfg.h  CPU TIMESTAMP CONFIGURATION  Note #1'
*                     & 'cpu_cfg.h  CPU INTERRUPTS DISABLED TIME MEASUREMENT CONFIGURATION  Note #1a'.
*
*               (2) (a) Timer count values MUST be returned via word-size-configurable 'CPU_TS_TMR' 
*                       data type.
*
*                       (1) If timer has more bits, truncate timer values' higher-order bits greater 
*                           than the configured 'CPU_TS_TMR' timestamp timer data type word size.
*
*                       (2) Since the timer MUST NOT have less bits than the configured 'CPU_TS_TMR' 
*                           timestamp timer data type word size; 'CPU_CFG_TS_TMR_SIZE' MUST be 
*                           configured so that ALL bits in 'CPU_TS_TMR' data type are significant.
*
*                           In other words, if timer size is not a binary-multiple of 8-bit octets 
*                           (e.g. 20-bits or even 24-bits), then the next lower, binary-multiple 
*                           octet word size SHOULD be configured (e.g. to 16-bits).  However, the 
*                           minimum supported word size for CPU timestamp timers is 8-bits.
*
*                       See also 'cpu_cfg.h   CPU TIMESTAMP CONFIGURATION  Note #2'
*                              & 'cpu_core.h  CPU TIMESTAMP DATA TYPES     Note #1'.
*
*                   (b) Timer SHOULD be an 'up'  counter whose values increase with each time count.
*
*                       (1) If timer is a 'down' counter whose values decrease with each time count,
*                           then the returned timer value MUST be ones-complemented.
*
*                   (c) (1) When applicable, the amount of time measured by CPU timestamps is 
*                           calculated by either of the following equations :
*
*                           (A) Time measured  =  Number timer counts  *  Timer period
*
*                                   where
*
*                                       Number timer counts     Number of timer counts measured 
*                                       Timer period            Timer's period in some units of 
*                                                                   (fractional) seconds
*                                       Time measured           Amount of time measured, in same 
*                                                                   units of (fractional) seconds 
*                                                                   as the Timer period
*
*                                                  Number timer counts
*                           (B) Time measured  =  ---------------------
*                                                    Timer frequency
*
*                                   where
*
*                                       Number timer counts     Number of timer counts measured
*                                       Timer frequency         Timer's frequency in some units 
*                                                                   of counts per second
*                                       Time measured           Amount of time measured, in seconds
*
*                       (2) Timer period SHOULD be less than the typical measured time but MUST be less 
*                           than the maximum measured time; otherwise, timer resolution inadequate to 
*                           measure desired times.
*********************************************************************************************************
*/

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
CPU_TS_TMR  CPU_TS_TmrRd (void)
{
    CPU_TS_TMR  ts_tmr_cnts;

                                                                
    ts_tmr_cnts = (CPU_TS_TMR)BSP_REG_DWT_CYCCNT;

    return (ts_tmr_cnts);
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                         CPU_TSxx_to_uSec()
*
* Description : Convert a 32-/64-bit CPU timestamp from timer counts to microseconds.
*
* Argument(s) : ts_cnts   CPU timestamp (in timestamp timer counts [see Note #2aA]).
*
* Return(s)   : Converted CPU timestamp (in microseconds           [see Note #2aD]).
*
* Caller(s)   : Application.
*
*               This function is an (optional) CPU module application programming interface (API) 
*               function which MAY be implemented by application/BSP function(s) [see Note #1] & 
*               MAY be called by application function(s).
*
* Note(s)     : (1) CPU_TS32_to_uSec()/CPU_TS64_to_uSec() are application/BSP functions that MAY be 
*                   optionally defined by the developer when either of the following CPU features is 
*                   enabled :
*
*                   (a) CPU timestamps
*                   (b) CPU interrupts disabled time measurements
*
*                   See 'cpu_cfg.h  CPU TIMESTAMP CONFIGURATION  Note #1'
*                     & 'cpu_cfg.h  CPU INTERRUPTS DISABLED TIME MEASUREMENT CONFIGURATION  Note #1a'.
*
*               (2) (a) The amount of time measured by CPU timestamps is calculated by either of 
*                       the following equations :
*
*                                                                        10^6 microseconds
*                       (1) Time measured  =   Number timer counts   *  -------------------  *  Timer period
*                                                                            1 second
*
*                                              Number timer counts       10^6 microseconds
*                       (2) Time measured  =  ---------------------  *  -------------------
*                                                Timer frequency             1 second
*
*                               where
*
*                                   (A) Number timer counts     Number of timer counts measured
*                                   (B) Timer frequency         Timer's frequency in some units 
*                                                                   of counts per second
*                                   (C) Timer period            Timer's period in some units of 
*                                                                   (fractional)  seconds
*                                   (D) Time measured           Amount of time measured, 
*                                                                   in microseconds
*
*                   (b) Timer period SHOULD be less than the typical measured time but MUST be less 
*                       than the maximum measured time; otherwise, timer resolution inadequate to 
*                       measure desired times.
*
*                   (c) Specific implementations may convert any number of CPU_TS32 or CPU_TS64 bits 
*                       -- up to 32 or 64, respectively -- into microseconds.
*********************************************************************************************************
*/

#if (CPU_CFG_TS_32_EN == DEF_ENABLED)
CPU_INT64U  CPU_TS32_to_uSec (CPU_TS32  ts_cnts)
{
    CPU_INT64U  ts_us;
    CPU_INT64U  fclk_freq;

    
    fclk_freq = BSP_CPU_ClkFreq();    
    ts_us     = ts_cnts / (fclk_freq / DEF_TIME_NBR_uS_PER_SEC);

    return (ts_us);
}
#endif


#if (CPU_CFG_TS_64_EN == DEF_ENABLED)
CPU_INT64U  CPU_TS64_to_uSec (CPU_TS64  ts_cnts)
{
    CPU_INT64U  ts_us;
    CPU_INT64U  fclk_freq;
    

    fclk_freq = BSP_CPU_ClkFreq();    
    ts_us     = ts_cnts / (fclk_freq / DEF_TIME_NBR_uS_PER_SEC);

    return (ts_us);
}
#endif
