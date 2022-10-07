/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */

//#include "stm32u5xx_hal.h"
#include "dma.h"
#include "stm32u5xx_hal.h"
#include "error.h"

DMA_HandleTypeDef handle_GPDMA1_Channel7;
DMA_HandleTypeDef handle_GPDMA1_Channel6;

/**
  * @brief GPDMA1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPDMA1_Init(void)
{
  /* Peripheral clock enable */
  __HAL_RCC_GPDMA1_CLK_ENABLE();

  /* GPDMA1 interrupt Init */
    HAL_NVIC_SetPriority(GPDMA1_Channel6_IRQn, PILOT_GPDMA_IRQ_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel6_IRQn);
    HAL_NVIC_SetPriority(GPDMA1_Channel7_IRQn, PILOT_GPDMA_IRQ_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel7_IRQn);
}


void DMA_Init(void)
{
  MX_GPDMA1_Init();
}
