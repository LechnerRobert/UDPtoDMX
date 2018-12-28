
#if DO_STRESSTEST==1

#include "dmxqueue.h"
#include "types.h"
#include "helper.h"
#include "log.h"
#include "worker.h"


void testQueue(){
  uint32_t start = millis();
  for (int ch = 0; ch < QUEUESIZE; ch++) {
    queue.add(1 + ch, 7, 100, 3, true); 
  };
  uint32_t stop = millis();
  DEBUG_BEGIN(LOG_INFO);
  DEBUG_PRINT(F("init queue: "));
  DEBUG_PRINT(stop - start);
  DEBUG_PRINT(F("ms"));
  DEBUG_END();
  start = millis();
  for (int ch = 0; ch < 100; ch++) {
    worker.step();
  };
  stop = millis();
  DEBUG_BEGIN(LOG_INFO);
  DEBUG_PRINT(F("100 steps: "));
  DEBUG_PRINT(stop - start);
  DEBUG_PRINT(F("ms"));
  DEBUG_END();
  


}

#endif