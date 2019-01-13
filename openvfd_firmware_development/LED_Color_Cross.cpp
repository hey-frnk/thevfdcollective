#include "LED_Color.hpp"
#include "LED_Color_Cross.hpp"
#include "LED_Shared.hpp"
#include "Arduino.h"

LED_Color_Cross::LED_Color_Cross(LED_Color *l, uint8_t delta) :
LED_Color_Mode(l), angle(0), lightness(LED_L_HIGH), delta(delta) {
  angleUpdater = newiE(60);
}

void LED_Color_Cross::loop() {
  if(updateIntervalEvent(&angleUpdater)) angle++; // Just let it overflow and begin from 0 :p
  uint8_t offset = 0;

  // Cycle position
  for(uint8_t i = 0; i < NUM_RGB; i++) {
    uint32_t phase = ledPhase(angle + (i * delta), 255, lightness);
    rgb_arr[offset] = (uint8_t)((phase >> 16) & 0xFF);     // G
    rgb_arr[offset + 1] = (uint8_t)((phase >> 8) & 0xFF);  // R
    rgb_arr[offset + 2] = (uint8_t)(phase & 0xFF);         // B
    offset += 3;
  }
  l->render();
}

void LED_Color_Cross::F3() {
  // Higher delta: wider rainbow
  if(delta == 42) {
    delta = 10;
    displayWrite(3, 0x00, 1000, LED7PM[0]);
  }
  else if(delta == 10) {
    delta = 21;
    displayWrite(3, 0x00, 1000, LED7PM[1]);
  }
  else if(delta == 21) {
    delta = 42;
    displayWrite(3, 0x00, 1000, LED7PM[2]);
  }
  else{
    delta = 10;
    displayWrite(3, 0x00, 1000, LED7PM[0]);
  }
}

void LED_Color_Cross::F3var() {
  char k[NUM_DIGITS_V];
  if(lightness == LED_L_REG) {
    lightness = LED_L_HIGH; // Change to bright
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L0 + i);
    k[0] = 'L';
    displayWrite(3, 0x00, 1000, k);
  }
  else if(lightness == LED_L_HIGH) {
    lightness = LED_L_LOW; // Change to dim
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L2 + i);
    k[0] = 'L';
    displayWrite(3, 0x00, 1000, k);
  }
  else{
    lightness = LED_L_REG; // Back to normal
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L1 + i);
    k[0] = 'L';
    displayWrite(3, 0x00, 1000, k);
  }
}

void LED_Color_Cross::hello() {
  char k[NUM_DIGITS_V];
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_CROSSFADE + i);
  displayWrite(3, 0x00, 1000, k);
}
