/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */

#include "spi.h"
#include "stm32u5xx_hal.h"
#include "error.h"
#include "stdio.h"
SPI_HandleTypeDef handle_SPI_1;
SPI_HandleTypeDef handle_SPI_3;

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
//static void MX_SPI1_Init(void)
//{
//
//  /* USER CODE BEGIN SPI1_Init 0 */
//
//  /* USER CODE END SPI1_Init 0 */
//
//  SPI_AutonomousModeConfTypeDef HAL_SPI_AutonomousMode_Cfg_Struct = {0};
//
//  /* USER CODE BEGIN SPI1_Init 1 */
//
//  /* USER CODE END SPI1_Init 1 */
//  /* SPI1 parameter configuration*/
//  handle_SPI_1.Instance = SPI1;
//  handle_SPI_1.Init.Mode = SPI_MODE_MASTER;
//  handle_SPI_1.Init.Direction = SPI_DIRECTION_2LINES;
//  handle_SPI_1.Init.DataSize = SPI_DATASIZE_8BIT;
//  handle_SPI_1.Init.CLKPolarity = SPI_POLARITY_LOW;
//  handle_SPI_1.Init.CLKPhase = SPI_PHASE_1EDGE;
//  handle_SPI_1.Init.NSS = SPI_NSS_HARD_OUTPUT;//SPI_NSS_SOFT;
//  handle_SPI_1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
//  handle_SPI_1.Init.FirstBit = SPI_FIRSTBIT_MSB;
//  handle_SPI_1.Init.TIMode = SPI_TIMODE_DISABLE;
//  handle_SPI_1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
//  handle_SPI_1.Init.CRCPolynomial = 0x7;
//  handle_SPI_1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
//  handle_SPI_1.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
//  handle_SPI_1.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
//  handle_SPI_1.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
//  handle_SPI_1.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
//  handle_SPI_1.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
//  handle_SPI_1.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
//  handle_SPI_1.Init.IOSwap = SPI_IO_SWAP_DISABLE;
//  handle_SPI_1.Init.ReadyMasterManagement = SPI_RDY_MASTER_MANAGEMENT_INTERNALLY;
//  handle_SPI_1.Init.ReadyPolarity = SPI_RDY_POLARITY_HIGH;
//  if (HAL_SPI_Init(&handle_SPI_1) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  HAL_SPI_AutonomousMode_Cfg_Struct.TriggerState = SPI_AUTO_MODE_DISABLE;
//  HAL_SPI_AutonomousMode_Cfg_Struct.TriggerSelection = SPI_GRP1_GPDMA_CH0_TCF_TRG;
//  HAL_SPI_AutonomousMode_Cfg_Struct.TriggerPolarity = SPI_TRIG_POLARITY_RISING;
//  if (HAL_SPIEx_SetConfigAutonomousMode(&handle_SPI_1, &HAL_SPI_AutonomousMode_Cfg_Struct) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /* USER CODE BEGIN SPI1_Init 2 */
//
//  /* USER CODE END SPI1_Init 2 */
//
//}
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  SPI_AutonomousModeConfTypeDef HAL_SPI_AutonomousMode_Cfg_Struct = {0};

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  handle_SPI_1.Instance = SPI1;
  handle_SPI_1.Init.Mode = SPI_MODE_MASTER;
  handle_SPI_1.Init.Direction = SPI_DIRECTION_2LINES;
  handle_SPI_1.Init.DataSize = SPI_DATASIZE_8BIT;
  handle_SPI_1.Init.CLKPolarity = SPI_POLARITY_LOW;
  handle_SPI_1.Init.CLKPhase = SPI_PHASE_1EDGE;
  handle_SPI_1.Init.NSS = SPI_NSS_HARD_OUTPUT;
  handle_SPI_1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  handle_SPI_1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  handle_SPI_1.Init.TIMode = SPI_TIMODE_DISABLE;
  handle_SPI_1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  handle_SPI_1.Init.CRCPolynomial = 0x7;
  handle_SPI_1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  handle_SPI_1.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  handle_SPI_1.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  handle_SPI_1.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  handle_SPI_1.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  handle_SPI_1.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  handle_SPI_1.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  handle_SPI_1.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  handle_SPI_1.Init.ReadyMasterManagement = SPI_RDY_MASTER_MANAGEMENT_INTERNALLY;
  handle_SPI_1.Init.ReadyPolarity = SPI_RDY_POLARITY_HIGH;
  if (HAL_SPI_Init(&handle_SPI_1) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_SPI_AutonomousMode_Cfg_Struct.TriggerState = SPI_AUTO_MODE_DISABLE;
  HAL_SPI_AutonomousMode_Cfg_Struct.TriggerSelection = SPI_GRP1_GPDMA_CH0_TCF_TRG;
  HAL_SPI_AutonomousMode_Cfg_Struct.TriggerPolarity = SPI_TRIG_POLARITY_RISING;
  if (HAL_SPIEx_SetConfigAutonomousMode(&handle_SPI_1, &HAL_SPI_AutonomousMode_Cfg_Struct) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

//static void MX_SPI3_Init(void)
//{
//
//  /* USER CODE BEGIN SPI3_Init 0 */
//
//  /* USER CODE END SPI3_Init 0 */
//
//  SPI_AutonomousModeConfTypeDef HAL_SPI_AutonomousMode_Cfg_Struct = {0};
//
//  /* USER CODE BEGIN SPI3_Init 1 */
//
//  /* USER CODE END SPI3_Init 1 */
//  /* SPI3 parameter configuration*/
//  handle_SPI_3.Instance = SPI3;
//  handle_SPI_3.Init.Mode = SPI_MODE_SLAVE;
//  handle_SPI_3.Init.Direction = SPI_DIRECTION_2LINES;
//  handle_SPI_3.Init.DataSize = SPI_DATASIZE_8BIT;
//  handle_SPI_3.Init.CLKPolarity = SPI_POLARITY_LOW;
//  handle_SPI_3.Init.CLKPhase = SPI_PHASE_1EDGE;
//  handle_SPI_3.Init.NSS = SPI_NSS_HARD_INPUT;//SPI_NSS_SOFT;
//  handle_SPI_3.Init.FirstBit = SPI_FIRSTBIT_MSB;
//  handle_SPI_3.Init.TIMode = SPI_TIMODE_DISABLE;
//  handle_SPI_3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
//  handle_SPI_3.Init.CRCPolynomial = 0x7;
//  handle_SPI_3.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
//  handle_SPI_3.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
//  handle_SPI_3.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
//  handle_SPI_3.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
//  handle_SPI_3.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
//  handle_SPI_3.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
//  handle_SPI_3.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
//  handle_SPI_3.Init.IOSwap = SPI_IO_SWAP_DISABLE;
//  handle_SPI_3.Init.ReadyMasterManagement = SPI_RDY_MASTER_MANAGEMENT_INTERNALLY;
//  handle_SPI_3.Init.ReadyPolarity = SPI_RDY_POLARITY_HIGH;
//  if (HAL_SPI_Init(&handle_SPI_3) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  HAL_SPI_AutonomousMode_Cfg_Struct.TriggerState = SPI_AUTO_MODE_DISABLE;
//#warning this is strange, why GPDMA0 and not SPI_GRP2_LPDMA_CH0_TCF_TRG?
//  HAL_SPI_AutonomousMode_Cfg_Struct.TriggerSelection = SPI_GRP1_GPDMA_CH0_TCF_TRG;
//  HAL_SPI_AutonomousMode_Cfg_Struct.TriggerPolarity = SPI_TRIG_POLARITY_RISING;
//  if (HAL_SPIEx_SetConfigAutonomousMode(&handle_SPI_3, &HAL_SPI_AutonomousMode_Cfg_Struct) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /* USER CODE BEGIN SPI3_Init 2 */
//
//  /* USER CODE END SPI3_Init 2 */
//
//}
static void MX_SPI3_Init(void)
{

  /* USER CODE BEGIN SPI3_Init 0 */

  /* USER CODE END SPI3_Init 0 */

  SPI_AutonomousModeConfTypeDef HAL_SPI_AutonomousMode_Cfg_Struct = {0};

  /* USER CODE BEGIN SPI3_Init 1 */

  /* USER CODE END SPI3_Init 1 */
  /* SPI3 parameter configuration*/
  handle_SPI_3.Instance = SPI3;
  handle_SPI_3.Init.Mode = SPI_MODE_SLAVE;
  handle_SPI_3.Init.Direction = SPI_DIRECTION_2LINES;
  handle_SPI_3.Init.DataSize = SPI_DATASIZE_8BIT;
  handle_SPI_3.Init.CLKPolarity = SPI_POLARITY_LOW;
  handle_SPI_3.Init.CLKPhase = SPI_PHASE_1EDGE;
  handle_SPI_3.Init.NSS = SPI_NSS_HARD_INPUT;
  handle_SPI_3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  handle_SPI_3.Init.TIMode = SPI_TIMODE_DISABLE;
  handle_SPI_3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  handle_SPI_3.Init.CRCPolynomial = 0x7;
  handle_SPI_3.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  handle_SPI_3.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  handle_SPI_3.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  handle_SPI_3.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  handle_SPI_3.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  handle_SPI_3.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  handle_SPI_3.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  handle_SPI_3.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  handle_SPI_3.Init.ReadyMasterManagement = SPI_RDY_MASTER_MANAGEMENT_INTERNALLY;
  handle_SPI_3.Init.ReadyPolarity = SPI_RDY_POLARITY_HIGH;
  if (HAL_SPI_Init(&handle_SPI_3) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_SPI_AutonomousMode_Cfg_Struct.TriggerState = SPI_AUTO_MODE_DISABLE;
  HAL_SPI_AutonomousMode_Cfg_Struct.TriggerSelection = SPI_GRP1_GPDMA_CH0_TCF_TRG;
  HAL_SPI_AutonomousMode_Cfg_Struct.TriggerPolarity = SPI_TRIG_POLARITY_RISING;
  if (HAL_SPIEx_SetConfigAutonomousMode(&handle_SPI_3, &HAL_SPI_AutonomousMode_Cfg_Struct) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI3_Init 2 */

  /* USER CODE END SPI3_Init 2 */

}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOH, GPIO_PIN_15|GPIO_PIN_13, GPIO_PIN_SET);

  /*Configure GPIO pins : PH15 PH13 */
  GPIO_InitStruct.Pin = GPIO_PIN_15|GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

}

void SPI_Init(void)
{
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_SPI3_Init();
}

void SPI_test (void)
{
  //HAL_NVIC_SetPendingIRQ(SPI3_IRQn);
  /* Buffer used for transmission */
  uint8_t masterTxBuffer[] = "SPIM_TX";
  uint8_t slaveTxBuffer[] = "SPIS_TX";
  /* Buffer used for reception */
  uint8_t masterRxBuffer[sizeof(slaveTxBuffer)];
  uint8_t slaveRxBuffer[sizeof(masterTxBuffer)];
  printf ("Slave TX: %s\r\n", (char *) &slaveTxBuffer);
  printf ("Master TX: %s\r\n", (char *) &masterTxBuffer);
  printf ("Slave RX: %s\r\n", (char *) &slaveRxBuffer);
  printf ("Master RX: %s\r\n", (char *) &masterRxBuffer);
  uint8_t cnt = 3;
  while (cnt)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
      HAL_Delay(50);
      //HAL_SPI_Transmit(&handle_SPI_3, slaveTxBuffer, sizeof(slaveTxBuffer), 100);
      HAL_SPI_TransmitReceive_IT(&handle_SPI_3, slaveTxBuffer, slaveRxBuffer, sizeof(slaveTxBuffer));
      HAL_SPI_TransmitReceive(&handle_SPI_1, masterTxBuffer, masterRxBuffer, sizeof(masterTxBuffer), 100);
      printf ("Slave RX: %s\r\n", (char *) &slaveRxBuffer);
      printf ("Master RX: %s\r\n", (char *) &masterRxBuffer);
      cnt--;
  }
}

