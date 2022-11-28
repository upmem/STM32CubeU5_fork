/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */

#ifndef __GI_CMD_HANDLER_H__
#define __GI_CMD_HANDLER_H__

pilot_error_t gi_init (void);
pilot_error_t gi_check_lnke_status (uint16_t ss_mask);
pilot_error_t gi_cfg_spi_slave (uint16_t ss_mask, uint16_t config);
pilot_error_t gi_set_cfg_pll_misc (uint16_t ss_mask, uint16_t config);
pilot_error_t gi_get_cfg_pll_misc (uint16_t ss_mask, uint16_t* pll_misc);

#endif
