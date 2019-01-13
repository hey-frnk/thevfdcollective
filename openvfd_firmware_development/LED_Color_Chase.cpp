#include <math.h>
#include "LED_Color.hpp"
#include "LED_Color_Chase.hpp"
#include "LED_Shared.hpp"

#ifdef DEBUG
#include "ArduinoSIM.h"
#else
#include <Arduino.h>
#endif

LED_Color_Chase::LED_Color_Chase(LED_Color *l, uint8_t MicPin, uint8_t *s, uint8_t flipdir, uint8_t direction) :
LED_Color_Mode(l), angle(0), secondFlip(0), directionFlag(0), state(0), MicPin(MicPin), flipdir(flipdir), s(s), lightness(LED_L_HIGH), direction(direction)
{
  stateUpdater = newiE(60);
  delayUpdater = newiE(250);
}

void LED_Color_Chase::loop() {
  if(direction < 3) {                                         // If reactive to second flip
    // DateTime now = rtc.now();                              // Get time
    if(secondFlip != *s) {                                    // If the second has changed
      if(direction == 2) directionFlag = !directionFlag;      // Change chase fade direction
      state = 0;                                              // Reset state machine
      secondFlip = *s;                                        // Overwrite old second with new second
      angle += 22;                                            // Let it overflow and get different values.
    }
  }
  else{                                                       // If reactive to sound
    if(getMicData(MicPin) > 196) {                            // If the intensity of the audio samples are higher than 196 - threshold
      if(updateIntervalEvent(&delayUpdater)) {                // And some time has elapsed
        directionFlag = !directionFlag;                       // Flip direction
        state = 0;                                            // Reset state machine
        angle += 29;                                          // And get some different color values!
      }
    }
  }

  if(state < 6) {                                             // Only run this code fragment if state is in range (< 6)
    uint32_t phase = ledPhase(angle, 255, lightness);         // Get new phase
    uint8_t offset = 0;
    if(!directionFlag) offset = state * 3;                    // Get manipulating position
    else offset = NUM_BYTES - ((state * 3) + 3);              // If direction backward, then backward!
    rgb_arr[offset] = (uint8_t)((phase >> 16) & 0xFF);        // Manipulate G
    rgb_arr[offset + 1] = (uint8_t)((phase >> 8) & 0xFF);     // Manipulate R
    rgb_arr[offset + 2] = (uint8_t)(phase & 0xFF);            // Manipulate B
  }
  l->render();

  if(updateIntervalEvent(&stateUpdater)) state++;
}

void LED_Color_Chase::F3() {
  // Short press results change in direction
  // direction = 0: From right to left (default)
  // direction = 1: From left to right
  // direction = 2: Direction flip
  // direction = 3: Flip on clap or any significant change in microphone input value
  direction++;
  if(direction == 0) directionFlag = FALSE;
  else if(direction == 1) directionFlag = TRUE;
  else if(direction == 2) directionFlag = !flipdir;
  else if(direction == 4) direction = 0;
  char LED8PMC[NUM_DIGITS_V];
  for(uint8_t i = 0; i < 2; i++) LED8PMC[i] = ' ';
  for(uint8_t i = 2; i < NUM_RGB; i++) LED8PMC[i] = LED8PM[direction][i - 2];
  displayWrite(3, 0x00, 1000, LED8PMC);
}

void LED_Color_Chase::F3var() {
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

void LED_Color_Chase::hello() {
  char k[NUM_DIGITS_V];
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_CHASEFADE + i);
  displayWrite(3, 0x00, 1000, k);
}
