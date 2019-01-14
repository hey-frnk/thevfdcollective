#ifndef LED_COLOR_SPECTRUM_HPP
#define LED_COLOR_SPECTRUM_HPP

#include "LED_Shared.hpp"
#include "LED_Color.hpp"
#include <stdint.h>

#ifdef DEBUG
#include "ArduinoSIM.h"
#else
#include <Arduino.h>
#endif

class LED_Color_Spectrum : public LED_Color_Mode {
private:
  uint8_t angle;                 // LED regular fade position
  LED_L_t lightness;             // LED regular fade lightness
  LED_S_t saturation;            // LED regular fade saturation
  intervalEvent angleUpdater;
public:
  LED_Color_Spectrum(LED_Color *l);
  void F3();    // Change of lightness
  void F3var(); // Change of saturatuion
  void loop();
  void hello();
};

#endif
