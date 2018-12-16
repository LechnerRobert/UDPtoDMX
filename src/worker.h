#ifndef WORKER_H
#define WORKER_H

#include <stdint.h>
#include "types.h"
#include "dmx.h"
#include "dmxqueue.h"
#include "helper.h"
#include "timetable.h"
#include "log.h"

class workerClass
{  
  private:
    uint32_t lastTime;
    void step();
  public:
    void init();  
    void loop();
};

extern workerClass worker;

#endif