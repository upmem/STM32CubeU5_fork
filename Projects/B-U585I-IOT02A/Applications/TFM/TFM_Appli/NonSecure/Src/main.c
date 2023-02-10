/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @brief   Main application file.
  *          This application demonstrates Firmware Update, protections
  *          and crypto testing functionalities.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "com.h"
#include "common.h"
#include "flash_layout.h"
/* Avoids the semihosting issue */
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
__asm("  .global __ARM_use_no_argv\n");
#endif /* defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050) */

#include "test_protections.h"
#include "fw_update_app.h"
#include "tfm_app.h"
#include "ns_data.h"

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "commit.h"

#include "dma.h"
#include "spi.h"
#include "i2c.h"
#include "system.h"
#include "error.h"
#include "gi_cmd_handler.h"


#include "pilot_config.h"
#include "board_config.h"

#include "tfm_ns_interface.h"
/* Temporary includes */
#include "crypto_tests_common.h"

QueueHandle_t host_requests_queue = NULL;


#ifdef __ICCARM__
__no_init volatile uint32_t TestNumber;
#else
volatile uint32_t TestNumber  __attribute__((section(".bss.NoInit"))) ;
#endif /* __ICCARM__ */

static void uart_putc(unsigned char c)
{
  COM_Transmit(&c, 1, 1000U);
}
#define HOST_REQUESTS_QUEUE_SIZE		(10)
static void RTOS_Init(void);

/* Redirects printf to TFM_DRIVER_STDIO in case of ARMCLANG*/
#if defined(__ARMCC_VERSION)
FILE __stdout;

/* __ARMCC_VERSION is only defined starting from Arm compiler version 6 */
int fputc(int ch, FILE *f)
{
  /* Send byte to USART */
  uart_putc(ch);

  /* Return character written */
  return ch;
}
#elif defined(__GNUC__)
/* Redirects printf to TFM_DRIVER_STDIO in case of GNUARM */
int _write(int fd, char *str, int len)
{
  int i;

  for (i = 0; i < len; i++)
  {
    /* Send byte to USART */
    uart_putc(str[i]);
  }

  /* Return the number of characters written */
  return len;
}
#elif defined(__ICCARM__)
int putchar(int ch)
{
  /* Send byte to USART */
  uart_putc(ch);

  /* Return character written */
  return ch;
}
#endif /*  __GNUC__ */

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(int argc, char **argv)
/*int main(void) */
{
  char version[] = "0.1";
  /* STM32U5xx HAL library initialization:
  - Systick timer is configured by default as source of time base, but user
  can eventually implement his proper time base source (a general purpose
  timer for example or other time source), keeping in mind that Time base
  duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
  handled in milliseconds basis.
  - Set NVIC Group Priority to 3
  - Low Level Initialization
  */
  HAL_Init();

  /* DeInitialize RCC to allow PLL reconfiguration when configuring system clock */
  HAL_RCC_DeInit();

  /* ICACHE is enabled in SBSFU */

  /* Configure the system clock */
  SystemClock_Config();
  /* Configure the System Power */
  SystemPower_Config();
  /* Configure Communication module */
  COM_Init();

  printf("\r\nApp version %s %s\r\n", version, commit);

  /* Configure DMA */
  DMA_Init();

  /* Configure SPI interfaces */
  SPI_Init();

  /* Configure I2C interfaces */
  I2C_Init();
  I2C_Master_scan(); // TODO : move this after all init done ?

  /* Attach DMA to peripherals (SPI, ...) */

  tfm_ns_interface_init();

  /* test if an automatic test protection is launched */
  if (((TestNumber & TEST_PROTECTION_MASK)==TEST_PROTECTION_MASK) ||
     ((TestNumber & TEST_PROTECTION_FAILED)==TEST_PROTECTION_FAILED))
  {
    TEST_PROTECTIONS_Run_SecUserMem();
  }

  if (
      (gi_init(DPU_DRAM_MASK_0) != PILOT_SUCCESS) ||
      (gi_check_lnke_status(DPU_DRAM_MASK_0) != PILOT_SUCCESS)
      ) {
      // TODO : do this for all SS pins in real HW.
      Error_Handler();
  }
  printf ("Non-secure image init all good\r\n");

  /* TODO uncomment it */
#ifdef  PILOT_RTOS_SUPPORT
  RTOS_Init();
#endif
  while (1U)
  {}

}

static void RTOS_Init(void) {
  /* PendSV_IRQn interrupt configuration */
  HAL_NVIC_SetPriority( PendSV_IRQn, PILOT_PENDSV_IRQ_PRIORITY, 0 );

  host_requests_queue = xQueueCreate(HOST_REQUESTS_QUEUE_SIZE, sizeof(uint32_t));

  if (xTaskCreate(gi_task_mailbox_polling, "mailbox_polling", configMINIMAL_STACK_SIZE, NULL, 50, NULL) != pdPASS) {
      Error_Handler();
  }
  if (xTaskCreate(gi_task_dpu_load, "dpu loading with facsimile", configMINIMAL_STACK_SIZE, NULL, 2, NULL) != pdPASS) {
      Error_Handler();
  }
  if (xTaskCreate(gi_task_fake_request, "trigger a fake host request", configMINIMAL_STACK_SIZE, NULL, 50, NULL) != pdPASS) {
      Error_Handler();
  }
  /* Start scheduler */
  vTaskStartScheduler();
  /* we should never reach here */
  while(1);
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
  /* User can add his own implementation to report the file name and line number,
   ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1U)
  {
  }
}
#endif /* USE_FULL_ASSERT */

/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
 * used by the Idle task. */
void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                    StackType_t ** ppxIdleTaskStackBuffer,
                                    uint32_t * pulIdleTaskStackSize )
{
    /* If the buffers to be provided to the Idle task are declared inside this
     * function then they must be declared static - otherwise they will be allocated on
     * the stack and so not exists after this function exits. */
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
     * state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
 * application must provide an implementation of vApplicationGetTimerTaskMemory()
 * to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                     StackType_t ** ppxTimerTaskStackBuffer,
                                     uint32_t * pulTimerTaskStackSize )
{
    /* If the buffers to be provided to the Timer task are declared inside this
     * function then they must be declared static - otherwise they will be allocated on
     * the stack and so not exists after this function exits. */
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
     * task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

#if configUSE_IDLE_HOOK == 1
/* The idle hook will only get called if configUSE_IDLE_HOOK is set to 1 within FreeRTOSConfig.h.
 * In the following example, not working in this project, the Idle Hook refresh a Watchdog */
void vApplicationIdleHook( void )
{
    /* Check / pet the watchdog */
    if( pxHwndIwdg != NULL )
    {
        HAL_IWDG_Refresh( pxHwndIwdg );
    }
}
#endif /* configUSE_IDLE_HOOK == 1 */
/*-----------------------------------------------------------*/


/* Override HAL Tick weak functions */
//HAL_StatusTypeDef HAL_InitTick( uint32_t TickPriority )
//{
//    ( void ) TickPriority;
//    ( void ) SysTick_Config( SystemCoreClock / 1000 );
//    return HAL_OK;
//}


/**
  * @}
  */

/**
  * @}
  */
