#include "LED_Color.hpp"
#include "LED_Color_Static.hpp"
#include "LED_Shared.hpp"
#include "Arduino.h"

LED_Color_Static::LED_Color_Static(LED_Color *l, uint8_t position) :
LED_Color_Mode(l), position(position) {}

void LED_Color_Static::loop() {
  // If not single Color
  if(position > LED0_mcOffset) {
    for(uint8_t i = 0; i < NUM_BYTES; i++) target_arr[i] = led_Presets[position - LED0_cOffset][i];
    ledSmoothWrite();
  }
  else{ // Save some RAM
    for(uint8_t offset = 0; offset < NUM_BYTES; offset += 3) {
      target_arr[offset] = led_scPresets[position][0];
      target_arr[offset + 1] = led_scPresets[position][1];
      target_arr[offset + 2] = led_scPresets[position][2];
    }
    ledSmoothWrite();
  }
}

void LED_Color_Static::F3() {
  position++;
  if(position == 16) position = 0;

  // Dynamic memory saving
  char LED0PMC[NUM_DIGITS_V];
  LED0PMC[0] = 'C';
  LED0PMC[1] = ' ';
  for(uint8_t i = 2; i < NUM_RGB; i++) LED0PMC[i] = LED0PM[position][i - 2];

  displayWrite(3, 0x00, 500, LED0PMC);    // Write change message
}

void LED_Color_Static::hello() {
  char k[NUM_DIGITS_V];
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_COLOR + i);
  displayWrite(3, 0x00, 1000, k);
}
