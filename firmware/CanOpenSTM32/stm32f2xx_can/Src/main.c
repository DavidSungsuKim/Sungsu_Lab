/*
 * This is an example of CANopenNode library running on STM32H735G-DK board
 * It features STM32H735 MCU with FDCAN peripheral
 *
 * It runs super loop with most essential CANopenNode components configured
 *
 * Example was prepared and is maintained by: Tilen Majerle <tilen@majerle.eu>
 */
#include "main.h"
#include "comm.h"
#include "CANopen.h"
#include "OD.h"

/* Local functions */
void SystemClock_Config(void);
static void led_btn_init(void);
static void mpu_config(void);
static void process_leds(void);

/* Lwmem buffer for allocation */
static uint8_t lwmem_buffer[0x4000];
static const lwmem_region_t lwmem_default_regions[] = {
        {lwmem_buffer, sizeof(lwmem_buffer)},
        {NULL, 0}
};

/* Local variables */
static CO_t* CO;
static uint32_t co_heap_used;
static CO_NMT_reset_cmd_t reset = CO_RESET_COMM;
static uint8_t LED_red_status, LED_green_status;

/* Default values for CO_CANopenInit() */
#define NMT_CONTROL                     (CO_NMT_STARTUP_TO_OPERATIONAL | CO_NMT_ERR_ON_ERR_REG | CO_ERR_REG_GENERIC_ERR | CO_ERR_REG_COMMUNICATION)
#define FIRST_HB_TIME                   500
#define SDO_SRV_TIMEOUT_TIME            1000
#define SDO_CLI_TIMEOUT_TIME            500
#define SDO_CLI_BLOCK                   false
#define OD_STATUS_BITS                  NULL

/**
 * \brief           The application entry point
 */
int
main(void) {
    uint32_t time_old, time_current;

    /* Setup MPU to prevent any Cortex-M speculative access */
    mpu_config();

    __HAL_RCC_SYSCFG_CLK_ENABLE();

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();
    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all peripherals */
    lwmem_assignmem(lwmem_default_regions);

    led_btn_init();

    comm_init();
    comm_printf("CANopenNode application running on STM32H735G-DK\r\n");

#if defined (TEST)
    int i = 0;
    while(1)
    {
    	i++;
    }
    return 0;
#endif

    /* Initialize new instance of CANopen */
    if ((CO = CO_new(NULL, &co_heap_used)) == NULL) {
        comm_printf("Error: Could not allocate CO instance\r\n");
        Error_Handler();
    }
    comm_printf("CO allocated, uses %u bytes of heap memory\r\n", (unsigned)co_heap_used);

    CO_CANgetDriverHandler();

    /* Application main while loop */
    time_old = time_current = HAL_GetTick();
    while (1) {
        /* Process CANopen tasks */
        if (reset == CO_RESET_COMM) {
            uint32_t errInfo = 0;
            uint16_t pendingBitRate = 125;
            uint8_t pendingNodeId = 0x12, activeNodeId = 0;
            CO_ReturnError_t err;

            /* Wait rt_thread. */
            CO->CANmodule->CANnormal = false;

            /* Enter CAN configuration. */
            CO_CANsetConfigurationMode(CO->CANmodule->CANptr);
            CO_CANmodule_disable(CO->CANmodule);

            /* Initialize CANopen */
            if ((err = CO_CANinit(CO, CO->CANmodule->CANptr, pendingBitRate)) != CO_ERROR_NO) {
                comm_printf("Error: CAN initialization failed: %d\n", err);
                Error_Handler();
            }
            comm_printf("CAN initialized\r\n");

            /* Setup LSS */
            CO_LSS_address_t lssAddress = {
                .identity = {
                    .vendorID = OD_PERSIST_COMM.x1018_identity.vendor_ID,
                    .productCode = OD_PERSIST_COMM.x1018_identity.productCode,
                    .revisionNumber = OD_PERSIST_COMM.x1018_identity.revisionNumber,
                    .serialNumber = OD_PERSIST_COMM.x1018_identity.serialNumber
                }
            };
            if ((err = CO_LSSinit(CO, &lssAddress, &pendingNodeId, &pendingBitRate)) != CO_ERROR_NO) {
                comm_printf("Error: LSS slave initialization failed: %d\n", err);
                Error_Handler();
            }
            comm_printf("LSS initialized\r\n");

            /* Initialite core stack */
            activeNodeId = pendingNodeId;
            err = CO_CANopenInit(CO,                /* CANopen object */
                                 NULL,              /* alternate NMT */
                                 NULL,              /* alternate em */
                                 OD,                /* Object dictionary */
                                 OD_STATUS_BITS,    /* Optional OD_statusBits */
                                 NMT_CONTROL,       /* CO_NMT_control_t */
                                 FIRST_HB_TIME,     /* firstHBTime_ms */
                                 SDO_SRV_TIMEOUT_TIME, /* SDOserverTimeoutTime_ms */
                                 SDO_CLI_TIMEOUT_TIME, /* SDOclientTimeoutTime_ms */
                                 SDO_CLI_BLOCK,     /* SDOclientBlockTransfer */
                                 activeNodeId,
                                 &errInfo);
            if (err != CO_ERROR_NO && err != CO_ERROR_NODE_ID_UNCONFIGURED_LSS) {
                if (err == CO_ERROR_OD_PARAMETERS) {
                    comm_printf("Error: Object Dictionary entry 0x%X\n", (unsigned)errInfo);
                } else {
                    comm_printf("Error: CANopen initialization failed: %d\n", (int)err);
                }
                Error_Handler();
            }
            comm_printf("CANOpen initialized\r\n");

            /* Initialize PDO */
            err = CO_CANopenInitPDO(CO, CO->em, OD, activeNodeId, &errInfo);
            if (err != CO_ERROR_NO) {
                if (err == CO_ERROR_OD_PARAMETERS) {
                    comm_printf("Error: Object Dictionary entry 0x%X\n", (unsigned)errInfo);
                } else {
                    comm_printf("Error: PDO initialization failed: %d\n", (int)err);
                }
                Error_Handler();
            }
            comm_printf("CAN PDO initialized\r\n");

            /* Configure Timer interrupt function for execution every 1 millisecond */

            /* Configure CANopen callbacks, etc */
            if (!CO->nodeIdUnconfigured) {
    #if (CO_CONFIG_STORAGE) & CO_CONFIG_STORAGE_ENABLE
                if (storageInitError != 0) {
                    CO_errorReport(CO->em, CO_EM_NON_VOLATILE_MEMORY,
                                   CO_EMC_HARDWARE, storageInitError);
                }
    #endif
            } else {
                comm_printf("CANopenNode - Node-id not initialized\n");
                Error_Handler();
            }

            /* Start CAN to receive messages */
            CO_CANsetNormalMode(CO->CANmodule);

            reset = CO_RESET_NOT;
            comm_printf("CANopenNode - Running and ready to communicate...\n");
        }

        /*
         * Process CANopenNode each millisecond
         *
         * For more frequent processing, a higher precision timer
         * could be configured on STM32 (TIM6 for example) with
         * 1MHz tick
         */
        if (reset == CO_RESET_NOT
                && (time_current = HAL_GetTick()) - time_old > 0) {
            uint32_t timeDifference_us = (time_current - time_old) * 1000;
            time_old = time_current;

            /* CANopen process */
            reset = CO_process(CO, false, timeDifference_us, NULL);

            process_leds();
            /*
             * Timer periodic actions
             *
             * These should be put to fixed-period timer instead
             */
            CO_LOCK_OD(CO->CANmodule);
            if (!CO->nodeIdUnconfigured && CO->CANmodule->CANnormal) {
                bool_t syncWas = false;

#if (CO_CONFIG_SYNC) & CO_CONFIG_SYNC_ENABLE
                syncWas = CO_process_SYNC(CO, timeDifference_us, NULL);
#endif
#if (CO_CONFIG_PDO) & CO_CONFIG_RPDO_ENABLE
                CO_process_RPDO(CO, syncWas, timeDifference_us, NULL);
#endif
#if (CO_CONFIG_PDO) & CO_CONFIG_TPDO_ENABLE
                CO_process_TPDO(CO, syncWas, timeDifference_us, NULL);
#endif
            }
            CO_UNLOCK_OD(CO->CANmodule);
        }

        /* Check what is happening with reset */
        if (reset != CO_RESET_NOT && reset != CO_RESET_COMM) {
            /* Analyze reset status and exit.. */
            /* and save data to non-volatile memory */
            NVIC_SystemReset();
        }

        /* Other application tasks... */
    }
}

/**
 * \brief System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 120000000
  *            HCLK(Hz)                       = 120000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 240
  *            PLL_P                          = 2
  *            PLL_Q                          = 5
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 3
  * @param  None
  * @retval None
  */
void
SystemClock_Config(void) {
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 240;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 5;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
	 clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
}

/**
 * \brief           MPU configuration
 */
static void
mpu_config(void) {
#if defined (CODES_FOR_STM32F7)
	MPU_Region_InitTypeDef MPU_InitStruct = {0};

    /* Setup background region, no access to normal memory to prevent any speculative access */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER0;
    MPU_InitStruct.BaseAddress = 0;
    MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
    MPU_InitStruct.SubRegionDisable = 0x87;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* Enable the MPU, use default memory access for regions not defined here */
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
#endif
}

static void process_leds(void)	{
#if defined (CODES_FOR_STM32F7)
            /* Process LEDs and react only on change */
            LED_red_status = CO_LED_RED(CO->LEDs, CO_LED_CANopen);
            LED_green_status = CO_LED_GREEN(CO->LEDs, CO_LED_CANopen);
            if (LED_red_status && !LL_GPIO_IsOutputPinSet(LED_RED_GPIO_Port, LED_RED_Pin)) {
                LL_GPIO_SetOutputPin(LED_RED_GPIO_Port, LED_RED_Pin);
            } else if (!LED_red_status && LL_GPIO_IsOutputPinSet(LED_RED_GPIO_Port, LED_RED_Pin)) {
                LL_GPIO_ResetOutputPin(LED_RED_GPIO_Port, LED_RED_Pin);
            }
            if (LED_green_status && !LL_GPIO_IsOutputPinSet(LED_GREEN_GPIO_Port, LED_GREEN_Pin)) {
                LL_GPIO_SetOutputPin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
            } else if (!LED_green_status && LL_GPIO_IsOutputPinSet(LED_GREEN_GPIO_Port, LED_GREEN_Pin)) {
                LL_GPIO_ResetOutputPin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
            }
#endif
}

/**
 * \brief GPIO Initialization Function
 */
void
led_btn_init(void) {
#if defined (CODES_FOR_STM32F7)
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOH);
    LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOC);
    LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOD);

    /* Disable leds by default */
    LL_GPIO_ResetOutputPin(GPIOC, LED_RED_Pin | LED_GREEN_Pin);

    /* Configure button as input */
    GPIO_InitStruct.Pin = BTN_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
    LL_GPIO_Init(BTN_GPIO_Port, &GPIO_InitStruct);

    /* Configure leds as outputs  */
    GPIO_InitStruct.Pin = LED_RED_Pin | LED_GREEN_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOC, &GPIO_InitStruct);
#endif
}

/**
 * \brief           This function is executed in case of error occurrence
 */
void
Error_Handler(void) {
    __disable_irq();
    while (1) {

    }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
