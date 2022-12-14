/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */

#ifndef __GI_CMD_HANDLER_H__
#define __GI_CMD_HANDLER_H__

pilot_error_t gi_init (uint16_t ss_mask);
pilot_error_t gi_check_lnke_status (uint16_t ss_mask);
void mailbox_polling (void *pvParameters);
void dpu_load (void *pvParameters);
void fake_request (void *pvParameters);
#endif
