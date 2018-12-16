#ifndef DMX_H
#define DMX_H

#include <stdint.h>
#include "types.h"

class virt_dmxClass
{
  public:
    void init(uint_dmxChannel maxChannels);
    uint8_t read(uint_dmxChannel channel);
    void write(uint_dmxChannel channel, uint_dmxValue value);
    void loop();
};

extern virt_dmxClass virt_dmx;

#endif