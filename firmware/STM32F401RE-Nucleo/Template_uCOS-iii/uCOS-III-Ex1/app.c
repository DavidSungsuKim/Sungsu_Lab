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

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <includes.h>

#include "Common.h"
#include "UartController.h"
#include "UserController.h"
#include "RpcController.h"

/*
*********************************************************************************************************
*                                           LOCAL VARIABLES
*********************************************************************************************************
*/
static  OS_SEM          AppSem; 

OS_SEM  Sem1, Sem2, Sem3, Sem4;

static  OS_TCB          AppTaskStart_TCB;
static  CPU_STK         AppTaskStart_Stk[APP_TASK_START_STK_SIZE];


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void AppTaskStart       (void *p_arg);


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

void InitUserSW         ();
void InitApp            ();

int  main (void)
{
    BSP_IntDisAll();                                            /* Disable all interrupts.                              */

    InitApp();  
}

void InitApp()
{
    OS_ERR     err;
  
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
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err); 
    
    OSStart(&err);                                              /* Start multitasking (i.e. give control to uC/OS-III). */
    
  /* OSStart() never returns, serious error had occured if
   * code execution reached this point                    */
  while(1U);
}

void InitUserSW()
{    
    UartSendMsg("*****************************************************\r\n");
    UartSendMsg("* SungsuApp-uCOS-III @ NUCLEO-F401RE \r\n");
    UartSendMsg("* Version\t:");
    UartSendMsg(_VERSION_PROG);  UartSendMsg("\r\n");
    UartSendMsg("* Date\t:");   
    UartSendMsg(_DATE_PROG);     UartSendMsg("\r\n");
    UartSendMsg("*****************************************************\r\n");
    UartSendMsg("\r\n");
    
    UserInitializeSW();
    UartInitializeSW();
    RpcInitializeSW();
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

    // Initialization of User Tasks
    InitUserSW();   
    
/*    
    while (TRUE)
    {                                                         
        OSTimeDlyHMSM(0, 0, 0, 50, OS_OPT_TIME_HMSM_STRICT, &err);
        
        BSP_SendMessage("DefaultTask..T=500[ms]\r\n");
        OSTimeDly(500, 0, &err);
    }
    */
}
