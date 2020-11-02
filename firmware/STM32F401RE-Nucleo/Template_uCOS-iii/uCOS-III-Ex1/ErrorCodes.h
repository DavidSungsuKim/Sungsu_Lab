
#ifndef _ERROR_CODES_H_
#define _ERROR_CODES_H_      1

typedef long ErrCode;

#define NO_ERR                  0
#define ERR_UART_TX_TIMEOUT     1
#define ERR_UART_RX_TIMEOUT     2
#define ERR_RPC_STARTER         3

int ErrorHandling (int errorCode);

#endif