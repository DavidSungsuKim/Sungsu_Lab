
#include <stdio.h>
#include "app.h"
#include "hardware.h"

int AppFunctionOne(void)
{
	int ret;

	int value = 1;
	ret = WriteRegister(value);
	if (ret != 0)
	{
		printf("error=%d\r\n", ret);
	}

	return ret;
}