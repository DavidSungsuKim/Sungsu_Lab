
#include <stdio.h>
#include "hardware.h"

int WriteRegister(int value)
{
	printf("%s, value=%d\r\n", __FUNCTION__, value);
	return 0;
}