/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */
#ifndef GI_CIPHER_EN
#include <stdint.h>
#include "error.h"
#include "gi_cmd.h"
#include "system.h"
#include "utils.h"
#include "gi_al_unsec.h"
#include "spi.h"
#include <string.h>
#include "FreeRTOS.h"

#ifdef GI_DEBUG
/* DEBUG enable printf, let's increase the timeout */
#define GI_TIMEOUT	(TIME_MS(1000U)) /* 1 sec */
#else
#define GI_TIMEOUT	(TIME_MS(10U)) /* 10 ms */
#endif

/* spi_recovery_ignored_words_nr variable is updated by calling gi_set_spi_recovery function */
uint16_t spi_recovery_ignored_words_nr = SPI_BUF_WORDS_NR;

/* ------------------------------------
 *  GI CIPHER disabled - functions defintion
 * ------------------------------------
*/
static void gi_resume (uint16_t ss_mask) {
  /* Send recovery frame */
  uint16_t *answ_seq = pvPortMalloc(spi_recovery_ignored_words_nr * sizeof(uint16_t));
  if (
      (SPI_GI_Transmit_Receive(ss_mask, (uint16_t *)bubble_seq, answ_seq, spi_recovery_ignored_words_nr, SPI_TRANSFERT_MODE_BURST_BLOCKING) != PILOT_SUCCESS) ||
      (SPI_GI_Transmit_Receive(ss_mask, (uint16_t *)resume_seq, answ_seq, COUNTOF(resume_seq), SPI_TRANSFERT_MODE_BURST_BLOCKING) != PILOT_SUCCESS) ||
      /* Only the last answer word is of interest, we don't need to check BUBBLE responses */
      (check_answer(&answ_seq[COUNTOF(resume_seq) - 1], 1, NULL) != PILOT_SUCCESS)
  ){
      Error_Handler();
  }
  vPortFree(answ_seq);
}

/*
 * In case of error response this function resumes the GI transmission as per specification.
 * Only valid answers are copied to the answ buffer while errors are discarded.
 * The first word of each answer, related to the previous sequence received by the GI, is discarded.
*/
pilot_error_t gi_al_transfer(uint16_t ss_mask, uint16_t* seq, uint16_t* answ, uint16_t word_nr, int mode) {
  pilot_error_t ret = PILOT_FAILURE;
  uint32_t valid_nr = 0;
  uint32_t timestamp = get_timestamp();
  uint8_t error = 0;
  uint16_t *answ_tmp = pvPortMalloc(word_nr * sizeof(uint16_t));
   /* The first word of the answ_tmp is never copied to the answ buffer, shift the pointer by 1 position */
  answ++;
  do {
    if (error) {
	/* Send BUBBLEs in accordance to RECOVERY CTRL register */
	gi_resume(ss_mask);
	error = 0;
    }
    /* Send words over SPI */
    if (SPI_GI_Transmit_Receive(ss_mask, seq, answ_tmp, word_nr, mode) != PILOT_SUCCESS) {
	Error_Handler();
    }

    /* Check the answer, the first answer word is ignored  */
    if (check_answer(&answ_tmp[SPI_DRAIN_BUBBLE_NR], word_nr - SPI_DRAIN_BUBBLE_NR, &valid_nr) != PILOT_SUCCESS) {
	error = 1;
    }

    /* Copy to the answ buffer the valid part of the response only, the first answer word is always ignored */
    memcpy(answ, &answ_tmp[SPI_DRAIN_BUBBLE_NR], valid_nr * sizeof(uint16_t));
    /* If needed re-send part of the sequence */
    answ += valid_nr;
    seq +=valid_nr;
    word_nr -=valid_nr;
  } while((check_timeout(timestamp, GI_TIMEOUT) != PILOT_SUCCESS) && (error));

  if (!error) {
    ret = PILOT_SUCCESS;
  }
  vPortFree(answ_tmp);
  return ret;
}

static pilot_error_t gi_set_spi_recovery (uint16_t ss_mask, uint16_t conf) {
  uint16_t gi_set_spi_recovery_seq[] =
  {
      CMD_SELECT_LNKE, CMD_WRITE_REG_A_SPI_RECOVERY(SPI_RECOVERY_516),
      CMD_SELECT_NONE, CMD_NOP,
      BUBBLE
  };
  uint16_t answ[COUNTOF(gi_set_spi_recovery_seq)];\

  pilot_error_t ret = PILOT_FAILURE;
  uint16_t ignored_words_nr;

  switch (conf) {
    case(SPI_RECOVERY_4):
	gi_set_spi_recovery_seq[1] = CMD_WRITE_REG_A_SPI_RECOVERY(SPI_RECOVERY_4);
	ignored_words_nr = 4;
	break;
    case(SPI_RECOVERY_132):
	gi_set_spi_recovery_seq[1] = CMD_WRITE_REG_A_SPI_RECOVERY(SPI_RECOVERY_132);
	ignored_words_nr = 132;
	break;
    case(SPI_RECOVERY_260):
	gi_set_spi_recovery_seq[1] = CMD_WRITE_REG_A_SPI_RECOVERY(SPI_RECOVERY_260);
	ignored_words_nr = 260;
	break;
    case(SPI_RECOVERY_516):
	gi_set_spi_recovery_seq[1] = CMD_WRITE_REG_A_SPI_RECOVERY(SPI_RECOVERY_516);
	ignored_words_nr = 516;
	break;
    default:
	gi_set_spi_recovery_seq[1] = CMD_WRITE_REG_A_SPI_RECOVERY(SPI_RECOVERY_516);
	ignored_words_nr = 516;
	break;
  }

  /* Configure the SPI recovery CNTR register */
  if (gi_al_transfer(ss_mask, (uint16_t *)gi_set_spi_recovery_seq, answ, COUNTOF(gi_set_spi_recovery_seq), SPI_TRANSFERT_MODE_BURST_BLOCKING) == PILOT_SUCCESS) {
	ret = PILOT_SUCCESS;
	spi_recovery_ignored_words_nr = ignored_words_nr;
  }
  return ret;
}

pilot_error_t gi_al_init(uint16_t ss_mask){
  return gi_set_spi_recovery(ss_mask, SPI_RECOVERY_516);
}

#endif
