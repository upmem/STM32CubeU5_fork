/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */

#ifndef __ERROR_H__
#define __ERROR_H__

#include <stddef.h>

#define Error_Handler() pilot_error(__FILE__, __LINE__)

void pilot_error(char* file, int line);

#endif /* __ERROR_H__ */
