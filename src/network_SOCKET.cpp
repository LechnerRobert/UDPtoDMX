#if (USE_SOCKET==1)


#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <winsock.h> 

#include "network.h"
#include "log.h"

//EthernetUDP udp;


void virt_networkClass::init() {
    uint8_t mymac[6] = {  0x54, 0x55, 0x58, 0x10, 0x00, 0xF5 };
    uint8_t mygw[4] = { 192, 168, 123, 254 };// ip of gateway
    uint8_t mymask[4] = { 255, 255, 255, 0 };// subnet mask

  DEBUG_PRINT(LOG_INFO, F("Init Network"));
  
  
  //  DEBUG_PRINT(LOG_INFO, F("Use STATIC IP"));
  //  Ethernet.begin(mymac, myip, mygw, mymask);
  // udp.begin(udpPort);
  isUP = true;
  DEBUG_PRINT(LOG_INFO, F("UDP UP"));
//delay(100);      
}


void virt_networkClass::loop() {
  int packetSize = 0; //udp.parsePacket(); 
  if(packetSize)
  {
    // read the packet into packetBufffer
//  udp.read(packetBuffer, maxPacketSize);
    if (packetSize > maxPacketSize) {
      packetSize = maxPacketSize;
    }
    DEBUG_PRINT(LOG_INFO, F("Network DATA"));
    
    onNetworkData(packetBuffer, packetSize);
  }
}

void virt_networkClass::Register_OnNetworkData(event_networkData callback) {
  DEBUG_PRINT(LOG_INFO, F("Network Callback registred"));
  onNetworkData = callback;
}

void virt_networkClass::beginPacket() {
  if (isUP) {
    myip[3] = DEBUG_IP;
//  udp.beginPacket(myip, DEBUG_PORT);
  }
}

#if (__FlashStringHelper != char)
void virt_networkClass::print(const __FlashStringHelper *ifsh) {
  if (isUP) {
//  udp.print(ifsh);
  }
}
#endif

void virt_networkClass::print(const int i){
  if (isUP) {
//  udp.print(i);
  }
}

void virt_networkClass::print(const char *c, int charlen){
  if (isUP) {
    if (charlen == 0) {
//    udp.print(c);
    } else {
//    udp.write(c, charlen);
    }
  }
}
void virt_networkClass::endPacket(){
  if (isUP) {
//  udp.endPacket();
  }
}


virt_networkClass virt_network;

#endif

