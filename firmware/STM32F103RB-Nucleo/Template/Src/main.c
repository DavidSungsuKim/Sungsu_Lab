
#include "common.h"
#include "main.h"
#include "interfaceHAL.h"

static void TaskPeriodic(unsigned int periodMs, unsigned int *pTickLast);

int main(void)
{
	HALIF_InitializeHW();

	unsigned int tickLast = HALIF_GetTick();

	while (1)
	{
		TaskPeriodic(1000, &tickLast);
	}
}

void TaskPeriodic(unsigned int periodMs, unsigned int *pTickLast)
{
	unsigned int tickCurrent = HALIF_GetTick();
	unsigned int tickForMs	= periodMs;

	if ((tickCurrent - *pTickLast) > tickForMs)
	{
		/* Do something here */

		HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);

		int ret;
		ret = HALIF_UARTSendSync("TaskPeriod...\r\n");
		if ( ret )
			ret = 0;

		/* Update tick */
		*pTickLast = tickCurrent;
	}
}
