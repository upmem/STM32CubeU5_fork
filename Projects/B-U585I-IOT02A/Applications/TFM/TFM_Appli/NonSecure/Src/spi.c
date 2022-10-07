/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */

#include "spi.h"
#include "spi_gi_cmd.h"
#include "stm32u5xx_hal.h"
#include "error.h"
#include "stdio.h"

SPI_HandleTypeDef handle_SPI_1;
SPI_HandleTypeDef handle_SPI_3;

#define SPI_DEBUG
#define SPI_16BIT_TIMEOUT_MS (5) /* should be >= 2; value of 1 can result in interrupted transfers */

//Configure ILink Nodes to get following ILink-Ring:
//Master->DPU1->DPU0->DPU2->DPU3-->DPU4->DPU5->DPU7->DPU6->Master
const uint16_t spi_gi_init_seq[] = {
  SPI_GI_CMD_SELECT_NO_IGNORE,
  SPI_GI_CMD_DPU1_WRITE_REG_IDENTITY,
  SPI_GI_CMD_WRITE_REG_IGNORE,
  SPI_GI_CMD_SELECT_NO_IGNORE,
  SPI_GI_CMD_DPU0_WRITE_REG_IDENTITY,
  SPI_GI_CMD_WRITE_REG_IGNORE,
  SPI_GI_CMD_SELECT_NO_IGNORE,
  SPI_GI_CMD_DPU2_WRITE_REG_IDENTITY,
  SPI_GI_CMD_WRITE_REG_IGNORE,
  SPI_GI_CMD_SELECT_NO_IGNORE,
  SPI_GI_CMD_DPU3_WRITE_REG_IDENTITY,
  SPI_GI_CMD_WRITE_REG_IGNORE,
  SPI_GI_CMD_SELECT_NO_IGNORE,
  SPI_GI_CMD_DPU4_WRITE_REG_IDENTITY,
  SPI_GI_CMD_WRITE_REG_IGNORE,
  SPI_GI_CMD_SELECT_NO_IGNORE,
  SPI_GI_CMD_DPU5_WRITE_REG_IDENTITY,
  SPI_GI_CMD_WRITE_REG_IGNORE,
  SPI_GI_CMD_SELECT_NO_IGNORE,
  SPI_GI_CMD_DPU7_WRITE_REG_IDENTITY,
  SPI_GI_CMD_WRITE_REG_IGNORE,
  SPI_GI_CMD_SELECT_NO_IGNORE,
  SPI_GI_CMD_DPU6_WRITE_REG_IDENTITY,
  SPI_GI_CMD_WRITE_REG_IGNORE,
};

const uint16_t spi_gi_chipid_lsb_seq[] = {
  SPI_GI_CMD_CHIP_ID_7_0,
  SPI_GI_CMD_NOP,
  SPI_GI_CMD_NOP,
};

const uint16_t spi_gi_chipid_msb_seq[] = {
  SPI_GI_CMD_CHIP_ID_15_8,
  SPI_GI_CMD_NOP,
  SPI_GI_CMD_NOP,
};


/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
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
  handle_SPI_1.Init.DataSize = SPI_DATASIZE_16BIT;
  handle_SPI_1.Init.CLKPolarity = SPI_POLARITY_LOW;
  handle_SPI_1.Init.CLKPhase = SPI_PHASE_1EDGE;
  handle_SPI_1.Init.NSS = SPI_NSS_HARD_OUTPUT;
  handle_SPI_1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128; // SYSCLK/128 = 1.25MBits/s @160MHz
  handle_SPI_1.Init.FirstBit = SPI_FIRSTBIT_LSB;
  handle_SPI_1.Init.TIMode = SPI_TIMODE_DISABLE;
  handle_SPI_1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  handle_SPI_1.Init.CRCPolynomial = 0x7;
  handle_SPI_1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  handle_SPI_1.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  handle_SPI_1.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  handle_SPI_1.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  handle_SPI_1.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  handle_SPI_1.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  handle_SPI_1.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE;
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

/**
  * @brief SPI3 Initialization Function
  * @param None
  * @retval None
  */
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
  handle_SPI_3.Init.DataSize = SPI_DATASIZE_16BIT;
  handle_SPI_3.Init.CLKPolarity = SPI_POLARITY_LOW;
  handle_SPI_3.Init.CLKPhase = SPI_PHASE_1EDGE;
  handle_SPI_3.Init.NSS = SPI_NSS_HARD_INPUT;
  handle_SPI_3.Init.FirstBit = SPI_FIRSTBIT_LSB;
  handle_SPI_3.Init.TIMode = SPI_TIMODE_DISABLE;
  handle_SPI_3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  handle_SPI_3.Init.CRCPolynomial = 0x7;
  handle_SPI_3.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  handle_SPI_3.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  handle_SPI_3.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  handle_SPI_3.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  handle_SPI_3.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  handle_SPI_3.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  handle_SPI_3.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE;
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
  HAL_GPIO_WritePin(GPIOH, GPIO_PIN_15|GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pins : PH15 PH13 */
  GPIO_InitStruct.Pin = GPIO_PIN_15|GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);


  /*Configure LED pins : PH6 PH7 */
  HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_SET); // SET = LED OFF
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
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

/**
  * @brief  Transmit and Receive data to Global Interface using SPI
  * @param  tx_buf: pointer to transmission data buffer, can be NULL if receive only
  * @param  rx_buf: pointer to reception data buffer, can be NULL if transmit only
  * @param  len   : amount of data to be sent and received
  * @param  mode  : SPI transfert mode, to send either 16bit mode or in burst mode
  * @retval HAL status
  */
static HAL_StatusTypeDef SPI_GI_Transmit_Receive(uint16_t* tx_buf, uint16_t* rx_buf, uint16_t len, SPI_TransfertMode mode)
{
  uint16_t i;
  HAL_StatusTypeDef status = HAL_ERROR;

  /* Send SPI buffer items one by one (ie ChipSelect asserted and de-asserted every uint16_t) */
  if (mode == SPI_TRANSFERT_MODE_16BIT_BLOCKING)
  {
    for (i=0; i<len; i++)
    {
      if (rx_buf != NULL && tx_buf != NULL)
        status = HAL_SPI_TransmitReceive(&handle_SPI_1, (uint8_t *)&(tx_buf[i]), (uint8_t *)&(rx_buf[i]), 1, SPI_16BIT_TIMEOUT_MS);
      else if (rx_buf == NULL && tx_buf != NULL)
        status = HAL_SPI_Transmit(&handle_SPI_1, (uint8_t *)&(tx_buf[i]), 1, SPI_16BIT_TIMEOUT_MS);
      else if (rx_buf != NULL && tx_buf == NULL)
        status = HAL_SPI_Receive(&handle_SPI_1, (uint8_t *)&(rx_buf[i]), 1, SPI_16BIT_TIMEOUT_MS);
      if (status != HAL_OK)
        break;
    }
  }

  /* Send SPI buffer in a single BURST (ie ChipSelect asserted only once) */
  else if (mode == SPI_TRANSFERT_MODE_BURST_BLOCKING)
  {
    if (rx_buf != NULL && tx_buf != NULL)
      status = HAL_SPI_TransmitReceive(&handle_SPI_1, (uint8_t *)tx_buf, (uint8_t *)rx_buf, len, SPI_16BIT_TIMEOUT_MS*len);
    else if (rx_buf == NULL && tx_buf != NULL)
      status = HAL_SPI_Transmit(&handle_SPI_1, (uint8_t *)tx_buf, len, SPI_16BIT_TIMEOUT_MS*len);
    else if (rx_buf != NULL && tx_buf == NULL)
      status = HAL_SPI_Receive(&handle_SPI_1, (uint8_t *)rx_buf, len, SPI_16BIT_TIMEOUT_MS*len);
  }

  /* Send SPI buffer using DMA (ChipSelect asserted only once) */
  else if (mode == SPI_TRANSFERT_MODE_DMA)
  {
    if (rx_buf != NULL && tx_buf != NULL)
      status = HAL_SPI_TransmitReceive_DMA(&handle_SPI_1, (uint8_t *)tx_buf, (uint8_t *)rx_buf, len);
    else if (rx_buf == NULL && tx_buf != NULL)
      status = HAL_SPI_Transmit_DMA(&handle_SPI_1, (uint8_t *)tx_buf, len);
    else if (rx_buf != NULL && tx_buf == NULL)
      status = HAL_SPI_Receive_DMA(&handle_SPI_1, (uint8_t *)rx_buf, len);
  }

#ifdef SPI_DEBUG
  printf ("SPI Debug:\r\n");
  printf ("  status: %d\r\n",status);
  if (tx_buf != NULL)
  {
    printf ("  TX: ");
    for (i=0; i<len; i++)
      printf ("0x%04x ",tx_buf[i]);
    printf ("\r\n");
  }
  if (rx_buf != NULL)
  {
    printf ("  RX: ");
    for (i=0; i<len; i++)
      printf ("0x%04x ",rx_buf[i]);
    printf ("\r\n");
  }
#endif

  return status;
}

void print_GI_word(GI_response_word_t word)
{
  printf("word: 0x%04x\r\n", word.word);
  printf("  result: 0x%02x\r\n", word.result);
  printf("  result_odd_flag: 0x%01x\r\n", word.result_odd_flag);
  printf("  reserved: 0x%01x\r\n", word.reserved);
  printf("  result_valid_flag: 0x%01x\r\n", word.result_valid_flag);
  printf("  previous_odd_flag: 0x%01x\r\n", word.previous_odd_flag);
}

void SPI_test (void)
{
  /* Buffer used for transmission */
  uint16_t masterTxBuffer[] = {'S', 'P', 'I', 'M', '_', 'T', 'X'};
  uint16_t slaveTxBuffer[] =  {'S', 'P', 'I', 'S', '_', 'T', 'X'};
  /* Buffer used for reception */
  uint16_t masterRxBuffer[COUNTOF(slaveTxBuffer)];
  uint16_t slaveRxBuffer[COUNTOF(masterTxBuffer)];
  uint16_t i = 0;
  HAL_StatusTypeDef status_spi1, status_spi3;

  printf ("Slave TX: 0x");
  for(i=0; i<COUNTOF(slaveTxBuffer); i++) printf("%04x ", slaveTxBuffer[i]);
  printf ("\r\nMaster TX: 0x");
  for(i=0; i<COUNTOF(masterTxBuffer); i++) printf("%04x ", masterTxBuffer[i]);
  printf ("\r\nSlave RX: 0x");
  for(i=0; i<COUNTOF(slaveRxBuffer); i++) printf("%04x ", slaveRxBuffer[i]);
  printf ("\r\nMaster RX: 0x");
  for(i=0; i<COUNTOF(masterRxBuffer); i++) printf("%04x ", masterRxBuffer[i]);
  printf ("\r\n");

  while (1)
  {
      HAL_Delay(250);
      HAL_GPIO_TogglePin(GPIOH, GPIO_PIN_6); // RED LED toggle
      status_spi3 = HAL_SPI_TransmitReceive_IT(&handle_SPI_3, (uint8_t*)slaveTxBuffer, (uint8_t*)slaveRxBuffer, COUNTOF(slaveTxBuffer));
      status_spi1 = HAL_SPI_TransmitReceive(&handle_SPI_1, (uint8_t*)masterTxBuffer, (uint8_t*)masterRxBuffer, COUNTOF(masterTxBuffer), SPI_16BIT_TIMEOUT_MS*COUNTOF(masterTxBuffer));
      printf ("status SPI1 = %d - SPI3 = %d\r\n", status_spi1, status_spi3);
      printf ("Slave RX: 0x");
      for(i=0; i<COUNTOF(slaveRxBuffer); i++) printf("%04x ", slaveRxBuffer[i]);
      printf ("\r\nMaster RX: 0x");
      for(i=0; i<COUNTOF(masterRxBuffer); i++) printf("%04x ", masterRxBuffer[i]);
      printf ("\r\n");
  }
}

HAL_StatusTypeDef SPI_GI_Send_InitSequence(void)
{
  /* send init sequence, 16bit spi transfert */
  return SPI_GI_Transmit_Receive((uint16_t*)spi_gi_init_seq, NULL,
      COUNTOF(spi_gi_init_seq), SPI_TRANSFERT_MODE_16BIT_BLOCKING);
}


HAL_StatusTypeDef SPI_GI_Read_ChipID(void)
{
  HAL_StatusTypeDef status_spi;
  uint16_t chip_id = 0;
  uint16_t spi_gi_response [COUNTOF(spi_gi_chipid_lsb_seq)];
  GI_response_word_t rx_word_lsb, rx_word_msb;

  /* Send SPI commands to get chip ID LSB */
  status_spi = SPI_GI_Transmit_Receive((uint16_t*)spi_gi_chipid_lsb_seq, spi_gi_response,
      COUNTOF(spi_gi_chipid_lsb_seq), SPI_TRANSFERT_MODE_BURST_BLOCKING);
  rx_word_lsb.word = spi_gi_response[2]; /* 2 first words are NOP */
  print_GI_word(rx_word_lsb);

  /* Send SPI commands to get chip ID MSB */
  status_spi = SPI_GI_Transmit_Receive((uint16_t*)spi_gi_chipid_msb_seq, spi_gi_response,
      COUNTOF(spi_gi_chipid_msb_seq), SPI_TRANSFERT_MODE_BURST_BLOCKING);
  rx_word_msb.word = spi_gi_response[2]; /* 2 first words are NOP */
  print_GI_word(rx_word_msb);

  /* build ChipID from LSB and MSB */
  chip_id = (rx_word_msb.result << 8) + rx_word_lsb.result;

  /* check result */
  printf ("Chip ID: 0x%04x - Expected: 0x%04x - SPI status: %d\r\n", chip_id, CHIP_ID_FPGA, status_spi);
  return status_spi;
}

void SPI_Com_Complete(SPI_HandleTypeDef *hspi)
{
  if (hspi == &handle_SPI_1) // SPI1 : toggle GREEN
  {
    HAL_GPIO_TogglePin(GPIOH, GPIO_PIN_7);
  }
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
  SPI_Com_Complete(hspi);
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
  SPI_Com_Complete(hspi);
}
