/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */

#include <stdio.h>
#include "error.h"

void pilot_error(char* file, int line) {
  printf("Error_Handler file %s line %d\n", file, line);
  while(1) {}
}
