/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */

//#include "stm32u5xx_hal.h"
#include "dma.h"
#include "stm32u5xx_hal.h"
#include "error.h"
#include "spi.h"

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

  /* GPDMA1_REQUEST_SPI1_TX Init */
  handle_GPDMA1_Channel7.Instance = GPDMA1_Channel7;
  handle_GPDMA1_Channel7.Init.Request = GPDMA1_REQUEST_SPI1_TX;
  handle_GPDMA1_Channel7.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
  handle_GPDMA1_Channel7.Init.Direction = DMA_MEMORY_TO_PERIPH;
  handle_GPDMA1_Channel7.Init.SrcInc = DMA_SINC_INCREMENTED;
  handle_GPDMA1_Channel7.Init.DestInc = DMA_DINC_FIXED;
  handle_GPDMA1_Channel7.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_HALFWORD;
  handle_GPDMA1_Channel7.Init.DestDataWidth = DMA_DEST_DATAWIDTH_HALFWORD;
  handle_GPDMA1_Channel7.Init.Priority = DMA_LOW_PRIORITY_HIGH_WEIGHT;
  handle_GPDMA1_Channel7.Init.SrcBurstLength = 1;
  handle_GPDMA1_Channel7.Init.DestBurstLength = 1;
  handle_GPDMA1_Channel7.Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT1|DMA_DEST_ALLOCATED_PORT0;
  handle_GPDMA1_Channel7.Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
  handle_GPDMA1_Channel7.Init.Mode = DMA_NORMAL;
  if (HAL_DMA_Init(&handle_GPDMA1_Channel7) != HAL_OK)
  {
    Error_Handler();
  }

  __HAL_LINKDMA(&handle_SPI_1, hdmatx, handle_GPDMA1_Channel7);

  if (HAL_DMA_ConfigChannelAttributes(&handle_GPDMA1_Channel7, DMA_CHANNEL_NPRIV) != HAL_OK)
  {
    Error_Handler();
  }

  /* GPDMA1_REQUEST_SPI1_RX Init */
  handle_GPDMA1_Channel6.Instance = GPDMA1_Channel6;
  handle_GPDMA1_Channel6.Init.Request = GPDMA1_REQUEST_SPI1_RX;
  handle_GPDMA1_Channel6.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;
  handle_GPDMA1_Channel6.Init.Direction = DMA_PERIPH_TO_MEMORY;
  handle_GPDMA1_Channel6.Init.SrcInc = DMA_SINC_FIXED;
  handle_GPDMA1_Channel6.Init.DestInc = DMA_DINC_INCREMENTED;
  handle_GPDMA1_Channel6.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_HALFWORD;
  handle_GPDMA1_Channel6.Init.DestDataWidth = DMA_DEST_DATAWIDTH_HALFWORD;
  handle_GPDMA1_Channel6.Init.Priority = DMA_LOW_PRIORITY_HIGH_WEIGHT;
  handle_GPDMA1_Channel6.Init.SrcBurstLength = 1;
  handle_GPDMA1_Channel6.Init.DestBurstLength = 1;
  handle_GPDMA1_Channel6.Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT1|DMA_DEST_ALLOCATED_PORT0;
  handle_GPDMA1_Channel6.Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;
  handle_GPDMA1_Channel6.Init.Mode = DMA_NORMAL;
  if (HAL_DMA_Init(&handle_GPDMA1_Channel6) != HAL_OK)
  {
    Error_Handler();
  }

  __HAL_LINKDMA(&handle_SPI_1, hdmarx, handle_GPDMA1_Channel6);

  if (HAL_DMA_ConfigChannelAttributes(&handle_GPDMA1_Channel6, DMA_CHANNEL_NPRIV) != HAL_OK)
  {
    Error_Handler();
  }

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
