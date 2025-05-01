// Dummy HID driver

#include "../Commons/vfdco_hid.h"

#ifdef _HID_IMPLEMENTATION
#error "An implementation of the HID driver already exists!"
#endif
#define _HID_IMPLEMENTATION

uint8_t buttonRegister = 0;

void _vfdco_hid_button_set(uint16_t count, uint8_t button) {
	uint8_t mask = BUTTON_EVENT_SHORTPRESS;
	if      (count < BUTTON_SHORTPRESS_THRESHOLD) mask = BUTTON_EVENT_OFF;
	else if (count > BUTTON_LONGPRESS_THRESHOLD)  mask = BUTTON_EVENT_LONGPRESS;

	buttonRegister |= (mask << (button << 1));
}

void vfdco_hid_button_reset(uint8_t button) {
	buttonRegister &= ~(0b00000011 << (button << 1));
}

uint8_t vfdco_hid_button_retrieve(uint8_t button) {
  return (buttonRegister >> (button << 1)) & 0b00000011;
}

void vfdco_hid_init() {
  return;
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
