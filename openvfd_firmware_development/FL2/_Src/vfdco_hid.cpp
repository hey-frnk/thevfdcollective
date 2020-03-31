
#include <Arduino.h>
#include <stdint.h>
#include "vfdco_hid.h"

#ifdef _HID_IMPLEMENTATION
#error "An implementation of the HID driver already exists!"
#endif
#define _HID_IMPLEMENTATION

#define    B_F1_PIN   5     // ATMEGA: 11   F1 Button
#define    B_F2_PIN   6     // ATMEGA: 12   F2 Button
#define    B_F3_PIN   7     // ATMEGA: 13   F3 Button
#define    B_F4_PIN   8     // ATMEGA: 14   F4 Button

static uint32_t button_timers[4] = {0};
static uint8_t button_enable[4] = {0};        // Button enable
static uint8_t long_press_enable[4] = {0};    // Long press enable

static uint16_t _vfdco_button_to_pin(uint8_t button) {
  switch(button) {
    case BUTTON_F1: return B_F1_PIN;
    case BUTTON_F2: return B_F2_PIN;
    case BUTTON_F3: return B_F3_PIN;
    case BUTTON_F4: return B_F4_PIN;
    default: return B_F1_PIN;
  }
  return B_F1_PIN;
}

void _vfdco_hid_button_set(uint16_t count, uint8_t button) {
  return;
}

uint8_t vfdco_hid_button_retrieve(uint8_t num) {
  // Button check function
  uint8_t button_state = BUTTON_STATE_OFF;                      // State return variable
  if(digitalRead(_vfdco_button_to_pin(num)) == HIGH) {
    if(button_enable[num] == 0) {                               // If button not pressed before
      button_enable[num] = 1;                                   // Set pressed flag
      button_timers[num] = millis();                            // Set timer as millis
    }
    if ((millis() - button_timers[num] > BUTTON_LONGPRESS_THRESHOLD) && (long_press_enable[num] == 0)) {
      long_press_enable[num] = 1;                               // Long press detected
      button_state = BUTTON_STATE_LONGPRESS;                    // Set alternative number
    }
  } else {                                                      // If digitalRead returns FALSE
    if(button_enable[num] == 1) {                               // If pressed flag set
      if(long_press_enable[num] == 1) {                         // If long press flag set
        long_press_enable[num] = 0;                             // Reset long press flag
      }else {
        button_state = BUTTON_STATE_SHORTPRESS;
      }
      button_enable[num] = 0;
    }
  }
  return button_state;
}

void vfdco_hid_button_retrieve_all(uint8_t *b0, uint8_t *b1, uint8_t *b2, uint8_t *b3) {
  *b0 = vfdco_hid_button_retrieve(BUTTON_F1);
  *b1 = vfdco_hid_button_retrieve(BUTTON_F2);
  *b2 = vfdco_hid_button_retrieve(BUTTON_F3);
  *b3 = vfdco_hid_button_retrieve(BUTTON_F4);
}

void vfdco_hid_init() {
  pinMode(B_F1_PIN, INPUT);
  pinMode(B_F2_PIN, INPUT);
  pinMode(B_F3_PIN, INPUT);
  pinMode(B_F4_PIN, INPUT);
}
