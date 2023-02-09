/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */

#include "i2c.h"
#include <stdio.h>

I2C_HandleTypeDef hi2c1_slave;
I2C_HandleTypeDef hi2c2_master;

#define I2C_SCAN_RETRY (2)
#define I2C_SCAN_TIMEOUT_MS (1)
#define I2C_TIMEOUT_MS (1)
/* I2C@100kHz
 *   => 1 byte   <=> 100us (8bit + 1ACK + Start/Stop)
 *   => 100bytes <=> 1ms
 */

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1_slave.Instance = I2C1;
  hi2c1_slave.Init.Timing = 0x30909DEC;
  hi2c1_slave.Init.OwnAddress1 = 0;
  hi2c1_slave.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1_slave.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1_slave.Init.OwnAddress2 = 0;
  hi2c1_slave.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1_slave.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1_slave.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1_slave) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1_slave, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1_slave, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2_master.Instance = I2C2;
  hi2c2_master.Init.Timing = 0x30909DEC;
  hi2c2_master.Init.OwnAddress1 = 0;
  hi2c2_master.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2_master.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2_master.Init.OwnAddress2 = 0;
  hi2c2_master.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2_master.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2_master.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2_master) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2_master, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2_master, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

void I2C_Init(void)
{
  MX_I2C1_Init();
  MX_I2C2_Init();
}

void I2C_Master_scan()
{
  printf("I2C Master scan :\r\n");
  for (uint8_t address = 0x00; address <= 0x7F; address++)
  {
    /* Ignore I2C reserved addresses */
    if (address <= 0b00000111  || // I2C reserved
	address >= 0b01111100) // I2C reserved (device ID)
    {
      printf("xx ");
    }
    /* Ping I2C addresses */
    else
    {
      if (HAL_I2C_IsDeviceReady(&hi2c2_master, address << 1, I2C_SCAN_RETRY, I2C_SCAN_TIMEOUT_MS) == HAL_OK)
	printf("%2x ", address);
      else
	printf("-- ");
    }
    if (address > 0 && (address + 1) % 16 == 0) printf("\r\n");
  }
}

pilot_error_t I2C_Transmit(uint16_t address, uint8_t* data, uint16_t len)
{
  pilot_error_t err = PILOT_FAILURE;
  HAL_StatusTypeDef status = HAL_ERROR;

  status = HAL_I2C_Master_Transmit(&hi2c2_master, address, data, len, I2C_TIMEOUT_MS);

  if (status == HAL_OK) {
    err = PILOT_SUCCESS;
  }
  return err;
}

pilot_error_t I2C_Receive(uint16_t address, uint8_t* data, uint16_t len)
{
  pilot_error_t err = PILOT_FAILURE;
  HAL_StatusTypeDef status = HAL_ERROR;

  status = HAL_I2C_Master_Receive(&hi2c2_master, address, data, len, I2C_TIMEOUT_MS);

  if (status == HAL_OK) {
    err = PILOT_SUCCESS;
  }
  return err;
}

static pilot_error_t I2C_PMBUS_Write(uint16_t address, uint8_t cmd, uint8_t* data, uint16_t len)
{
  pilot_error_t err = PILOT_FAILURE;
  HAL_StatusTypeDef status = HAL_ERROR;

  status = HAL_I2C_Mem_Write(&hi2c2_master, address, cmd, I2C_MEMADD_SIZE_8BIT, data, len, I2C_TIMEOUT_MS);

  if (status == HAL_OK) {
    err = PILOT_SUCCESS;
  }
  return err;
}
pilot_error_t I2C_PMBUS_Write_Byte(uint16_t address, uint8_t cmd, uint8_t byte)
{
  return I2C_PMBUS_Write(address, cmd, &byte, 1);
}
pilot_error_t I2C_PMBUS_Write_Word(uint16_t address, uint8_t cmd, uint16_t word)
{
  uint8_t data[2];
  data[0] = (uint8_t)(word >> 8); // LSB first
  data[1] = (uint8_t)(word & 0x00FF); // MSB last
  return I2C_PMBUS_Write(address, cmd, data, 2);
}
pilot_error_t I2C_PMBUS_Write_Block(uint16_t address, uint8_t cmd, uint8_t* block, uint16_t len)
{
  return I2C_PMBUS_Write(address, cmd, block, len);
}

static pilot_error_t I2C_PMBUS_Read(uint16_t address, uint8_t cmd, uint8_t* data, uint16_t len)
{
  pilot_error_t err = PILOT_FAILURE;
  HAL_StatusTypeDef status = HAL_ERROR;

  status = HAL_I2C_Mem_Read(&hi2c2_master, address, cmd, I2C_MEMADD_SIZE_8BIT, data, len, I2C_TIMEOUT_MS);

  if (status == HAL_OK) {
    err = PILOT_SUCCESS;
  }
  return err;
}
pilot_error_t I2C_PMBUS_Read_Byte(uint16_t address, uint8_t cmd, uint8_t* byte)
{
  return I2C_PMBUS_Read(address, cmd, byte, 1);
}
pilot_error_t I2C_PMBUS_Read_Word(uint16_t address, uint8_t cmd, uint16_t* word)
{
  pilot_error_t err = PILOT_FAILURE;
  uint8_t data[2];
  do{
    err = I2C_PMBUS_Read(address, cmd, data, 2);
    if(err != PILOT_SUCCESS)
      break;
    *word = ((((uint16_t)data[1]) & 0x00FF) << 8) + data[0];
  } while(0);
  return err;
}
pilot_error_t I2C_PMBUS_Read_Block(uint16_t address, uint8_t cmd, uint8_t* block, uint16_t len)
{
  return I2C_PMBUS_Read(address, cmd, block, len);
}
