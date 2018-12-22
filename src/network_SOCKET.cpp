#if (USE_SOCKET==1)

#define _WIN32_WINNT 0x0501

#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>


#include "network.h"
#include "log.h"

//EthernetUDP udp;

int startWinsock(void);
  SOCKET socket_in;
  SOCKET socket_out;
  char buf[1024];
  int buflen;
  SOCKADDR_IN addr; //? can bei local, maybe ?
  

void virt_networkClass::init() {
    uint8_t mymac[6] = {  0x54, 0x55, 0x58, 0x10, 0x00, 0xF5 };
    uint8_t mygw[4] = { 192, 168, 123, 254 };// ip of gateway
    uint8_t mymask[4] = { 255, 255, 255, 0 };// subnet mask

  long rc;
  char ipstr[17];
  
  DEBUG_PRINT(LOG_INFO, F("Init Network"));
  rc = startWinsock();
  if (rc == 0) { 
    DEBUG_PRINT(LOG_INFO, F("Winsock gestartet!"));
  } else {
    DEBUG_BEGIN(LOG_ERR);
    DEBUG_PRINT(F("Fehler: startWinsock, fehler code: "));
    DEBUG_PRINT(rc);
    DEBUG_END();
    return;
  }
  // UDP Socket erstellen
  socket_in = socket(AF_INET, SOCK_DGRAM, 0);
  if(socket_in == INVALID_SOCKET) {
    DEBUG_BEGIN(LOG_ERR);
    DEBUG_PRINT(F("Fehler: Der Socket konnte nicht erstellt werden, fehler code: "));
    DEBUG_PRINT(WSAGetLastError());
    DEBUG_END();
    return ;
  } else {
    DEBUG_PRINT(LOG_INFO, F("UDP Socket erstellt!"));
  }
  
  DEBUG_PRINT(LOG_INFO, F("Use STATIC IP"));
// addr vorbereiten
  ZeroMemory((char*) &addr, sizeof(addr));

  
  addr.sin_family = AF_INET;
  addr.sin_port = htons(udpPort);
  sprintf(ipstr, "%d.%d.%d.%d", myip[0], myip[1], myip[2], myip[3]);

  addr.sin_addr.s_addr = INADDR_ANY;  //inet_addr(ipstr);
    
  rc = bind(socket_in, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));
    DEBUG_BEGIN(LOG_ERR);
    DEBUG_PRINT(F("UDP Bind Errorcode: "));
    DEBUG_PRINT(WSAGetLastError());
    DEBUG_PRINT(F(" "));
    DEBUG_PRINT(rc);
    DEBUG_END();
  
  //  Ethernet.begin(mymac, myip, mygw, mymask);
  // udp.begin(udpPort);
  
  uint32_t nTimeout = 1000; // 30 seconds
  setsockopt(socket_in, SOL_SOCKET, SO_RCVTIMEO, (const char*)&nTimeout, sizeof(int));


   u_long nonblocking_enabled = TRUE;

   ioctlsocket(socket_in, FIONBIO, &nonblocking_enabled );

  DEBUG_PRINT(LOG_INFO, ipstr);
  DEBUG_PRINT(LOG_INFO, F("UDP UP"));
//delay(100);      

// DEBUG UDP Socket erstellen
  socket_out = socket(AF_INET, SOCK_DGRAM, 0);
  if(socket_out == INVALID_SOCKET) {
    DEBUG_BEGIN(LOG_ERR);
    DEBUG_PRINT(F("Fehler: Der Socket konnte nicht erstellt werden, fehler code: "));
    DEBUG_PRINT(WSAGetLastError());
    DEBUG_END();
    return ;
  } else {
    DEBUG_PRINT(LOG_INFO, F("UDP Socket erstellt!"));
  }
    
  isUP = true;
  DEBUG_PRINT(LOG_INFO, ipstr);
//

}


void virt_networkClass::loop() {
  int packetSize = 0; 
  long rc = recv(socket_in, packetBuffer, maxPacketSize, 0);
  if(rc == SOCKET_ERROR) {
      int le = WSAGetLastError();
      if (le != WSAEWOULDBLOCK) {
        DEBUG_BEGIN(LOG_ERR);
        DEBUG_PRINT(F("UDP recv errorcode: "));
        DEBUG_PRINT(le);
        DEBUG_END();
        return;
      }  
  } else {
    packetSize = rc; 
  }  
  if (packetSize) { 
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
    char tmp[17];
    sprintf(tmp, "%d", i);
    print(tmp);
  }
}

void virt_networkClass::print(const char *c, int charlen){
  if (isUP) {
    if (charlen == 0) {
      charlen = strlen(c);
//    udp.print(c);
    } else {
//    udp.write(c, charlen);
    }
    memcpy(&buf[buflen], c, charlen);
    buflen = buflen + charlen;
  }
}
void virt_networkClass::endPacket(){
  if (isUP) {
//  udp.endPacket();
    SOCKADDR_IN addr_out;
    char ipstr[17];
  
    ZeroMemory((char*) &addr_out, sizeof(SOCKADDR_IN));
    addr_out.sin_family = AF_INET;
    addr_out.sin_port = htons(DEBUG_PORT);
    sprintf(ipstr, "%d.%d.%d.%d", myip[0], myip[1], myip[2], DEBUG_IP);

    addr_out.sin_addr.s_addr = inet_addr(ipstr);
  
    buf[buflen] = 0;
    long rc = sendto(socket_out, buf, buflen + 1, 0, (SOCKADDR*)&addr_out, sizeof(SOCKADDR_IN));
    buflen = 0;
    if(rc == SOCKET_ERROR) {
      DEBUG_BEGIN(LOG_ERR);
      DEBUG_PRINT(F("UDP send errorcode: "));
      DEBUG_PRINT(WSAGetLastError());
      DEBUG_END();
      return;
    }
  }
}


virt_networkClass virt_network;


//Prototypen
int test()
{

  long rc;
  
  char buf2[300];
  SOCKADDR_IN remoteAddr;
  int remoteAddrLen=sizeof(SOCKADDR_IN);

}

int startWinsock(void)
{
  WSADATA wsa;
  return WSAStartup(MAKEWORD(2,0),&wsa);
}

#endif

