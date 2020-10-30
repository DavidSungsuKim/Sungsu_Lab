
//***********************************************************
//      STM32F401-Nucleo Personal Application
// File name    : UartController.h
// Designer     : Sungsu Kim
// Email        : sheld2@naver.com
// All rights reserved
// 
//***********************************************************

#ifndef _UART_CONTROLLER_H_
#define _UART_CONTROLLER_H_      1

#include "ErrorCodes.h"

#define UART_RECV_BUFFER_SIZE   16

typedef struct stUartController
{
  uint8_t       index;
  uint8_t       buffer[UART_RECV_BUFFER_SIZE];
  
}StUartCtrl;

#define OPT_NONE                (uint32_t)0x0
#define OPT_ENDLINE             (uint32_t)0x1

void    UartInitializeSW        ();
void    UartInitializeHW        ();

ErrCode UartSendPackets         (const char* pData, unsigned int aSize);
ErrCode UartSendMsg             (const char* cString);
ErrCode UartSendMsg2             (const char* cString, ...);
ErrCode UartSendData            (uint8_t  data, uint32_t option);
ErrCode UartNotifyRecv          (uint8_t data);

#endif