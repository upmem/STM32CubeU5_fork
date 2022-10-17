/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */

#ifndef __GI_MSG_HANDLER_H__
#define __GI_MSG_HANDLER_H__

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
#define DPU_NR		(0x8)

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



void gi_DPU_init (void);
void gi_dump_LNKE_status (void);
#endif
