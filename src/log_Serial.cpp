#if DEBUG_SERIAL0 == 1 
#include <Arduino.h>
#include "log.h"

void DEBUG_BEGIN(uint8_t level){
  Serial.print(level);
  Serial.print(F(": ")) ;
}

void DEBUG_PRINT(const __FlashStringHelper *ifsh) {
  Serial.print(ifsh);
}

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