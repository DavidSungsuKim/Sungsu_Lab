
#include "mockHardware.h"
#include "hardware.h"

int WriteRegister(int value)
{
	return g_mockHardware->WriteRegister(value);
}