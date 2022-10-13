/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */

#include <stdio.h>
#include "error.h"
#include "gi_msg_handler.h"
#include "spi.h"

#define MAX_SPI_MSG_SIZE	32
uint16_t msg_buffer [MAX_SPI_MSG_SIZE];


void init_dpu_id (void) {
  uint16_t msg [DPU_NR * 3];
  for (uint8_t i= DPU_ID_0 ; i<= DPU_ID_7; i++) {
      msg[i] = CMD_SELECT_FIRST;
      msg[i] = CMD_WRITE_REG_SET_ID(i);
      msg[i] = CMD_WRITE_REG_UNSELECT_FIRST;
  }
  if (SPI_tx(msg, sizeof (msg)/sizeof(uint16_t)) != 0) {
      Error_Handler();
  }
}

void dump_LNKE_status (void) {
  uint16_t msg[] = {
      CMD_GET_1RESULT_CHIP_ID_LSB, CMD_NOP, CMD_NOP,
      CMD_GET_1RESULT_CHIP_ID_MSB, CMD_NOP, CMD_NOP,
      CMD_GET_1RESULT_PLL_LOCK, CMD_NOP, CMD_NOP,
      CMD_GET_1RESULT_CHECK_RES, CMD_NOP, CMD_NOP,
      CMD_GET_1RESULT_TEMP_DEFECT, CMD_NOP, CMD_NOP,
      CMD_GET_1RESULT_SPINE_TEMP, CMD_NOP, CMD_NOP,
  };
  uint32_t size = sizeof(msg)/sizeof(uint16_t);
  uint16_t rsp[size / 3]; /* NOP doesn't have responses */

  for (int i = 0; i< (size / 3) ; i++) {
      if (SPI_tx(msg[i*3], 3) != 0) {
	Error_Handler();
      }
      if (SPI_rx(rsp[i], 1) != 0) {
	  Error_Handler();
      }
  }

}
