#include "helper.h"

uint_dmxValue c255(uint8_t proz, uint8_t gamma) {
  
  uint32_t p2 = proz;
  uint_dmxValue result;
  if (gamma == 3) {
    #if (maxValue % 1000 == 0)
      result = p2* p2 * p2 * (maxValue / 1000) / 1000; 
    #else
      result = p2 * p2 * p2 * maxValue / 1000000;
    #endif  
  } else if (gamma == 2) {
    return p2 * p2 * maxValue / 10000;
  } else if (gamma == 1) {
    uint32_t tmp = p2 * p2 * maxValue / 10000;
    tmp = tmp + (p2 * maxValue / 100);
    return tmp / 2;
  } else {
    return p2 * maxValue / 100;
  }
  if ((proz > 0) && (result == 0)) {
    result = 1;
  }
  return result;
};


uint32_t millis(){
  struct timeb stop;
  ftime(&stop);
  return (uint32_t) (1000 * stop.time + stop.millitm);
}

uint32_t micros(){
  return 10;
}