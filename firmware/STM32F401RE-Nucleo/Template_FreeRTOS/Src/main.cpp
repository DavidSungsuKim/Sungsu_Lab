/**
  ******************************************************************************
  * @file    GPIO/GPIO_IOToggle/Src/main.c 
  * @author  MCD Application Team
  * @version V1.2.2
  * @date    09-October-2015
  * @brief   This example describes how to configure and use GPIOs through 
  *          the STM32F4xx HAL API.
  ******************************************************************************
  */

#include "main.h"

#include "common.h"
#include "HALInterface.h"
    
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#define TASK_PRIORITY(x) (tskIDLE_PRIORITY + x)

SemaphoreHandle_t g_sem;

static void LEDTurnOnTask     ( void *pvParameters );
static void LEDTurnOffTask    ( void *pvParameters );

int main(void)
{
	HALIF_Initialize();

	g_sem = xSemaphoreCreateBinary();
	if ( g_sem == NULL )
		return -1;

	xSemaphoreGive( g_sem );

	// The bigger number the higher priority?
	xTaskCreate( LEDTurnOnTask,  "LEDTurnOnTask",  configMINIMAL_STACK_SIZE, NULL, TASK_PRIORITY(2), NULL );
	xTaskCreate( LEDTurnOffTask, "LEDTurnOffTask", configMINIMAL_STACK_SIZE, NULL, TASK_PRIORITY(1), NULL );

	vTaskStartScheduler();
}

void LEDTurnOnTask( void *pvParameters )
{
    TickType_t tickWait = portMAX_DELAY;
    
    for(;;)
    {    
      xSemaphoreTake( g_sem, tickWait );
     
      HALIF_TurnOnLED1();

      int test = 1;
      _printf("LEDTurnOnTask, test=%d\r\n", test);
      vTaskDelay(100);

      xSemaphoreGive( g_sem );
      vTaskDelay(1);
    }
}

void LEDTurnOffTask( void *pvParameters )
{
    TickType_t tickWait = portMAX_DELAY;
          
    for(;;)
    {    
      xSemaphoreTake( g_sem, tickWait );
     
      HALIF_TurnOffLED1();

      int test = 1;
      _printf("LEDTurnOffTask, test=%d\r\n", test);

      vTaskDelay(1000);
      
      xSemaphoreGive( g_sem );
    }
}
