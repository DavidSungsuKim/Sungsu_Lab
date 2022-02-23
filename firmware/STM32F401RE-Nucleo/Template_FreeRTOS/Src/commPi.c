/**************************************************************************
 * @file    commPi.c
 * @author  Sungsu Kim
 *
 * @date	Dec, 2021
 * @brief
 */

/********************************* Include *******************************/
#include "common.h"
#include "commPi.h"

#include "os.h"
#include "HALInterface.h"

/********************************* Const *********************************/

/********************************* Types *********************************/

/********************************* Macro *********************************/

/***************************** Local Variable ****************************/

/**************************** Global Variable ****************************/

/************************* Function Declaration **************************/
static void TestComm ( void );

/************************* Function Definition **************************/
void COMM_PI_task( void *apParams )
{
	_printf("%s runs...\r\n", __FUNCTION__ );

	for(;;)
	{
		vTaskDelay(1);
		TestComm();
	}
}

static void TestComm( void )
{
	eStatus ret = eOK;

	uint8_t txByte = 0xaa;
	uint8_t rxByte = 0x00;

	HALIF_WriteReadByteSPI( txByte, &rxByte );

	uint8_t bSame = 0;
	if ( txByte == rxByte )
	{
		bSame = 1;
		_printf("same\r\n");
	}
}
