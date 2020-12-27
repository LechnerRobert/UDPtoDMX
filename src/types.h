#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

#if (defined(ESP8266) || defined(__AVR__))
#define ADRUINO 10805
#include <WString.h>
#else
  #define __FlashStringHelper char
  #define F(string_literal) (string_literal)
#endif

typedef uint16_t  uint_dmxChannel;
#if (USE_H801==1) || (USE_DMXDUMMY==1)
#define QUEUESIZE DMX_SIZE
typedef uint16_t  uint_dmxValue;
typedef uint32_t uint_times;
#define maxValue 5000
#define simulatorUpdateMillis 10

#define RefreshTime 10000
#define RefreshBaseTime 11090    /*(1109 * RefreshTime / 10000)  * 10  */  

#else
#ifndef QUEUESIZE
#define QUEUESIZE 128
#endif
typedef uint8_t  uint_dmxValue;
typedef uint16_t uint_times;
#define maxValue 255
#define simulatorUpdateMillis 50
#define RefreshTime 10000
#define RefreshBaseTime 1109    /*(1109 * RefreshTime / 10000)*/
#endif

#endif