#ifndef LED_COLOR_STATIC_HPP
#define LED_COLOR_STATIC_HPP

#include "LED_Color.hpp"
#include <stdint.h>

class LED_Color_Static : public LED_Color_Mode {
private:
  uint8_t position;
public:
  LED_Color_Static(LED_Color *l);
  void F3();
  void loop();
};

#endif
