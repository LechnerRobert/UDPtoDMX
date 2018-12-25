#ifndef HELPER_H
#define HELPER_H

#include <stdint.h>
#include "types.h"

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <sys\timeb.h>
uint32_t millis();
uint32_t micros();
#define max(a,b) ((a)>(b)?(a):(b))

void StartCounter();

#define PROGMEM
#endif

inline uint32_t timeDiff(uint32_t from, uint32_t to) {
  return to - from;
}

uint_dmxValue c255(uint8_t proz, uint8_t gamma);

  

#endif

