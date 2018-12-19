#ifndef LOG_H
#define LOG_H

#define LOG_EMERG 0 /* system is unusable */
#define LOG_ALERT 1 /* action must be taken immediately */
#define LOG_CRIT  2 /* critical conditions */
#define LOG_ERR   3 /* error conditions */
#define LOG_WARNING 4 /* warning conditions */
#define LOG_NOTICE  5 /* normal but significant condition */
#define LOG_INFO  6 /* informational */
#define LOG_DEBUG 7 /* debug-level messages */


#if (DEBUG_SERIAL0 == 0) && (DEBUG_UDP == 0) && (DEBUG_SYSLOG == 0)
  #define DEBUG_INIT
  #define DEBUG_BEGIN(level)
  #define DEBUG_PRINT(str)
  #define DEBUG_END()
  #define DEBUG_PRINT(level, str)
#else

#include <inttypes.h>
#include <WString_.h>


void DEBUG_INIT();
void DEBUG_BEGIN(uint8_t level);
void DEBUG_PRINT(const char *c); 
void DEBUG_PRINT(const __FlashStringHelper *ifsh); 
void DEBUG_PRINT(const int i); 
void DEBUG_END();
void DEBUG_PRINT(uint8_t level, const __FlashStringHelper *ifsh); 

#endif

#endif