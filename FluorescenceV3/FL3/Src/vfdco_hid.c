#include "stm32l4xx_hal.h"
#include "../../Commons/vfdco_hid.h"
#include "../../Commons/vfdco_time.h"
#include "../../Commons/vfdco_config.h"

#ifdef _HID_IMPLEMENTATION
#error "An implementation of the HID driver already exists!"
#endif
#define _HID_IMPLEMENTATION

typedef enum {
  BUTTON_FSM_IDLE,
  BUTTON_FSM_DEBOUNCE_PRESS,
  BUTTON_FSM_PRESSED,
  BUTTON_FSM_LONG_HELD,
  BUTTON_FSM_DEBOUNCE_RELEASE,
} BUTTON_STATE_t;

typedef struct {
  BUTTON_STATE_t state;
  uint32_t timestamp;
  uint8_t event;
  uint8_t longpress_handled;
} BUTTON_FSM_t;

static BUTTON_FSM_t button_fsms[4] = {0};

static uint16_t _vfdco_button_to_pin(uint8_t button) {
  switch(button) {
    case BUTTON_F1: return GPIO_PIN_5;
    case BUTTON_F2: return GPIO_PIN_6;
    case BUTTON_F3: return GPIO_PIN_7;
    case BUTTON_F4: return GPIO_PIN_3;
    default: return GPIO_PIN_5;
  }
  return GPIO_PIN_5;
}

static GPIO_TypeDef* _vfdco_button_to_port(uint8_t button) {
	switch(button) {
		case BUTTON_F1: return GPIOB;
		case BUTTON_F2: return GPIOB;
		case BUTTON_F3: return GPIOB;
		case BUTTON_F4: return GPIOH;
		default: return GPIOB;
	}
	return GPIOB;
}

void _vfdco_hid_button_set(uint16_t count, uint8_t button) {
  return;
}

void vfdco_hid_button_retrieve(uint8_t num, uint8_t is_pressed) {
  BUTTON_FSM_t *fsm = &button_fsms[num];
  uint32_t now = vfdco_time_get_milliseconds();

  switch(fsm->state) {

    case BUTTON_FSM_IDLE:
      if(is_pressed) {
        fsm->state = BUTTON_FSM_DEBOUNCE_PRESS;
        fsm->timestamp = now;
        fsm->longpress_handled = 0;
      }
      break;
    
    case BUTTON_FSM_DEBOUNCE_PRESS:
      if(!is_pressed) {
        fsm->state = BUTTON_FSM_IDLE;
      } else if(now - fsm->timestamp >= BUTTON_SHORTPRESS_THRESHOLD) {
        fsm->state = BUTTON_FSM_PRESSED;
        fsm->timestamp = now;
      }
      break;
    
    case BUTTON_FSM_PRESSED:
      if(!is_pressed) {
        fsm->state = BUTTON_FSM_DEBOUNCE_RELEASE;
        fsm->timestamp = now;
      } else if(now - fsm->timestamp >= BUTTON_LONGPRESS_THRESHOLD) {
        fsm->state = BUTTON_FSM_LONG_HELD;
        fsm->event = BUTTON_EVENT_LONGPRESS;
        fsm->longpress_handled = 1; // <--- Important
      }
      break;

    case BUTTON_FSM_LONG_HELD:
      if(!is_pressed) {
        fsm->state = BUTTON_FSM_DEBOUNCE_RELEASE;
        fsm->timestamp = now;
      }
      break;

    case BUTTON_FSM_DEBOUNCE_RELEASE:
      if(is_pressed) {
        fsm->state = BUTTON_FSM_PRESSED; // Re-bounce
      } else if(now - fsm->timestamp >= BUTTON_SHORTPRESS_THRESHOLD) {
        if(fsm->longpress_handled == 0) { // <--- Important: Only if not already a longpress
          fsm->event = BUTTON_EVENT_SHORTPRESS;
        }
        fsm->state = BUTTON_FSM_IDLE;
        fsm->longpress_handled = 0; // Reset
      }
      break;
  }
}

void vfdco_hid_init() {
  return;
}

void vfdco_hid_button_retrieve_all(uint8_t *b0, uint8_t *b1, uint8_t *b2, uint8_t *b3) {
  uint8_t states[4] = {
    HAL_GPIO_ReadPin(_vfdco_button_to_port(BUTTON_F1), _vfdco_button_to_pin(BUTTON_F1)) == GPIO_PIN_RESET,
    HAL_GPIO_ReadPin(_vfdco_button_to_port(BUTTON_F2), _vfdco_button_to_pin(BUTTON_F2)) == GPIO_PIN_RESET,
    HAL_GPIO_ReadPin(_vfdco_button_to_port(BUTTON_F3), _vfdco_button_to_pin(BUTTON_F3)) == GPIO_PIN_RESET,
    HAL_GPIO_ReadPin(_vfdco_button_to_port(BUTTON_F4), _vfdco_button_to_pin(BUTTON_F4)) == GPIO_PIN_RESET
  };

  for(uint_fast8_t i = 0; i < 4; ++i) {
	  vfdco_hid_button_retrieve(i, states[i]);
  }

  *b0 = button_fsms[0].event;
  *b1 = button_fsms[1].event;
  *b2 = button_fsms[2].event;
  *b3 = button_fsms[3].event;

  // Clear events after reading
  for(uint_fast8_t i = 0; i < 4; ++i) {
    button_fsms[i].event = BUTTON_EVENT_OFF;
  }
}
