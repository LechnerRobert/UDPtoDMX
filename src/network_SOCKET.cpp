#if (USE_SOCKET==1)

#define _WIN32_WINNT 0x0501

#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>


#include "network.h"
#include "log.h"

//EthernetUDP udp;

int startWinsock(void);
  SOCKET s;

void virt_networkClass::init() {
    uint8_t mymac[6] = {  0x54, 0x55, 0x58, 0x10, 0x00, 0xF5 };
    uint8_t mygw[4] = { 192, 168, 123, 254 };// ip of gateway
    uint8_t mymask[4] = { 255, 255, 255, 0 };// subnet mask

  long rc;
  SOCKADDR_IN addr;
  
  DEBUG_PRINT(LOG_INFO, F("Init Network"));
  rc=startWinsock();
  if(rc!=0) 
  {
    printf("Fehler: startWinsock, fehler code: %d\n",rc);
    return;
  }
  else
  {
    printf("Winsock gestartet!\n");
  }
  // UDP Socket erstellen
  s=socket(AF_INET,SOCK_DGRAM,0);
  if(s==INVALID_SOCKET)
  {
    printf("Fehler: Der Socket konnte nicht erstellt werden, fehler code: %d\n",WSAGetLastError());
    return ;
  }
  else
  {
    printf("UDP Socket erstellt!\n");
  }
  
// addr vorbereiten
  addr.sin_family=AF_INET;
  addr.sin_port=htons(1234);
  addr.sin_addr.s_addr=inet_addr("127.0.0.1");

  rc=connect(s,(SOCKADDR*)&addr,sizeof(SOCKADDR_IN));
  
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


//Prototypen
int test()
{

  long rc;
  
  char buf[256];
  char buf2[300];
  SOCKADDR_IN remoteAddr;
  int remoteAddrLen=sizeof(SOCKADDR_IN);



  while(1)
  {
    printf("Text eingeben: ");
    gets(buf);
    //rc=sendto (s,buf,strlen(buf),0,(SOCKADDR*)&addr,sizeof(SOCKADDR_IN));
    rc=send(s,buf,strlen(buf),0);
    if(rc==SOCKET_ERROR)
    {
      //printf("Fehler: sendto, fehler code: %d\n",WSAGetLastError());
      printf("Fehler: send, fehler code: %d\n",WSAGetLastError());
      return 1;
    }
    else
    {
      printf("%d Bytes gesendet!\n", rc);
    }    //rc=recvfrom(s,buf,256,0,(SOCKADDR*)&remoteAddr,&remoteAddrLen);

    rc=recv(s,buf,256,0);

    if(rc==SOCKET_ERROR)
    {
      //printf("Fehler: recvfrom, fehler code: %d\n",WSAGetLastError());
      printf("Fehler: recv, fehler code: %d\n",WSAGetLastError());
      return 1;
    }
    else
    {
      printf("%d Bytes empfangen!\n", rc);
      buf[rc]='\0';
      printf("Empfangene Daten: %s\n",buf);
    }
  }
  return 0;

}

int startWinsock(void)
{
  WSADATA wsa;
  return WSAStartup(MAKEWORD(2,0),&wsa);
}

#endif

