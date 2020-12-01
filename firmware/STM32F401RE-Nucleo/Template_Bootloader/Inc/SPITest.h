
#ifndef _SPI_TEST_H_
#define _SPI_TEST_H_

#include <stdint.h>

void InitSPIMaster			(void);
void TestMasterLoopback		(void);

void InitSPISlave			(void);
void ReceiveWaitSPISlave	(void);
void ReceiveWaitSendSPISlave(void);

#endif
