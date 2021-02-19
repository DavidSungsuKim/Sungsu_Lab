#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <util/delay.h> 

#include "Cpu.h"
#include "XbeeController.h"

extern struct RingBuff 	g_SendBuff;

unsigned short xbSend(char* str)
{
	unsigned short i = 0;
	while((str[i] != '\0'))			/*(str[i] != '\r') &&*/
	{
		SendUART(&str[i]);
		i++;
	}
	return 0;
}

unsigned short xbRecv(char* str)
{
	return 0;
}

unsigned short xbSendData(char cData)
{
	SendUART(&cData);
	return 0;
}

unsigned short xbDecodeCmd(void)
{
	char cmd 	= 0;
	char length = 0;
	char data[16];
	char cChar;
	unsigned short i = 0;
	unsigned short bCmdExist = 0;
	
	while(PopRingBuff(&g_SendBuff,&cChar))
	{		
		if(cChar == 'X')			
		{
			bCmdExist = 1;
			break;
		}
	}

	if(bCmdExist)
	{
		while(PopRingBuff(&g_SendBuff,&cChar));
		cmd = cChar;

		while(PopRingBuff(&g_SendBuff,&cChar));
		length = cChar;	
	}

	for(i = 0; i < length; i++)
	{
//		PopRingBuff(&g_SendBuff,&cChar);
//		data[count] = cChar;
	}

	switch(cmd)
	{
		case 'A':
		xbSend("\r");
		xbSend("Cmd:");
		xbSendData(cmd);
		xbSend("\r");
		xbSend("Length:");
		xbSendData(length);
		xbSend("\r");			
		break;

		case 'B':
		xbSend("\r");
		xbSend("Cmd:");
		xbSendData(cmd);
		xbSend("\r");
		xbSend("Length:");
		xbSendData(length);
		xbSend("\r");			
		break;

		default:
		xbSend("\r");
		xbSend("Not defined Cmd\r");
		break;
	}

}
