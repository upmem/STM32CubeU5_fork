/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */

#ifndef __BOARD_CONFIG_H__
#define __BOARD_CONFIG_H__

#include "stm32u5xx.h"

#define NB_DPU_DRAM       16

/* BitMask for chipSelect */
#define DPU_DRAM_MASK_0   ((uint16_t)(0x0001))
#define DPU_DRAM_MASK_1   ((uint16_t)(0x0002))
#define DPU_DRAM_MASK_2   ((uint16_t)(0x0004))
#define DPU_DRAM_MASK_3   ((uint16_t)(0x0008))
#define DPU_DRAM_MASK_4   ((uint16_t)(0x0010))
#define DPU_DRAM_MASK_5   ((uint16_t)(0x0020))
#define DPU_DRAM_MASK_6   ((uint16_t)(0x0040))
#define DPU_DRAM_MASK_7   ((uint16_t)(0x0080))
#define DPU_DRAM_MASK_8   ((uint16_t)(0x0100))
#define DPU_DRAM_MASK_9   ((uint16_t)(0x0200))
#define DPU_DRAM_MASK_10  ((uint16_t)(0x0400))
#define DPU_DRAM_MASK_11  ((uint16_t)(0x0800))
#define DPU_DRAM_MASK_12  ((uint16_t)(0x1000))
#define DPU_DRAM_MASK_13  ((uint16_t)(0x2000))
#define DPU_DRAM_MASK_14  ((uint16_t)(0x4000))
#define DPU_DRAM_MASK_15  ((uint16_t)(0x8000))
#define DPU_DRAM_MASK_ALL ((uint16_t)(0xFFFF))

/* DPU-DRAM SS pins */
// TODO : assign one pin per SS
#define GPIO_PORT_SS0  GPIOA
#define GPIO_PIN_SS0   GPIO_PIN_4
#define GPIO_PORT_SS1  GPIOH
#define GPIO_PIN_SS1   GPIO_PIN_6 // RED LED for testing purposes
#define GPIO_PORT_SS2  GPIOH
#define GPIO_PIN_SS2   GPIO_PIN_7 // GREEN LED for testing purposes
#define GPIO_PORT_SS3  GPIOA
#define GPIO_PIN_SS3   GPIO_PIN_4
#define GPIO_PORT_SS4  GPIOA
#define GPIO_PIN_SS4   GPIO_PIN_4
#define GPIO_PORT_SS5  GPIOA
#define GPIO_PIN_SS5   GPIO_PIN_4
#define GPIO_PORT_SS6  GPIOA
#define GPIO_PIN_SS6   GPIO_PIN_4
#define GPIO_PORT_SS7  GPIOA
#define GPIO_PIN_SS7   GPIO_PIN_4
#define GPIO_PORT_SS8  GPIOA
#define GPIO_PIN_SS8   GPIO_PIN_4
#define GPIO_PORT_SS9  GPIOA
#define GPIO_PIN_SS9   GPIO_PIN_4
#define GPIO_PORT_SS10 GPIOA
#define GPIO_PIN_SS10  GPIO_PIN_4
#define GPIO_PORT_SS11 GPIOA
#define GPIO_PIN_SS11  GPIO_PIN_4
#define GPIO_PORT_SS12 GPIOA
#define GPIO_PIN_SS12  GPIO_PIN_4
#define GPIO_PORT_SS13 GPIOA
#define GPIO_PIN_SS13  GPIO_PIN_4
#define GPIO_PORT_SS14 GPIOA
#define GPIO_PIN_SS14  GPIO_PIN_4
#define GPIO_PORT_SS15 GPIOA
#define GPIO_PIN_SS15  GPIO_PIN_4

/* DPU-DRAM Hardware configuration */
typedef struct {
  GPIO_TypeDef* ss_port;
  uint32_t ss_pin;
} dpu_dram_board_config_t;

extern const dpu_dram_board_config_t dpu_dram_config[];

#endif /* __BOARD_CONFIG_H__ */
