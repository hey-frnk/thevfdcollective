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

void        vfdco_display_set_dim_factor(uint8_t dim_factor);

// Given a time or date, the time or date will be sent to the display
// Decimal dot overlay function: uint8_t decimal_dot_register
// [ reserved | reserved | dot5. | dot4. | dot3. | dot2. | dot1. | dot0. ]
// 7                                                                     0
void        vfdco_display_render_time(vfdco_time_t *time, const uint8_t decimal_dot_register, time_format_t time_mode);
void        vfdco_display_render_date(vfdco_date_t *date, const uint8_t decimal_dot_register, date_format_t date_mode);

void        vfdco_display_render_message(const char *message, const uint8_t decimal_dot_register, uint16_t delay);

void        vfdco_display_init(uint8_t dim_factor);

#endif

#ifdef __cplusplus
}
#endif
