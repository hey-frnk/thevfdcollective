/*MIT License

Copyright (c) The VFD Collective, Frank from The VFD Collective (Fu Zheng)
Date: 04/25/2020

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

/**
  ******************************************************************************
  * @file     vfdco_display.h
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     13-April-2020
  * @brief    This file contains declarations for communication functions to an
  *           array of (alpha)numeric displays
  ******************************************************************************
 **/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _VFDCOLLECTIVE_DISPLAY_H
#define _VFDCOLLECTIVE_DISPLAY_H

#include <stdint.h>
#include "vfdco_time.h"

/**
 * @brief Initialize display with an initial brightness dimming factor
 * @param dim_factor used to divide brightness by 2^(dim_factor). Hence, e.g. dim_factor == 2 divides brightness by 4, and the display is 1/4 bright
 */
void        vfdco_display_init(uint8_t dim_factor);

/**
 * @brief Set a dimming factor to the display, in order to reduce brightness
 * @param dim_factor used to divide brightness by 2^(dim_factor). Hence, e.g. dim_factor == 2 divides brightness by 4, and the display is 1/4 bright
 */
void        vfdco_display_set_dim_factor(uint8_t dim_factor);

/**
 * @brief Send time to the display
 * @param time Time structure of type vfdco_time_t as defined in vfdco_time.h consisting of h (uint8_t), m (uint8_t), s (uint8_t)
 * @param decimal_dot_register Decimal dot overlay [ reserved | reserved | dot5. | dot4. | dot3. | dot2. | dot1. | dot0. ]
 *                                            Bit: 7                                                                     0
 * @param time_mode According to time_format_t in vfdco_time.h
 */
void        vfdco_display_render_time(const vfdco_time_t *time, const uint8_t decimal_dot_register, time_format_t time_mode);

/**
 * @brief Sent date to the display with fixed decimal dots at segment 2 and 4 (left to right)
 * @param date Date structure of type vfdco_date_t as defined in vfdco_time.h consisting of d (uint8_t), m (uint8_t), y (uint8_t)
 * @param date_mode According to date_format_t in vfdco_time.h
 */
void        vfdco_display_render_date(const vfdco_date_t *date, date_format_t date_mode);

/**
 * @brief Send custom text to the display
 * @param message ASCII (char) representation of the text
 * @param decimal_dot_register Decimal dot overlay [ reserved | reserved | dot5. | dot4. | dot3. | dot2. | dot1. | dot0. ]
 *                                            Bit: 7                                                                     0
 * @param delay in milliseconds. Adds a blocking time delay to the rendered message. Ignored if zero
 */
void        vfdco_display_render_message(const char *message, const uint8_t decimal_dot_register, uint16_t delay);

#endif

#ifdef __cplusplus
}
#endif
