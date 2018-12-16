#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>


typedef uint16_t  uint_dmxChannel;
#if USE_H801==1
#define QUEUESIZE 5
typedef uint16_t  uint_dmxValue;
typedef uint32_t uint_times;
#define maxValue 5000
#else
#define QUEUESIZE 128
typedef uint8_t  uint_dmxValue;
typedef uint16_t uint_times;
#define maxValue 255
#endif

#endif