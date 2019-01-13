#ifndef LED_COLOR_COP_HPP
#define LED_COLOR_COP_HPP

#include "LED_Shared.hpp"
#include "LED_Color.hpp"
#include <stdint.h>

#ifdef DEBUG
#include "ArduinoSIM.h"
#else
#include <Arduino.h>
#endif

class LED_Color_Cop : public LED_Color_Mode {
private:
  uint8_t state;
  intervalEvent stateUpdater;

  void copHalfRender(uint8_t right, uint8_t left);
public:
  uint8_t pattern;      // LED cop mode pattern (LED11_pt, shared, saved)
  LED_Color_Cop(LED_Color *l, uint8_t pattern);
  void loop();
  void hello();
};

#endif
