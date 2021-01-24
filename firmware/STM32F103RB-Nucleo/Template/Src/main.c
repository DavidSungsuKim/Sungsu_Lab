
#include "common.h"
#include "main.h"
#include "interfaceHAL.h"

static void TaskPeriodic(unsigned int periodMs, unsigned int *pTickLast);

int main(void)
{
	HALIF_InitializeHW();

	unsigned int tickLast = HALIF_GetSysTick();

	while (1)
	{
	//	TaskPeriodic(100, &tickLast);
		HALIF_TestTickTimer();
	}
}

void TaskPeriodic(unsigned int periodMs, unsigned int *pTickLast)
{
	unsigned int tickCurrent = HALIF_GetSysTick();
	unsigned int tickForMs	= periodMs;

	if ((tickCurrent - *pTickLast) > tickForMs)
	{
		/* Do something here */

	//	HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);
		unsigned int tick = HALIF_GetTimerTick();
		_printf("TaskPeriodic, T=%d[ms], tick=%d\r\n", periodMs, tick);

		/* Update tick */
		*pTickLast = tickCurrent;
	}
}

