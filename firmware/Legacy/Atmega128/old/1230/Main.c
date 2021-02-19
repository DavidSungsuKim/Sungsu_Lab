

#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <util/delay.h> 

#include "Cpu.h"
#include "XbeeController.h"

#define F_CPU 						16000000UL
#define DEBOUNCING 					40
#define PERIOD_DOT_MATRIX_UPDATE 	5
#define MAX_DOT_MATRIX_ROW			8
#define MAX_DOT_MATRIX_NUM			10

unsigned char pattern[4]  = {0x02, 0x08, 0x20, 0x80};
unsigned char segment[17] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x7D, 0x07,
							 0x7F, 0x6F, 0x77, 0x7C, 0x58, 0x5E, 0x79,
							 0x71, 0x00};

unsigned char gr_dot_scanline[MAX_DOT_MATRIX_ROW] =  {0xFE, 0xFD, 0xFB, 0xF7,	// 1~8 line 선택
							  					      0xEF, 0xDF, 0xBF, 0x7F};

unsigned char gr_dot_num[MAX_DOT_MATRIX_NUM][MAX_DOT_MATRIX_ROW]
					  =  {{0x07, 0x05, 0x05, 0x05, 0x05, 0x05, 0x07, 0x00 }, // 0
						  {0x02, 0x06, 0x02, 0x02 ,0x02, 0x02, 0x07, 0x00 }, // 1
						  {0x07, 0x01, 0x01, 0x07 ,0x04, 0x04, 0x07, 0x00 }, // 2
						  {0x07, 0x01, 0x01, 0x07 ,0x01, 0x01, 0x07, 0x00 }, // 3
						  {0x0A, 0x0A, 0x0A, 0x0F ,0x02, 0x02, 0x02, 0x00 }, // 4
						  {0x07, 0x04, 0x04, 0x07 ,0x01, 0x01, 0x07, 0x00 }, // 5 
						  {0x04, 0x04, 0x04, 0x07 ,0x05, 0x05, 0x07, 0x00 }, // 6
						  {0x07, 0x05, 0x05, 0x01 ,0x01, 0x01, 0x01, 0x00 }, // 7
						  {0x07, 0x05, 0x05, 0x07 ,0x05, 0x05, 0x07, 0x00 }, // 8 
						  {0x07, 0x05, 0x05, 0x07 ,0x01, 0x01, 0x07, 0x00 }};// 9
					  	
unsigned char	ucLeftNum = 0;
unsigned char	ucRightNum = 0;

//******************
// test variables 	
unsigned short test_i = 0;
//******************

void InitDotMatrix();
void DisplayDotMatrix(unsigned char ucRightNum, unsigned char ucLeftNum);

int main()
{
	InitCPU();
	InitCMT0();
	InitUART0();

//	InitDotMatrix();

	xbSend("+++");
  	
	while(1)
	{	
		
		char test[3] = {0,0,0};

		
		if(UCSR0A & 0x80)
			test[0] = UDR0;
		
		/*
		RecvUART0(&test[0]);

		if(test[0] == 'O')
			RecvUART0(&test[1]);

		xbSend("ATMY8989\r");
		xbSend("ATCN\r");
		*/
		/*
		//******************************************
		// 도트매트릭스 테스트
		test_i++;

		if(test_i > 10)
		{
			ucLeftNum++;
			test_i = 0;

			if(ucLeftNum >= MAX_DOT_MATRIX_NUM)
			{
				ucLeftNum = 0;
				ucRightNum++;

				if(ucRightNum >= MAX_DOT_MATRIX_NUM)
				{
					ucRightNum = 0;
				}
			}
		}	
		DisplayDotMatrix(ucRightNum,ucLeftNum);	
		//******************************************
		*/
	};

	return 0;

}

void InitDotMatrix()
{

}

/*
void DisplayDotMatrix(unsigned char ucTenNum, unsigned char ucOneNum)
{
	unsigned short i;
	
	for(i = 0; i <= MAX_DOT_MATRIX_ROW; i++)
	{
		PORT_DOTMATRIX_ROW = gr_dot_scanline[i];
		PORT_DOTMATRIX_COL = ~(gr_dot_num[ucOneNum][i]);

		_delay_ms(PERIOD_DOT_MATRIX_UPDATE);

		PORT_DOTMATRIX_COL = ~(gr_dot_num[ucTenNum][i] << 4);

		_delay_ms(PERIOD_DOT_MATRIX_UPDATE);
	}

}
*/
