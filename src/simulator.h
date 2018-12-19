#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <inttypes.h>
#include <string.h>
#include "log.h"
#include "helper.h"
#include "network.h"
#include "dmx.h"
#include "types.h"

class simulatorClass
{  
  private:
    uint32_t lastUpdate;
    void step();
  public:
    void init();  
    void loop();
    void sendAction(uint_dmxChannel channel, const char* action);
};

extern simulatorClass simulator;



#endif