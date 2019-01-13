#ifndef LED_COLOR_SERIAL_HPP
#define LED_COLOR_SERIAL_HPP

#include "LED_Shared.hpp"
#include "LED_Color.hpp"
#include <stdint.h>

#ifdef DEBUG
#include "ArduinoSIM.h"
#else
#include <Arduino.h>
#endif

class LED_Color_Serial0 : public LED_Color_Mode {
public:
  LED_Color_Serial0(LED_Color *l) : LED_Color_Mode(l) {}
  void loop() { return; }
  void hello() { return; }
};

class LED_Color_Serial1 : public LED_Color_Mode {
public:
  LED_Color_Serial1(LED_Color *l) : LED_Color_Mode(l) {}
  void loop() { ledSmoothWrite(); }
  void hello() { return; }
};


#endif
