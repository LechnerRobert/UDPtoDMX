#if DEBUG_SYSLOG == 1 
#include "log.h"
#include "network.h"

void DEBUG_BEGIN(uint8_t level){
  if (virt_network.isUP) {
    virt_network.beginPacket();
    virt_network.print(F("<")); 
    virt_network.print(8 + level); 
    virt_network.print(F(">DMX "));     
  }
}

void DEBUG_PRINT(const __FlashStringHelper *ifsh) {
  virt_network.print(ifsh);
}

void DEBUG_PRINT(const int i) {
  virt_network.print(i);
}


void DEBUG_PRINT(const char *c) {
  virt_network.print(c);
}

void DEBUG_END(){
  virt_network.endPacket();
}

void DEBUG_INIT(){
  
};

#endif