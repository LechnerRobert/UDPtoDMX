#if USE_ESPDMX == 1
#include "dmx.h"
#include <Arduino.h>
#include "ESPDMX.h"
#include "log.h"
#include "helper.h"

DMXESPSerial dmx;
uint32_t lastUpdate;

uint8_t virt_dmxClass::read(uint_dmxChannel channel) {
  return dmx.read(channel);
};

void virt_dmxClass::write(uint_dmxChannel channel, uint_dmxValue value){
  dmx.write(channel, value);
};

void  virt_dmxClass::init(uint_dmxChannel maxChannels) {
  DEBUG_PRINT(LOG_INFO, F("Init ESPDMX"));
  lastUpdate = millis();
  dmx.init(maxChannels);
};

void  virt_dmxClass::loop() {
  if (timeDiff(lastUpdate, millis()) > 40) {
    lastUpdate = millis();
    dmx.update();
  }
}

virt_dmxClass virt_dmx;

#endif