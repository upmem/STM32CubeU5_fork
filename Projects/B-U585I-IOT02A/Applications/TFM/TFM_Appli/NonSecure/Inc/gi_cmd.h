/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */

#ifndef __GI_CMD_H__
#define __GI_CMD_H__

#define CHIP_ID_FPGA 0x0515

/* CMD for debug */
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

#define GI_RESPONSE_GET_PREVIOUS_ODD_FLAG(x) ((x & 0xE000) >> 13)
#define GI_RESPONSE_GET_RESULT_VALID_FLAG(x) ((x & 0x1C00) >> 10)
#define GI_RESPONSE_GET_RESERVED(x)          ((x & 0x0200) >> 9)
#define GI_RESPONSE_GET_ODD_FLAG(x)          ((x & 0x0100) >> 8)
#define GI_RESPONSE_GET_RESULT(x)            ((x & 0x00FF) >> 0)

/* Words type */
#define WORD_CMD_1	(0b0001 << 12)
#define WORD_CMD_3	(0b0011 << 12)
#define WORD_CMD_6	(0b0110 << 12)
#define WORD_CMD_7	(0b0111 << 12)
#define WORD_CMD_C	(0b1100 << 12)
#define WORD_CMD_E	(0b1110 << 12)

#define WORD_ESC_5	(0b0101 << 12)
#define WORD_ESC_8	(0b1000 << 12)

#define WORD_INSTR	(0b1010 << 12)

/* Slave node indexes (AKA DPU IDs) */
#define DPU_ID_0	(0x0)
#define DPU_ID_1	(0x1)
#define DPU_ID_2	(0x2)
#define DPU_ID_3	(0x3)
#define DPU_ID_4	(0x4)
#define DPU_ID_5	(0x5)
#define DPU_ID_6	(0x6)
#define DPU_ID_7	(0x7)

/* Parity bit logic */
//#define PARITY16(x) PARITY8((x) ^ ((x) >> 8))
//#define PARITY8(x)  PARITY4((x) ^ ((x) >> 4))
//#define PARITY4(x)  PARITY2((x) ^ ((x) >> 2))
//#define PARITY2(x)  (((x) ^ ((x) >> 1)) & 1)
//#define EVEN_PARITY(word)              (~(PARITY16(word))& 1)
#define EVEN_PARITY(word)		(~(__builtin_parity(word)) & 1)

#define AB_BITS_DELTA(word)		((__builtin_popcount(word) < 3 ) ? (word | (((1 << (3 - __builtin_popcount(word))) -1) << 8)) : word )
#define AB_PARITY(word)			((word & (1 << 8)) ? (word | EVEN_PARITY(word) << 9) : (word | EVEN_PARITY(word) << 8))
/* Not correct, to rework*/
#define SET_AB(word)			(AB_PARITY(AB_BITS_DELTA(word)))
#define SET_MSB_PARITY(word)		(word |  EVEN_PARITY(word) << 12)

/* CMD GET_1RESULT */
#define CMD_GET_1RESULT_CHIP_ID_LSB	(SET_AB(WORD_CMD_1 | 0x0))
#define CMD_GET_1RESULT_CHIP_ID_MSB	(SET_AB(WORD_CMD_1 | 0x1))
#define CMD_GET_1RESULT_PLL_LOCK	(SET_AB(WORD_CMD_1 | 0x2))
#define CMD_GET_1RESULT_CHECK_RES	(SET_AB(WORD_CMD_1 | 0x3))
#define CMD_GET_1RESULT_TEMP_DEFECT	(SET_AB(WORD_CMD_1 | 0x4))
#define CMD_GET_1RESULT_SPINE_TEMP	(SET_AB(WORD_CMD_1 | 0x5))

/*CMD SELECT */
#define CMD_SELECT_MODE_FIRST		(0x4 << 9)
#define CMD_SELECT_MODE_ID		(0x5 << 9)
#define CMD_SELECT_MODE_ALL		(0x6 << 9)
#define CMD_SELECT_MODE_LNKE		(0x7 << 9)

#define CMD_SELECT_FIRST 		(SET_AB(WORD_CMD_3 | CMD_SELECT_MODE_FIRST))
#define CMD_SELECT_ID(dpu_id)		(SET_AB(WORD_CMD_3 | CMD_SELECT_MODE_ID, | dpu_id))

/* CMD WRITE_REG */
#define CMD_WRITE_REG_ADDR_NODE_ID	(0x0 << 8)
#define CMD_WRITE_REG_ADDR_IGNORE	(0x3 << 8)

#define CMD_WRITE_REG_SET_ID(dpu_id)	(SET_MSB_PARITY(WORD_CMD_C | CMD_WRITE_REG_ADDR_NODE_ID | dpu_id))
#define CMD_WRITE_REG_UNSELECT_FIRST	(SET_MSB_PARITY(WORD_CMD_C | CMD_WRITE_REG_ADDR_IGNORE | 0x1))

/* CMD NOP */
#define CMD_NOP				(WORD_CMD_E)

const uint16_t gi_init_seq[] = {
  CMD_SELECT_FIRST, CMD_WRITE_REG_SET_ID(DPU_ID_0), CMD_WRITE_REG_UNSELECT_FIRST,
  CMD_SELECT_FIRST, CMD_WRITE_REG_SET_ID(DPU_ID_1), CMD_WRITE_REG_UNSELECT_FIRST,
  CMD_SELECT_FIRST, CMD_WRITE_REG_SET_ID(DPU_ID_2), CMD_WRITE_REG_UNSELECT_FIRST,
  CMD_SELECT_FIRST, CMD_WRITE_REG_SET_ID(DPU_ID_3), CMD_WRITE_REG_UNSELECT_FIRST,
  CMD_SELECT_FIRST, CMD_WRITE_REG_SET_ID(DPU_ID_4), CMD_WRITE_REG_UNSELECT_FIRST,
  CMD_SELECT_FIRST, CMD_WRITE_REG_SET_ID(DPU_ID_5), CMD_WRITE_REG_UNSELECT_FIRST,
  CMD_SELECT_FIRST, CMD_WRITE_REG_SET_ID(DPU_ID_6), CMD_WRITE_REG_UNSELECT_FIRST,
  CMD_SELECT_FIRST, CMD_WRITE_REG_SET_ID(DPU_ID_7), CMD_WRITE_REG_UNSELECT_FIRST
};

const uint16_t spi_gi_lnke_status[] = {
    CMD_GET_1RESULT_CHIP_ID_LSB, CMD_NOP, CMD_NOP,
    CMD_GET_1RESULT_CHIP_ID_MSB, CMD_NOP, CMD_NOP,
    CMD_GET_1RESULT_PLL_LOCK, CMD_NOP, CMD_NOP,
//    CMD_GET_1RESULT_CHECK_RES, CMD_NOP, CMD_NOP,
//    CMD_GET_1RESULT_TEMP_DEFECT, CMD_NOP, CMD_NOP,
//    CMD_GET_1RESULT_SPINE_TEMP, CMD_NOP, CMD_NOP,
};


#endif /* __SPI_GI_CMD_H__ */
