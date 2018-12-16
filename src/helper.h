#ifndef HELPER_H
#define HELPER_H

#include <stdint.h>
#include "types.h"

inline uint32_t timeDiff(uint32_t from, uint32_t to) {
  return to - from;
}

uint_dmxValue c255(uint8_t proz, uint8_t gamma);


#endif

