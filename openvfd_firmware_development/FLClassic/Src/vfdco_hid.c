// Dummy HID driver

#include "stm32f0xx_hal.h"
#include "../../vfdco_hid.h"
#include "../../vfdco_time.h"

#ifdef _HID_IMPLEMENTATION
#error "An implementation of the HID driver already exists!"
#endif
#define _HID_IMPLEMENTATION

static uint32_t button_timers[4] = {0};
static uint8_t button_enable[4] = {0};        // Button enable
static uint8_t long_press_enable[4] = {0};    // Long press enable

static uint16_t _vfdco_button_to_pin(uint8_t button) {
  switch(button) {
    case BUTTON_F1: return GPIO_PIN_5;
    case BUTTON_F2: return GPIO_PIN_6;
    case BUTTON_F3: return GPIO_PIN_7;
    case BUTTON_F4: return GPIO_PIN_8;
    default: return GPIO_PIN_5;
  }
  return GPIO_PIN_5;
}

void _vfdco_hid_button_set(uint16_t count, uint8_t button) {
  return;
}

uint8_t vfdco_hid_button_retrieve(uint8_t num) {
  // Button check function
  uint8_t button_state = BUTTON_STATE_OFF;                      // State return variable
  if(HAL_GPIO_ReadPin(GPIOB, _vfdco_button_to_pin(num)) == 0) {
    if(button_enable[num] == 0) {                               // If button not pressed before
      button_enable[num] = 1;                                   // Set pressed flag
      button_timers[num] = vfdco_time_get_milliseconds();       // Set timer as millis
    }
    if ((vfdco_time_get_milliseconds() - button_timers[num] > BUTTON_LONGPRESS_THRESHOLD) && (long_press_enable[num] == 0)) {
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

void vfdco_hid_init() {
  return;
}

void vfdco_hid_button_retrieve_all(uint8_t *b0, uint8_t *b1, uint8_t *b2, uint8_t *b3) {
  *b0 = vfdco_hid_button_retrieve(BUTTON_F1);
  *b1 = vfdco_hid_button_retrieve(BUTTON_F2);
  *b2 = vfdco_hid_button_retrieve(BUTTON_F3);
  *b3 = vfdco_hid_button_retrieve(BUTTON_F4);
}
