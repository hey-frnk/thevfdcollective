#ifndef LED_COLOR_MUSIC_HPP
#define LED_COLOR_MUSIC_HPP

#include "LED_Shared.hpp"
#include "LED_Color.hpp"
#include <stdint.h>

#ifdef DEBUG
#include "ArduinoSIM.h"
#else
#include <Arduino.h>
#endif

class LED_Color_Music : public LED_Color_Mode {
private:
  uint8_t angle,        // LED cross fade position
          delta,        // LED cross fade delta
          state,        // LED microphone mode off fader state
          delayState,   // LED microphone mode blink delay state
          MicPin;       // Microphone pin

  LED_L_t lightness;    // LED cross fade lightness

  intervalEvent angleUpdater, stateUpdater, delayUpdater;
public:
  LED_Color_Music(LED_Color *l, uint8_t MicPin);
  void F3();      // Change of delta
  void F3var();   // Change of lightness
  void loop();
  void hello();
};

#endif
