
#include "SPITest.h"
#include "stm32f4xx_nucleo.h"

void InitSPI (void)
{
	// When I call this function, LED2 doesn't work. I need to check.
	mySPI_Init();
}

void DoSPITest ( uint8_t TxData )
{
	uint8_t RxData = 0xFF;
	mySPI_WriteReadByte( TxData, &RxData );

	uint8_t bSame = 1;
	if ( TxData == RxData )
		bSame = 0;
}
