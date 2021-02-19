#include <stdio.h>    // 순전히 sprintf()를 위해 #include 했음
#include <avr/io.h>
#include "delay.h"

int main(void)
{
    delay_ms(10);    // 아주 잠깐 초기 시간 지연을 준다.
                    // LCD와 MCU 등은 전원이 들어온 후 약간의 시간이 지나야 정상 동작한다.

    DDRA = 0x00;    // ADC를 입력으로 한다.

    adc_init();    // AC 컨버터 초기화
    lcd_init() ;    // LCD 초기화

    uint16_t result;            // A/D 컨버팅 결과를 받아올 임시 변수
    char string[20];    // LCD에 나타낼 문자열을 저장할 임시 변수


    lcd_display_string(0, 0, "- Thermometer - ");    // 디지털 온도계라는 뜻. LCD 첫 줄 표시

    
    // 무한 루프 - 프로그램은 종료되지 않는다.
    while(1)
    {
        
        result = (uint16_t) get_adc(0);    // 1024 분해능으로 ADC0 결과를 받아온다.
        result = (result * 125) / 128;     // 섭씨온도 x 100으로 변환

        // 결과 출력
        sprintf(string, "    %2d.%d %cC", result/10, result%10, 0xDF);    

                                              // 결과를 포맷에 맞게 문자열로 생성한다.
        lcd_display_string(0, 1, string);            // 생성된 문자열을 LCD에 뿌린다.

        delay_ms(100);    // 0.1초 시간 지연
    }

    return 0;
}


