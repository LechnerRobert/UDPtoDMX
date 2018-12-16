#include "log.h"

void DEBUG_PRINT(uint8_t level, const __FlashStringHelper *ifsh) {
  DEBUG_BEGIN(level);
  DEBUG_PRINT(ifsh);
  DEBUG_END();

}
