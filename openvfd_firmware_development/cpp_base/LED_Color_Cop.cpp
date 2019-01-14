#include "LED_Color.hpp"
#include "LED_Color_Cop.hpp"
#include "LED_Shared.hpp"

#ifdef DEBUG
#include "ArduinoSIM.h"
#else
#include <Arduino.h>
#endif

LED_Color_Cop::LED_Color_Cop(LED_Color *l, uint8_t pattern) : LED_Color_Mode(l), state(0), pattern(pattern)
{
  stateUpdater = newiE(25);
}

void LED_Color_Cop::loop() {
  // cfUpdater has update time of 25 ms (ideal for cop mode)
  if(updateIntervalEvent(&stateUpdater)) {
    if(state < 13) state++;
    else if(state == 13) state = 0;
  }

  if(pattern == 0) {
    if(state == 0) copHalfRender(0, 1);                      // b | r fill

    else if(state == 5) {
      for(uint8_t i = 0; i < NUM_BYTES; i += 3) for(uint8_t j = 0; j < 3; j++) rgb_arr[i + j] = led_scPresets[0][j];
      l->render();                                                 // off fill
    }

    else if(state == 6) copHalfRender(0, 1);                 // b | r fill
    else if(state == 7) copHalfRender(1, 0);                 // r | b fill

    else if(state == 12) {
      for(uint8_t i = 0; i < NUM_BYTES; i += 3) for(uint8_t j = 0; j < 3; j++) rgb_arr[i + j] = led_scPresets[1][j];
      l->render();                                                 // white fill
    }

    else if(state == 13) copHalfRender(1, 0);                // r | b fill
  }
}

// Fill LED array left half with one and the right half with other color
inline void LED_Color_Cop::copHalfRender(uint8_t right, uint8_t left) {
  for(uint8_t i = 0; i < (NUM_BYTES >> 1); i+= 3) for(uint8_t j = 0; j < 3; j++) rgb_arr[i + j] = LED11_colors[right][j];
  for(uint8_t i = (NUM_BYTES >> 1); i < NUM_BYTES; i+= 3) for(uint8_t j = 0; j < 3; j++) rgb_arr[i + j] = LED11_colors[left][j];
  l->render();
}

void LED_Color_Cop::hello() {
  char k[NUM_DIGITS_V];
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_CCOP + i);
  displayWrite(3, 0x00, 1000, k);
}
