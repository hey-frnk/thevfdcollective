
#include <Arduino.h>
#include <stdint.h>
#include "vfdco_hid.h"

#ifdef _HID_IMPLEMENTATION
#error "An implementation of the HID driver already exists!"
#endif
#define _HID_IMPLEMENTATION

uint8_t buttonRegister = 0;

// ---- Menu/Interface selector variables
long p_t[4] = {0, 0, 0, 0};    // Button press timer
const long lp_t = BUTTON_LONGPRESS_THRESHOLD;         // Long press threshold
uint8_t p[4] = {0, 0, 0, 0};    // Button enable
uint8_t lp[4] = {0, 0, 0, 0};   // Long press enable

#define    B_F1_PIN   5     // ATMEGA: 11   F1 Button
#define    B_F2_PIN   6     // ATMEGA: 12   F2 Button
#define    B_F3_PIN   7     // ATMEGA: 13   F3 Button
#define    B_F4_PIN   8     // ATMEGA: 14   F4 Button

static inline uint8_t _vfdco_button_to_pin(uint8_t button) {
  switch(button) {
    case BUTTON_F1: return B_F1_PIN;
    case BUTTON_F2: return B_F2_PIN;
    case BUTTON_F3: return B_F3_PIN;
    case BUTTON_F4: return B_F4_PIN;
    default: return B_F1_PIN;
  }
  return B_F1_PIN;
}

void _vfdco_hid_button_set(uint8_t state, uint8_t button) {
  buttonRegister |= (state << (button << 1));
}

uint8_t checkOption(uint8_t num) {
    // Button check function
    uint8_t rV = BUTTON_STATE_OFF;                // State return variable
    if(digitalRead(_vfdco_button_to_pin(num)) == HIGH) { // v2.1 or earlier: pulldown
      if(p[num] == 0) {     // If button not pressed before
        p[num] = 1;          // Set pressed flag
        p_t[num] = millis();    // Set timer as millis
      }

      if ((millis() - p_t[num] > lp_t) && (lp[num] == 0)) {
        lp[num] = 1;         // Long press detected
        rV = BUTTON_STATE_LONGPRESS;                 // Set alternative number
      }
    } else {                      // If digitalRead returns FALSE
      if(p[num] == 1) {     // If pressed flag set
        if(lp[num] == 1) {  // If long press flag set
          lp[num] = 0;      // Reset long press flag
        }else{
          rV = BUTTON_STATE_SHORTPRESS;
        }
        p[num] = 0;
      }
    }
    return rV;
  }

void vfdco_hid_button_reset(uint8_t button) {
  buttonRegister &= ~(0b00000011 << (button << 1));
}

uint8_t vfdco_hid_button_retrieve(uint8_t button) {
  _vfdco_hid_button_set(checkOption(button), button);
  return (buttonRegister >> (button << 1)) & 0b00000011;
}

void vfdco_hid_init() {
  pinMode(B_F1_PIN, INPUT);
  pinMode(B_F2_PIN, INPUT);
  pinMode(B_F3_PIN, INPUT);
  pinMode(B_F4_PIN, INPUT);
}

void vfdco_hid_button_retrieve_all(uint8_t *b0, uint8_t *b1, uint8_t *b2, uint8_t *b3) {
  *b0 = vfdco_hid_button_retrieve(BUTTON_F1);
  vfdco_hid_button_reset(BUTTON_F1);
  *b1 = vfdco_hid_button_retrieve(BUTTON_F2);
  vfdco_hid_button_reset(BUTTON_F2);
  *b2 = vfdco_hid_button_retrieve(BUTTON_F3);
  vfdco_hid_button_reset(BUTTON_F3);
  *b3 = vfdco_hid_button_retrieve(BUTTON_F4);
  vfdco_hid_button_reset(BUTTON_F4);
}
