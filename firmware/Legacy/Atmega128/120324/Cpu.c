		#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include "Cpu.h"

#define F_CPU 						16000000UL

void InitCPU()
{
	// Port setting
    DDRA = 0xFF;							// portA를 모두 output으로 설정.
	DDRB = 0xFF;							// 도트매트릭스용.
//	DDRE = 0xFF; 							// 도트매트릭스용.
	DDRE = 0x02;							// TXD0는 출력, RXD0는 입력으로 설정.

	DDRD = 0x00;							// 인터럽트 스위치용.

	sei();									// 전역인터럽트 허용

	EICRA = (2<<ISC00);						// 하강엣지 트리거
	EIMSK = (1<<INT0);						// interrupt 0 허용
	EIMSK = (1<<INT1);						// interrupt 1 허용

	return;
}

void InitCMT0()
{
	TCCR0 = 0x00; 							// 표준모드, 타이머정지
	TCCR0 |= 0x07;							// 분주비 셋팅(1024)
	TIMSK = 0x01;							// 인터럽트셋팅
}

void InitCMT2()
{
	TCCR2 = 0x40;							// Phase Correct PWM 모드, 타이머정지
	TCCR2 |= 0x20;							// Compare Match 출력모드 설정
	TCCR2 |= 0x03;							// 분주비 셋팅(64)	
	OCR2 = 0;
//	TIMSK |= 0x80;							// 인터럽트셋팅 -> ※이거하면 정상적으로 동작하지 않는다. 왜 그런가?
}

void InitUART0()
{
	unsigned short ubrr;	
	ubrr = (F_CPU / 16) / BAUD_RATE - 1;					// Set Baud Rate	
//	UBRR0H = (unsigned char)(ubrr >> 8);
//	UBRR0L = (unsigned char)(ubrr);
	UBRR0H = 0;
	UBRR0L = 103;

	UCSR0B = 0xD8;
	UCSR0C = 0x06;											// Set frame format
}

unsigned short SendUART0(char cData)
{
	unsigned short time = 0;
	while(!(UCSR0A & 0x20))							// Check if tx is enable.
	{
		time++;
		if(time > ERR_UART_TIME_OUT)
		{
			return 1;
		}
	}
	
	UDR0 = cData;
	return 0;
}

unsigned short RecvUART0(char* cData)
{
	unsigned short time = 0;
	while(!(UCSR0A & 0x80))							// Check if there is a new rx packet.
	{
		time++;
		if(time > ERR_UART_TIME_OUT)
		{
			*cData = 0xFF;
			return 1;
		}
	}
	
	*cData = UDR0;
	return 0;
}


















