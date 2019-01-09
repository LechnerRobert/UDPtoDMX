#ifndef DMXQUEUE_H
#define DMXQUEUE_H

#include "types.h"

typedef
struct dimItem {
   uint_times    dimtime;
   uint_dmxValue dimTo;
   uint8_t       dimSpeed;
   uint8_t       channelLO;
   uint8_t       channelHI:1;
   uint8_t       gamma:2;
   uint8_t       isPercent:1;
   uint8_t       aktiv:1;
   uint8_t       wait:2;
   uint8_t       waitUp:1;
} dimItem;

typedef void (*event_queueState)(uint_dmxChannel channel);


class dmxqueueClass
{
  private:
     event_queueState onStart;
     event_queueState onStop;
  public:
    void Register_OnStart(event_queueState callback);
    void Register_OnStop(event_queueState callback);

    void init();
    uint8_t add(uint_dmxChannel channel, uint8_t dimSpeed, uint_dmxValue dimTo, uint8_t gamma, bool isPercent);
    void update(uint_dmxChannel channel, uint8_t dimSpeed, bool doUpdate);
    dimItem* GetItem(int channel);
    void stop(uint_dmxChannel channel);
};

extern dmxqueueClass queue;
#endif
