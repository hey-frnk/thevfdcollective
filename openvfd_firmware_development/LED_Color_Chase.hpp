#ifndef LED_COLOR_CHASE_HPP
#define LED_COLOR_CHASE_HPP

#include "LED_Shared.hpp"
#include "LED_Color.hpp"
#include <stdint.h>

#ifdef DEBUG
#include "ArduinoSIM.h"
#else
#include <Arduino.h>
#endif

class LED_Color_Chase : public LED_Color_Mode {
private:
  uint8_t angle,          // LED chase fade position (LED8_ph)
          secondFlip,     // LED chase fade second flip (LED8_ds)
          directionFlag,  // LED chase fade direction flag (LED8_dr)
          state,          // LED chase fade FSM position (LED8_st)
          MicPin,
          flipdir,        // Based on interface 0 dot flip
          *s;             // Second

  LED_L_t lightness;

  intervalEvent stateUpdater, delayUpdater;
public:
  uint8_t direction;      // LED chase fade direction state (LED8_dp, shared, saved)

  LED_Color_Chase(LED_Color *l, uint8_t MicPin, uint8_t *s, uint8_t flipdir, uint8_t direction);
  void F3();
  void F3var();
  void loop();
  void hello();
};

#endif
