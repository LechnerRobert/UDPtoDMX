#if (defined(ESP8266) || defined(__AVR__))
#include <WString.h>
#else
  #define __FlashStringHelper char
  #define F(string_literal) (string_literal)
#endif
