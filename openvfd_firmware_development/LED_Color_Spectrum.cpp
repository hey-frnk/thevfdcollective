#include "LED_Color.hpp"
#include "LED_Color_Spectrum.hpp"
#include "LED_Shared.hpp"

#ifdef DEBUG
#include "ArduinoSIM.h"
#else
#include <Arduino.h>
#endif

LED_Color_Spectrum::LED_Color_Spectrum(LED_Color *l) :
LED_Color_Mode(l), angle(0), lightness(LED_L_HIGH), saturation(LED_S_HIGH) {
  angleUpdater = newiE(60);
}

void LED_Color_Spectrum::loop() {
  if(updateIntervalEvent(&angleUpdater)) angle++;

  uint32_t phase = ledPhase(angle, saturation, lightness);
  printf("Phase: %d\n", phase);

  for(uint8_t offset = 0; offset < NUM_BYTES; offset += 3) {
    target_arr[offset] = (uint8_t)((phase >> 16) & 0xFF);     // G
    target_arr[offset + 1] = (uint8_t)((phase >> 8) & 0xFF);  // R
    target_arr[offset + 2] = (uint8_t)(phase & 0xFF);         // B
  }

  ledSmoothWrite();
}

void LED_Color_Spectrum::F3() {
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

void LED_Color_Spectrum::F3var() {
  char k[NUM_DIGITS_V];
  if(saturation == LED_S_HIGH) {
    saturation = LED_S_LOW; // Change to super low sat
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L2 + i);
    k[0] = 'S';
    displayWrite(3, 0x00, 1000, k);
  }
  else if(saturation == LED_S_LOW) {
    saturation = LED_S_REG; // Change to low sat
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L1 + i);
    k[0] = 'S';
    displayWrite(3, 0x00, 1000, k);
  }
  else{
    saturation = LED_S_HIGH; // Change to normal (high) sat
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L0 + i);
    k[0] = 'S';
    displayWrite(3, 0x00, 1000, k);
  }
}

void LED_Color_Spectrum::hello() {
  char k[NUM_DIGITS_V];
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_FADE + i);
  displayWrite(3, 0x00, 1000, k);
}
