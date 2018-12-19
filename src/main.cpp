#include "dmx.h"
#include "helper.h"
#include "log.h"
#include "network.h"
#include "dmxqueue.h"
#include "worker.h"
#include "simulator.h"
#include "otaupdate.h"

uint32_t extractNumber(const char *data, int *start) {
  uint32_t result = 0;
  while ((*start < maxPacketSize) && (data[*start] >= '0') && (data[*start] <= '9')) {
    result = result * 10;
    result = result + uint8_t(data[*start] - '0');
    (*start)++;
  } 
  (*start)++;
  return result;
}


void networkData(char *data, int datalen){
   int p1 = 0;
   uint_dmxChannel startChannel = 0;
   uint32_t newValue = 0;
   uint8_t updSp = 0;
   uint8_t onoffSpeed = 0;
   uint8_t gamma = 0;
   bool isOnOff = false;
   DEBUG_PRINT(LOG_INFO, F("Network DATA2"));

   if ((datalen > 3) && 
       (data[0] == 'D')  &&
       (data[0 + 1] == 'M')  &&
       (data[0 + 2] == 'X')  &&
       ((data[0 + 3] == 'C') || 
      (data[0 + 3] == 'R') || 
      (data[0 + 3] == 'P') || 
      (data[0 + 3] == 'V') || 
      (data[0 + 3] == 'W') ||
      (data[0 + 3] == 'S'))) {
       DEBUG_BEGIN(LOG_INFO);
       DEBUG_PRINT(F("UDP DATA OK2 Size: "));
       DEBUG_PRINT(datalen);
       DEBUG_END();
 
     p1 = p1 + 4;
     startChannel = extractNumber(data, &p1);
     newValue = extractNumber(data, &p1);
     updSp = extractNumber(data, &p1);
     if (updSp == 0) {
       updSp = 2;  
     }
     gamma = extractNumber(data, &p1);
     onoffSpeed = extractNumber(data, &p1);
     if (onoffSpeed == 0) {
      onoffSpeed = updSp;
     }
     
     /*DEBUG_PRINT(LOG_INFO, F("Channel: "));
     DEBUG_PRINTln(LOG_INFO, startChannel);
     DEBUG_PRINT(LOG_INFO, F("newValue: "));
     DEBUG_PRINT(LOG_INFO, newValue);
     DEBUG_PRINT(LOG_INFO, F(" = "));
     DEBUG_PRINTln(LOG_INFO, F("?"));
    // DEBUG_PRINTln(LOG_INFO, c255(newValue, gamma));        
     DEBUG_PRINT(LOG_INFO, F("Speed: "));
     DEBUG_PRINTln(LOG_INFO, updSp);
     DEBUG_PRINT(LOG_INFO, F("Gamma: "));
     DEBUG_PRINTln(LOG_INFO, gamma);
     DEBUG_PRINT(LOG_INFO, F("onoff Speed: "));
     DEBUG_PRINTln(LOG_INFO, onoffSpeed);
     DEBUG_PRINT(LOG_INFO, F("old Value: "));
     DEBUG_PRINTln(LOG_INFO, dmxHW.read(startChannel));
     DEBUG_PRINT(LOG_INFO, "Typ: ");
     DEBUG_PRINT(LOG_INFO, packetBuffer[0 + 3]);
     DEBUG_PRINTln(LOG_INFO, "");*/
     if  (data[0 + 3] == 'R') {
       isOnOff = queue.add(startChannel, updSp, newValue % 1000, gamma, true);
       newValue = newValue / 1000;
       isOnOff = queue.add(startChannel + 1, updSp, newValue % 1000, gamma, true) | isOnOff;
       newValue = newValue / 1000;
       isOnOff = queue.add(startChannel + 2, updSp, newValue % 1000, gamma, true) | isOnOff;
       queue.update(startChannel, onoffSpeed, isOnOff);
       queue.update(startChannel + 1, onoffSpeed, isOnOff);
       queue.update(startChannel + 2, onoffSpeed, isOnOff);
     } else if  (data[0 + 3] == 'P') {
       isOnOff = queue.add(startChannel, updSp, newValue, gamma, true);
       queue.update(startChannel, onoffSpeed, isOnOff);
     } else if  (data[0 + 3] == 'W') {
       isOnOff = queue.add(startChannel, updSp, newValue, 3, true);
       isOnOff = queue.add(startChannel + 1, updSp, newValue, 2, true) | isOnOff;
       queue.update(startChannel, onoffSpeed, isOnOff);
       queue.update(startChannel + 1, onoffSpeed, isOnOff);
     } else if  (data[0 + 3] == 'V') {
       isOnOff = queue.add(startChannel, updSp, newValue, 2, true);
       isOnOff = queue.add(startChannel + 1, updSp, newValue, 3, true) | isOnOff;       
       queue.update(startChannel, onoffSpeed, isOnOff);
       queue.update(startChannel + 1, onoffSpeed, isOnOff);
     } else if  (data[0 + 3] == 'S') {
       for (int ch = 0; ch < QUEUESIZE; ch++) {
         queue.add(startChannel + ch, updSp, newValue, gamma, true); 
       };
//     stress = true;
     } else {
       isOnOff = queue.add(startChannel, updSp, newValue, 0, false);
       queue.update(startChannel, onoffSpeed, isOnOff);
     }
  }
     
  for(int i=0; i<maxPacketSize; i++){
     data[i] = '\0';
  }
  
}

void QueueStart(uint_dmxChannel channel) {
  simulator.sendAction(channel, "A");
}

void QueueStop(uint_dmxChannel channel) {
  simulator.sendAction(channel, "Z");
}

void setup() {
  DEBUG_INIT();
  
  // put your setup code here, to run once:
  virt_network.Register_OnNetworkData(&networkData);
  virt_network.init();
  otaUpdate.init();
  queue.Register_OnStart(&QueueStart);
  queue.Register_OnStop(&QueueStop);
  queue.init();
  virt_dmx.init(16);
  worker.init();
  simulator.init();

}

void loop() {
  virt_network.loop();
  otaUpdate.loop();
  virt_dmx.loop();
  worker.loop();
  simulator.loop();

}

int main(int argc, char const *argv[])
{
  setup();
  while (true) {
    loop();
  }
  return 0;
}
