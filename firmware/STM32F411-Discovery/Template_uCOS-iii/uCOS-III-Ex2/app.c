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

#define LOOP_DELAY_CNT          100
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

/*
*********************************************************************************************************
*                                           LOCAL VARIABLES
*********************************************************************************************************
*/
static  OS_SEM          AppSem; 

static  OS_TCB          AppTaskStart_TCB;
static  CPU_STK         AppTaskStart_Stk[APP_TASK_START_STK_SIZE];


OS_SEM  Sem1, Sem2, Sem3, Sem4;

static  OS_TCB        AppTask1TCB;
static  OS_TCB        AppTask2TCB;
static  OS_TCB        AppTask3TCB;
static  OS_TCB        AppTask4TCB;
static  CPU_STK_SIZE  AppTask1Stk[APP_CFG_TASK1_STK_SIZE];
static  CPU_STK_SIZE  AppTask2Stk[APP_CFG_TASK2_STK_SIZE];
static  CPU_STK_SIZE  AppTask3Stk[APP_CFG_TASK3_STK_SIZE];
static  CPU_STK_SIZE  AppTask4Stk[APP_CFG_TASK4_STK_SIZE];

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void AppTaskCreate      (void);
static  void App_ObjCreate      (void);

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
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
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

    App_ObjCreate();                                                 /* Create Application Kernel objects                 */

    AppTaskCreate();                                                  /* Create application tasks                          */
    
#if 0	    
    while (DEF_TRUE) {                                                /* Task body, always written as an infinite loop.    */
        BSP_LED_Toggle(0);
	
        OSTimeDlyHMSM(0, 0, 0, 200,                                   /* Delay task for 200 ms                             */
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);
    }
#endif   
    
    OSTaskDel((OS_TCB *)0, &err);
    
    while(1U) ;
}

/*
*********************************************************************************************************
*                                      App_ObjCreate()
*
* Description:  Creates the application kernel objects.
*
* Argument(s) :  none.
*
* Return(s)   :  none.
*
* Caller(s)   :  App_TaskStart().
*
* Note(s)     :  none.
*********************************************************************************************************
*/
static  void  App_ObjCreate (void)
{
  OS_ERR err;
  
  OSSemCreate(&Sem1, "Sem1", (OS_SEM_CTR)0, &err);
  OSSemCreate(&Sem2, "Sem2", (OS_SEM_CTR)0, &err);
  OSSemCreate(&Sem3, "Sem3", (OS_SEM_CTR)0, &err);
  OSSemCreate(&Sem4, "Sem4", (OS_SEM_CTR)0, &err);
}


static  void  AppTask1 (void *p_arg)
{
    CPU_INT08U cnt;
    OS_ERR err;
    CPU_TS ts;
   (void)p_arg;

    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
        OSSemPend(&Sem1, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
        printf("Task1 started\n");
        cnt = LOOP_DELAY_CNT;
        while(cnt--)
        {
            BSP_LED_Toggle(1);
            OSTimeDlyHMSM(0,0,0,30,OS_OPT_TIME_HMSM_STRICT, &err);
        }
        printf("Task1 finished\n");
    }
}

static  void  AppTask2 (void *p_arg)
{
    CPU_INT08U cnt;
    OS_ERR err;
    CPU_TS ts;
   (void)p_arg;

    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
        OSSemPend(&Sem2, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
        printf("Task2 started\n");
        cnt = LOOP_DELAY_CNT;
        while(cnt--)
        {
            BSP_LED_Toggle(2);
            OSTimeDlyHMSM(0,0,0,30,OS_OPT_TIME_HMSM_STRICT, &err);
        }
        printf("Task2 finished\n");
    }
}

static  void  AppTask3 (void *p_arg)
{
    CPU_INT08U cnt;
    OS_ERR err;
    CPU_TS ts;
   (void)p_arg;

    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
        OSSemPend(&Sem3, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
        printf("Task3 started\n");
        cnt = LOOP_DELAY_CNT;
        while(cnt--)
        {
            BSP_LED_Toggle(3);
            OSTimeDlyHMSM(0,0,0,30,OS_OPT_TIME_HMSM_STRICT, &err);
        }
        printf("Task3 finished\n");
    }
}

static  void  AppTask4 (void *p_arg)
{
    CPU_INT08U cnt;
    OS_ERR err;
    CPU_TS ts;
   (void)p_arg;

    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
        OSSemPend(&Sem4, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
        printf("Task4 started\n");
        cnt = LOOP_DELAY_CNT;
        while(cnt--)
        {
            BSP_LED_Toggle(4);
            OSTimeDlyHMSM(0,0,0,30,OS_OPT_TIME_HMSM_STRICT, &err);
        }
        printf("Task4 finished\n");
    }
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
    CPU_ERR  err;
 
    OSTaskCreate((OS_TCB     *)&AppTask1TCB,               /* Create the start task                                    */
                 (CPU_CHAR   *)"App Task1",
                 (OS_TASK_PTR ) AppTask1,
                 (void       *) 0,
                 (OS_PRIO     ) APP_CFG_TASK1_PRIO,
                 (CPU_STK    *)&AppTask1Stk[0],
                 (CPU_STK     )(APP_CFG_TASK1_STK_SIZE / 10u),
                 (CPU_STK_SIZE) APP_CFG_TASK1_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    OSTaskCreate((OS_TCB     *)&AppTask2TCB,               /* Create the start task                                    */
                 (CPU_CHAR   *)"App Task2",
                 (OS_TASK_PTR ) AppTask2,
                 (void       *) 0,
                 (OS_PRIO     ) APP_CFG_TASK2_PRIO,
                 (CPU_STK    *)&AppTask2Stk[0],
                 (CPU_STK     )(APP_CFG_TASK2_STK_SIZE / 10u),
                 (CPU_STK_SIZE) APP_CFG_TASK2_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    OSTaskCreate((OS_TCB     *)&AppTask3TCB,               /* Create the start task                                    */
                 (CPU_CHAR   *)"App Task3",
                 (OS_TASK_PTR ) AppTask3,
                 (void       *) 0,
                 (OS_PRIO     ) APP_CFG_TASK3_PRIO,
                 (CPU_STK    *)&AppTask3Stk[0],
                 (CPU_STK     )(APP_CFG_TASK3_STK_SIZE / 10u),
                 (CPU_STK_SIZE) APP_CFG_TASK3_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    OSTaskCreate((OS_TCB     *)&AppTask4TCB,               /* Create the start task                                    */
                 (CPU_CHAR   *)"App Task4",
                 (OS_TASK_PTR ) AppTask4,
                 (void       *) 0,
                 (OS_PRIO     ) APP_CFG_TASK4_PRIO,
                 (CPU_STK    *)&AppTask4Stk[0],
                 (CPU_STK     )(APP_CFG_TASK4_STK_SIZE / 10u),
                 (CPU_STK_SIZE) APP_CFG_TASK4_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
    
}


