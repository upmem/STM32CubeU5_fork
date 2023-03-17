/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */

#include "spi.h"
#include "stm32u5xx_hal.h"
#include "stdio.h"
#include "board_config.h"

#include "FreeRTOS.h"
#include "semphr.h"
SPI_HandleTypeDef handle_SPI_1;
SPI_HandleTypeDef handle_SPI_3;
#ifdef DEBUG
#define SPI_DEBUG
#endif
#define SPI_16BIT_TIMEOUT_MS (5) /* should be >= 2; value of 1 can result in interrupted transfers */
/* If DPU PLL disabled, DPU speed is based on External Oscillator (typ 10MHz) with div8 configuration = 1.25MHz
 * Reset hold time should be at least superior to 2x DPU Period = 3.2µs
 */
#define HARDWARE_RESET_HOLD_TIME_MS (1)

static SemaphoreHandle_t spi1_mutex;
static SemaphoreHandle_t dma_semaphore;

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
  handle_SPI_1.Init.NSS = SPI_NSS_SOFT;
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
  spi1_mutex = xSemaphoreCreateMutex();
  dma_semaphore = xSemaphoreCreateBinary();
  if ((spi1_mutex == NULL) || (dma_semaphore == NULL)) {
    Error_Handler();
  }
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

}

void SPI_Init(void)
{
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_SPI3_Init();
}

static void SPI_debug (uint16_t* tx_buf, uint16_t* rx_buf, uint16_t len, int mode, HAL_StatusTypeDef status) {
#ifdef SPI_DEBUG
  uint16_t i;
  printf ("SPI Debug:\r\n");
  printf ("  mode: %d\r\n",mode);
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
}

static void set_ss(uint16_t ss_mask, uint32_t state)
{
  uint32_t i;
  for (i=0; i<NB_DPU_DRAM; i++)
  {
    if(ss_mask & ((uint16_t)1 << i))
      HAL_GPIO_WritePin(dpu_dram_config[i].ss_port, dpu_dram_config[i].ss_pin, state);
  }
}

/**
  * @brief  Transmit and Receive data to Global Interface using SPI
  * @param  ss_mask: Slave Select bit mask
  * @param  tx_buf: pointer to transmission data buffer, can be NULL if receive only
  * @param  rx_buf: pointer to reception data buffer, can be NULL if transmit only
  * @param  len   : amount of data to be sent and received
  * @param  mode  : SPI transfert mode, to send either 16bit mode or in burst mode
  * @retval HAL status
  */
pilot_error_t SPI_GI_Transmit_Receive(uint16_t ss_mask, uint16_t* tx_buf, uint16_t* rx_buf, uint16_t len, int mode)
{
  uint16_t i;
  pilot_error_t err = PILOT_FAILURE;
  HAL_StatusTypeDef status = HAL_ERROR;

  if (
      ((tx_buf != NULL) || (rx_buf != NULL)) &&
      (len != 0)
    ) {
      xSemaphoreTake(spi1_mutex, portMAX_DELAY );
      switch(mode)
      {
	/* Send SPI buffer items one by one (ie ChipSelect asserted and de-asserted every uint16_t) */
	case SPI_TRANSFERT_MODE_16BIT_BLOCKING:
	  for (i=0; i<len; i++)
	  {
	    set_ss(ss_mask, GPIO_PIN_RESET); // Set SS Low
	    if (rx_buf != NULL && tx_buf != NULL) {
	      status = HAL_SPI_TransmitReceive(&handle_SPI_1, (uint8_t *)&(tx_buf[i]), (uint8_t *)&(rx_buf[i]), 1, SPI_16BIT_TIMEOUT_MS);
	    } else if (tx_buf != NULL) {
	      status = HAL_SPI_Transmit(&handle_SPI_1, (uint8_t *)&(tx_buf[i]), 1, SPI_16BIT_TIMEOUT_MS);
	    } else {
	      status = HAL_SPI_Receive(&handle_SPI_1, (uint8_t *)&(rx_buf[i]), 1, SPI_16BIT_TIMEOUT_MS);
	    }
	    set_ss(ss_mask, GPIO_PIN_SET); // Set SS high
	    if (status != HAL_OK)
	      break;
	  }
	  break;

	/* Send SPI buffer in a single BURST (ie ChipSelect asserted only once) */
	case SPI_TRANSFERT_MODE_BURST_BLOCKING:
	  set_ss(ss_mask, GPIO_PIN_RESET); // Set SS Low
	  if (rx_buf != NULL && tx_buf != NULL) {
	    status = HAL_SPI_TransmitReceive(&handle_SPI_1, (uint8_t *)tx_buf, (uint8_t *)rx_buf, len, SPI_16BIT_TIMEOUT_MS*len);
	  } else if (tx_buf != NULL) {
	    status = HAL_SPI_Transmit(&handle_SPI_1, (uint8_t *)tx_buf, len, SPI_16BIT_TIMEOUT_MS*len);
	  } else {
	    status = HAL_SPI_Receive(&handle_SPI_1, (uint8_t *)rx_buf, len, SPI_16BIT_TIMEOUT_MS*len);
	  }
	  set_ss(ss_mask, GPIO_PIN_SET); // Set SS high
	  break;

	/* Send SPI buffer using DMA (ChipSelect asserted only once) */
	case SPI_TRANSFERT_MODE_DMA:
	  set_ss(ss_mask, GPIO_PIN_RESET);  // Set SS Low.
	  if (rx_buf != NULL && tx_buf != NULL) {
	    status = HAL_SPI_TransmitReceive_DMA(&handle_SPI_1, (uint8_t *)tx_buf, (uint8_t *)rx_buf, len);
	  } else if (tx_buf != NULL) {
	    status = HAL_SPI_Transmit_DMA(&handle_SPI_1, (uint8_t *)tx_buf, len);
	  } else {
	    status = HAL_SPI_Receive_DMA(&handle_SPI_1, (uint8_t *)rx_buf, len);
	  }
	  /* wait the end of DMA transfer */
	  xSemaphoreTake(dma_semaphore, portMAX_DELAY);
	  set_ss(ss_mask, GPIO_PIN_SET); // Set SS high
	  break;
	/* unknown mode : return error */
	default:
	  status = HAL_ERROR;
	  break;
      }
      /* Always put SPI_debug before the mutex release*/
      SPI_debug (tx_buf, rx_buf, len, mode, status);
      xSemaphoreGive(spi1_mutex);
  }

  if (status == HAL_OK) {
    err = PILOT_SUCCESS;
  }
  return err;
}

/**
  * @brief  Trigger the DPU-DRAM Hardware Reset procedure : SS goes low and high with no data
  * @param  ss_mask: Slave Select bit mask
  * @retval HAL status
  */
void SPI_DRAM_HW_Reset(uint16_t ss_mask)
{
  xSemaphoreTake(spi1_mutex, portMAX_DELAY );
  set_ss(ss_mask, GPIO_PIN_RESET); // Set SS low
  HAL_Delay(HARDWARE_RESET_HOLD_TIME_MS);
  set_ss(ss_mask, GPIO_PIN_SET); // Set SS high
  xSemaphoreGive(spi1_mutex);
}

void SPI_Com_Complete(SPI_HandleTypeDef *hspi)
{
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  if (hspi == &handle_SPI_1)
  {
    xSemaphoreGiveFromISR(dma_semaphore, &xHigherPriorityTaskWoken);
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
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
  SPI_Com_Complete(hspi);
}
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
  // TODO : handle error ?
  SPI_Com_Complete(hspi);
}
