#ifndef LED_COLOR_CROSS_HPP
#define LED_COLOR_CROSS_HPP

#include "LED_Shared.hpp"
#include "LED_Color.hpp"
#include "Arduino.h"
#include <stdint.h>

class LED_Color_Cross : public LED_Color_Mode {
private:
  uint8_t angle;
  uint8_t delta;
  LED_L_t lightness;
  intervalEvent angleUpdater;
public:
  LED_Color_Cross(LED_Color *l);
  void F3();
  void F3var();
  void loop();
};

#endif
