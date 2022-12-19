/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */
#ifdef GI_CIPHER_EN
#include "stdint.h"
#include "error.h"
#include "spi.h"
#include "gi_cmd.h"
#include "utils.h"
#include "gi_al_sec.h"

/* ------------------------------------
 *  GI CIPHER enabled - functions defintion
 * ------------------------------------
*/
/*
 * In case of any error answer this function call the Error_Handler.
*/
pilot_error_t gi_al_transfer(uint16_t ss_mask, uint16_t* seq, uint16_t* answ, uint16_t word_nr) {
  pilot_error_t ret = PILOT_FAILURE;
  /* Send words over SPI */
  if (SPI_GI_Transmit_Receive(ss_mask, seq, answ, word_nr, SPI_TRANSFERT_MODE_BURST_BLOCKING) != PILOT_SUCCESS) {
      Error_Handler();
  }

  /* Check the answer, the first answer word is ignored  */
  if (check_answer(&answ[SPI_DRAIN_BUBBLE_NR], word_nr - SPI_DRAIN_BUBBLE_NR, NULL) != PILOT_SUCCESS) {
      Error_Handler();
  }

  ret = PILOT_SUCCESS;

  return ret;
}

inline pilot_error_t gi_al_init(uint16_t ss_mask) {
  uint16_t answ[COUNTOF(gi_cipher_en_seq)];
  return gi_al_transfer(ss_mask, (uint16_t *)gi_cipher_en_seq, answ, COUNTOF(gi_cipher_en_seq));
}

#endif

