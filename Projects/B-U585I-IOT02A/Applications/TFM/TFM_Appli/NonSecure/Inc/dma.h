/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */

#ifndef __DMA_H__
#define __DMA_H__

#include <stdint.h>
#include "stm32u5xx.h"
#include "stm32u5xx_hal.h"

extern DMA_HandleTypeDef handle_GPDMA1_Channel7;
extern DMA_HandleTypeDef handle_GPDMA1_Channel6;

void DMA_Init(void);

#endif /* __DMA_H__ */
