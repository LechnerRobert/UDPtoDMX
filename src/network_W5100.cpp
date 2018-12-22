#if USE_W5100 == 1

#include "network.h"
#include "log.h"
#include <Ethernet.h>
#include <EthernetUdp.h>

EthernetUDP udp;

#if USE_EEPROM_MAC == 1
  #include <Wire.h>                // for MAC from 24AA02E48
  #define UUID_I2C_ADDRESS 0x50    // i2c Address of UUID EEPROM

  byte readUUIDRegister(byte r) {
    //from http://www.freetronics.com.au/pages/setting-the-mac-address-using-the-mcp-24aa025e48
    unsigned char v;
    Wire.beginTransmission(UUID_I2C_ADDRESS);
    Wire.write(r);  // Register to read
    Wire.endTransmission();

    Wire.requestFrom(UUID_I2C_ADDRESS, 1); // Read a byte
    while (!Wire.available()) {} //wait
    v = Wire.read();
    return v;
  }
#endif


void virt_networkClass::init() {
    uint8_t mymac[6] = {  0x54, 0x55, 0x58, 0x10, 0x00, 0xF5 };
    uint8_t mygw[4] = { 192, 168, 123, 254 };// ip of gateway
    uint8_t mymask[4] = { 255, 255, 255, 0 };// subnet mask

  DEBUG_PRINT(LOG_INFO, F("Init Network"));
  
  #if USE_EEPROM_MAC == 1
    DEBUG_PRINT(LOG_INFO, F("Use EEPROM for MAC"));
    //read bytes for MAC from 24AA025E48 via i2c
    Wire.begin();
    mymac[0] = readUUIDRegister(0xFA);
    mymac[1] = readUUIDRegister(0xFB);
    mymac[2] = readUUIDRegister(0xFC);
    mymac[3] = readUUIDRegister(0xFD);
    mymac[4] = readUUIDRegister(0xFE);
    mymac[5] = readUUIDRegister(0xFF);
  //char tmpBuf[17];
 // sprintf(tmpBuf, "%02X:%02X:%02X:%02X:%02X:%02X", mymac[0], mymac[1], mymac[2], mymac[3], mymac[4], mymac[5]);
 // DEBUG_PRINT(LOG_INFO, F("My MAC: "));
 // DEBUG_PRINTln(LOG_INFO, tmpBuf);
  #endif

  pinMode(SS_SD_CARD, OUTPUT);
  pinMode(SS_ETHERNET, OUTPUT);
  digitalWrite(SS_SD_CARD, HIGH);  // SD Card not active
  digitalWrite(SS_ETHERNET, LOW); // Ethernet active  
  
 #if USE_DHCP == 1
    DEBUG_PRINT(LOG_INFO, F("Use DHCP"));
    if (Ethernet.begin(mymac) == 0) {
      DEBUG_PRINT(LOG_ERR, F("IP: Failed to configure Ethernet using DHCP"));
      // no point in carrying on, so do nothing forevermore:
      for (;;)
        ;
    }    
  #else
    DEBUG_PRINT(LOG_INFO, F("Use STATIC IP"));
    Ethernet.begin(mymac, myip, mygw, mymask);
  #endif
  udp.begin(udpPort);
  isUP = true;
  DEBUG_PRINT(LOG_INFO, F("UDP UP"));
  delay(100);      
}


void virt_networkClass::loop() {
  int packetSize = udp.parsePacket(); 
  if (packetSize) {
    // read the packet into packetBufffer
    udp.read(packetBuffer, maxPacketSize);
    if (packetSize > maxPacketSize) {
      packetSize = maxPacketSize;
    }
    DEBUG_PRINT(LOG_INFO, F("Network DATA"));
    
    onNetworkData(packetBuffer, packetSize);
  }
  #ifdef USE_DHCP
  Ethernet.maintain();
  #endif
}

void virt_networkClass::Register_OnNetworkData(event_networkData callback) {
  DEBUG_PRINT(LOG_INFO, F("Network Callback registred"));
  onNetworkData = callback;
}

void virt_networkClass::beginPacket() {
  if (isUP) {
    myip[3] = DEBUG_IP;
    udp.beginPacket(myip, DEBUG_PORT);
  }
}
void virt_networkClass::print(const __FlashStringHelper *ifsh) {
  if (isUP) {
    udp.print(ifsh);
  }
}

void virt_networkClass::print(const int i){
  if (isUP) {
    udp.print(i);
  }
}

void virt_networkClass::print(const char *c, int charlen){
  if (isUP) {
    if (charlen == 0) {
      udp.print(c);
    } else {
      udp.write(c, charlen);
    }
  }
}
void virt_networkClass::endPacket(){
  if (isUP) {
    udp.write(uint8_t(0));
    udp.endPacket();
  }
}


virt_networkClass virt_network;

#endif