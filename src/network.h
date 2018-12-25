#ifndef NETWORK_H
#define NETWORK_H

#include <types.h>
#include <inttypes.h>

//SPI
#define SS_SD_CARD   4
#define SS_ETHERNET 10


#define maxPacketSize (32*2)  //MAX Message Size
#define udpPort 7000

typedef void (*event_networkData)(char *data, int datalen);

class virt_networkClass
{
  private:
    char packetBuffer[maxPacketSize]; //buffer to hold incoming packet, 

    //STATIC IP, ignored if USE_DHCP is defined
    uint8_t myip[4] = { 192, 168, ST_IP_3, ST_IP_4 };
    
    event_networkData onNetworkData;
    
  public:
    bool isUP = false;
    void init();
    void loop();
    void Register_OnNetworkData(event_networkData callback);
    void beginPacket();    
    #ifdef ARDUINO
      void print(const __FlashStringHelper *ifsh); 
    #endif
    void print(const int i); 
    void print(const char *c, int charlen = 0); 
    void endPacket();
};

extern virt_networkClass virt_network;

#endif