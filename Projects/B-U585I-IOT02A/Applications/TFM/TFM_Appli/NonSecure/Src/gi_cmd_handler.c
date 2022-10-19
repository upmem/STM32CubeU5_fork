/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */

#include <stdio.h>
#include "error.h"
#include "gi_cmd_handler.h"
#include "gi_cmd.h"
#include "spi.h"

void gi_init (void) {
  if (SPI_GI_Transmit_Receive((uint16_t*)gi_init_seq, NULL, sizeof (gi_init_seq)/sizeof(uint16_t), SPI_TRANSFERT_MODE_16BIT_BLOCKING) != PILOT_SUCCESS) {
      Error_Handler();
  }
}

static void print_gi_word(uint16_t word)
{
  printf("word: 0x%04x\r\n", word);
  printf("  result: 0x%02x\r\n", GI_RESPONSE_GET_RESULT(word));
  printf("  result_odd_flag: 0x%01x\r\n", GI_RESPONSE_GET_ODD_FLAG(word));
  printf("  reserved: 0x%01x\r\n", GI_RESPONSE_GET_RESERVED(word));
  printf("  result_valid_flag: 0x%01x\r\n", GI_RESPONSE_GET_RESULT_VALID_FLAG(word));
  printf("  previous_odd_flag: 0x%01x\r\n", GI_RESPONSE_GET_PREVIOUS_ODD_FLAG(word));
}

void gi_check_lnke_status (void) {

  uint16_t rsp[sizeof(spi_gi_lnke_status)/sizeof(uint16_t)]; /* NOP doesn't have responses */
  uint16_t chip_id = 0;
  uint16_t pll_lock = 0;
  if (SPI_GI_Transmit_Receive((uint16_t*)spi_gi_lnke_status, rsp, sizeof (spi_gi_lnke_status)/sizeof(uint16_t), SPI_TRANSFERT_MODE_BURST_BLOCKING) != PILOT_SUCCESS) {
      Error_Handler();
  }
  chip_id = (GI_RESPONSE_GET_RESULT(rsp[5]) << 8) | GI_RESPONSE_GET_RESULT(rsp[2]);
  pll_lock = GI_RESPONSE_GET_RESULT(rsp[8]);

  if ((chip_id != CHIP_ID_FPGA) || (pll_lock != 0x1)) {
      for(uint8_t i = 0; i< sizeof(spi_gi_lnke_status)/sizeof(uint16_t); i+=3) {
          print_gi_word(rsp[i+2]);
      }
      printf ("LNKE status: chipid=0x%x, pll_lock=0x%x\r\n", chip_id, pll_lock);
      Error_Handler();
  }
}

