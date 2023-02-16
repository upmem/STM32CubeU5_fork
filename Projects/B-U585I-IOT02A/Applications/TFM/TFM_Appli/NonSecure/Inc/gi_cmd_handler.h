/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */

#ifndef __GI_CMD_HANDLER_H__
#define __GI_CMD_HANDLER_H__

#define ITS_MASTER_KEY_UID	(0x1)
#define AES_128_KEY_SIZE	(16)

pilot_error_t gi_init (uint16_t ss_mask);
pilot_error_t gi_check_lnke_status (uint16_t ss_mask);
void gi_task_mailbox_polling (void *pvParameters);
pilot_error_t gi_dpu_load (void);
#endif
