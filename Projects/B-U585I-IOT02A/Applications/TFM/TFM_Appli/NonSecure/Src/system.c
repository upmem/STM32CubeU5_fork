/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */

#include "system.h"
#include "bitops.h"

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = OSC_INIT_OSCILLATORTYPE;
  RCC_OscInitStruct.HSI48State = OSC_INIT_HSI48STATE;
  RCC_OscInitStruct.MSIState = OSC_INIT_MSISTATE;
  RCC_OscInitStruct.MSICalibrationValue = OSC_INIT_MSICALIBRATION;
  RCC_OscInitStruct.MSIClockRange = OSC_INIT_MSIRANGE;
  RCC_OscInitStruct.PLL.PLLState = OSC_INIT_PLLSTATE;
  RCC_OscInitStruct.PLL.PLLSource = OSC_INIT_PLLSOURCE;
  RCC_OscInitStruct.PLL.PLLMBOOST = OSC_INIT_PLLMBOOST;
  RCC_OscInitStruct.PLL.PLLM = OSC_INIT_PLLM;
  RCC_OscInitStruct.PLL.PLLN = OSC_INIT_PLLN;
  RCC_OscInitStruct.PLL.PLLP = OSC_INIT_PLLP;
  RCC_OscInitStruct.PLL.PLLQ = OSC_INIT_PLLQ;
  RCC_OscInitStruct.PLL.PLLR = OSC_INIT_PLLR;
  RCC_OscInitStruct.PLL.PLLRGE = OSC_INIT_PLLRGE;
  RCC_OscInitStruct.PLL.PLLFRACN = OSC_INIT_PLLFRACN;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = CLK_INIT_CLOCKTYPE;
  RCC_ClkInitStruct.SYSCLKSource = CLK_INIT_SYSCLKSOURCE;
  RCC_ClkInitStruct.AHBCLKDivider = CLK_INIT_AHBCLKDIV;
  RCC_ClkInitStruct.APB1CLKDivider = CLK_INIT_APB1CLKDIV;
  RCC_ClkInitStruct.APB2CLKDivider = CLK_INIT_APB2LKDIV;
  RCC_ClkInitStruct.APB3CLKDivider = CLK_INIT_APB3LKDIV;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY) != HAL_OK)
  {
    Error_Handler();
  }
}
/**
  * @brief Power Configuration
  * @retval None
  */
void SystemPower_Config(void)
{
  HAL_PWREx_EnableVddIO2();

  /*
   * Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral
   */
  HAL_PWREx_DisableUCPDDeadBattery();

#ifdef USE_SMPS
  /* Switch to SMPS regulator instead of LDO */
  if (HAL_PWREx_ConfigSupply(PWR_SMPS_SUPPLY)!= HAL_OK)
  {
    Error_Handler();
  }
#endif

}

pilot_error_t check_timeout (uint32_t timestamp, uint32_t timeout) {
  pilot_error_t ret = PILOT_SUCCESS;
  if (timeout != HAL_MAX_DELAY)
  {
    if ((HAL_GetTick() - timestamp) < timeout)
    {
      ret = PILOT_FAILURE;
    }
  }
  return ret;
}
