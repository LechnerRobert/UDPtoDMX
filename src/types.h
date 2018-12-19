#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>


typedef uint16_t  uint_dmxChannel;
#if (USE_H801==1) || (USE_DMXDUMMY==1)
#define QUEUESIZE 5
typedef uint16_t  uint_dmxValue;
typedef uint32_t uint_times;
#define maxValue 5000

#define RefreshTime 10000
#define RefreshBaseTime 11090    /*(1109 * RefreshTime / 10000)  * 10  */  

#else
#define QUEUESIZE 128
typedef uint8_t  uint_dmxValue;
typedef uint16_t uint_times;
#define maxValue 255
#define RefreshTime 10000
#define RefreshBaseTime 1109    /*(1109 * RefreshTime / 10000)*/
#endif

#endif