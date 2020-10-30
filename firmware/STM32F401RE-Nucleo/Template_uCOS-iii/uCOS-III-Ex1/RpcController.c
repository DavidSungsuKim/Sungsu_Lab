
//***********************************************************
//      STM32F401-Nucleo Personal Application
// File name    : RpcController.h
// Designer     : Sungsu Kim
// Email        : sheld2@naver.com
// All rights reserved
// 
//***********************************************************

#include <includes.h>
#include "RpcController.h"
#include "UartController.h"
#include "RoverBody.h"

//#define _CHECK_RPC_RECV_    1

//***********************************************************
// Global Variables
//***********************************************************
static  OS_TCB          TCB_RpcTask;
static  CPU_STK         Stk_RpcTask     [APP_RPC_TASK_STK_SIZE];
        OS_Q            g_RpcQ;
        
        StRpcCtrl       g_Rpc;
        StRpcMsg        g_RpcMsg;

//***********************************************************
// Local Function Declarations
//***********************************************************
static  void            RpcTask                 (void *p_arg);

        ErrCode         ParsingCmd              (StRpcMsg* pMsg);
        ErrCode         CmdControlLED2          (StRpcMsg* pMsg);
        ErrCode         CmdControlLED3          (StRpcMsg* pMsg);
        ErrCode         CmdControlPWM           (StRpcMsg* pMsg);
        ErrCode         CmdChangeDirection      (StRpcMsg* pMsg);
        ErrCode         CmdControlSpeed         (StRpcMsg* pMsg);

//***********************************************************
// Function Definitions
//***********************************************************
void RpcInitializeSW ()
{
  OS_ERR       err;
  
  // Initialize StRpcMsg
  g_RpcMsg.size = 0;
  memset(g_RpcMsg.msg, 0, RPC_MSG_SIZE);
  
  // Initialize the task
  OSQCreate(&g_RpcQ,
            "RpcQueue",
            10,
            &err);
  
  if ( err != OS_ERR_NONE )
    return;

  OSTaskCreate((OS_TCB     *)&TCB_RpcTask,                 
               (CPU_CHAR   *)"RpcTask",
               (OS_TASK_PTR )RpcTask,
               (void       *)0,
               (OS_PRIO     )APP_RPC_TASK_PRIO,
               (CPU_STK    *)&Stk_RpcTask[0],
               (CPU_STK_SIZE)(APP_RPC_TASK_STK_SIZE / 10),
               (CPU_STK_SIZE)APP_RPC_TASK_STK_SIZE,
               (OS_MSG_QTY  )0,
               (OS_TICK     )0,
               (void       *)0,
               (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               (OS_ERR     *)&err);
  
  if ( err == OS_ERR_NONE )
    UartSendMsg("RpcTask initialized...\r\n");    
}

void RpcInitializeHW ()
{
  
}

void RpcNotifyMsg( StRpcMsg* pMsg )
{
  OS_ERR        err;  
  
  OSQPost(&g_RpcQ,
          (void*)pMsg,
          sizeof(void*),
          OS_OPT_POST_FIFO,
          &err);
  
  if ( err == OS_ERR_NONE )
    UartSendMsg("Rpc:msg posted\r\n");
  else
  {
    UartSendMsg("Rpc:msg error\r\n");
  }
}

static void RpcTask(void *p_arg)
{
  OS_ERR       err;
  
  CPU_TS        ts;
  OS_MSG_SIZE   msgSize;
  
  (void)p_arg;
  
  StRpcMsg* pMsg;
    
  while(1) 
  {  
    pMsg = (StRpcMsg*)OSQPend(&g_RpcQ,
                              0,
                              OS_OPT_PEND_BLOCKING,
                              &msgSize,
                              &ts,
                              &err);
    
#ifdef _CHECK_RPC_RECV_
    UartSendMsg("Rpc:Received, size=");
    UartSendPackets(&(pMsg->size), sizeof(pMsg->size));
    UartSendMsg(" ,data=");
    UartSendPackets(pMsg->msg, RPC_MSG_SIZE);
    UartSendMsg("\r\n");
#endif
    
#ifdef _PRINTOUT_LOG
    static int rpcRecv = 0;
    printf("Rpc:Received, rpcRecv=%d\r\n", rpcRecv++);
#endif
    
    ParsingCmd( pMsg );
  }
}

ErrCode ParsingCmd(StRpcMsg* pMsg)
{
  uint8_t       starter;
  uint8_t       cmd;
  
  starter       = pMsg->msg[MSG_IDX_START];
  
  if ( starter != MSG_STARTER )
  {
    UartSendMsg("Rpc:Not Starter\r\n");
    return ERR_RPC_STARTER;
  }
  
  cmd           = pMsg->msg[MSG_IDX_CMD];
  
  switch(cmd)
  {
  case MSG_CMD_TEST:            UartSendMsg             ("Rpc:Cmd A\r\n");      break;
  case MSG_CMD_CTRL_LED2:       CmdControlLED2          ( pMsg );               break;
  case MSG_CMD_CTRL_LED3:       CmdControlLED3          ( pMsg );               break;
  case MSG_CMD_CTRL_PWM1:       CmdControlPWM           ( pMsg );               break;
  case MSG_CMD_CTRL_STEERING:   CmdChangeDirection      ( pMsg );               break;
  case MSG_CMD_CTRL_SPEED:      CmdControlSpeed         ( pMsg );               break; 
  default:                      UartSendMsg("Rpc:Cmd Not defined\r\n");         break;
  }
  
  return NO_ERR;
}

ErrCode CmdControlLED2(StRpcMsg* pMsg)
{
  uint8_t data = pMsg->msg[MSG_IDX_DATA];
  
  UartSendMsg("Rpc:Cmd LED2\r\n"); 
  
  if ( _ASCII_NUM(data) )
    BSP_LED_On(LED2);
  else
    BSP_LED_Off(LED2);
  
  return NO_ERR;
}

ErrCode CmdControlLED3(StRpcMsg* pMsg)
{
  uint8_t data = pMsg->msg[MSG_IDX_DATA];
  
  UartSendMsg("Rpc:Cmd LED3\r\n"); 
  
  if ( _ASCII_NUM(data) )
    BSP_LED_On(LED3);
  else
    BSP_LED_Off(LED3);
  
  return NO_ERR;
}

ErrCode CmdControlPWM(StRpcMsg* pMsg)
{
  uint8_t channel       = _ASCII_NUM(pMsg->msg[MSG_IDX_DATA]); 
  uint8_t num10th       = _ASCII_NUM(pMsg->msg[MSG_IDX_DATA + 1]);
  uint8_t num1th        = _ASCII_NUM(pMsg->msg[MSG_IDX_DATA + 2]);
  uint8_t duty          = ( num10th * 10 ) + num1th;
  
  if ( duty > 99 )
    duty = 99;
  else if ( duty < 1 )
    duty = 0;
  
  BSP_PWM1( (uint32_t)channel, (uint32_t)duty ); 
  UartSendMsg("Rpc:Cmd PWM, duty="); 
  UartSendData(duty, OPT_ENDLINE);
  
  return NO_ERR;
  
/* For HS-311 servo  
  uint8_t data = pMsg->msg[MSG_IDX_DATA];
  
  if ( _ASCII_NUM(data) )
  {
    BSP_PWM1( 25 ); 
    UartSendMsg("Rpc:Cmd PWM+\r\n"); 
  }
  else
  {
    BSP_PWM1( 0 ); 
    UartSendMsg("Rpc:Cmd PWM-\r\n"); 
  }
  return NO_ERR;
  */
}

ErrCode CmdChangeDirection(StRpcMsg* pMsg)
{
  uint8_t num10th       = _ASCII_NUM(pMsg->msg[MSG_IDX_DATA]); 
  uint8_t num1th        = _ASCII_NUM(pMsg->msg[MSG_IDX_DATA + 1]);
  
  int8_t percent        = ( num10th * 10 ) + num1th;
   
  RbChangeDirection( percent ); 
  
  UartSendMsg("Rpc:Cmd PWM, percent="); 
  UartSendData(percent, OPT_ENDLINE);
  
  return NO_ERR;
}

ErrCode CmdControlSpeed (StRpcMsg* pMsg)
{
  uint8_t num10th       = _ASCII_NUM(pMsg->msg[MSG_IDX_DATA]);
  uint8_t num1th        = _ASCII_NUM(pMsg->msg[MSG_IDX_DATA + 1]);

  uint8_t percent       = ( num10th * 10 ) + num1th;
  
  RbChangeSpeed( percent ); 
                  
  UartSendMsg("Rpc:Cmd RoberWheel, percent="); 
  UartSendData(percent, OPT_ENDLINE);
    
  return NO_ERR;
}
