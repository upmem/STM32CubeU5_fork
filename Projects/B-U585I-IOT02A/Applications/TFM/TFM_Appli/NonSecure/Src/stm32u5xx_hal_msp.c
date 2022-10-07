/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         stm32u5xx_hal_msp.c
  * @brief        This file provides code for the MSP Initialization
  *               and de-Initialization codes.
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
//#include "main.h"
#include "stm32u5xx.h"
#include "error.h"
#include "string.h"
#include "dma.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */

/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  __HAL_RCC_PWR_CLK_ENABLE();

  /* System interrupt init*/

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}

/**
* @brief SPI MSP Initialization
* This function configures the hardware resources used in this example
* @param hspi: SPI handle pointer
* @retval None
*/
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(hspi->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspInit 0 */

  /* USER CODE END SPI1_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SPI1;
    PeriphClkInit.Spi1ClockSelection = RCC_SPI1CLKSOURCE_SYSCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();

    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**SPI1 GPIO Configuration
    PE14     ------> SPI1_MISO
    PE13     ------> SPI1_SCK
    PE15     ------> SPI1_MOSI
    PA4     ------> SPI1_NSS
    */
    GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_13|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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

   __HAL_LINKDMA(hspi, hdmatx, handle_GPDMA1_Channel7);

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

   __HAL_LINKDMA(hspi, hdmarx, handle_GPDMA1_Channel6);

   if (HAL_DMA_ConfigChannelAttributes(&handle_GPDMA1_Channel6, DMA_CHANNEL_NPRIV) != HAL_OK)
   {
     Error_Handler();
   }

    /* SPI1 interrupt Init */
    HAL_NVIC_SetPriority(SPI1_IRQn, PILOT_SPI_IRQ_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);
  /* USER CODE BEGIN SPI1_MspInit 1 */

  /* USER CODE END SPI1_MspInit 1 */
  }
  else if(hspi->Instance==SPI3)
  {
  /* USER CODE BEGIN SPI3_MspInit 0 */

  /* USER CODE END SPI3_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SPI3;
    PeriphClkInit.Spi3ClockSelection = RCC_SPI3CLKSOURCE_SYSCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_SPI3_CLK_ENABLE();

    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**SPI3 GPIO Configuration
    PG9     ------> SPI3_SCK
    PD6     ------> SPI3_MOSI
    PG10     ------> SPI3_MISO
    PG12     ------> SPI3_NSS
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* SPI3 interrupt Init */
    HAL_NVIC_SetPriority(SPI3_IRQn, PILOT_SPI_IRQ_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(SPI3_IRQn);
  /* USER CODE BEGIN SPI3_MspInit 1 */

  /* USER CODE END SPI3_MspInit 1 */
  }

}

/**
* @brief SPI MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hspi: SPI handle pointer
* @retval None
*/
void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
{
  if(hspi->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspDeInit 0 */

  /* USER CODE END SPI1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI1_CLK_DISABLE();

    /**SPI1 GPIO Configuration
    PE14     ------> SPI1_MISO
    PE13     ------> SPI1_SCK
    PE15     ------> SPI1_MOSI
    PA4     ------> SPI1_NSS
    */
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_14|GPIO_PIN_13|GPIO_PIN_15);

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4);

    /* SPI1 interrupt DeInit */
    HAL_NVIC_DisableIRQ(SPI1_IRQn);
  /* USER CODE BEGIN SPI1_MspDeInit 1 */

  /* USER CODE END SPI1_MspDeInit 1 */
  }
  else if(hspi->Instance==SPI3)
  {
  /* USER CODE BEGIN SPI3_MspDeInit 0 */

  /* USER CODE END SPI3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI3_CLK_DISABLE();

    /**SPI3 GPIO Configuration
    PG9     ------> SPI3_SCK
    PD6     ------> SPI3_MOSI
    PG10     ------> SPI3_MISO
    PG12     ------> SPI3_NSS
    */
    HAL_GPIO_DeInit(GPIOG, GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_12);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_6);

    /* SPI3 interrupt DeInit */
    HAL_NVIC_DisableIRQ(SPI3_IRQn);
  /* USER CODE BEGIN SPI3_MspDeInit 1 */

  /* USER CODE END SPI3_MspDeInit 1 */
  }

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
