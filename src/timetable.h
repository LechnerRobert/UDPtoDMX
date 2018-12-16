#ifndef TIMETABLE_H
#define TIMETABLE_H

#include <Arduino.h>
#include <stdint.h>
#include "types.h"

#define RefreshTime 10000
#define RefreshBaseTime 1109    /*(1109 * RefreshTime / 10000)*/

uint_times _dimStepTime(uint_dmxChannel value, uint8_t gamma);

#endif