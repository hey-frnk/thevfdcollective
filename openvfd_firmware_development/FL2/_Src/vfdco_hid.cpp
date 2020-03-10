
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
const long lp_t = 500;         // Long press threshold
uint8_t p[4] = {0, 0, 0, 0};    // Button enable
uint8_t lp[4] = {0, 0, 0, 0};   // Long press enable

#define    B_F1_PIN   5     // ATMEGA: 11   F1 Button
#define    B_F2_PIN   6     // ATMEGA: 12   F2 Button
#define    B_F3_PIN   7     // ATMEGA: 13   F3 Button
#define    B_F4_PIN   8     // ATMEGA: 14   F4 Button

static inline uint8_t _vfdco_hid_button_mapper(uint16_t GPIO_Pin) {
  switch(GPIO_Pin) {
    case B_F1_PIN: return BUTTON_F1;
    case B_F2_PIN: return BUTTON_F2;
    case B_F3_PIN: return BUTTON_F3;
    case B_F4_PIN: return BUTTON_F4;
    default: return BUTTON_F1;
  }
  return BUTTON_F1;
}

void _vfdco_hid_button_set(uint16_t count, uint8_t button) {
  uint8_t mask = BUTTON_STATE_SHORTPRESS;
  if      (count < BUTTON_SHORTPRESS_THRESHOLD) mask = BUTTON_STATE_OFF;
  else if (count > BUTTON_LONGPRESS_THRESHOLD)  mask = BUTTON_STATE_LONGPRESS;

  buttonRegister |= (mask << (button << 1));
}

inline uint8_t getNum(uint8_t num) {
  if(num == B_F1_PIN) return 0;
  if(num == B_F2_PIN) return 1;
  if(num == B_F3_PIN) return 2;
  if(num == B_F4_PIN) return 3;
  return -1;
}

uint8_t checkOption(uint8_t buttonPin) {
    // Button check function
    uint8_t num = getNum(buttonPin);
    uint8_t rV = BUTTON_STATE_OFF;                // State return variable
    if(digitalRead(buttonPin) == HIGH) { // v2.1 or earlier: pulldown
      if(p[num] == 0) {     // If button not pressed before
        p[num] = 1;          // Set pressed flag
        p_t[num] = millis();    // Set timer as millis
      }

      if ((millis() - p_t[num] > lp_t) && (lp[num] == 0)) {
        lp[num] = 1;         // Long press detected
        rV = BUTTON_STATE_LONGPRESS;                 // Set alternative number
      }
    }else{                      // If digitalRead returns FALSE
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
  return checkOption(_vfdco_hid_button_mapper(button)); // (buttonRegister >> (button << 1)) & 0b00000011;
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
