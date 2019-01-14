#ifndef LED_COLOR_R_HPP
#define LED_COLOR_R_HPP

#include "LED_Shared.hpp"
#include "LED_Color.hpp"
#include <stdint.h>

#ifdef DEBUG
#include "ArduinoSIM.h"
#else
#include <Arduino.h>
#endif

class LED_Color_R : public LED_Color_Mode {
private:
  uint8_t *h, *m, *s;
public:
  LED_Color_R(LED_Color *l, uint8_t *h, uint8_t *m, uint8_t *s); // Pass current time ref as argument
  void loop();
  void hello();
};

#endif
