/**
 *
 * Copyright (c) 2023 UPMEM.
 *
 */

#include "i2c.h"
#include "pmbus.h"
#include <stdio.h>
#include "board_config.h"

I2C_HandleTypeDef hi2c1_slave;
I2C_HandleTypeDef hi2c2_master;

#ifdef DEBUG
#define I2C_DEBUG
#endif

#define I2C_SCAN_RETRY (2)
#define I2C_SCAN_TIMEOUT_MS (1)
#define I2C_TIMEOUT_MS (10)
/* I2C@100kHz
 *   => 1 byte    <=> 100us (8bit + 1ACK + Start/Stop)
 *   => 100 bytes <=> 10ms (100 bytes will never been transmitted at once)
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
/*
 * Timing :
 *  Bits 31:28 PRESC[3:0]: Timing prescaler (PRESC + 1)
 *  Bits 27:24 Reserved, must be kept at reset value.
 *  Bits 23:20 SCLDEL[3:0]: Data setup time
 *  Bits 19:16 SDADEL[3:0]: Data hold time
 *  Bits 15:8 SCLH[7:0]: SCL high period (master mode)
 *  Bits 7:0 SCLL[7:0]: SCL low period (master mode)
 *
 *  0x30909DEC :
 *    PRESC = 3
 *    SCLH = 0x9D = 157
 *    SCLL = 0xEC = 236
 *  => F(I2C) = 160MHz/(PRESC+1)/(157+236) = 101kHz
 */
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

static void I2C_debug(uint16_t address, uint8_t* tx_buf, uint16_t tx_len, uint8_t* rx_buf, uint16_t rx_len, HAL_StatusTypeDef status)
{
#ifdef I2C_DEBUG
  uint16_t i;
  printf ("I2C Debug:\r\n");
  printf ("  Address: 0x%02x\r\n", address);
  if (tx_buf != NULL && tx_len > 0)
  {
    printf ("  TX:");
    for (i=0; i<tx_len; i++)
      printf (" 0x%02x",tx_buf[i]);
    printf ("\r\n");
  }
  if (rx_buf != NULL && rx_len > 0)
  {
    printf ("  RX:");
    for (i=0; i<rx_len; i++)
      printf (" 0x%02x",rx_buf[i]);
    printf ("\r\n");
  }
  printf ("  Status: 0x%02x\r\n", status);
#endif
}

static void I2C_PMBUS_debug(uint16_t address, uint8_t cmd, uint8_t* tx_buf, uint16_t tx_len, uint8_t* rx_buf, uint16_t rx_len, HAL_StatusTypeDef status)
{
#ifdef I2C_DEBUG
  uint16_t i;
  printf ("PMBUS Debug:\r\n");
  printf ("  Address: 0x%02x\r\n", address);
  printf ("  Command: 0x%02x\r\n", cmd);
  if (tx_buf != NULL && tx_len > 0)
  {
    printf ("  TX:");
    for (i=0; i<tx_len; i++)
      printf (" 0x%02x",tx_buf[i]);
    printf ("\r\n");
  }
  if (rx_buf != NULL && rx_len > 0)
  {
    printf ("  RX:");
    for (i=0; i<rx_len; i++)
      printf (" 0x%02x",rx_buf[i]);
    printf ("\r\n");
  }
  printf ("  Status: 0x%02x\r\n", status);
#endif
}

void I2C_Init(void)
{
  MX_I2C1_Init();
  MX_I2C2_Init();
}

void I2C_Master_test()
{
  uint8_t i2c_rx[9] = {0};
  uint16_t word;
  pilot_error_t err = PILOT_FAILURE;
  int i;

  /* Read DC/DC (address 0x31) MFR_MODEL (command 0x9A) 8+1 bytes*/
  err = I2C_PMBUS_Read_Block(I2C_ADDRESS_DCDC, PMBUS_CMD_MFR_MODEL, i2c_rx, 9);
  printf("Read DC/DC Manufacturer model : ");
  if(err != PILOT_SUCCESS)
    printf("I2C Error\r\n");
  else
  {
    for(i=8; i>0; i--)
      printf("%c", (char)i2c_rx[i]);
    printf("\r\n");
  }

  /* Read DC/DC (address 0x31) Temperature (command 0x8D)*/
  // TODO : 2complement should be applied to word, for negative values handling
  err = I2C_PMBUS_Read_Word(I2C_ADDRESS_DCDC, PMBUS_CMD_READ_TEMPERATURE_1, &word);
  printf("Read DC/DC Temperature : ");
  if(err != PILOT_SUCCESS)
    printf("I2C Error\r\n");
  else
  {
    printf ("%d\r\n", word);
  }
}

pilot_error_t I2C_Master_ping(uint8_t address)
{
  pilot_error_t err = PILOT_FAILURE;
  if (HAL_I2C_IsDeviceReady(&hi2c2_master, address << 1, I2C_SCAN_RETRY, I2C_SCAN_TIMEOUT_MS) == HAL_OK)
    err = PILOT_SUCCESS;
  return err;
}

uint16_t I2C_Master_scan(void)
{
  uint16_t nb_devices = 0;

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
      {
	printf("%2x ", address);
	nb_devices++;
      }
      else
      {
	printf("-- ");
      }
    }
    if (address > 0 && (address + 1) % 16 == 0) printf("\r\n");
  }
  return nb_devices;
}

pilot_error_t I2C_Transmit(uint16_t address, uint8_t* data, uint16_t len)
{
  pilot_error_t err = PILOT_FAILURE;
  HAL_StatusTypeDef status = HAL_ERROR;

  status = HAL_I2C_Master_Transmit(&hi2c2_master, address << 1, data, len, I2C_TIMEOUT_MS);
  I2C_debug(address, data, len, NULL, 0, status);

  if (status == HAL_OK) {
    err = PILOT_SUCCESS;
  }
  return err;
}

pilot_error_t I2C_Receive(uint16_t address, uint8_t* data, uint16_t len)
{
  pilot_error_t err = PILOT_FAILURE;
  HAL_StatusTypeDef status = HAL_ERROR;

  status = HAL_I2C_Master_Receive(&hi2c2_master, address << 1, data, len, I2C_TIMEOUT_MS);
  I2C_debug(address, NULL, 0, data, len, status);

  if (status == HAL_OK) {
    err = PILOT_SUCCESS;
  }
  return err;
}

static pilot_error_t I2C_PMBUS_Write(uint16_t address, uint8_t cmd, uint8_t* data, uint16_t len)
{
  pilot_error_t err = PILOT_FAILURE;
  HAL_StatusTypeDef status = HAL_ERROR;

  status = HAL_I2C_Mem_Write(&hi2c2_master, address << 1, cmd, I2C_MEMADD_SIZE_8BIT, data, len, I2C_TIMEOUT_MS);
  I2C_PMBUS_debug(address, cmd, data, len, NULL, 0, status);

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

  status = HAL_I2C_Mem_Read(&hi2c2_master, address << 1, cmd, I2C_MEMADD_SIZE_8BIT, data, len, I2C_TIMEOUT_MS);
  I2C_PMBUS_debug(address, cmd, NULL, 0, data, len, status);

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
