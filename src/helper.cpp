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
    result = p2 * p2 * maxValue / 10000;
  } else if (gamma == 1) {
    uint32_t tmp = p2 * p2 * maxValue / 10000;
    tmp = tmp + (p2 * maxValue / 100);
    result = tmp / 2;
  } else {
    result = p2 * maxValue / 100;
  }
  if ((proz > 0) && (result == 0)) {
    result = 1;
  }
  return result;
};

#ifndef ARDUINO
#include <windows.h>
#include <stdio.h>

uint32_t PCFreq = 0;
LARGE_INTEGER start_li;

void StartCounter()
{
    LARGE_INTEGER li;
    if(!QueryPerformanceFrequency(&li)) {
      printf("QueryPerformanceFrequency failed!\n");
    }
    PCFreq = (li.QuadPart)/1000000.0;
    QueryPerformanceCounter(&start_li);

}

double GetCounter()
{
}

uint32_t millis(){
  LARGE_INTEGER li;
  QueryPerformanceCounter(&li);
  return (uint32_t) ((li.QuadPart - start_li.QuadPart) / (PCFreq * 1000));
}

uint32_t micros(){
  LARGE_INTEGER li;
  QueryPerformanceCounter(&li);
  return (uint32_t) ((li.QuadPart - start_li.QuadPart) / (PCFreq));
}

#endif