/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */

#ifndef __ERROR_H__
#define __ERROR_H__

#include <stddef.h>

typedef uint32_t pilot_error_t;
#define PILOT_SUCCESS	((pilot_error_t)0x5A)
#define PILOT_FAILURE	((pilot_error_t)0xA5)

#define Error_Handler() pilot_error(__FILE__, __LINE__)

void pilot_error(char* file, int line);

#endif /* __ERROR_H__ */
