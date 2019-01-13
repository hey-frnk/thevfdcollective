#include <math.h>
#include "LED_Color.hpp"
#include "LED_Color_Music.hpp"
#include "LED_Shared.hpp"

#ifdef DEBUG
#include "ArduinoSIM.h"
#else
#include <Arduino.h>
#endif

LED_Color_Music::LED_Color_Music(LED_Color *l, uint8_t MicPin) :
LED_Color_Mode(l), angle(0), delta(21), state(0), delayState(FALSE), MicPin(MicPin), lightness(LED_L_HIGH) {
  angleUpdater = newiE(60);
  stateUpdater = newiE(25);
  delayUpdater = newiE(250);
}

void LED_Color_Music::loop() {
  // If time interval passed, decrease turned on LEDs by one (regular state update)
  if(updateIntervalEvent(&stateUpdater)) if(state < 7) state++;

  // Cross fade LED color position update
  if(updateIntervalEvent(&angleUpdater)) angle++; // Just let it overflow and begin from 0 :p

  // Read microphone information, convert value to updateable state
  // Get mic data (log), divide by 36
  uint8_t rLevel = 6 - (uint8_t)(roundf(getMicData(MicPin) / 42.5));
  // Write the less valued LEDs only when the sound is actively changed

  if(state >= rLevel) {
    state = rLevel;                                        // If the new state is lower than the previous state: Overwrite current state with new rLevel (interrupt state)
    delayState = FALSE;                                        // Delay state = 0 (reset)
  }
  else{
    if(updateIntervalEvent(&delayUpdater)) delayState = TRUE;    // If the time has elapsed, write empty
  }

  if(state < 7) {                                           // If new information
    uint8_t offset = 0;

    for(uint8_t i = 0; i < (6 - state); i++) {
      uint32_t phase = ledPhase(angle + (i * delta), 255, lightness);
      rgb_arr[offset] = (uint8_t)((phase >> 16) & 0xFF);      // G
      rgb_arr[offset + 1] = (uint8_t)((phase >> 8) & 0xFF);   // R
      rgb_arr[offset + 2] = (uint8_t)(phase & 0xFF);          // B
      offset += 3;
    }

    // And set all the others zero
    for(uint8_t lOffset = offset; lOffset < NUM_BYTES; lOffset++) rgb_arr[lOffset] = 0;

    // Black out the inactives
    if(delayState) {
      if(state < 6) {
        uint8_t tOffset = 0;                                  // Temporary offset variable
        for(uint8_t i = 0; i < (5 - state); i++) {
          rgb_arr[tOffset] = 0;
          rgb_arr[tOffset + 1] = 0;
          rgb_arr[tOffset + 2] = 0;
          tOffset += 3;
        }
      }
    }

    l->render();
  }
}

void LED_Color_Music::F3() {
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

void LED_Color_Music::F3var() {
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

void LED_Color_Music::hello() {
  char k[NUM_DIGITS_V];
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_CSOUND + i);
  displayWrite(3, 0x00, 1000, k);
}
