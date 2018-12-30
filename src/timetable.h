#ifndef TIMETABLE_H
#define TIMETABLE_H


#include <stdint.h>
#include "types.h"
#include "helper.h"

uint_times _dimStepTime(uint_dmxChannel value, uint8_t gamma, bool fake1);
uint_dmxValue _prozToDim(uint8_t value, uint8_t gamma);

#endif