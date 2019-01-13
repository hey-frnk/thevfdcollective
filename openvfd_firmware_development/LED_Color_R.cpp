#include "LED_Color.hpp"
#include "LED_Color_R.hpp"
#include "LED_Shared.hpp"

#ifdef DEBUG
#include "ArduinoSIM.h"
#else
#include <Arduino.h>
#endif

LED_Color_R::LED_Color_R(LED_Color *l, uint8_t *h, uint8_t *m, uint8_t *s) :
LED_Color_Mode(l), h(h), m(m), s(s) {}

void LED_Color_R::loop() {
  uint8_t clockData[NUM_RGB];

  // Get the time once again
  // DateTime now = rtc.now();
  clockData[0] = *s % 10;
  clockData[1] = *s / 10;
  clockData[2] = *m % 10;
  clockData[3] = *m / 10;
  clockData[4] = *h % 10;
  clockData[5] = *h / 10;

  uint8_t offset = 0;
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) {
    target_arr[offset] = led_Resistor[clockData[i]][0];        // G
    target_arr[offset + 1] = led_Resistor[clockData[i]][1];    // R
    target_arr[offset + 2] = led_Resistor[clockData[i]][2];    // B
    offset += 3;
  }

  ledSmoothWrite();
}

void LED_Color_R::hello() {
  char k[NUM_DIGITS_V];
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_CRCODE + i);
  displayWrite(3, 0x00, 1000, k);
}
