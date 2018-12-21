#if USE_DMXDUMMY == 1

#include "dmx.h"
#include "log.h"
#include "helper.h"

const uint32_t period = maxValue;  //5000 * 200ns ^= 1 kHz


uint_dmxValue dmxV[DMX_SIZE];
uint32_t lastUpdate;

uint_dmxValue virt_dmxClass::read(uint_dmxChannel channel) {
  if ((channel > 0) && (channel <= DMX_SIZE)) {
    return dmxV[channel-1];
  } else {
    return 0;
  }
};

void virt_dmxClass::write(uint_dmxChannel channel, uint_dmxValue value){
  if ((channel > 0) && (channel <= DMX_SIZE)) {
    dmxV[channel-1] = value;
  }
  DEBUG_BEGIN(LOG_INFO);
  DEBUG_PRINT(value);
  DEBUG_END();
};

void  virt_dmxClass::init(uint_dmxChannel maxChannels) {
  DEBUG_PRINT(LOG_INFO, F("Init DMX DUMMY"));
  lastUpdate = millis();
};

void  virt_dmxClass::loop() {
  if (timeDiff(lastUpdate, millis()) > 20) {
    lastUpdate = millis();
  }
}

virt_dmxClass virt_dmx;

#endif