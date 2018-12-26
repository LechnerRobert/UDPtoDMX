#if DEBUG_SERIAL0 == 1 
#include <Arduino.h>
#include "log.h"

void DEBUG_BEGIN(uint8_t level){
  while(!Serial); /*   For Lenoardo: wait for serial monitor. */
  Serial.print(level);
  Serial.print(F(": ")) ;
}

#ifdef ARDUINO
void DEBUG_PRINT(const __FlashStringHelper *ifsh) {
  Serial.print(ifsh);
}
#endif

void DEBUG_PRINT(const int i) {
  Serial.print(i);
}

void DEBUG_PRINT(const char *c) {
  Serial.print(c);
}

void DEBUG_END(){
  Serial.println("");
}

void DEBUG_INIT(){
  Serial.begin(115200);
};

#endif