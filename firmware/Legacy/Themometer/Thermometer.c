#include <stdio.h>    // ������ sprintf()�� ���� #include ����
#include <avr/io.h>
#include "delay.h"

int main(void)
{
    delay_ms(10);    // ���� ��� �ʱ� �ð� ������ �ش�.
                    // LCD�� MCU ���� ������ ���� �� �ణ�� �ð��� ������ ���� �����Ѵ�.

    DDRA = 0x00;    // ADC�� �Է����� �Ѵ�.

    adc_init();    // AC ������ �ʱ�ȭ
    lcd_init() ;    // LCD �ʱ�ȭ

    uint16_t result;            // A/D ������ ����� �޾ƿ� �ӽ� ����
    char string[20];    // LCD�� ��Ÿ�� ���ڿ��� ������ �ӽ� ����


    lcd_display_string(0, 0, "- Thermometer - ");    // ������ �µ����� ��. LCD ù �� ǥ��

    
    // ���� ���� - ���α׷��� ������� �ʴ´�.
    while(1)
    {
        
        result = (uint16_t) get_adc(0);    // 1024 ���ش����� ADC0 ����� �޾ƿ´�.
        result = (result * 125) / 128;     // �����µ� x 100���� ��ȯ

        // ��� ���
        sprintf(string, "    %2d.%d %cC", result/10, result%10, 0xDF);    

                                              // ����� ���˿� �°� ���ڿ��� �����Ѵ�.
        lcd_display_string(0, 1, string);            // ������ ���ڿ��� LCD�� �Ѹ���.

        delay_ms(100);    // 0.1�� �ð� ����
    }

    return 0;
}


