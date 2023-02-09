/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */

#ifndef __I2C_H__
#define __I2C_H__

#include <stdint.h>
#include "stm32u5xx.h"
#include "stm32u5xx_hal.h"
#include "error.h"


extern I2C_HandleTypeDef hi2c1_slave;
extern I2C_HandleTypeDef hi2c2_master;

void I2C_Init(void);
void I2C_Master_scan();

pilot_error_t I2C_PMBUS_Write_Byte(uint16_t address, uint8_t cmd, uint8_t byte);
pilot_error_t I2C_PMBUS_Write_Word(uint16_t address, uint8_t cmd, uint16_t word);
pilot_error_t I2C_PMBUS_Write_Block(uint16_t address, uint8_t cmd, uint8_t* block, uint16_t len);

pilot_error_t I2C_PMBUS_Read_Byte(uint16_t address, uint8_t cmd, uint8_t* byte);
pilot_error_t I2C_PMBUS_Read_Word(uint16_t address, uint8_t cmd, uint16_t* word);
pilot_error_t I2C_PMBUS_Read_Block(uint16_t address, uint8_t cmd, uint8_t* block, uint16_t len);


#endif /* __I2C_H__ */
