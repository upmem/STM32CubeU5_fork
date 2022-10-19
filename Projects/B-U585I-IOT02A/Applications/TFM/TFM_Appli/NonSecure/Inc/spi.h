/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */

#ifndef __SPI_H__
#define __SPI_H__

#include <stdint.h>
#include "stm32u5xx.h"
#include "stm32u5xx_hal.h"
#include "error.h"

void SPI_Init(void);
pilot_error_t SPI_GI_Transmit_Receive(uint16_t* tx_buf, uint16_t* rx_buf, uint16_t len, int mode);

/* possible transfert modes for SPI */
#define SPI_TRANSFERT_MODE_16BIT_BLOCKING  0
#define SPI_TRANSFERT_MODE_BURST_BLOCKING  1
#define SPI_TRANSFERT_MODE_DMA             2

#define COUNTOF(array) (sizeof(array)/sizeof(array[0]))

extern SPI_HandleTypeDef handle_SPI_1;
extern SPI_HandleTypeDef handle_SPI_3;

#endif /* __SPI_H__ */
