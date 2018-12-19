#include "worker.h"

void workerClass::init() {
  DEBUG_PRINT(LOG_INFO, F("Init worker"));
  lastTime = micros();
}


void workerClass::loop() { 
  uint32_t dt;
  uint32_t ct;
  
  ct = micros();
  dt = timeDiff(lastTime, ct);
  if (dt >= RefreshTime) {
    if (dt<(RefreshTime * 2)) {
      lastTime = lastTime + RefreshTime;
    } else {
      lastTime = ct;
      DEBUG_PRINT(LOG_ALERT, F("out of time"));
    }
    step();  
  }
}

void workerClass::step() { 
  int ch;
  uint_dmxValue oldValue, newValue;
  uint8_t chInc, chStep;
  uint_dmxValue dimTo;
  uint_times divTime;
  dimItem *queueItem;
 /* uint32_t stTime = micros();*/
  for (int i = 0; i < QUEUESIZE; i++) {
    queueItem = queue.GetItem(i);
    if (queueItem->aktiv) {
      dimTo = queueItem->dimTo;
      if (queueItem->isPercent) {
        dimTo = c255(dimTo, queueItem->gamma);
      }
      ch = ((queueItem->channelHI << 8) | queueItem->channelLO) + 1;
      if (queueItem->dimSpeed == 255) {
        newValue = dimTo;
      } else {
        oldValue = virt_dmx.read(ch);
        newValue = oldValue;
        chStep = queueItem->dimSpeed % 100;
        if (chStep == 0) {
          chStep = 1;  
        }
        if (queueItem->dimSpeed >= 200) {
          chInc = 8;
        } else if (queueItem->dimSpeed >= 100) {
          chInc = 4;
        }  else {
          chInc = 1;
        }  
        
        divTime = RefreshBaseTime * chInc / chStep;
        
        if (queueItem->dimtime == 0xFFFF) {
          newValue = newValue - 1; 
          queueItem->dimtime = 0;
        }
        
        queueItem->dimtime = queueItem->dimtime + divTime;
        if (dimTo < 10) {
        /*DEBUG_PRINTln("T");
        DEBUG_PRINTln(millis());
        DEBUG_PRINTln(queueItem->dimtime);
        DEBUG_PRINTln(_dimStepTime(newValue, queueItem->gamma));
        DEBUG_PRINTln("");*/
        }
        
        if (dimTo > oldValue) {
          while (queueItem->dimtime >= _dimStepTime(newValue, queueItem->gamma) ) {
            queueItem->dimtime = queueItem->dimtime - _dimStepTime(newValue, queueItem->gamma);
            newValue = newValue + 1;
            if (newValue == dimTo) {
              break;
            }
          }
        } else if (dimTo < oldValue) {
          while (queueItem->dimtime >= _dimStepTime(newValue, queueItem->gamma)) {
            queueItem->dimtime = queueItem->dimtime - _dimStepTime(newValue, queueItem->gamma);
            newValue = newValue - 1;
            if (newValue == dimTo) {
              break;
            }
          }
        
        } else if (queueItem->wait > 0) {
        
          uint16_t waitTime;
          if (queueItem->waitUp) {
            waitTime = _dimStepTime(newValue, queueItem->gamma) * 2;
          } else {
            waitTime = _dimStepTime(newValue - 1, queueItem->gamma) * 2;
          }
          /*DEBUG_PRINTln("*");   
          DEBUG_PRINTln(waitTime);   
          DEBUG_PRINTln(queueItem->dimtime );   
          DEBUG_PRINTln("*");   
          */
          if (queueItem->dimtime > waitTime) {
            queueItem->wait = queueItem->wait - 2;
          }
        }  
      }
      
      virt_dmx.write(ch, newValue);
      if ((newValue == dimTo) && (queueItem->wait == 0)) {
        queueItem->aktiv = false;     
        queueItem->dimtime = 0;
        queue.stop(ch);
     //   DEBUG_PRINTln("INAKTIV");   
      }
    }  
  }
/*  if (stress) {
  DEBUG_PRINTln("T ");
  DEBUG_PRINTln(micros() - stTime);
  }*/
}

workerClass worker;