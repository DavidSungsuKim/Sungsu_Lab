
#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <util/delay.h> 
#include "User.h"
#include "Cpu.h"
#include "XbeeController.h"

#define F_CPU 16000000UL
#define DEBOUNCING 16

extern unsigned char pattern[4];
extern unsigned char bit[8];

extern unsigned char ucLeftNum;
extern unsigned char ucRightNum;

ISR(INT0_vect)
{
	_delay_ms(DEBOUNCING);

	static int duty = 0;
	/*
	static int i 	= 0;
	// portA LED 컨트롤
	i++;
	if(i == 4)
		i = 0;
	PORTA = pattern[i];
	*/

	/*
	// Timer2 duty ratio 조절
	duty += 10;

	OCR2 = duty;

	if(duty >= 245)
		duty = 0;
	*/

	while(~PIND & 0x01);

	_delay_ms(DEBOUNCING);

}

ISR(INT1_vect)
{
	_delay_ms(DEBOUNCING);

	static int num = 0;
	num++;

	if(num == 10)
		num = 0;

	while(~PIND & 0x02);

	_delay_ms(DEBOUNCING);

}

// Timer0(8bit) overflow ISP
ISR(TIMER0_OVF_vect)	
{
	static int index = 0;	 	// pattern index
	static char n_enter = 0; 	// count interrupt
	static int i = 0;	

	TCNT0 = 10;			 		// 100msec 후에 interrupt 발생
	
	n_enter++;

	if(n_enter == 5)		 	// 100msec 마다
	{
		PORTA = pattern[index];
		
		index++;

		if(index == 4)
			index = 0;

		n_enter = 0;
	}

}


// Timer2(8bit) overflow ISP 
ISR(TIMER2_OVF_vect)
{
	
}	


