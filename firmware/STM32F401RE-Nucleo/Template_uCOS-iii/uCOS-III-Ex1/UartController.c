
//***********************************************************
//      STM32F401-Nucleo Personal Application
// File name    : UartController.c
// Designer     : Sungsu Kim
// Email        : sheld2@naver.com
// All rights reserved
// 
//***********************************************************

#include <includes.h>
#include <stdarg.h>
#include "Common.h"
#include "UartController.h"
#include "RpcController.h"

// #define _CHECK_UART_PACKET_RECV_ 1

//***********************************************************
// Global Variables
//***********************************************************
static  OS_TCB          TCB_TaskUartRecv;
static  CPU_STK         Stk_TaskUartRecv[APP_TASK_UART_RECEIVE_STK_SIZE];
        OS_Q            g_UartQ;
   
        StUartCtrl      g_UartCtrl;        
        uint8_t         g_UartQData;

//***********************************************************
// Local Function Declarations
//***********************************************************

static  void    TaskUartRecv    (void *p_arg);
        void    sInitBuffer     ();
        uint8_t sPutIntoBuffer  ( uint8_t data );

//***********************************************************
// Function Definitions
//***********************************************************
void UartInitializeSW ()
{
  OS_ERR       err;
  
  // Initialize StUartCtrl
  sInitBuffer();
  
  // Initialize the task
  OSQCreate(&g_UartQ,
            "UartQueue",
            10,
            &err);

  if ( err != OS_ERR_NONE )
    return;
      
  OSTaskCreate((OS_TCB     *)&TCB_TaskUartRecv,                 
               (CPU_CHAR   *)"TaskUartRecv",
               (OS_TASK_PTR )TaskUartRecv,
               (void       *)0,
               (OS_PRIO     )APP_TASK_UART_RECV_PRIO,
               (CPU_STK    *)&Stk_TaskUartRecv[0],
               (CPU_STK_SIZE)(APP_TASK_UART_RECEIVE_STK_SIZE / 10),
               (CPU_STK_SIZE)APP_TASK_UART_RECEIVE_STK_SIZE,
               (OS_MSG_QTY  )0,
               (OS_TICK     )0,
               (void       *)0,
               (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               (OS_ERR     *)&err);
  
  if ( err == OS_ERR_NONE )                 
    UartSendMsg("Uart:Task initialized...\r\n");     
}

void UartInitializeHW ()
{
  
}

ErrCode UartSendPackets (const char* pData, unsigned int aSize)
{
  ErrCode       eRet            = NO_ERR;
  uint32_t      ulTimeout       = 10;
  
  BSP_Send_Uart(pData, aSize, ulTimeout);

  return eRet;
}

ErrCode UartSendMsg (const char* cString)
{
  ErrCode       eRet    = NO_ERR;
 
  BSP_SendMessage(cString);
 
  return eRet;  
}

ErrCode UartSendMsg2 (const char* cString, ...)
{
  ErrCode       eRet    = NO_ERR;

  char buf[256] = {0,};
  va_list ap;
  
  va_start(ap, cString);
  vsprintf(buf + strlen(buf), cString, ap);
  va_end(ap);
	
  puts(buf);
  
  BSP_SendMessage((const char*)buf);
 
  return eRet;    
}

ErrCode UartSendData (uint8_t  data, uint32_t option)
{
  ErrCode       eRet    = NO_ERR;
  uint8_t       dataOri = data;  
 
  data = _NUM_ASCII(dataOri / 100);
  UartSendPackets((const char*)&data, sizeof(uint8_t));
  
  dataOri = dataOri % 100; 
  
  data = _NUM_ASCII(dataOri / 10);
  UartSendPackets((const char*)&data, sizeof(uint8_t)); 
  data = _NUM_ASCII(dataOri % 10);
  UartSendPackets((const char*)&data, sizeof(uint8_t)); 
  
  if ( option & OPT_ENDLINE )
      UartSendMsg("\r\n"); 
  
  return eRet;  
}

ErrCode UartNotifyRecv (uint8_t data)
{
  ErrCode eRet = NO_ERR;
 
  OS_ERR        err;  

  g_UartQData = data;

  OSQPost(&g_UartQ,
          (void*)&g_UartQData,
          sizeof(void*),
          OS_OPT_POST_FIFO,
          &err);
  
  return eRet;
}

static void TaskUartRecv(void *p_arg)
{
  uint8_t*      pData;
  OS_ERR        err;
  CPU_TS        ts;
  OS_MSG_SIZE   msgSize;
  
  (void)p_arg;

  while(1) 
  {    
    pData = (uint8_t*)OSQPend( &g_UartQ,
                              0,
                              OS_OPT_PEND_BLOCKING,
                              &msgSize,
                              &ts,
                              &err);
            
    if ( err == OS_ERR_NONE )
    {
#ifdef _CHECK_UART_PACKET_RECV_   
      UartSendMsg("Uart:msg received=");
      UartSendPackets(pData, 1);
      UartSendMsg("\r\n");
#endif
      
      if ( sPutIntoBuffer( *pData ) == 0 )
        continue;

      g_RpcMsg.size = _NUM_ASCII(1);   // test..
      memcpy( g_RpcMsg.msg, g_UartCtrl.buffer, RPC_MSG_SIZE);
      
      sInitBuffer(); 
      RpcNotifyMsg( &g_RpcMsg );
    }
    else
    {
      UartSendMsg("Uart:msg error\r\n");
    }
  }
}

void sInitBuffer()
{
  g_UartCtrl.index = 0;
  memset(g_UartCtrl.buffer, 0, UART_RECV_BUFFER_SIZE);
}

uint8_t sPutIntoBuffer( uint8_t data )
{
  if ( g_UartCtrl.index >= RPC_MSG_SIZE )
    return 1;
    
  g_UartCtrl.buffer[g_UartCtrl.index] = data;
  
  g_UartCtrl.index++;
  
  if ( g_UartCtrl.index >= RPC_MSG_SIZE )
  {    
    return 1;
  }

  return 0;
}