/**
 *
 * Copyright (c) 2022 UPMEM.
 *
 */

#ifndef __BITOPS_H__
#define __BITOPS_H__

inline int popcount (int x) {
  return __builtin_popcount(x);
}

inline int odd_parity (int x) {
  return __builtin_parity(x);
}
#endif