
//***********************************************************
//      STM32F401-Nucleo Personal Application
// File name    : UserController.c
// Designer     : Sungsu Kim
// Email        : sheld2@naver.com
// All rights reserved
// 
//***********************************************************

#include <includes.h>
#include "UserController.h"
#include "UartController.h"

//***********************************************************
// Global Variables
//***********************************************************
static  OS_TCB          TCB_UserTask;
static  CPU_STK         Stk_UserTask[APP_USER_TASK1_STK_SIZE];

//***********************************************************
// Local Function Declarations
//***********************************************************

static  void UserTask  (void *p_arg);

//***********************************************************
// Function Definitions
//***********************************************************
void UserInitializeSW ()
{
    OS_ERR       err;
      
    OSTaskCreate((OS_TCB     *)&TCB_UserTask,                 
                 (CPU_CHAR   *)"UserTask",
                 (OS_TASK_PTR )UserTask,
                 (void       *)0,
                 (OS_PRIO     )APP_USER_TASK_PRIO,
                 (CPU_STK    *)&Stk_UserTask[0],
                 (CPU_STK_SIZE)(APP_USER_TASK1_STK_SIZE / 10),
                 (CPU_STK_SIZE)APP_USER_TASK1_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void       *)0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
    
  if ( err == OS_ERR_NONE )
    UartSendMsg("User:Task initialized...\r\n");
}

void UserInitializeHW ()
{
  
}

static void UserTask(void *p_arg)
{
  OS_ERR  err;
  
  (void)p_arg;
  
  while(1) 
  {  
    UartSendMsg("UserTask..T=3000[ms]\r\n"); 
    OSTimeDly(3000, 0, &err);  
  }
}
