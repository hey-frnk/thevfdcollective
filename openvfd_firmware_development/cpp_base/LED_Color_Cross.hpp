#ifndef LED_COLOR_CROSS_HPP
#define LED_COLOR_CROSS_HPP

#include "LED_Shared.hpp"
#include "LED_Color.hpp"
#include <stdint.h>

#ifdef DEBUG
#include "ArduinoSIM.h"
#else
#include <Arduino.h>
#endif


class LED_Color_Cross : public LED_Color_Mode {
private:
  uint8_t angle;
  LED_L_t lightness;
  intervalEvent angleUpdater;
public:
  uint8_t delta; // (LED7_delta, shared, saved)
  LED_Color_Cross(LED_Color *l, uint8_t delta);
  void F3();
  void F3var();
  void loop();
  void hello();
};

#endif
