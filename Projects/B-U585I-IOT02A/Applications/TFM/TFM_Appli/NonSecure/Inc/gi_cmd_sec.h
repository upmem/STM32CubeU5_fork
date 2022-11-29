/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */

#ifndef __GI_CMD_SEC_H__
#define __GI_CMD_SEC_H__

/* ---------------------
 * Secure sequences
 * ---------------------
*/

const uint16_t gi_cipher_en_seq [] = {
    0x5b00, 0x5800, 0x3aa4, 0x5f0b,
    0xfb74, 0xfbaf, 0x3c01, 0xfa19,
    0xfb3c, 0xba2e,
    0x0000
};

/*
 * DPU shall be numbered in a specific older (1,0,2,3,4,5,7,6)
 * due to Local Interface constraints.
*/
const uint16_t init_dpu_id_seq[] = {
    0x38c0, 0xe234, 0xcc8a, 0x89b7,
    0xd74f, 0x6aa1, 0x31bb, 0x8614,
    0xd2f1, 0x4ffc, 0xc3f4, 0x4b01,
    0x364a, 0x4393, 0xd273, 0x0727,
    0xc623, 0x1a33, 0x3bd2, 0x72fb,
    0xc200, 0x6026, 0xc7fa, 0x2149,
    0x3b90, 0x3c13, 0xd20b, 0x9f13,
    0xdda4, 0x4faf, 0x3c58, 0x6190,
    0xcd47, 0x19a5, 0xd4ef, 0x6a32,
    0x3b77, 0xcbdb, 0xd51e, 0x6ebb,
    0xc122, 0x2d1c, 0x3bbb, 0x5878,
    0xdf69, 0x6206, 0xc0bf, 0x9d9c,
    0x0000,
};

/*
* Ilink needs to close a cluster (word-signature, word-signature) in order to process a reading request,
* if two reading are in the same cluster the second would overwrite the first result.
* Build un-secure sequence with the same logic in order to make it as much as possible
* portable to secure scenario (signatures are anyway missed)
*/
const uint16_t spi_gi_lnke_status_seq[] = {
    0x1f9a, 0x65a4, 0xea18, 0x7672,
    0x1475, 0x2734, 0xe3a6, 0x4bab,
    0x1fa3, 0xca75, 0xf463, 0x6073,
    0xe0fc, 0xafc7, 0xf1aa, 0x5818,
    0x0000,
};
/*
 * The first answer word, related to the previous SPI sequence,
 * is not copied in the response buffer
*/
#define  CHIPID_MSB_ANSW_POS     (8)
#define  CHIPID_LSB_ANSW_POS     (4)
#define  PLL_LOCK_ANSW_POS       (12)

/* ---------------------
 * Secure functions
 * ---------------------
*/
static pilot_error_t GI_transfer(uint16_t* seq, uint16_t* answ, uint16_t word_nr);
static pilot_error_t gi_set_spi_recovery (uint8_t conf) {
  return PILOT_SUCCESS;
}

static inline pilot_error_t gi_set_lnke_security(void) {
  uint16_t answ[sizeof(gi_cipher_en_seq)/sizeof(uint16_t)];
  return GI_transfer((uint16_t *)gi_cipher_en_seq, answ, sizeof(gi_cipher_en_seq)/sizeof(uint16_t));
}

static void gi_resume () {
  /* We should never enter here */
  Error_Handler();
}

#endif /* __GI_CMD_H__ */
