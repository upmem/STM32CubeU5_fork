/**
 *
 * Copyright (c) 2022-2023 UPMEM.
 *
 */
#include <stdio.h>
#include <string.h>
#include "error.h"
#include "gi.h"
#include "spi.h"
#include "system.h"
#include "bitops.h"
#include "gi_cmd.h"
#ifdef GI_CIPHER_EN
#include "gi_al_sec.h"
#else
#include "gi_al_unsec.h"
#endif
#include "FreeRTOS.h"
#include "task.h"
#include "board_config.h"
#include "psa/error.h"
#include "psa/internal_trusted_storage.h"
#include "pilot_keys.h"


uint16_t answ_buffer[SPI_BUF_WORDS_NR];

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

pilot_error_t check_answer(uint16_t *answ, uint32_t word_nr, uint32_t *valid_nr){
  /* Check for errors in the answer */
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

pilot_error_t gi_init (uint16_t ss_mask) {
  pilot_error_t ret = PILOT_FAILURE;
  uint16_t seq_answ[COUNTOF(init_dpu_id_seq)];
  do{
    if (gi_al_init(ss_mask) !=  PILOT_SUCCESS) {
	break;
    }
    if (gi_al_transfer(ss_mask, (uint16_t *)init_dpu_id_seq, seq_answ, COUNTOF(init_dpu_id_seq), SPI_TRANSFERT_MODE_BURST_BLOCKING) != PILOT_SUCCESS) {
	break;
    }
    ret = PILOT_SUCCESS;
  } while(0);
  return ret;
}

pilot_error_t gi_check_lnke_status (uint16_t ss_mask) {
  uint16_t chip_id = 0;
  uint16_t pll_lock = 0;
  pilot_error_t ret = PILOT_FAILURE;
  uint16_t seq_answ[COUNTOF(spi_gi_lnke_status_seq)];
  do {
      /* read the LNKE status registers*/
      if ((gi_al_transfer(ss_mask, (uint16_t*)spi_gi_lnke_status_seq, seq_answ, COUNTOF(spi_gi_lnke_status_seq), SPI_TRANSFERT_MODE_BURST_BLOCKING) != PILOT_SUCCESS) ||
          /* Verify there are valid results in the appropriate answer words */
	  (popcount(GI_RESPONSE_GET_RESULT_VALID_FLAG(seq_answ[CHIPID_MSB_ANSW_POS])) < 2) ||
	  (popcount(GI_RESPONSE_GET_RESULT_VALID_FLAG(seq_answ[CHIPID_LSB_ANSW_POS])) < 2) ||
	  (popcount(GI_RESPONSE_GET_RESULT_VALID_FLAG(seq_answ[PLL_LOCK_ANSW_POS])) < 2)
         ){
	  break;
      }
      chip_id = (GI_RESPONSE_GET_RESULT(seq_answ[CHIPID_MSB_ANSW_POS]) << 8) | GI_RESPONSE_GET_RESULT(seq_answ[CHIPID_LSB_ANSW_POS]);
      pll_lock = GI_RESPONSE_GET_RESULT(seq_answ[PLL_LOCK_ANSW_POS]);

      if ((chip_id != CHIP_ID_FPGA) || (pll_lock != 0x1)) {
	  print_gi_word(seq_answ[CHIPID_MSB_ANSW_POS]);
	  print_gi_word(seq_answ[CHIPID_LSB_ANSW_POS]);
	  print_gi_word(seq_answ[PLL_LOCK_ANSW_POS]);
	  printf ("LNKE status issue: chipid=0x%x, pll_lock=0x%x\r\n", chip_id, pll_lock);
	  break;
      }
      ret = PILOT_SUCCESS;
    } while (0);
  return ret;
}

#define  MAILBOX_ANSW_DATA_POS       (4)
pilot_error_t mailbox_read_write (uint16_t ss_mask, uint8_t dpu_id, uint8_t dpu_wr_data, uint8_t host_wr_data, uint8_t* dpu_rd_data, uint8_t* host_rd_data) {
  pilot_error_t ret = PILOT_FAILURE;
  uint16_t mailbox_seq[] = {
      ESC_SELECT(dpu_id), ESC_NOP,
      ESC_READ_WRITE_MAIL(host_wr_data, dpu_wr_data), ESC_NOP,
      BUBBLE
  };
  uint16_t seq_answ[COUNTOF(mailbox_seq)];
  *dpu_rd_data = 0;
  *host_rd_data = 0;
  do {
      if (gi_al_transfer(ss_mask, (uint16_t *)mailbox_seq, seq_answ, COUNTOF(mailbox_seq), SPI_TRANSFERT_MODE_BURST_BLOCKING) != PILOT_SUCCESS) {
        break;
      }
      if (popcount(GI_RESPONSE_GET_RESULT_VALID_FLAG(seq_answ[MAILBOX_ANSW_DATA_POS])) < 2) {
	  break;
      }
      *dpu_rd_data = MAILBOX_GET_DPU_DATA(GI_RESPONSE_GET_RESULT(seq_answ[MAILBOX_ANSW_DATA_POS]));
      *host_rd_data = MAILBOX_GET_HOST_DATA(GI_RESPONSE_GET_RESULT(seq_answ[MAILBOX_ANSW_DATA_POS]));
      ret = PILOT_SUCCESS;
  } while (0);

  return ret;
}

/*
 * TODO This function is an inital draft of the MAILBOX protocol:
 * In case of Host/DPU has toggled the rd tocken, Pilot reads the related data and toggles the wr tocken
 */
void gi_task_mailbox_polling (void *pvParameters) {
  uint8_t dpu_rd, host_rd;
  uint8_t dpu_wr = 0 , host_wr = 0;
  uint8_t dpu_rd_token = 0 , host_rd_token = 0;
  uint8_t dpu_wr_token = 0 , host_wr_token = 0;
  while(1) {
    //TODO add loop on all DPU-DRAMs
    for (uint8_t dpu_id = 0; dpu_id < DPU_NR; dpu_id++) {
      if (mailbox_read_write(DPU_DRAM_MASK_0, dpu_id, dpu_wr, host_wr, &dpu_rd, &host_rd) != PILOT_SUCCESS) {
	  // TODO call DPU-DRAM reset procedure;
	  Error_Handler();
      }
      if (dpu_rd_token != MAILBOX_GET_TOKEN(dpu_rd)) {
	  dpu_rd_token = MAILBOX_GET_TOKEN(dpu_rd);
	  dpu_wr_token = MAILBOX_INVERT_TOKEN(dpu_wr_token);
	  dpu_wr = dpu_wr_token ;
      }
      if (host_rd_token != MAILBOX_GET_TOKEN(host_rd)) {
	  host_rd_token = MAILBOX_GET_TOKEN(host_rd);
	  host_wr_token = MAILBOX_INVERT_TOKEN(host_wr_token);
	  host_wr =  host_wr_token ;
      }
      if (mailbox_read_write(DPU_DRAM_MASK_0, dpu_id, dpu_wr, host_wr, &dpu_rd, &host_rd)!= PILOT_SUCCESS) {
	  // TODO call DPU-DRAM reset procedure;
	  Error_Handler();
      }
    }
    /* Move the task to blocked state for 1ms */
    vTaskDelay(pdMS_TO_TICKS(1));
  }
}


/*
 * This function read the keys from the ITS
 * TODO share the keys with the DPUs once integrated
 */
pilot_error_t gi_share_keys (void) {
  uint8_t master_key[AES_128_KEY_SIZE];
  uint8_t server_pub_key[ECDSA_P256_PUB_KEY_SIZE];
  size_t size;
  pilot_error_t status =  PILOT_FAILURE;
  do {
    if (
	(psa_its_get(ITS_MASTER_KEY_UID, 0 , sizeof(master_key), master_key, &size) != PSA_SUCCESS) ||
	(size != sizeof(master_key))
	){
	break;
    }

    if (
	(psa_its_get(ITS_SERVER_PUB_KEY_UID, 0 , sizeof(server_pub_key), server_pub_key, &size) != PSA_SUCCESS) ||
	(size != sizeof(server_pub_key))
	){
	break;
    }
    /* do not print any secret key */
    printf("server public key:\r\n");
    for (uint8_t i=0; i< sizeof(server_pub_key); i++){
	printf("0x%x ", server_pub_key[i]);
    }
    printf("\r\n");
    status = PILOT_SUCCESS;
  } while(0);
  return status;
}


/*
 * This function loads a fake binary on all the DPU IRAMs of the given DPU-DRAM
 * TODO replace with DPU FW binaries.
 * */
pilot_error_t gi_dpu_load (void) {
  pilot_error_t status = PILOT_FAILURE;
  uint32_t seq_len = COUNTOF(secure_loader_facsimile_seq);
  uint32_t transfer_len = 0;
  uint32_t offset = 0;
  uint16_t *seq_answ = pvPortMalloc(SPI_BUF_WORDS_NR * sizeof(uint16_t));
  do {
    while (seq_len) {
	transfer_len = (seq_len > SPI_BUF_WORDS_NR) ? SPI_BUF_WORDS_NR : seq_len;

	status = gi_al_transfer(DPU_DRAM_MASK_0, (uint16_t *)&secure_loader_facsimile_seq[offset], seq_answ, transfer_len, SPI_TRANSFERT_MODE_DMA);
	if (status != PILOT_SUCCESS) {
	    break;
	}
	seq_len -= transfer_len;
	offset += transfer_len;
    }

    if (status != PILOT_SUCCESS) {
	break;
    }

    printf("IRAM successfully loaded\r\n");

    if (gi_share_keys() != PILOT_SUCCESS) {
	break;
    }

    status = PILOT_SUCCESS;
  } while(0);
  vPortFree(seq_answ);
  return status;
}

