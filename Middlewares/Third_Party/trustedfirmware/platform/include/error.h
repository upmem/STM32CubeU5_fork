#ifndef _ERROR_H
#define _ERROR_H

#include "tfm_spm_log.h"

static inline void pilot_tfm_error(char* file, int size, int line) {
  tfm_hal_output_spm_log("pilot_tfm_error\r\n", sizeof("pilot_tfm_error\r\n"));
#ifdef TFM_DEV_MODE
  tfm_hal_output_spm_log(file, size);
  spm_log_msgval("\r\nline: ", sizeof("\r\nline: "), line);
  while(1) {}
#else
  /* Reset the system */
  NVIC_SystemReset();
#endif
}

#define Error_Handler() pilot_tfm_error(__FILE__, sizeof(__FILE__), __LINE__)

#endif
