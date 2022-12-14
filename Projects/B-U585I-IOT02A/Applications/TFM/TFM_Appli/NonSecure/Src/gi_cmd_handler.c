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
#include "gi_cmd.h"
#ifdef GI_CIPHER_EN
#define gi_cipher_en (1)
#include "gi_sec.h"
#else
#define gi_cipher_en (0)
#include "gi_unsec.h"
#endif
#include "FreeRTOS.h"
#include "task.h"
#include "board_config.h"
#include "queue.h"

#ifdef DEBUG
#define GI_DEBUG
#endif
#ifdef GI_DEBUG
/* DEBUG enable printf, let's increase the timeout */
#define GI_TIMEOUT	(TIME_MS(1000U)) /* 1 sec */
#else
#define GI_TIMEOUT	(TIME_MS(10U)) /* 10 ms */
#endif

/*
 * NR of BUBBLEs needed to overcome the SPI latency
 * we should not check the response for the BUBBLE
*/
#define SPI_DRAIN_BUBBLE_NR	(1)

#define SPI_IGNORE_WORDS_4     (4)
#define SPI_IGNORE_WORDS_132   (132)
#define SPI_IGNORE_WORDS_260   (260)
#define SPI_IGNORE_WORDS_516   (516)

#define CHIP_ID_FPGA        (0x0515)

#define SPI_MAX_BUF_SIZE	(512)
uint16_t gi_tmp_buffer[SPI_MAX_BUF_SIZE];
uint16_t common_answ_buffer[SPI_MAX_BUF_SIZE];
extern QueueHandle_t host_requests_queue;

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


/*
 * This function takes care of possible SPI transmission errors:
 * - If GI cipher is enabled the function fails on transmission error
 * - If the GI cipher is disabled the function resume the GI transmission as per specification
 * Only valid answers are copied to the answ buffer:
 * - Answer words related to transmission errors are not copied to the answ buffer
 * - The first answer word, related to the previous sequence, is not copied to the answ buffer
*/
static pilot_error_t GI_transfer(uint16_t ss_mask, uint16_t* seq, uint16_t* answ, uint16_t word_nr) {
  pilot_error_t ret = PILOT_FAILURE;
  uint32_t valid_nr = 0;
  uint32_t timestamp = get_timestamp();
  uint8_t error = 0;
  do {
    if (error) {
	/* Send BUBBLEs in accordance to RECOVERY CTRL register */
	gi_resume(ss_mask);
	error = 0;
    }
    /* Send words over SPI */
    if (SPI_GI_Transmit_Receive(ss_mask, seq, gi_tmp_buffer, word_nr, SPI_TRANSFERT_MODE_BURST_BLOCKING) != PILOT_SUCCESS) {
	Error_Handler();
    }

    /* Check the answer, the first answer word is ignored  */
    if (check_answer(&gi_tmp_buffer[SPI_DRAIN_BUBBLE_NR], word_nr - SPI_DRAIN_BUBBLE_NR, &valid_nr) != PILOT_SUCCESS) {
	error = 1;
    }

    /* Copy to the answer buffer the valid response only, the first answer word is ignored */
    memcpy(answ, &gi_tmp_buffer[SPI_DRAIN_BUBBLE_NR], valid_nr * sizeof(uint16_t));
    /* If needed re-send part of the sequence */
    answ += valid_nr;
    seq +=valid_nr;
    word_nr -=valid_nr;
  } while((!gi_cipher_en) && ((check_timeout(timestamp, GI_TIMEOUT) != PILOT_SUCCESS) && (error)));

  if (!error) {
    ret = PILOT_SUCCESS;
  }
  return ret;
}

pilot_error_t gi_init (uint16_t ss_mask) {
  uint16_t answ[COUNTOF(init_dpu_id_seq)];
  pilot_error_t ret = PILOT_FAILURE;
  do {

    if (gi_cipher_en) {
      if (gi_set_lnke_security(ss_mask) != PILOT_SUCCESS){
	break;
      }
    } else {
      if (gi_set_spi_recovery(ss_mask, SPI_IGNORE_WORDS_516) != PILOT_SUCCESS){
	break;
      }
    }
    if (GI_transfer(ss_mask, (uint16_t *)init_dpu_id_seq, answ, COUNTOF(init_dpu_id_seq)) != PILOT_SUCCESS) {
	break;
    }
    ret = PILOT_SUCCESS;
  } while(0);

  return ret;
}

pilot_error_t gi_check_lnke_status (uint16_t ss_mask) {
  uint16_t answ[COUNTOF(spi_gi_lnke_status_seq)];
  uint16_t chip_id = 0;
  uint16_t pll_lock = 0;
  pilot_error_t ret = PILOT_FAILURE;

  do {
      /* read the LNKE status registers*/
      if ((GI_transfer(ss_mask, (uint16_t*)spi_gi_lnke_status_seq, answ, COUNTOF(spi_gi_lnke_status_seq)) == PILOT_FAILURE) ||
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

#define  MAILBOX_ANSW_DATA_POS       (3)
pilot_error_t mailbox_read_write (uint16_t ss_mask, uint8_t dpu_id, uint8_t dpu_wr_data, uint8_t host_wr_data, uint8_t* dpu_rd_data, uint8_t* host_rd_data) {
  pilot_error_t ret = PILOT_FAILURE;
  uint16_t mailbox_seq[] = {
      ESC_SELECT(dpu_id), ESC_NOP,
      ESC_READ_WRITE_MAIL(host_wr_data, dpu_wr_data), ESC_NOP,
      BUBBLE
  };
  uint16_t answ[COUNTOF(mailbox_seq)];
  *dpu_rd_data = 0;
  *host_rd_data = 0;
  do {
      if (GI_transfer(ss_mask, (uint16_t *)mailbox_seq, answ, COUNTOF(mailbox_seq)) != PILOT_SUCCESS) {
        break;
      }
      if (popcount(GI_RESPONSE_GET_RESULT_VALID_FLAG(answ[MAILBOX_ANSW_DATA_POS])) < 2) {
	  break;
      }
      *dpu_rd_data = MAILBOX_GET_DPU_DATA(GI_RESPONSE_GET_RESULT(answ[MAILBOX_ANSW_DATA_POS]));
      *host_rd_data = MAILBOX_GET_HOST_DATA(GI_RESPONSE_GET_RESULT(answ[MAILBOX_ANSW_DATA_POS]));
      ret = PILOT_SUCCESS;
  } while (0);

  return ret;
}

void mailbox_polling (void *pvParameters) {
  uint8_t dpu_rd, host_rd;
  uint8_t dpu_wr = 0 , host_wr = 0;
  uint8_t dpu_rd_token = 0 , host_rd_token = 0;
  uint8_t dpu_wr_token = 0 , host_wr_token = 0;
  while(1) {
    for (uint8_t dpu_id = 0; dpu_id < DPU_NR; dpu_id++) {
      if (mailbox_read_write(DPU_DRAM_MASK_0, dpu_id, dpu_wr, host_wr, &dpu_rd, &host_rd) != PILOT_SUCCESS) {
	  // TODO call DPU-DRAM reset procedure;
	  Error_Handler();
      }
      if (dpu_rd_token != MAILBOX_GET_TOCKEN(dpu_rd)) {
	  dpu_rd_token = MAILBOX_GET_TOCKEN(dpu_rd);
	  dpu_wr_token = MAILBOX_INVERT_TOCKEN(dpu_wr_token);
	  dpu_wr = dpu_wr_token ;
      }
      if (host_rd_token != MAILBOX_GET_TOCKEN(host_rd)) {
	  host_rd_token = MAILBOX_GET_TOCKEN(host_rd);
	  host_wr_token = MAILBOX_INVERT_TOCKEN(host_wr_token);
	  host_wr =  host_wr_token ;
      }
      if (mailbox_read_write(DPU_DRAM_MASK_0, dpu_id, dpu_wr, host_wr, &dpu_rd, &host_rd)!= PILOT_SUCCESS) {
	  // TODO call DPU-DRAM reset procedure;
	  Error_Handler();
      }
    }
    /* Move the task in the blocked state for 1ms */
    vTaskDelay(pdMS_TO_TICKS( 1 ));
  }
}

void fake_request (void *pvParameters) {
  /* Move the task in the blocked state for 5s */
  vTaskDelay(pdMS_TO_TICKS( 5000 ));
  /* test fake message queueuing */
  uint32_t* fake_request = pvPortMalloc(sizeof(uint32_t));
  *fake_request = 0x87654321;
  if (xQueueSendToBack(host_requests_queue, &fake_request, 0) != pdPASS) {
    /* Could not send to the queue */
    Error_Handler();
  }
  vTaskSuspend(NULL);
}

void dpu_load (void *pvParameters) {
  pilot_error_t status = PILOT_FAILURE;
  uint32_t len = COUNTOF(secure_loader_facsimile);
  uint32_t offset = 0;
  uint32_t *request = 0;
  while(1) {
    xQueueReceive( host_requests_queue, &request, portMAX_DELAY);
    printf ("request 0x%x\r\n",*request);
    vPortFree((void *)request);
    while (len/SPI_MAX_BUF_SIZE) {
	status = GI_transfer(DPU_DRAM_MASK_0, (uint16_t *)&secure_loader_facsimile[offset], common_answ_buffer, SPI_MAX_BUF_SIZE);
	if (status != PILOT_SUCCESS) {
	    break;
	}
	len -= SPI_MAX_BUF_SIZE;
	offset += SPI_MAX_BUF_SIZE;
    }
    if (status == PILOT_SUCCESS) {
	GI_transfer(DPU_DRAM_MASK_0, (uint16_t *)&secure_loader_facsimile[offset], common_answ_buffer, len);
    }
  }
}

