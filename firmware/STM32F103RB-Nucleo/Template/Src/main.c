
#include "common.h"
#include "main.h"
#include "interfaceHAL.h"

static void TaskPeriod(uint32_t periodMs, uint32_t *pTickLast);

int main(void)
{
	HALIF_InitializeHW();

	uint32_t tickLast = HAL_GetTick();

	while (1)
	{
		TaskPeriod(1000, &tickLast);
	}
}

void TaskPeriod(uint32_t periodMs, uint32_t *pTickLast)
{
	uint32_t tick = HAL_GetTick();

	if ((tick - *pTickLast) > periodMs)
	{
		HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);
		*pTickLast = tick;
	}
}
