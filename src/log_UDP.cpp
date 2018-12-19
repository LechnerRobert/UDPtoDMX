#if DEBUG_UDP == 1 
#include "log.h"
#include "network.h"

void DEBUG_BEGIN(uint8_t level){
  if (virt_network.isUP) {
    virt_network.beginPacket();
    virt_network.print(level);
    virt_network.print(F(": ")) ;    
  }
}

#if (__FlashStringHelper != char) 
void DEBUG_PRINT(const __FlashStringHelper *ifsh) {
  virt_network.print(ifsh);
}
#endif

void DEBUG_PRINT(const int i) {
  virt_network.print(i);
}

void DEBUG_PRINT(const char *c) {
  virt_network.print(c);
}

void DEBUG_END(){
  virt_network.print("\0", 1);
  virt_network.endPacket();
}

void DEBUG_INIT(){
  
};


#endif