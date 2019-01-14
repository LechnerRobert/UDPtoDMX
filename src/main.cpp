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

#define onOffthreshold 10

uint8_t scaleSpeed(uint8_t sp, uint8_t dx, uint8_t dmax) {
  if ((dx == 0) || (sp == 255) || (dx == dmax)) {
    return sp;
  } else {
    uint8_t f = 1;
    if (sp < 100) {
      f = 8;
    } else if (sp < 200) {
      f = 2;
    }
    uint16_t ret = sp % 100;
    ret = ret * f * dmax / dx;
    if (ret > 54) {
      ret = ret / 2;
      if (ret > 99) {
        return min((ret / 4),99);
      } else {
        return 100 + (int) ret;
      }
    } else {
      return 200 + ret;
    }  
  }
}

void networkData(char *data, int datalen){
   int p1 = 0;
   uint_dmxChannel startChannel = 0;
   uint32_t newValue = 0;
   uint8_t updSp = 0;
   uint8_t onoffSpeed = 0;
   uint8_t gamma = 0;
   uint8_t d1, d2, d3, dmax = 0;
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
       d1 = queue.add(startChannel, updSp, newValue % 1000, gamma, true); //RED
       newValue = newValue / 1000;
       d2 = queue.add(startChannel + 1, updSp, newValue % 1000, gamma, true); //GREEN
       newValue = newValue / 1000;
       d3 = queue.add(startChannel + 2, updSp, newValue % 1000, gamma, true); //BLUE
       isOnOff = (d1 > onOffthreshold) || (d2 > onOffthreshold) || (d3 > onOffthreshold);
       if (!isOnOff) {
         onoffSpeed  = updSp;
         isOnOff = true; 
       }
       dmax = max(d1, d2);
       dmax = max(d3, dmax);
       DEBUG_BEGIN(LOG_INFO);
       DEBUG_PRINT(F("RGB Speed: "));
       DEBUG_PRINT(scaleSpeed(onoffSpeed, d1, dmax));
       DEBUG_PRINT(F(" "));
       DEBUG_PRINT(scaleSpeed(onoffSpeed, d2, dmax));
       DEBUG_PRINT(F(" "));
       DEBUG_PRINT(scaleSpeed(onoffSpeed, d3, dmax));
       DEBUG_END();

       queue.update(startChannel, scaleSpeed(onoffSpeed, d1, dmax), isOnOff);
       queue.update(startChannel + 1, scaleSpeed(onoffSpeed, d2, dmax), isOnOff);
       queue.update(startChannel + 2, scaleSpeed(onoffSpeed, d3, dmax), isOnOff);
     } else if  (data[0 + 3] == 'P') {
       isOnOff = queue.add(startChannel, updSp, newValue, gamma, true) > onOffthreshold;
       queue.update(startChannel, onoffSpeed, isOnOff);
     } else if  (data[0 + 3] == 'W') {
       d1 = queue.add(startChannel, updSp, plus10(newValue), gamma, true);                                  /*CW*/
       d2 = queue.add(startChannel + 1, updSp, minus10(newValue), gamma /*or gamma - 1*/, true);            /*WW*/
       isOnOff = (d1 > onOffthreshold) || (d2 > onOffthreshold);
       queue.update(startChannel, onoffSpeed, isOnOff);
       queue.update(startChannel + 1, onoffSpeed, isOnOff);
     } else if  (data[0 + 3] == 'V') {
       d1 = queue.add(startChannel, updSp, minus10(newValue), gamma /*or gamma - 1*/, true);
       d2 = queue.add(startChannel + 1, updSp, plus10(newValue), gamma, true);       
       isOnOff = (d1 > onOffthreshold) || (d2 > onOffthreshold);
       queue.update(startChannel, onoffSpeed, isOnOff);
       queue.update(startChannel + 1, onoffSpeed, isOnOff);
     } else if  (data[0 + 3] == 'S') {
       for (int ch = 0; ch < QUEUESIZE; ch++) {
         queue.add(startChannel + ch, updSp, newValue, gamma, true); 
       };
//     stress = true;
     } else {
       queue.add(startChannel, updSp, newValue, 0, false);
       /*queue.update(startChannel, onoffSpeed, isOnOff);*/
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