
#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <util/delay.h> 
#include "Cpu.h"

#include "SsNetController.h"

#define DEBOUNCING 16

unsigned short g_usIRQ0Ack 		= 0;
unsigned short g_usIRQ1Ack 		= 0;

extern unsigned short	g_usUARTTxcAck;
extern RingBuff 		g_SendBuff;

extern unsigned short	g_usPacketTick;
extern unsigned short	g_usMsgTick;

extern unsigned short	g_usPacketTick;
extern unsigned short	g_usPacketTimeLimit;

unsigned char g_testChar;

ISR(INT0_vect)
{
	_delay_ms(DEBOUNCING);

	g_usIRQ0Ack = TRUE;

	while(~PIND & 0x04);

	_delay_ms(DEBOUNCING);
}

ISR(INT1_vect)
{
	_delay_ms(DEBOUNCING);

	g_usIRQ1Ack = TRUE;

	while(~PIND & 0x08);

	_delay_ms(DEBOUNCING);

}

ISR(USART_RXC_vect)
{		
	char cChar;
	
	g_usPacketTick = 0;
	SsNetCtrlRxTimeout(RESET);

	if(!RecvUART(&cChar))
		PushRingBuff(&g_SendBuff,cChar);
		
	if(UCSRA & 0x10)				// If frame error, 
		CtrlPortALED(ON,LED3);

	if(UCSRA & 0x08)				// If data overrun	
		CtrlPortALED(ON,LED4);		
}

ISR(USART_TXC_vect)
{
/*	g_usUARTTxcAck = TRUE;
	CtrlPortALED(OFF,LED2);			// Main loop에서는 UDSRA의 TXC가 check가 확인이 안된다. 
	*/
}

ISR(TIMER0_OVF_vect)	
{
	ToggleLED(LED2);

//	SendUART("A");		// Test

	if(SsNetGetRxTimeout() == SET)
		return;

	g_usPacketTick++;
	g_usMsgTick++;

	if(g_usPacketTick > g_usPacketTimeLimit)
	{
		SsNetCtrlRxTimeout(SET);
		SsNetSetNextTask(TASK_RX_PREP);
	}
}

ISR(TIMER2_OVF_vect)
{
	
}	


