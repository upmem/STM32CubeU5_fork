/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <stdint.h>
#include "stm32u5xx_hal.h"
#include "tfm_api.h"
#include "tfm_ns_interface.h"
#include "FreeRTOS.h"
#include "semphr.h"

#ifdef VERBOSE
#define vprintf printf
#else
#define vprintf(...)
#endif

static SemaphoreHandle_t tfm_mutex = NULL;

/**
  * \brief NS world, NS lock based dispatcher
  */
int32_t tfm_ns_interface_dispatch(veneer_fn fn,
                                  uint32_t arg0, uint32_t arg1,
                                  uint32_t arg2, uint32_t arg3)
{
  vprintf("tfm_ns_interface_dispatch\r\n");
  uint32_t ret = PSA_ERROR_GENERIC_ERROR;
  if (xSemaphoreTake(tfm_mutex, portMAX_DELAY) == pdTRUE) {
	  ret = (uint32_t)fn(arg0, arg1, arg2, arg3);
	  xSemaphoreGive(tfm_mutex);
  }
  return ret;
}

enum tfm_status_e tfm_ns_interface_init(void)
{
  enum tfm_status_e status = TFM_ERROR_GENERIC;

  tfm_mutex = xSemaphoreCreateMutex();
  if (tfm_mutex != NULL) {
    status = TFM_SUCCESS;
  }
  return status;
}

