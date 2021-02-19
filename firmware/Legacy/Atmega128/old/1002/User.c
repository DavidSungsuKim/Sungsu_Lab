
//#include <User.h>

unsigned short freescale_timer0[8] = {0,1,8,32,64,128,256,1024};



//***************************************************************
// Normal mode에서 timer 호출간격을 위한 클록수를 계산해주는 함수
// TCNTn 의 입력으로 들어간다.
// ※ 만들다가 말았음.
//***************************************************************
unsigned char CalClock(int sec, unsigned char tccr)
{
	unsigned char tcntn;
	unsigned char index;

	index = tccr & 0x07;
	tcntn = (sec * F_CPU) / freescale_timer0[index];
	
	return tcntn;
}
