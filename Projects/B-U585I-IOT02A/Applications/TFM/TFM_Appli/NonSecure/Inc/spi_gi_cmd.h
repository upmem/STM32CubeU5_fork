/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */

#ifndef __SPI_GI_CMD_H__
#define __SPI_GI_CMD_H__

#define CHIP_ID_FPGA 0x0515

#define SPI_GI_CMD_SELECT_NO_IGNORE 0x3800
#define SPI_GI_CMD_WRITE_REG_IGNORE 0xC301

#define SPI_GI_CMD_DPU0_WRITE_REG_IDENTITY 0xD000
#define SPI_GI_CMD_DPU1_WRITE_REG_IDENTITY 0xC001
#define SPI_GI_CMD_DPU2_WRITE_REG_IDENTITY 0xC002
#define SPI_GI_CMD_DPU3_WRITE_REG_IDENTITY 0xD003
#define SPI_GI_CMD_DPU4_WRITE_REG_IDENTITY 0xC004
#define SPI_GI_CMD_DPU5_WRITE_REG_IDENTITY 0xD005
#define SPI_GI_CMD_DPU6_WRITE_REG_IDENTITY 0xD006
#define SPI_GI_CMD_DPU7_WRITE_REG_IDENTITY 0xC007

#define SPI_GI_CMD_CHIP_ID_15_8 0x1101
#define SPI_GI_CMD_CHIP_ID_7_0 0x1300
#define SPI_GI_CMD_BUBBLE 0x0000
#define SPI_GI_CMD_NOP 0xE000

/*
 * When PILOT transmits a 16-bits word on MOSI line through SPI interface,
 * the DPU-DRAM transmits at the same time a 16-bits answer on MISO line
 * whom the bitfield is described as follows:
 *
 * [15] Word Odd Parity Flag of previously received SPI word
 * [14] Word Odd Parity Flag of previously received SPI word
 * [13] Word Odd Parity Flag of previously received SPI word
 * [12] Result Valid Flag
 * [11] Result Valid Flag
 * [10] Result Valid Flag
 * [9] Reserved - 0b0
 * [8] Odd Parity Flag of Result data (RESULT[7:0])
 * [7:0] Result data (RESULT[7:0])
 */

typedef union {
  struct {
    uint16_t result:8;
    uint16_t result_odd_flag:1;
    uint16_t reserved:1;
    uint16_t result_valid_flag:3;
    uint16_t previous_odd_flag:3;
  };
  uint16_t word;
} GI_response_word_t;

#endif /* __SPI_GI_CMD_H__ */
