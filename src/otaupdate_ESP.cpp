#if USE_OTAUPDATE==1

#include "otaupdate.h"
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>

ESP8266WebServer httpServer(81);
ESP8266HTTPUpdateServer httpUpdater;



void otaUpdateClass::loop() { 
  httpServer.handleClient();
}

void otaUpdateClass::init() { 
  httpUpdater.setup(&httpServer);
  httpServer.begin();  
}

otaUpdateClass otaUpdate;

#endif