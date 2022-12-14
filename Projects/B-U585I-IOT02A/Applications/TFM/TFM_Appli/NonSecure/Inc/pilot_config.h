/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */
#ifndef __PILOT_CONFIG_H__
#define __PILOT_CONFIG_H__

#define PILOT_PRIORITY_HIGHEST	(0)
#define PILOT_PRIORITY_LOWEST	(15)

/* Highest priority as STM hal may use tick inside interrupt handlers */
#define  PILOT_TICK_IRQ_PRIORITY      (PILOT_PRIORITY_HIGHEST)
#define  PILOT_SPI_IRQ_PRIORITY       (1)
#define  PILOT_GPDMA_IRQ_PRIORITY     (1)
#define  PILOT_LPDMA_IRQ_PRIORITY     (1)

/* Lowest priority to ensure that the context is switched only once in case of nested interrupts*/
#define  PILOT_PENDSV_IRQ_PRIORITY    (PILOT_PRIORITY_LOWEST)

#define  PILOT_RTOS_SUPPORT
#endif
