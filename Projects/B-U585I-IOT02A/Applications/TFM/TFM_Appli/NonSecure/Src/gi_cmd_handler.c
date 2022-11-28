/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */

#include <stdio.h>
#include <string.h>
#include "error.h"
#include "gi_cmd_handler.h"
#include "spi.h"
#include "system.h"
#include "bitops.h"
#ifdef DEBUG
#define GI_DEBUG
#endif
#ifdef GI_DEBUG
/* DEBUG enable printf, let's increase the timeout */
#define GI_TIMEOUT	(TIME_MS(1000U)) /* 1 sec */
#else
#define GI_TIMEOUT	(TIME_MS(10U)) /* 10 ms */
#endif

static pilot_error_t GI_transfer(uint16_t* seq, uint16_t* answ, uint16_t word_nr);
static pilot_error_t check_answer(uint16_t *answ, uint32_t word_nr, uint32_t *valid_nr);
uint16_t spi_recovery_ignored_words_nr = 516;
#define COUNTOF(array) (sizeof(array)/sizeof(array[0]))

#ifdef SEC_EN
#define cipher_en (1)
#include "gi_cmd_sec.h"
/*
 * The first answer word, related to the previous SPI sequence,
 * is not copied in the response buffer
*/
#define  CHIPID_MSB_ANSW_POS     (8)
#define  CHIPID_LSB_ANSW_POS     (4)
#define  PLL_LOCK_ANSW_POS       (12)
#else
#define cipher_en (0)
#include "gi_cmd.h"
/*
 * The first answer word, related to the previous SPI sequence,
 * is not copied in the response buffer
*/
#define  CHIPID_MSB_ANSW_POS     (4-1)
#define  CHIPID_LSB_ANSW_POS     (2-1)
#define  PLL_LOCK_ANSW_POS       (6-1)
#endif
/*
 * NR of BUBBLEs needed to overcome the SPI latency
 * we should not check the response for the BUBBLE
*/
#define SPI_DRAIN_BUBBLE_NR	(1)

#define SPI_IGNORE_WORDS_4	(4)
#define SPI_IGNORE_WORDS_132	(132)
#define SPI_IGNORE_WORDS_260	(260)
#define SPI_IGNORE_WORDS_516	(516)

#define  CHIP_ID_FPGA        (0x0515)

uint16_t gi_tmp_buffer[512];

static void print_gi_word(uint16_t word)
{
#ifdef GI_DEBUG
  printf("word: 0x%04x\r\n", word);
  printf("  result: 0x%02x\r\n", GI_RESPONSE_GET_RESULT(word));
  printf("  result_odd_flag: 0x%01x\r\n", GI_RESPONSE_GET_ODD_FLAG(word));
  printf("  reserved: 0x%01x\r\n", GI_RESPONSE_GET_RESERVED(word));
  printf("  result_valid_flag: 0x%01x\r\n", GI_RESPONSE_GET_RESULT_VALID_FLAG(word));
  printf("  previous_odd_flag: 0x%01x\r\n", GI_RESPONSE_GET_PREVIOUS_ODD_FLAG(word));
#endif
}

static pilot_error_t check_answer(uint16_t *answ, uint32_t word_nr, uint32_t *valid_nr){
  pilot_error_t ret = PILOT_SUCCESS;
  if (valid_nr != NULL){
      *valid_nr = 0;
  }

  for (uint32_t i = 0; i < word_nr; i++){
      if ((popcount(GI_RESPONSE_GET_PREVIOUS_ODD_FLAG(answ[i])) < 2) ||
          (odd_parity(GI_RESPONSE_GET_RESULT(answ[i])) != GI_RESPONSE_GET_ODD_FLAG(answ[i]))
	 ) {
	  print_gi_word(answ[i]);
          ret = PILOT_FAILURE;
          break;
      }
      if (valid_nr != NULL){
	  *valid_nr+=1;
      }
  }
  return ret;
}

#ifdef GI_ERROR
static void parity_toggle (uint16_t *word) {
  printf("before toggle 0x%x\r\n", *word);
  *word = *word ^ (1 << 8);
  printf("after toggle 0x%x\r\n", *word);
}
#endif


/*
 * This function takes care of possible SPI transmission errors
 * Only valid answers are copied to the answ buffer:
 * - Answer words related to transmission errors are not copied to the answ buffer
 * - The first answer word, related to the previous sequence, is not copied to the answ buffer
*/
static pilot_error_t GI_transfer(uint16_t* seq, uint16_t* answ, uint16_t word_nr) {
  pilot_error_t ret = PILOT_FAILURE;
  uint32_t valid_nr = 0;
  uint32_t timestamp = get_timestamp();
  uint8_t error = 0;
  do {
    if (error) {
	/* Send BUBBLEs in accordance to RECOVERY CTRL register */
	gi_resume();
	error = 0;
#ifdef GI_ERROR
	parity_toggle(seq);
#endif
    }
    /* Send words over SPI */
    if (SPI_GI_Transmit_Receive(seq, gi_tmp_buffer, word_nr, SPI_TRANSFERT_MODE_BURST_BLOCKING) != PILOT_SUCCESS) {
	Error_Handler();
    }

    /* Send the answer skipping the drain bubble */
    if (check_answer(&gi_tmp_buffer[SPI_DRAIN_BUBBLE_NR], word_nr - SPI_DRAIN_BUBBLE_NR, &valid_nr) != PILOT_SUCCESS) {
	error = 1;
    }

    /* Copy to the answer buffer the valid response only, the first answer word is ignored */
    memcpy(answ, &gi_tmp_buffer[SPI_DRAIN_BUBBLE_NR], valid_nr * sizeof(uint16_t));
    /* If needed re-send part of the sequence */
    answ += valid_nr;
    seq +=valid_nr;
    word_nr -=valid_nr;
  } while((!cipher_en) && ((check_timeout(timestamp, GI_TIMEOUT) != PILOT_SUCCESS) && (error)));

  if (!error) {
    ret = PILOT_SUCCESS;
  }
  return ret;
}

pilot_error_t gi_init (void) {
  uint16_t answ[COUNTOF(init_dpu_id_seq)];
  pilot_error_t ret = PILOT_FAILURE;
  do {

    if (cipher_en) {
      if (gi_set_lnke_security() != PILOT_SUCCESS){
	break;
      }
    } else {
      if (gi_set_spi_recovery(SPI_IGNORE_WORDS_132) != PILOT_SUCCESS){
	break;
      }
    }
    if (GI_transfer((uint16_t *)init_dpu_id_seq, answ, COUNTOF(init_dpu_id_seq)) != PILOT_SUCCESS) {
	break;
    }
    ret = PILOT_SUCCESS;
  } while(0);

  return ret;
}


pilot_error_t gi_check_lnke_status (void) {
  uint16_t answ[COUNTOF(spi_gi_lnke_status_seq)];
  uint16_t chip_id = 0;
  uint16_t pll_lock = 0;
  pilot_error_t ret = PILOT_FAILURE;

  do {
#ifdef GI_ERROR
      //parity_toggle(&spi_gi_lnke_status_seq[1]); /* to be used for GI bug verification*/
      parity_toggle(&spi_gi_lnke_status_seq[2]);
      parity_toggle(&spi_gi_lnke_status_seq[4]);
#endif
      /* read the LNKE status registers*/
      if ((GI_transfer((uint16_t*)spi_gi_lnke_status_seq, answ, COUNTOF(spi_gi_lnke_status_seq)) == PILOT_FAILURE) ||
          /* Verify there are valid results in the appropriate answer words */
	  (popcount(GI_RESPONSE_GET_RESULT_VALID_FLAG(answ[CHIPID_MSB_ANSW_POS])) < 2) ||
	  (popcount(GI_RESPONSE_GET_RESULT_VALID_FLAG(answ[CHIPID_LSB_ANSW_POS])) < 2) ||
	  (popcount(GI_RESPONSE_GET_RESULT_VALID_FLAG(answ[PLL_LOCK_ANSW_POS])) < 2)
         ){
	  break;
      }
      chip_id = (GI_RESPONSE_GET_RESULT(answ[CHIPID_MSB_ANSW_POS]) << 8) | GI_RESPONSE_GET_RESULT(answ[CHIPID_LSB_ANSW_POS]);
      pll_lock = GI_RESPONSE_GET_RESULT(answ[PLL_LOCK_ANSW_POS]);

      if ((chip_id != CHIP_ID_FPGA) || (pll_lock != 0x1)) {
	  print_gi_word(answ[CHIPID_MSB_ANSW_POS]);
	  print_gi_word(answ[CHIPID_LSB_ANSW_POS]);
	  print_gi_word(answ[PLL_LOCK_ANSW_POS]);
	  printf ("LNKE status issue: chipid=0x%x, pll_lock=0x%x\r\n", chip_id, pll_lock);
	  break;
      }
      ret = PILOT_SUCCESS;
    } while (0);
  return ret;
}

