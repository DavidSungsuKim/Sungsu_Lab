
#include <includes.h>
#include <stdarg.h>
#include "Common.h"
#include "SPITest.h"

//***********************************************************
// Global Variables
//***********************************************************
static  OS_TCB          TCB_TaskSPITest;
static  CPU_STK         Stk_TaskSPITest[APP_TASK_SPI_TEST_STK_SIZE];
        OS_Q            g_SPITestQ;

//***********************************************************
// Local Function Declarations
//***********************************************************

static  void    TaskSPITest    (void *p_arg);

//***********************************************************
// Function Definitions
//***********************************************************
        
void SPITestInitializeSW ()
{
  OS_ERR       err;
  
  // Initialize the task
  OSQCreate(&g_SPITestQ,
            "SPITest",
            10,
            &err);

  if ( err != OS_ERR_NONE )
    return;
      
  OSTaskCreate((OS_TCB     *)&TCB_TaskSPITest,                 
               (CPU_CHAR   *)"TaskSPITest",
               (OS_TASK_PTR )TaskSPITest,
               (void       *)0,
               (OS_PRIO     )APP_TASK_SPI_TEST_PRIO,
               (CPU_STK    *)&Stk_TaskSPITest[0],
               (CPU_STK_SIZE)(APP_TASK_SPI_TEST_STK_SIZE / 10),
               (CPU_STK_SIZE)APP_TASK_SPI_TEST_STK_SIZE,
               (OS_MSG_QTY  )0,
               (OS_TICK     )0,
               (void       *)0,
               (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               (OS_ERR     *)&err);
  
  if ( err == OS_ERR_NONE )                 
    ; //UartSendMsg("Uart:Task initialized...\r\n");   
}

void SPITestInitializeHW ()
{
  
}

void SPITestDoTest ()
{
  
}

static void TaskSPITest(void *p_arg)
{
  uint8_t*      pData;
  OS_ERR        err;
  CPU_TS        ts;
  OS_MSG_SIZE   msgSize;
  
  (void)p_arg;

  uint8_t byte = 0x00;
  
  while(1) 
  {    
    BSP_MY_SPI_WriteByte(byte++);
    OSTimeDly(3000, 0, &err);  
  
    
  }
}