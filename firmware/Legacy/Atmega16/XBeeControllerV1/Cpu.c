//******************************************************
//
// This is for ATmega16
//
//******************************************************

#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include "Cpu.h"

unsigned short g_usUARTTxcAck 	= TRUE;	

RingBuff g_SendBuff;
RingBuff g_RecvBuff;

void InitCPU()
{
	// Port setting
    DDRA = PORTA_ASSIGNED_LED;				// PA0, PA1, PA2, PA3을 output으로 설정
	DDRB = 0xFF;
	DDRD = 0x02;							// TXD를 출력, RXD를 입력으로 설정							
	
	sei();									// 전역인터럽트 허용

	InitCMT0();

	MCUCR	= 0x0A;							// INT0, INT1 Falling edge 
	GICR	= 0xC0;							// INT0, INT1 Enable

	return;
}

void InitCMT0()
{
	TCCR0	= 	0x00;							// Stop Timer
	TCCR0 	= 	0x04; 							// Clear Timer on Compare Match Mode
	TCCR0 	|= 	0x05;							// Prescaler = (Clk_io / 1024)
	OCR0	=	0x10;							// 
	TIMSK 	= 0x01;								// Interrupt enable
}

void InitCMT2()
{
	TCCR2 = 0x40;							// Phase Correct PWM 모드, 타이머정지
	TCCR2 |= 0x20;							// Compare Match 출력모드 설정
	TCCR2 |= 0x03;							// 분주비 셋팅(64)	
	OCR2 = 0;
//	TIMSK |= 0x80;							// 인터럽트셋팅 -> ※이거하면 정상적으로 동작하지 않는다. 왜 그런가?
}


void InitUART()
{
	unsigned short ubrr;	
	ubrr = (F_CPU / 16) / BAUD_RATE - 1;	// Set Baud Rate	
//	UBRRH = (unsigned char)(ubrr >> 8);
//	UBRRL = (unsigned char)(ubrr);
	UBRRH = 0;
	UBRRL = 103;							// Baud Rate 9600 under 16Mhz

	UCSRA = 0x00;
	UCSRB = 0xD8;							// Bit4 : Transmitter Enable
											// Bit3 : Receiver Enable
											// Bit7 : RX Complete Interrupt Enable
											// Bit6 : TX Complete Interrupt Enable	
	UCSRC = 0xA6;							// Even Parity, 8bit Data
}

unsigned short SendUART(char* cData)
{
	unsigned short time = 0;
	while(!(UCSRA & 0x20))					// Check if buffer register is empty
	{									
		time++;
		if(time > ERR_UART_SEND_TIME_OUT)
		{
		//	return 1;
		}
	}

	UDR = *cData;
	return 0;
}

unsigned short RecvUART(char* cData)
{
	unsigned short time = 0;
	while(!(UCSRA & 0x80))					// Check if there is a new rx packet.
	{
		time++;
		if(time > ERR_UART_RECV_TIME_OUT)
		{
			*cData = 0xFF;
		//	return 1;
		}
	}
	
	*cData = UDR;

	return 0;
}

unsigned short CtrlPortALED(unsigned char state, unsigned char numLed)
{
	if(!(PORTA_ASSIGNED_LED & numLed))
		return 1;
	
	if(state == ON)
		PORTA |= numLed;
	else if(state == OFF)
		PORTA &= ~(numLed);

	return 0;
}

void ToggleLED(unsigned char numLed)
{
	if(!(PORTA_ASSIGNED_LED & numLed))
		return 1;

	PORTA ^= numLed;
}


void InitRingBuff(RingBuff* buff)
{
	(*buff).head	= 0;
	(*buff).tail	= 0;
	(*buff).num		= 0;
	(*buff).full	= 0;
}

void PushRingBuff(RingBuff* buff, unsigned char data)
{
	(*buff).buffer[(*buff).tail] = data;
	(*buff).tail = ((*buff).tail + 1) % SIZE_RING_BUFF;
	
	if(!(*buff).full)
		(*buff).num++;

	if((*buff).tail == (*buff).head)
	{
		(*buff).full = 1;
	}	
}

unsigned short PopRingBuff(RingBuff* buff, unsigned char* data)
{
	if((*buff).num)
	{	
		if((*buff).full)
		{
			(*buff).head 	= (*buff).tail;
			(*buff).head 	= (*buff).head % SIZE_RING_BUFF;			
			(*buff).full 	= 0;	
		}

		*data = (*buff).buffer[(*buff).head];

		(*buff).buffer[(*buff).head] = 0x00;
		(*buff).head = ((*buff).head + 1) % SIZE_RING_BUFF;
		(*buff).num--;

		return 1;
	}
	else
	{
		return 0;
	}

}




