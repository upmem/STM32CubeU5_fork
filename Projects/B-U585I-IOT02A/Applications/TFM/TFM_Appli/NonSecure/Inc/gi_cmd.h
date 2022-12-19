/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */

#ifndef __GI_CMD_H__
#define __GI_CMD_H__

#include "bitops.h"

pilot_error_t check_answer(uint16_t *answ, uint32_t word_nr, uint32_t *valid_nr);
#ifdef DEBUG
#define GI_DEBUG
#endif
/* ---------------
 * Defines
 * --------------
 */

#define SPI_MAX_BUF_SIZE	(512)
/*
 * NR of BUBBLEs needed to overcome the SPI latency
 * we should not check the response for the BUBBLE
*/
#define SPI_DRAIN_BUBBLE_NR	(1)

#define CHIP_ID_FPGA        (0x0515)

/* -------------------
 * Words definition
 * -------------------
*/
/* Words type */
#define WORD_CMD_1	(0x1 << 12)
#define WORD_CMD_3	(0x3 << 12)
#define WORD_CMD_6	(0x6 << 12)
#define WORD_CMD_7	(0x7 << 12)
#define WORD_CMD_C	(0xC << 12)
#define WORD_CMD_A	(0xA << 12)
#define WORD_CMD_E	(0xE << 12)

#define WORD_ESC_5	(0x5 << 12)
#define WORD_ESC_8	(0x8 << 12)

#define WORD_INSTR	(0xA << 12)

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
#define EVEN_PARITY(word)		(~(odd_parity(word)) & 1)

#define AB_BITS_DELTA(word)		((popcount(word) < 3 ) ? (word | (((1 << (3 - popcount(word))) -1) << 8)) : word )
#define AB_PARITY(word)			((word & (1 << 8)) ? (word | EVEN_PARITY(word) << 9) : (word | EVEN_PARITY(word) << 8))

#ifndef SET_AB
#define SET_AB(word)			(AB_PARITY(AB_BITS_DELTA(word)))
#endif
#ifndef SET_MSB_PARITY
#define SET_MSB_PARITY(word)		(word |  EVEN_PARITY(word) << 12)
#endif

/* CMD GET_1RESULT */
#define LNKE_CTRL_REG_ID_LSB_ADDR		(0x0)
#define LNKE_CTRL_REG_ID_MSB_ADDR		(0x1)
#define LNKE_CTRL_REG_PLL_MISC_ADDR		(0x2)
#define LNKE_CTRL_REG_PLL_LOCK_ADDR		(0x3)
#define LNKE_CTRL_REG_TEMP_CONF_DEFECT_ADDR	(0x4)
#define LNKE_CTRL_REG_CHECK_OUTCOME_ADDR	(0x5)
#define LNKE_CTRL_REG_TEMP_ADDR			(0x6)


#define CMD_GET_1RESULT_CHIP_ID_LSB		(SET_AB(WORD_CMD_1 | LNKE_CTRL_REG_ID_LSB_ADDR))
#define CMD_GET_1RESULT_CHIP_ID_MSB		(SET_AB(WORD_CMD_1 | LNKE_CTRL_REG_ID_MSB_ADDR))
#define CMD_GET_1RESULT_PLL_MISC		(SET_AB(WORD_CMD_1 | LNKE_CTRL_REG_PLL_MISC_ADDR))
#define CMD_GET_1RESULT_PLL_LOCK		(SET_AB(WORD_CMD_1 | LNKE_CTRL_REG_PLL_LOCK_ADDR))
#define CMD_GET_1RESULT_TEMP_CONF_DEFECT	(SET_AB(WORD_CMD_1 | LNKE_CTRL_REG_TEMP_CONF_DEFECT_ADDR))
#define CMD_GET_1RESULT_CHECK_RES_OUTCOME	(SET_AB(WORD_CMD_1 | LNKE_CTRL_REG_CHECK_OUTCOME_ADDR))
#define CMD_GET_1RESULT_TEMP			(SET_AB(WORD_CMD_1 | LNKE_CTRL_REG_TEMP_ADDR))

/*CMD SELECT */
#define CMD_SELECT_MODE_NONE		(0x0 << 9)
#define CMD_SELECT_MODE_FIRST		(0x4 << 9)
#define CMD_SELECT_MODE_ID		(0x5 << 9)
#define CMD_SELECT_MODE_ALL		(0x6 << 9)
#define CMD_SELECT_MODE_LNKE		(0x7 << 9)

#define CMD_SELECT_NONE 		(SET_AB(WORD_CMD_3 | CMD_SELECT_MODE_NONE))
#define CMD_SELECT_FIRST 		(SET_AB(WORD_CMD_3 | CMD_SELECT_MODE_FIRST))
#define CMD_SELECT_ID(dpu_id)		(SET_AB(WORD_CMD_3 | CMD_SELECT_MODE_ID, | dpu_id))
#define CMD_SELECT_LNKE 		(SET_AB(WORD_CMD_3 | CMD_SELECT_MODE_LNKE))
#define CMD_SELECT_ALL	 		(SET_AB(WORD_CMD_3 | CMD_SELECT_MODE_ALL))


/* CMD WRITE_REG */
#define CMD_WRITE_REG_ADDR_NODE_ID	(0x0 << 8)
#define CMD_WRITE_REG_ADDR_IGNORE	(0x3 << 8)

#define CMD_WRITE_REG_SET_ID(dpu_id)	(SET_MSB_PARITY(WORD_CMD_C | CMD_WRITE_REG_ADDR_NODE_ID | dpu_id))
#define CMD_WRITE_REG_UNSELECT_FIRST	(SET_MSB_PARITY(WORD_CMD_C | CMD_WRITE_REG_ADDR_IGNORE | 0x1))

/* CMD WRITE_REG_A */
#define CMD_WRITE_REG_A_ADDR_SPI_RECOVERY	(0x0 << 8)
#define SPI_RECOVERY_516			(0)
#define SPI_RECOVERY_260			(1)
#define SPI_RECOVERY_132			(2)
#define SPI_RECOVERY_4				(3)
#define CMD_WRITE_REG_A_SPI_RECOVERY(x)	(SET_MSB_PARITY(WORD_CMD_C | CMD_WRITE_REG_A_ADDR_SPI_RECOVERY | (x & 0x3)))

/* CMD WRITE_REG_B */
#define CMD_WRITE_REG_B_ADDR_CFG_SEC		(0x1 << 8)
#define CFG_SEC_RANDOM_EN			(0x1 << 0)
#define CFG_SEC_DPU_MOD_EN			(0x1 << 1)
#define CFG_SEC_DPU_REG_EN			(0x1 << 2)
#define CFG_SEC_DPU_MUTED_DIS			(0x1 << 3)
#define CFG_SEC_DPU_SECURE_EN			(0x1 << 4)
#define CMD_WRITE_REG_B_CFG_SEC(cfg) (SET_MSB_PARITY(WORD_CMD_E | CMD_WRITE_REG_B_ADDR_CFG_SEC | (cfg & 0x1f)))

/* CMD NOP */
#define CMD_NOP				(WORD_CMD_E)

/* CMD RESUME*/
#define RESUME				(WORD_CMD_E)
#define BUBBLE				(0)

/* CMD INSTRUCTION */
#define CMD_INSTRUCTION(data)		(SET_MSB_PARITY(WORD_CMD_A | (data & 0xFFF)))

/* ESC INIT_KEY */
#define ESC_INIT_KEY_PRIMING_DIS	(0 << 0)
#define ESC_INIT_KEY_PRIMING_EN		(0 << 1)
#define ESC_INIT_KEY(prim)		(SET_AB(WORD_ESC_5 | (8 << 8) | (prim)))

/* ESC NOP */
#define ESC_NOP				(WORD_ESC_5 | (0xB << 8) | 0x3)

/* ESC PARAM_CLER */
#define ESC_PARAM_CLEAR			(WORD_ESC_5 | (0xB << 8))

/*ESC READ_WRITE_MAIL */
#define ESC_READ_WRITE_MAIL(host_data, dpu_data) (SET_AB(WORD_ESC_5 | (6 << 8) | ((host_data & 0xF) << 4) | (dpu_data & 0xF)))
#define MAILBOX_GET_HOST_DATA(answ)	((answ >> 4) & 0x7)
#define MAILBOX_GET_DPU_DATA(answ)	(answ & 0x7)
#define MAILBOX_GET_TOCKEN(answ_data)	(answ_data & 0x8)
#define MAILBOX_INVERT_TOCKEN(data)	((data ^ 0x8) & 0x8)

/* ESC SELECT */
#define ESC_SELECT(dpu_id)			(SET_AB(WORD_ESC_5 | dpu_id))

/* -------------------
 * Answer definition
 * -------------------
 * when PILOT transmits a 16-bits word on MOSI line through SPI interface,
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
#endif /* __GI_CMD_H__ */
