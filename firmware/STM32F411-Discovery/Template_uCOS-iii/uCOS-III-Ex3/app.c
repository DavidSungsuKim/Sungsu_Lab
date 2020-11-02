/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2013; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/
 
/*
*********************************************************************************************************
*                                       uC/OS-III Application Code
*                                                Template
*
* The template application code provided here can be used as a foundation for your own uC/OS-III-based
* projects.  The code includes a declaration of main() that creates a single task named AppTaskStart().
*
* Filename      : app.c
* Version       : V1.00
*********************************************************************************************************
*/
#define ABS(x)                    (x < 0) ? (-x) : x
#define L3G_Sensitivity_250dps    (float)114.285f        /*!< gyroscope sensitivity with 250 dps full scale [LSB/dps]  */
#define L3G_Sensitivity_500dps    (float)57.1429f        /*!< gyroscope sensitivity with 500 dps full scale [LSB/dps]  */
#define L3G_Sensitivity_2000dps   (float)14.285f         /*!< gyroscope sensitivity with 2000 dps full scale [LSB/dps] */

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <includes.h>

/*
*********************************************************************************************************
*                                           LOCAL VARIABLES
*********************************************************************************************************
*/
static  OS_SEM          AppSem; 

OS_SEM  Sem1, Sem2, Sem3, Sem4;

OS_Q	 Gyro_Q;
uint8_t  Q_Buf[6];

static  OS_TCB          AppTaskStart_TCB;
static  OS_TCB          AppTaskMotionSensorTCB; 
static  OS_TCB          AppTaskLogTCB;

static  CPU_STK         AppTaskStart_Stk[APP_TASK_START_STK_SIZE];
static  CPU_STK         AppTaskMotionSensorStk[APP_TASK_MOTION_SENSOR_STK_SIZE];
static  CPU_STK         AppTaskLogStk[APP_TASK_LOG_STK_SIZE];

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void AppTaskCreate      (void);
static  void AppEventCreate     (void);

static  void AppTaskStart       (void *p_arg);
static void AppTaskLog(void *p_arg);
static  void AppTaskMotionSensor(void *p_arg);

static void MEMS_Test(void);
static void Demo_GyroConfig(void);
static void Demo_GyroReadAngRate (float* pfData);

/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/

int  main (void)
{
    OS_ERR     err;


    BSP_IntDisAll();                                            /* Disable all interrupts.                              */

    OSInit(&err);                                               /* Init uC/OS-III.                                      */
    
    OSTaskCreate((OS_TCB     *)&AppTaskStart_TCB,               /* Create the start task                                */
                 (CPU_CHAR   *)"Start",
                 (OS_TASK_PTR )AppTaskStart,
                 (void       *)0,
                 (OS_PRIO     )APP_TASK_START_PRIO,
                 (CPU_STK    *)&AppTaskStart_Stk[0],
                 (CPU_STK_SIZE)APP_TASK_START_STK_SIZE / 10,
                 (CPU_STK_SIZE)APP_TASK_START_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void       *)0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR ),
                 (OS_ERR     *)&err);

    OSStart(&err);                                              /* Start multitasking (i.e. give control to uC/OS-III). */

  /* OSStart() never returns, serious error had occured if
   * code execution reached this point                    */
  while(1U) ;
}


/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart_()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
    CPU_INT32U   freq;
    CPU_INT32U   cnts;
    OS_ERR       err;

   (void)p_arg;

    OSSemCreate(&AppSem, "Test Sem", 0, &err);
   
    BSP_Init();                                                       /* Initialize BSP functions                          */
    CPU_Init();                                                       /* Initialize the uC/CPU services                    */

    freq = BSP_CPU_ClkFreq();                                         /* Determine SysTick reference freq.                 */                                                                        
    cnts = freq / (CPU_INT32U)OSCfg_TickRate_Hz;                      /* Determine nbr SysTick increments                  */
    OS_CPU_SysTickInit(cnts);                                         /* Init uC/OS periodic time src (SysTick).           */

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                                     /* Compute CPU capacity with no task running         */
#endif

#ifdef  CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif

    AppEventCreate();                                                 /* Create Application Kernel objects                 */

    AppTaskCreate();                                                  /* Create application tasks                          */
    
    BSP_LED_Off(0);    

    OSTaskDel((OS_TCB *)0, &err);
}


/*
*********************************************************************************************************
*                                      CREATE APPLICATION TASKS
*
* Description:  This function creates the application tasks.
*
* Arguments  :  none
*
* Returns    :  none
*********************************************************************************************************
*/

static  void  AppTaskCreate (void)
{
    OS_ERR  err;
     
    OSTaskCreate((OS_TCB     *)&AppTaskMotionSensorTCB, 
                 (CPU_CHAR   *)"App Motion Sensor Start",
                 (OS_TASK_PTR )AppTaskMotionSensor, 
                 (void       *)0,
                 (OS_PRIO     )APP_TASK_MOTION_SENSOR_PRIO,
                 (CPU_STK    *)&AppTaskMotionSensorStk[0],
                 (CPU_STK_SIZE)APP_TASK_MOTION_SENSOR_STK_SIZE / 10,
                 (CPU_STK_SIZE)APP_TASK_MOTION_SENSOR_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void       *)0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP ),
                 (OS_ERR     *)&err);
    
    OSTaskCreate((OS_TCB     *)&AppTaskLogTCB, 
                 (CPU_CHAR   *)"App Log",
                 (OS_TASK_PTR )AppTaskLog, 
                 (void       *)0,
                 (OS_PRIO     )APP_TASK_LOG_PRIO,
                 (CPU_STK    *)&AppTaskLogStk[0],
                 (CPU_STK_SIZE)APP_TASK_LOG_STK_SIZE / 10,
                 (CPU_STK_SIZE)APP_TASK_LOG_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void       *)0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP ),
                 (OS_ERR     *)&err);
    
}

/*
*********************************************************************************************************
*                                          AppEventCreate()
*
* Description : Create application kernel objects tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/

static void AppEventCreate(void)
{

}

static void AppTaskLog(void *p_arg)
{
  OS_ERR		err;  
  OS_MSG_SIZE	        RxQ_Size;
  CPU_TS		ts;
  uint8_t		*pMsg;
  
  (void)p_arg;

  //OSQ Create
  OSQCreate(&Gyro_Q, "Gyro Q", 6, &err);
  
  while( DEF_TRUE )
  {
    //OSQ Pend
    pMsg = (uint8_t *)OSQPend(&Gyro_Q, 0, OS_OPT_PEND_BLOCKING, &RxQ_Size, &ts, &err);
	
    //Printf
    printf("%d\t", RxQ_Size);
    printf("%d\t", pMsg[0]);
    printf("%d\t", pMsg[1]);
    printf("%d\n", pMsg[2]);
  }
}

static void AppTaskMotionSensor(void *p_arg)
{
  OS_ERR		err;  
  
  (void)p_arg;

  Demo_GyroConfig();
  
  while( DEF_TRUE )
  {
    MEMS_Test();

    OSTimeDlyHMSM(0, 0, 0, 200,                                   /* Delay task for 200 ms                             */
                     OS_OPT_TIME_HMSM_STRICT,
                     &err);
  }
}

static void MEMS_Test(void)
{
  float    Buffer[6];
  uint8_t  Xval, Yval, Zval = 0x00;
  OS_ERR   err;  

  /* Demo Gyroscope */
  //Demo_GyroConfig();
  
  
  /* Read Gyro Angular data */
  Demo_GyroReadAngRate(Buffer);
 
  /* Update autoreload and capture compare registers value*/
  Xval = ABS((int8_t)(Buffer[0]));
  Yval = ABS((int8_t)(Buffer[1])); 
  Zval = ABS((int8_t)(Buffer[2]));
 
  Q_Buf[0] = Xval;
  Q_Buf[1] = Yval;
  Q_Buf[2] = Zval;
  
  //OSQ Post
  OSQPost(&Gyro_Q, Q_Buf, sizeof(Q_Buf), OS_OPT_POST_FIFO + OS_OPT_POST_ALL + OS_OPT_POST_NO_SCHED, &err);
  
  if ( Xval>Yval)
  {
    if ((int8_t)Buffer[0] > 15.0f)
    {       
      STM_EVAL_LEDOn(LED4);
      STM_EVAL_LEDOff(LED3);
      STM_EVAL_LEDOff(LED5);
      STM_EVAL_LEDOff(LED6);
    }
    if ((int8_t)Buffer[0] < -15.0f)
    {
      STM_EVAL_LEDOn(LED5);
      STM_EVAL_LEDOff(LED3);
      STM_EVAL_LEDOff(LED4);
      STM_EVAL_LEDOff(LED6);
    }
  }
  else
  {
    if ((int8_t)Buffer[1] < -15.0f)
    {
      STM_EVAL_LEDOn(LED3);
      STM_EVAL_LEDOff(LED4);
      STM_EVAL_LEDOff(LED5);
      STM_EVAL_LEDOff(LED6);
    }
    if ((int8_t)Buffer[1] > 15.0f)
    {
      STM_EVAL_LEDOn(LED6);
      STM_EVAL_LEDOff(LED3);
      STM_EVAL_LEDOff(LED4);
      STM_EVAL_LEDOff(LED5);
    } 
  }
}

/**
* @brief  Configure the Mems to gyroscope application.
* @param  None
* @retval None
*/
static void Demo_GyroConfig(void)
{
  L3GD20_InitTypeDef L3GD20_InitStructure;
  L3GD20_FilterConfigTypeDef L3GD20_FilterStructure;
  /* Configure Mems L3GD20 */
  L3GD20_InitStructure.Power_Mode = L3GD20_MODE_ACTIVE;
  L3GD20_InitStructure.Output_DataRate = L3GD20_OUTPUT_DATARATE_1;
  L3GD20_InitStructure.Axes_Enable = L3GD20_AXES_ENABLE;
  L3GD20_InitStructure.Band_Width = L3GD20_BANDWIDTH_4;
  L3GD20_InitStructure.BlockData_Update = L3GD20_BlockDataUpdate_Continous;
  L3GD20_InitStructure.Endianness = L3GD20_BLE_LSB;
  L3GD20_InitStructure.Full_Scale = L3GD20_FULLSCALE_500; 
  L3GD20_Init(&L3GD20_InitStructure);
  
  L3GD20_FilterStructure.HighPassFilter_Mode_Selection =L3GD20_HPM_NORMAL_MODE_RES;
  L3GD20_FilterStructure.HighPassFilter_CutOff_Frequency = L3GD20_HPFCF_0;
  L3GD20_FilterConfig(&L3GD20_FilterStructure) ;
  
  L3GD20_FilterCmd(L3GD20_HIGHPASSFILTER_ENABLE);
}

/**
* @brief  Calculate the angular Data rate Gyroscope.
* @param  pfData : Data out pointer
* @retval None
*/

void Demo_GyroReadAngRate (float* pfData)
{
  uint8_t tmpbuffer[6] ={0};
  int16_t RawData[3] = {0};
  uint8_t tmpreg = 0;
  float sensitivity = 0;
  uint32_t wCounter =0;
  
  L3GD20_Read(&tmpreg,L3GD20_CTRL_REG4_ADDR,1);
  
  L3GD20_Read(tmpbuffer,L3GD20_OUT_X_L_ADDR,6);

  /* check in the control register 4 the data alignment (Big Endian or Little Endian)*/
  if(!(tmpreg & 0x40))
  {
    for(wCounter=0; wCounter<3; wCounter++)
    {
      RawData[wCounter]=(int16_t)(((uint16_t)tmpbuffer[2*wCounter+1] << 8) + tmpbuffer[2*wCounter]);
    }
  }
  else
  {
    for(wCounter=0; wCounter<3; wCounter++)
    {
      RawData[wCounter]=(int16_t)(((uint16_t)tmpbuffer[2*wCounter] << 8) + tmpbuffer[2*wCounter+1]);
    }
  }
  
  /* Switch the sensitivity value set in the CRTL4 */
  switch(tmpreg & 0x30)
  {
  case 0x00:
    sensitivity=L3G_Sensitivity_250dps;
    break;
    
  case 0x10:
    sensitivity=L3G_Sensitivity_500dps;
    break;
    
  case 0x20:
    sensitivity=L3G_Sensitivity_2000dps;
    break;
  }
  /* divide by sensitivity */
  for(wCounter=0; wCounter<3; wCounter++)
  {
    pfData[wCounter]=(float)RawData[wCounter]/sensitivity;
  }
}

/**
* @brief  Basic management of the timeout situation.
* @param  None.
* @retval None.
*/
uint32_t L3GD20_TIMEOUT_UserCallback(void)
{
  return 0;
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