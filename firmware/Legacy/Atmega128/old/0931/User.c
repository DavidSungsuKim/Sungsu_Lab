
//#include <User.h>

unsigned short freescale_timer0[8] = {0,1,8,32,64,128,256,1024};



//***************************************************************
// Normal mode���� timer ȣ�Ⱓ���� ���� Ŭ�ϼ��� ������ִ� �Լ�
// TCNTn �� �Է����� ����.
// �� ����ٰ� ������.
//***************************************************************
unsigned char CalClock(int sec, unsigned char tccr)
{
	unsigned char tcntn;
	unsigned char index;

	index = tccr & 0x07;
	tcntn = (sec * F_CPU) / freescale_timer0[index];
	
	return tcntn;
}
