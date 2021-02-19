#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>

#include "Cpu.h"
#include "XbeeController.h"

unsigned short xbSend(char* str)
{
	unsigned short i = 0;
	while((str[i] != '\r') && (str[i] != '\0'))
	{
		SendUART0(str[i]);
		i++;
	}

	return 0;
}
unsigned short xbRecv(char* str)
{
	return 0;
}
