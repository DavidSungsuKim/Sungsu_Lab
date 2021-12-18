/**************************************************************************
 * @file    main.cpp
 * @author  Sungsu Kim
 *
 * @date	July, 2021
 * @brief
 */

/********************************* Include *******************************/
#include "common.h"
#include "HALInterface.h"
#include "driver_motor_servo_hs311.h"
#include "driver_motor_step_unipolar.h"
#include "servo.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/********************************* Const *********************************/

/********************************* Types *********************************/

/********************************* Macro *********************************/
#define TASK_PRIORITY(x) (tskIDLE_PRIORITY + x)
#define TEST_SPI
//#define TEST_PWM
//#define TEST_SERVO

/***************************** Local Variable ****************************/
static SemaphoreHandle_t g_sem;
static CServo g_servo( 10.0, 1.0, 2.0, 90.0 );

/**************************** Global Variable ****************************/

/************************* Function Declaration **************************/
void 	LEDTurnOnTask     	( void *pvParameters );
void 	LEDTurnOffTask    	( void *pvParameters );
void	TestTask			( void *pvparameters );

void	TestPWM				( void );
void	TestSPI				( void );
void 	TestGPIO			( void );

/************************* Function Definition ***************************/
int main(void)
{
	HALIF_Init();

#if defined (TEST_SERVO)
    //HS311_Init();
	g_servo.Init();
	//STEP_UNI_Init(4096);
#endif /* TEST_SERVO */

	g_sem = xSemaphoreCreateBinary();
	if ( g_sem == NULL )
		return -1;

	xSemaphoreGive( g_sem );

	// The bigger number the higher priority?
	xTaskCreate( LEDTurnOnTask,  	"LEDTurnOnTask",  	configMINIMAL_STACK_SIZE, NULL, TASK_PRIORITY(2), NULL );
	xTaskCreate( LEDTurnOffTask, 	"LEDTurnOffTask", 	configMINIMAL_STACK_SIZE, NULL, TASK_PRIORITY(1), NULL );
	xTaskCreate( TestTask, 			"TestTask",			configMINIMAL_STACK_SIZE, NULL, TASK_PRIORITY(1), NULL );

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

void TestTask( void *pvparameters )
{
	for(;;)
	{
		vTaskDelay(1000);

		TestGPIO();

#if defined (TEST_PWM)
		TestPWM();
#endif /* TEST_PWM */

#if defined (TEST_SPI)
		HALIF_TestSPI();
#endif /* TEST_SPI */

#if defined (TEST_SERVO)
		g_servo.Test();
	//	HS311_Test();
#endif /* TEST_SERVO */
	}
}

void TestPWM( void )
{
	static uint32_t duty = 0;
	HALIF_ControlPWM( ePWM_CH1, duty );
	HALIF_ControlPWM( ePWM_CH2, duty );
	HALIF_ControlPWM( ePWM_CH3, duty );
	HALIF_ControlPWM( ePWM_CH4, duty );
	duty += 10;
	if ( duty > 100 )
		duty = 0;

	return;
}

void TestGPIO( void )
{
	static bool bOn = false;
	bOn = bOn ? false : true;

	HALIF_setGPIO1( bOn );
	HALIF_setGPIO2( bOn );
	HALIF_setGPIO3( bOn );
	HALIF_setGPIO4( bOn );
}
