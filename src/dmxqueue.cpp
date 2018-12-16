#include "dmxqueue.h"
#include "log.h"
#include "helper.h"

dimItem intQueue[QUEUESIZE];

void dmxqueueClass::init() {
  DEBUG_PRINT(LOG_INFO, F("Init Queue"));
  for (int ch = 0; ch < QUEUESIZE; ch++) {
    intQueue[ch].aktiv = false;
    intQueue[ch].dimtime = 0;
    intQueue[ch].channelLO = 0;
    intQueue[ch].dimTo = 0;
    intQueue[ch].dimSpeed = 0;
    intQueue[ch].channelHI = 0;
    intQueue[ch].gamma = 0;
    intQueue[ch].isPercent = false;
    intQueue[ch].wait = 0;
    intQueue[ch].waitUp = false;
  }
  add(1, 0, 0, 0, true); //channel 1 always on pos 0
  intQueue[0].aktiv = false; 
}

void dmxqueueClass::update(uint_dmxChannel channel, uint8_t dimSpeed, bool doUpdate) { 
  uint16_t ch;
  if (doUpdate) {
    for (int i = 0; i < QUEUESIZE; i++) {
      if (intQueue[i].aktiv) {
        ch = ((intQueue[i].channelHI << 8) | intQueue[i].channelLO) + 1;
        if (ch == channel) {
          intQueue[i].dimSpeed = dimSpeed;
          break;  
        }        
      }  
    }
  }
} 

bool dmxqueueClass::add(uint_dmxChannel channel, uint8_t dimSpeed, uint_dmxValue dimTo, uint8_t gamma, bool isPercent) { 
  int q = -1;
  uint16_t ch;
  int first = -1;  

  for (int i = 0; i < QUEUESIZE; i++) {
    if ((i > 0) && (first == -1) && (!intQueue[i].aktiv)) {
      first = i;
    }
    ch = ((intQueue[i].channelHI << 8) | intQueue[i].channelLO) + 1;
    if ((ch == channel) || ((i > 0) && (ch == 1))) {
      q = i;
      /*_dmxWrite(ch, c255(dimTo, intQueue[i].gamma));*/ /* JUMP to last destination */ 

      DEBUG_BEGIN(LOG_INFO);
      DEBUG_PRINT(F("reuse: "));
      DEBUG_PRINT(q);
      DEBUG_PRINT(F(" channel: "));
      DEBUG_PRINT(channel);
      DEBUG_PRINT(F(" aktiv: "));
      DEBUG_PRINT(intQueue[i].aktiv);
      DEBUG_PRINT(F(" dimFrom: "));
      DEBUG_PRINT(intQueue[i].dimTo);
      DEBUG_PRINT(F(" dimTo: "));
      DEBUG_PRINT(dimTo);
      DEBUG_PRINT(F(" = "));
      DEBUG_PRINT(c255(dimTo, intQueue[i].gamma));
    //DEBUG_PRINT(F(" oldDMX "));
    //DEBUG_PRINT(dmx. read(ch)); 
      DEBUG_END();
      break;
    }
  }
  if (q == -1) {  
    q = first;
    intQueue[q].dimTo = dimTo;
    intQueue[q].dimtime = 0;
  }
     
   
      bool ret = false;
      
      intQueue[q].wait = 0;
      intQueue[q].waitUp = false;
      if (dimTo < intQueue[q].dimTo) {
        ret = ((intQueue[q].dimTo - dimTo) > 10);
        intQueue[q].wait = 2;
        intQueue[q].waitUp = false;
        if (!intQueue[q].aktiv) {
          intQueue[q].dimtime = 0xFFFF;
        }
      } else if (dimTo > intQueue[q].dimTo) {
        ret = ((dimTo - intQueue[q].dimTo) > 10);
        intQueue[q].wait = 2;
        intQueue[q].waitUp = true;
      };
      
      
      intQueue[q].channelLO = channel - 1;
      intQueue[q].channelHI = (((channel - 1) & 0x100) == 0x100);
      intQueue[q].dimSpeed = dimSpeed;
      intQueue[q].dimTo = dimTo;
      intQueue[q].gamma = gamma;
      intQueue[q].isPercent = isPercent;

      if (!intQueue[q].aktiv) {
        intQueue[q].aktiv = true;
        onStart(channel);
      }    
      
        DEBUG_BEGIN(LOG_INFO);
        DEBUG_PRINT(F("queue: "));
        DEBUG_PRINT(first);
        DEBUG_PRINT(F(" at pos: "));
        DEBUG_PRINT(q);
        DEBUG_PRINT(F(" ch: "));
        DEBUG_PRINT(intQueue[q].channelLO + 1) ;
        DEBUG_PRINT(F(" chh: "));
        DEBUG_PRINT(intQueue[q].channelHI);
        DEBUG_PRINT(F(" dimSP: "));
        DEBUG_PRINT(intQueue[q].dimSpeed);
        DEBUG_PRINT(F(" gamma: "));
        DEBUG_PRINT(intQueue[q].gamma);
        DEBUG_PRINT(F(" dimto: "));
        DEBUG_PRINT(intQueue[q].dimTo);
        DEBUG_PRINT(F(" isPercent: "));
        DEBUG_PRINT(intQueue[q].isPercent);
        DEBUG_PRINT(F(" isFast: "));
        DEBUG_PRINT(ret);
        DEBUG_END();

  /* result only works if both (old and new) are "isPercent" */
  
  return ret;
}

dimItem* dmxqueueClass::GetItem(int channel){
  return &intQueue[channel];
}

void dmxqueueClass::Register_OnStart(event_queueState callback) {
  DEBUG_PRINT(LOG_INFO, F("queue onStart Callback registred"));
  onStart = callback;
}

void dmxqueueClass::Register_OnStop(event_queueState callback) {
  DEBUG_PRINT(LOG_INFO, F("queue onStop Callback registred"));
  onStop = callback;
}

void dmxqueueClass::stop(uint_dmxChannel channel){
  onStop(channel);
}


dmxqueueClass queue;
