#if USE_DMXSerial == 1
#include "dmx.h"
#include "DMXSerial.h"
#include "log.h"

uint_dmxValue virt_dmxClass::read(uint_dmxChannel channel) {
  return DMXSerial.read(channel);
};

void virt_dmxClass::write(uint_dmxChannel channel, uint_dmxValue value){
  DMXSerial.write(channel, value);
};

void  virt_dmxClass::init(uint_dmxChannel maxChannels) {
  DEBUG_PRINT(LOG_INFO, F("Init DMXSerial"));
  DMXSerial.init(DMXController);
//DMXSerial.maxChannel(maxChannels);
};

void  virt_dmxClass::loop() {
}

virt_dmxClass virt_dmx;

#endif