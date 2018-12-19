#if USE_H801 == 1
#include "dmx.h"
#include <Arduino.h>
#include "log.h"
#include "helper.h"
#include "pwm.h"

// RGB FET
#define redPIN    15 //12
#define greenPIN  13 //15
#define bluePIN   12 //13


// W FET
#define w1PIN     14
#define w2PIN     4

const uint32_t period = maxValue;  //5000 * 200ns ^= 1 kHz


// PWM setup
uint32_t io_info[DMX_SIZE][3] = {
  // MUX, FUNC, PIN
  {PERIPHS_IO_MUX_MTCK_U,  FUNC_GPIO12, bluePIN},
  {PERIPHS_IO_MUX_MTDO_U,  FUNC_GPIO13, greenPIN},
  {PERIPHS_IO_MUX_MTDI_U,  FUNC_GPIO15, redPIN}, 
  {PERIPHS_IO_MUX_MTMS_U,  FUNC_GPIO14, w1PIN},
  {PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4 , w2PIN},
};

// initial duty: all off
uint32 pwm_duty_init[DMX_SIZE] = {0, 0, 0, 0, 0};

uint_dmxValue dmxV[DMX_SIZE];
uint32_t lastUpdate;

uint_dmxValue virt_dmxClass::read(uint_dmxChannel channel) {
  if ((channel > 0) && (channel <= DMX_SIZE)) {
    return dmxV[channel-1];
  } else {
    return 0;
  }
};

void virt_dmxClass::write(uint_dmxChannel channel, uint_dmxValue value){
  if ((channel > 0) && (channel <= DMX_SIZE)) {
    dmxV[channel-1] = value;
  }
};

void  virt_dmxClass::init(uint_dmxChannel maxChannels) {
  DEBUG_PRINT(LOG_INFO, F("Init H801 noDMX"));
  lastUpdate = millis();
  pinMode(redPIN, OUTPUT);
  pinMode(greenPIN, OUTPUT);
  pinMode(bluePIN, OUTPUT);
  pinMode(w1PIN, OUTPUT);
  pinMode(w2PIN, OUTPUT);    

  pwm_init(period, pwm_duty_init, DMX_SIZE, io_info);
  pwm_start();
};

void  virt_dmxClass::loop() {
  if (timeDiff(lastUpdate, millis()) > 20) {
    lastUpdate = millis();

    pwm_set_duty(dmxV[2], 0);  //BLUE
    pwm_set_duty(dmxV[1], 1);  //GREEN
    pwm_set_duty(dmxV[0], 2);  //RED
    pwm_set_duty(dmxV[3], 3);  
    pwm_set_duty(dmxV[4], 4); 
    pwm_start();           // commit
  }
}

virt_dmxClass virt_dmx;

#endif