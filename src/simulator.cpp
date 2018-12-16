#include "simulator.h"

void  simulatorClass::init() {
  DEBUG_PRINT(LOG_INFO, F("Init Simulator"));
  lastUpdate = millis();
};

void  simulatorClass::loop() {
  if (timeDiff(lastUpdate, millis()) > 40) {
    lastUpdate = millis();
    step();
  }
}

void  simulatorClass::step() {
  char buffer[6] = "DMX\x00\x0F"; 
  virt_network.beginPacket();
  virt_network.print(buffer, 5);
  for (int i = 0; i < 16; i++) {
    buffer[0] = virt_dmx.read(i + 1);
    virt_network.print(buffer, 1);
  }
  virt_network.endPacket();
}


void simulatorClass::sendAction(uint_dmxChannel channel, const char* action){
  char buffer[4] = "DML"; 
  virt_network.beginPacket();
  virt_network.print(buffer, 5);
  buffer[0] = channel / 0xFF;
  virt_network.print(buffer, 1);
  buffer[0] = channel % 0xFF;
  virt_network.print(buffer, 1);
  virt_network.print(action);
  virt_network.endPacket();
}


simulatorClass simulator;