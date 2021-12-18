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
#include "commPi.h"

#include "os.h"

/********************************* Const *********************************/

/********************************* Types *********************************/

/********************************* Macro *********************************/
#define FW_VERSION			"V00.00.00"

#define TASK_PRIORITY(x) 	(tskIDLE_PRIORITY + x)			// The bigger number the higher priority?
//#define TEST_SPI
//#define TEST_PWM
//#define TEST_SERVO

/***************************** Local Variable ****************************/
static SemaphoreHandle_t g_sem;
static CServo g_servo( 10.0, 1.0, 2.0, 90.0 );

/**************************** Global Variable ****************************/

/************************* Function Declaration **************************/
static void		Welcome				( void );
static void 	LEDTurnOnTask     	( void *pvParameters );
static void 	LEDTurnOffTask    	( void *pvParameters );
static void		TestTask			( void *pvparameters );
static void		TestPWM				( void );
static void 	TestGPIO			( void );

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

	xTaskCreate( COMM_PI_task, 		"CommPiTask",		configMINIMAL_STACK_SIZE, NULL, TASK_PRIORITY(1), NULL );
	xTaskCreate( LEDTurnOnTask,  	"LEDTurnOnTask",  	configMINIMAL_STACK_SIZE, NULL, TASK_PRIORITY(2), NULL );
	xTaskCreate( LEDTurnOffTask, 	"LEDTurnOffTask", 	configMINIMAL_STACK_SIZE, NULL, TASK_PRIORITY(1), NULL );
	xTaskCreate( TestTask, 			"TestTask",			configMINIMAL_STACK_SIZE, NULL, TASK_PRIORITY(1), NULL );

	Welcome();
	_printf("Run scheduler...\r\n");
	vTaskStartScheduler();
}

static void Welcome( void )
{
	_printf("---------------------------\r\n");
	_printf("FreeRTOS_F401RE Project\r\n");
	_printf("---------------------------\r\n");
	_printf("  version=%s\r\n\r\n", FW_VERSION);
}

static void LEDTurnOnTask( void *pvParameters )
{
	_printf("%s runs...\r\n", __FUNCTION__ );

	TickType_t tickWait = portMAX_DELAY;

    for(;;)
    {    
      xSemaphoreTake( g_sem, tickWait );
     
      HALIF_TurnOnLED1();
      vTaskDelay(100);

      xSemaphoreGive( g_sem );
      vTaskDelay(1);
    }
}

static void LEDTurnOffTask( void *pvParameters )
{
	_printf("%s runs...\r\n", __FUNCTION__ );

    TickType_t tickWait = portMAX_DELAY;
          
    for(;;)
    {    
      xSemaphoreTake( g_sem, tickWait );

      HALIF_TurnOffLED1();
      vTaskDelay(1000);

      xSemaphoreGive( g_sem );
    }
}

static void TestTask( void *pvparameters )
{
	_printf("%s runs...\r\n", __FUNCTION__ );

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

static void TestPWM( void )
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

static void TestGPIO( void )
{
	static bool bOn = false;
	bOn = bOn ? false : true;

	HALIF_setGPIO1( bOn );
	HALIF_setGPIO2( bOn );
	HALIF_setGPIO3( bOn );
	HALIF_setGPIO4( bOn );
}
