#include "dmx.h"
#include "helper.h"
#include "log.h"
#include "network.h"
#include "dmxqueue.h"
#include "worker.h"
#include "simulator.h"
#include "otaupdate.h"
#include "stresstest.h"

uint32_t extractNumber(const char *data, int *start, bool *eof) {
  uint32_t result = 0;
  *eof = true;
  while ((*start < maxPacketSize) && (data[*start] >= '0') && (data[*start] <= '9')) {
    *eof = false;
    result = result * 10;
    result = result + uint8_t(data[*start] - '0');
    (*start)++;
  } 
  (*start)++;
  return result;
}

#define proz10 20

uint8_t plus10(uint16_t value) {
  if (value >= proz10) {
    return (value - proz10) * 100 / (100 - proz10);
  } else {
    return 0;
  }
}

uint8_t minus10(uint16_t value) {
  return min(100, value * 100 / (100 - proz10));
}

void networkData(char *data, int datalen){
   int p1 = 0;
   uint_dmxChannel startChannel = 0;
   uint32_t newValue = 0;
   uint8_t updSp = 0;
   uint8_t onoffSpeed = 0;
   uint8_t gamma = 0;
   bool isOnOff = false;

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
     bool eof = false;
     startChannel = extractNumber(data, &p1, &eof);
     newValue = extractNumber(data, &p1, &eof);
     updSp = extractNumber(data, &p1, &eof);
     if (updSp == 0) {
       updSp = 2;  
     }
     gamma = extractNumber(data, &p1, &eof);
     if (eof) {
       gamma = 2;
     };
     
     onoffSpeed = extractNumber(data, &p1, &eof);
     if (onoffSpeed == 0) {
      onoffSpeed = updSp;
     }
     
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
       isOnOff = queue.add(startChannel, updSp, plus10(newValue), 3, true);
       isOnOff = queue.add(startChannel + 1, updSp, minus10(newValue), 3 /*or 2*/, true) | isOnOff;
       queue.update(startChannel, onoffSpeed, isOnOff);
       queue.update(startChannel + 1, onoffSpeed, isOnOff);
     } else if  (data[0 + 3] == 'V') {
       isOnOff = queue.add(startChannel, updSp, minus10(newValue), 3 /*or 2*/, true);
       isOnOff = queue.add(startChannel + 1, updSp, plus10(newValue), 3, true) | isOnOff;       
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
  #if DO_STRESSTEST==1
  testQueue();
  #endif
}

void loop() {
  virt_network.loop();
  otaUpdate.loop();
  virt_dmx.loop();
  worker.loop();
  simulator.loop();
  
}

#ifndef ARDUINO
int main(int argc, char const *argv[])
{
  StartCounter();
  setup();
  while (true) {
    loop();
  }
  return 0;
}
#endif