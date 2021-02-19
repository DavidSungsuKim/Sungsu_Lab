
#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <util/delay.h> 

#include "Cpu.h"

extern unsigned short	g_usIRQ0Ack;
extern unsigned short	g_usIRQ1Ack;
extern unsigned short	g_usUARTTxcAck;
extern struct RingBuff 	g_SendBuff;

int main()
{
	unsigned short	usTest			= 0x0000;	
	unsigned short	usToggle		= 0x0000;
	unsigned short	usCntRxHandle 	= 0;
	unsigned short	result;
	char			cTest;

	InitCPU();
	InitUART();
	InitRingBuff(&g_SendBuff);

	while(1)
	{	
		SsNetMainTask();
	/*
		usTest++;

		if((usTest % 10000 ) == 0)
		{
			if(usToggle)
			{
				CtrlPortALED(ON,LED1);
				usToggle = FALSE;			
			}
			else
			{
				CtrlPortALED(OFF,LED1);
				usToggle = TRUE;	
			}

			CtrlPortALED(OFF,LED2);
			CtrlPortALED(OFF,LED3);
			CtrlPortALED(OFF,LED4);
		}

		if(g_usIRQ0Ack)
		{
			
		//	xbSend("\rPopStart\r");
		//	while(PopRingBuff(&g_SendBuff,&cTest))
		//	{
		//		CtrlPortALED(ON,LED3);
		//		SendUART(&cTest);
		//	}
		//	xbSend("\rPopEnd\r");
			
			g_usIRQ0Ack = FALSE;
		}

		xbDecodeCmd();

		if(g_usIRQ1Ack)
		{
			xbSend("ABCDEF");
			g_usIRQ1Ack = FALSE;
		}
	*/
	}

	return 0;
}

