
//***********************************************************
//      STM32F401-Nucleo Personal Application
// File name    : RpcController.h
// Designer     : Sungsu Kim
// Email        : sheld2@naver.com
// All rights reserved
// 
//***********************************************************

#ifndef _RPC_CONTROLLER_H_
#define _RPC_CONTROLLER_H_      1

#include "Common.h"
#include "UartController.h"

#define RPC_MSG_SIZE    16

#define MSG_IDX_START   0
#define MSG_IDX_CMD     1
#define MSG_IDX_DATA    2
#define MSG_IDX_CKH_SUM 15

#define MSG_STARTER             'X'
#define MSG_CMD_TEST            'A'
#define MSG_CMD_CTRL_LED2       '1'
#define MSG_CMD_CTRL_LED3       '2'
#define MSG_CMD_CTRL_PWM1       '3'
#define MSG_CMD_CTRL_STEERING   '4'
#define MSG_CMD_CTRL_SPEED      '5'

typedef struct stRpcMsg
{
  uint8_t       size;
  uint8_t       msg[RPC_MSG_SIZE];
  
}StRpcMsg;

extern StRpcMsg g_RpcMsg;

typedef struct stRpcController
{
  int test;
  
}StRpcCtrl;

void    RpcInitializeSW         ();
void    RpcInitializeHW         ();
void    RpcNotifyMsg            (StRpcMsg* pMsg);

#endif