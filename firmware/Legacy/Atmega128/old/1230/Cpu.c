#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include "Cpu.h"

#define F_CPU 						16000000UL

void InitCPU()
{
	// Port setting
    DDRA = 0xFF;							// portA�� ��� output���� ����.
	DDRB = 0xFF;							// ��Ʈ��Ʈ������.
	DDRE = 0xFF; 							// ��Ʈ��Ʈ������.

	DDRD = 0x00;							// ���ͷ�Ʈ ����ġ��.

	sei();									// �������ͷ�Ʈ ���

	EICRA = (2<<ISC00);						// �ϰ����� Ʈ����
	EIMSK = (1<<INT0);						// interrupt 0 ���
	EIMSK = (1<<INT1);						// interrupt 1 ���

	return;
}

void InitCMT0()
{
	TCCR0 = 0x00; 							// ǥ�ظ��, Ÿ�̸�����
	TCCR0 |= 0x07;							// ���ֺ� ����(1024)
	TIMSK = 0x01;							// ���ͷ�Ʈ����
}

void InitCMT2()
{
	TCCR2 = 0x40;							// Phase Correct PWM ���, Ÿ�̸�����
	TCCR2 |= 0x20;							// Compare Match ��¸�� ����
	TCCR2 |= 0x03;							// ���ֺ� ����(64)	
	OCR2 = 0;
//	TIMSK |= 0x80;							// ���ͷ�Ʈ���� -> ���̰��ϸ� ���������� �������� �ʴ´�. �� �׷���?
}

void InitUART0()
{
	unsigned short ubrr;	
	ubrr = (F_CPU / 16) / BAUD_RATE - 1;					// Set Baud Rate	
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)(ubrr);

	UCSR0A = 0x00;
	UCSR0B = (1<<RXEN0) | (1<<TXEN0)						// Enable receiver and transmitter	
		 	|(1<<RXCIE0)| (1<<TXCIE0); 						// Enable interrupt	
	UCSR0C = (1<<USBS0) | (1<<UCSZ01) | (1<<UCSZ00);		// Set frame format
}

unsigned short SendUART0(char cData)
{
	unsigned short time = 0;
	while(!(UCSR0A & (1<<UDRE0)))							// Check if tx is enable.
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
	while(!(UCSR0A & (1<<RXC0)))							// Check if there is a new rx packet.
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

















