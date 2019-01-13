#ifndef LED_COLOR_STATIC_HPP
#define LED_COLOR_STATIC_HPP

#include "LED_Color.hpp"
#include <stdint.h>

class LED_Color_Static : public LED_Color_Mode {
public:
  uint8_t position;
  LED_Color_Static(LED_Color *l, uint8_t position);
  void F3();
  void loop();
  void hello();
};

#endif
