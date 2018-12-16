#if USE_DMXSimple == 1
#include "dmx.h"
#include "DMXSimple.h"
#include "log.h"


uint8_t virt_dmxClass::read(uint_dmxChannel channel) {
  return DmxSimple.read(channel);
};

void virt_dmxClass::write(uint_dmxChannel channel, uint_dmxValue value){
  DmxSimple.write(channel, value);
};

void  virt_dmxClass::init(uint_dmxChannel maxChannels) {
  DEBUG_PRINT(LOG_INFO, F("Init DMXSimple"));
 
  /* The most common pin for DMX output is pin 3, which DmxSimple
  ** uses by default. If you need to change that, do it here. */
  DmxSimple.usePin(3);
  /* DMX devices typically need to receive a complete set of channels
  ** even if you only need to adjust the first channel. You can
  ** easily change the number of channels sent here. If you don't
  ** do this, DmxSimple will set the maximum channel number to the
  ** highest channel you dmx.write() to. */
  DmxSimple.maxChannel(maxChannels);
 };


void  virt_dmxClass::loop() {
}


virt_dmxClass virt_dmx;






#endif
