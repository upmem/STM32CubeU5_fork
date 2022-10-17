/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */

#include <stdio.h>
#include "error.h"
#include "gi_msg_handler.h"
#include "spi_gi_cmd.h"
#include "spi.h"

#define MAX_SPI_MSG_SIZE	32
uint16_t msg_buffer [MAX_SPI_MSG_SIZE];


void gi_DPU_init (void) {
  uint16_t msg [DPU_NR * 3];
  printf ("gi_DPU_init\r\n");
  for (uint8_t i= DPU_ID_0 ; i<= DPU_ID_7; i++) {
      msg[i] = CMD_SELECT_FIRST;
      msg[i] = CMD_WRITE_REG_SET_ID(i);
      msg[i] = CMD_WRITE_REG_UNSELECT_FIRST;
  }
  if (SPI_GI_Transmit_Receive(msg, NULL, sizeof (msg)/sizeof(uint16_t), SPI_TRANSFERT_MODE_16BIT_BLOCKING) != PILOT_SUCCESS) {
      Error_Handler();
  }
}

static void print_GI_word(uint16_t word)
{
  printf("word: 0x%04x\r\n", word);
  printf("  result: 0x%02x\r\n", GI_RESPONSE_GET_RESULT(word));
  printf("  result_odd_flag: 0x%01x\r\n", GI_RESPONSE_GET_ODD_FLAG(word));
  printf("  reserved: 0x%01x\r\n", GI_RESPONSE_GET_RESERVED(word));
  printf("  result_valid_flag: 0x%01x\r\n", GI_RESPONSE_GET_RESULT_VALID_FLAG(word));
  printf("  previous_odd_flag: 0x%01x\r\n", GI_RESPONSE_GET_PREVIOUS_ODD_FLAG(word));
}

void gi_dump_LNKE_status (void) {
  uint16_t msg[] = {
      CMD_GET_1RESULT_CHIP_ID_LSB, CMD_NOP, CMD_NOP,
      CMD_GET_1RESULT_CHIP_ID_MSB, CMD_NOP, CMD_NOP,
//      CMD_GET_1RESULT_PLL_LOCK, CMD_NOP, CMD_NOP,
//      CMD_GET_1RESULT_CHECK_RES, CMD_NOP, CMD_NOP,
//      CMD_GET_1RESULT_TEMP_DEFECT, CMD_NOP, CMD_NOP,
//      CMD_GET_1RESULT_SPINE_TEMP, CMD_NOP, CMD_NOP,
  };
  uint16_t rsp[sizeof(msg)/sizeof(uint16_t)]; /* NOP doesn't have responses */
  printf ("gi_dump_LNKE_status\r\n");
  if (SPI_GI_Transmit_Receive(msg, rsp, sizeof (msg)/sizeof(uint16_t), SPI_TRANSFERT_MODE_BURST_BLOCKING) != PILOT_SUCCESS) {
      Error_Handler();
  }

  for(uint8_t i = 2; i< sizeof(msg)/sizeof(uint16_t); i+=3) {
      print_GI_word(rsp[i]);
  }
}
