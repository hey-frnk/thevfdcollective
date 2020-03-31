
#ifdef __cplusplus
extern "C" {
#endif

#ifndef _VFDCO_HID_H
#define _VFDCO_HID_H

#include <stdint.h>

#define   BUTTON_SHORTPRESS_THRESHOLD   50
#define   BUTTON_LONGPRESS_THRESHOLD    600

#define   BUTTON_STATE_OFF         0b00000000
#define   BUTTON_STATE_SHORTPRESS  0b00000001
#define   BUTTON_STATE_LONGPRESS   0b00000010

#define   BUTTON_F1                0
#define   BUTTON_F2                1
#define   BUTTON_F3                2
#define   BUTTON_F4                3

// This function sets button data to 0b00 state of the provided button
void      vfdco_hid_button_reset(uint8_t button);

// This function retrieves the 2 bit current state of the provided button
uint8_t   vfdco_hid_button_retrieve(uint8_t button);

// Retrieve all buttons
void      vfdco_hid_button_retrieve_all(uint8_t *b0, uint8_t *b1, uint8_t *b2, uint8_t *b3);

void      _vfdco_hid_button_set(uint16_t count, uint8_t button);

void      vfdco_hid_init();

#endif

#ifdef __cplusplus
}
#endif