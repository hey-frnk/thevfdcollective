/*
 * Being part of something special makes you special
 *
 * Created summer 2018
 * Author: Copyright (C) The VFD Collective, Frank Zheng
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _VFDCOLLECTIVE_DISPLAY_H
#define _VFDCOLLECTIVE_DISPLAY_H

#include <stdint.h>
#include "vfdco_time.h"

uint8_t     num_digits;

uint8_t     vfdco_display_char_convert(char input);

// Given a time or date, the time or date will be sent to the display
// Decimal dot overlay function: uint8_t decimal_dot_register
// [ reserved | reserved | dot5. | dot4. | dot3. | dot2. | dot1. | dot0. ]
// 7                                                                     0
void        vfdco_display_render_time(vfdco_time_t *time, uint8_t decimal_dot_register);
void        vfdco_display_render_date(vfdco_date_t *date, uint8_t decimal_dot_register, uint_fast8_t date_mode);

void        vfdco_display_render_message(const char *message, uint8_t decimal_dot_register);

void        vfdco_display_render_direct(uint8_t *data);

void        vfdco_display_init(uint_fast8_t _num_digits);

#endif

#ifdef __cplusplus
}
#endif
