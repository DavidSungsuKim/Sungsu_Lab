
#include "SPITest.h"
#include "stm32f4xx_nucleo.h"

void InitSPIMaster (void)
{
	SPI_InitTypeDef param;

	param.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	param.Direction 		= SPI_DIRECTION_2LINES;
	param.CLKPhase 			= SPI_PHASE_2EDGE;
	param.CLKPolarity 		= SPI_POLARITY_HIGH;
	param.CRCCalculation 	= SPI_CRCCALCULATION_DISABLED;
	param.CRCPolynomial 	= 7;
	param.DataSize 			= SPI_DATASIZE_8BIT;
	param.FirstBit 			= SPI_FIRSTBIT_MSB;
	param.NSS 				= SPI_NSS_SOFT;
	param.TIMode 			= SPI_TIMODE_DISABLED;
	param.Mode 				= SPI_MODE_MASTER;

	BSP_SPIx_Init(&param);
}

void TestMasterLoopback (void)
{
	uint8_t txByte = 0xaa;
	uint8_t rxByte = 0x00;

	BSP_SPIx_WriteReadByte( txByte, &rxByte );

	uint8_t bSame = 0;
	if ( txByte == rxByte )
		bSame = 1;
}

void InitSPISlave (void)
{
	SPI_InitTypeDef param;

	param.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8; 	// effective on master mode only
	param.Direction 		= SPI_DIRECTION_2LINES;		// SPI_DIRECTION_2LINES;
	param.CLKPhase 			= SPI_PHASE_1EDGE;			// SPI_PHASE_2EDGE;
	param.CLKPolarity 		= SPI_POLARITY_LOW;			// SPI_POLARITY_HIGH;
	param.CRCCalculation 	= SPI_CRCCALCULATION_DISABLED;
	param.CRCPolynomial 	= 7;
	param.DataSize 			= SPI_DATASIZE_8BIT;
	param.FirstBit 			= SPI_FIRSTBIT_MSB;			// effective on master mode only
	param.NSS 				= SPI_NSS_SOFT;				// SPI_NSS_HARD_INPUT;//SPI_NSS_SOFT; // this SPI module in slave mode is internally selected
	param.TIMode 			= SPI_TIMODE_DISABLED;
	param.Mode 				= SPI_MODE_SLAVE;			// SPI_MODE_MASTER;

	BSP_SPIx_Init(&param);
}

void ReceiveWaitSPISlave (void)
{
	enum
	{
		eLength = 32
	};

	uint8_t rxBuffer[ eLength ] = {0,};
	uint16_t timeOut = 1000;

	BSP_SPIx_Read( (uint32_t)eLength, rxBuffer, timeOut );

	if ( rxBuffer[0] != 0x00 )
		BSP_LED_Toggle(LED2);

	return;
}

void ReceiveWaitSendSPISlave(void)
{
	enum
	{
		eLength = 32
	};

	uint16_t i;
	uint8_t txBuffer[ eLength ];
	uint8_t rxBuffer[ eLength ] = {0,};
	uint8_t *pTx = txBuffer;
	uint16_t timeOut = 1000;

	for (i = 1; i <= eLength; i++)
	{
		*pTx = i;
		pTx++;
	}

	BSP_SPIx_ReadWrite( (uint32_t)eLength, txBuffer, rxBuffer, timeOut );

	if ( rxBuffer[0] != 0x00 )
		BSP_LED_Toggle(LED2);

	return;
}
