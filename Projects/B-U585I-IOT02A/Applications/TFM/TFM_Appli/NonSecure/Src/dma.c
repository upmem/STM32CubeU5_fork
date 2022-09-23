/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */

//#include "stm32u5xx_hal.h"
#include "dma.h"
#include "stm32u5xx_hal.h"
#include "error.h"

DMA_HandleTypeDef handle_GPDMA1_Channel3;
DMA_HandleTypeDef handle_GPDMA1_Channel4;

DMA_HandleTypeDef handle_LPDMA1_Channel0;
DMA_HandleTypeDef handle_LPDMA1_Channel1;

/**
  * @brief GPDMA1 Initialization Function
  * @param None
  * @retval None
  */
void MX_GPDMA1_Init(void)
{
  /* USER CODE BEGIN GPDMA1_Init 0 */

  /* USER CODE END GPDMA1_Init 0 */

  /* Peripheral clock enable */
  __HAL_RCC_GPDMA1_CLK_ENABLE();

  /* GPDMA1 interrupt Init */
  HAL_NVIC_SetPriority(GPDMA1_Channel3_IRQn, PILOT_GPDMA_IRQ_PRIORITY, 0);
  HAL_NVIC_EnableIRQ(GPDMA1_Channel3_IRQn);
  HAL_NVIC_SetPriority(GPDMA1_Channel4_IRQn, PILOT_GPDMA_IRQ_PRIORITY, 0);
  HAL_NVIC_EnableIRQ(GPDMA1_Channel4_IRQn);
  HAL_NVIC_SetPriority(GPDMA1_Channel5_IRQn, PILOT_GPDMA_IRQ_PRIORITY, 0);
  HAL_NVIC_EnableIRQ(GPDMA1_Channel5_IRQn);
  HAL_NVIC_SetPriority(GPDMA1_Channel6_IRQn, PILOT_GPDMA_IRQ_PRIORITY, 0);
  HAL_NVIC_EnableIRQ(GPDMA1_Channel6_IRQn);
  /* USER CODE BEGIN GPDMA1_Init 1 */

  /* USER CODE END GPDMA1_Init 1 */
  handle_GPDMA1_Channel3.Instance = GPDMA1_Channel3;
  handle_GPDMA1_Channel3.InitLinkedList.Priority = DMA_LOW_PRIORITY_HIGH_WEIGHT;
  handle_GPDMA1_Channel3.InitLinkedList.LinkStepMode = DMA_LSM_FULL_EXECUTION;
  handle_GPDMA1_Channel3.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT1;
  handle_GPDMA1_Channel3.InitLinkedList.TransferEventMode = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
  handle_GPDMA1_Channel3.InitLinkedList.LinkedListMode = DMA_LINKEDLIST_NORMAL;
  if (HAL_DMAEx_List_Init(&handle_GPDMA1_Channel3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DMA_ConfigChannelAttributes(&handle_GPDMA1_Channel3, DMA_CHANNEL_NPRIV) != HAL_OK)
  {
    Error_Handler();
  }
  handle_GPDMA1_Channel4.Instance = GPDMA1_Channel4;
  handle_GPDMA1_Channel4.InitLinkedList.Priority = DMA_LOW_PRIORITY_HIGH_WEIGHT;
  handle_GPDMA1_Channel4.InitLinkedList.LinkStepMode = DMA_LSM_FULL_EXECUTION;
  handle_GPDMA1_Channel4.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT1;
  handle_GPDMA1_Channel4.InitLinkedList.TransferEventMode = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
  handle_GPDMA1_Channel4.InitLinkedList.LinkedListMode = DMA_LINKEDLIST_NORMAL;
  if (HAL_DMAEx_List_Init(&handle_GPDMA1_Channel4) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DMA_ConfigChannelAttributes(&handle_GPDMA1_Channel4, DMA_CHANNEL_NPRIV) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN GPDMA1_Init 2 */

  /* USER CODE END GPDMA1_Init 2 */

}

#define DMAx_CLKAM_ENABLE()             __HAL_RCC_LPDMA1_CLKAM_ENABLE()
#define DMAx_CLK_SLEEP_ENABLE()         __HAL_RCC_LPDMA1_CLK_SLEEP_ENABLE()

static void MX_LPDMA1_Init(void)
{

  /* USER CODE BEGIN LPDMA1_Init 0 */
    DMAx_CLKAM_ENABLE();
    DMAx_CLK_SLEEP_ENABLE();
  /* USER CODE END LPDMA1_Init 0 */

  /* Peripheral clock enable */
  __HAL_RCC_LPDMA1_CLK_ENABLE();

  /* LPDMA1 interrupt Init */
    HAL_NVIC_SetPriority(LPDMA1_Channel0_IRQn, PILOT_LPDMA_IRQ_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(LPDMA1_Channel0_IRQn);
    HAL_NVIC_SetPriority(LPDMA1_Channel1_IRQn, PILOT_LPDMA_IRQ_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(LPDMA1_Channel1_IRQn);

  /* USER CODE BEGIN LPDMA1_Init 1 */

  /* USER CODE END LPDMA1_Init 1 */
  handle_LPDMA1_Channel0.Instance = LPDMA1_Channel0;
  handle_LPDMA1_Channel0.InitLinkedList.Priority = DMA_LOW_PRIORITY_HIGH_WEIGHT;
  handle_LPDMA1_Channel0.InitLinkedList.LinkStepMode = DMA_LSM_FULL_EXECUTION;
  handle_LPDMA1_Channel0.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT1;
  handle_LPDMA1_Channel0.InitLinkedList.TransferEventMode = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
  handle_LPDMA1_Channel0.InitLinkedList.LinkedListMode = DMA_LINKEDLIST_NORMAL;
  if (HAL_DMAEx_List_Init(&handle_LPDMA1_Channel0) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DMA_ConfigChannelAttributes(&handle_LPDMA1_Channel0, DMA_CHANNEL_NPRIV) != HAL_OK)
  {
    Error_Handler();
  }
  handle_LPDMA1_Channel1.Instance = LPDMA1_Channel1;
  handle_LPDMA1_Channel1.InitLinkedList.Priority = DMA_LOW_PRIORITY_HIGH_WEIGHT;
  handle_LPDMA1_Channel1.InitLinkedList.LinkStepMode = DMA_LSM_FULL_EXECUTION;
  handle_LPDMA1_Channel1.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT1;
  handle_LPDMA1_Channel1.InitLinkedList.TransferEventMode = DMA_TCEM_LAST_LL_ITEM_TRANSFER;
  handle_LPDMA1_Channel1.InitLinkedList.LinkedListMode = DMA_LINKEDLIST_NORMAL;
  if (HAL_DMAEx_List_Init(&handle_LPDMA1_Channel1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DMA_ConfigChannelAttributes(&handle_LPDMA1_Channel1, DMA_CHANNEL_NPRIV) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPDMA1_Init 2 */

  /* USER CODE END LPDMA1_Init 2 */

}


void DMA_Init(void)
{
  MX_GPDMA1_Init();
  MX_LPDMA1_Init();
}
