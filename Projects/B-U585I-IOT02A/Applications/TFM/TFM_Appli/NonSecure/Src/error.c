/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */

#include <stdio.h>
#include "error.h"

void pilot_error(char* file, int line) {
  printf("Pilot_error\r\n");
  printf("file %s line %d\r\n", file, line);
  while(1) {}
}
