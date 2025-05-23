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
  * @file     vfdco_time.h
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     26-November-2019
  * @brief    Time functions, clocked operations and RTC interface
  *           Designed to be used with Fluorescence by The VFD Collective
  ******************************************************************************
  * @tableofcontents Table of contents, enter to navigate:
  * SECTION_TIME_FORMAT_DEFINITIONS
  * SECTION_TIME_CLOCK_HARDWARE_FUNCTIONS
  * SECTION_TIME_DATE_TYPE
  * SECTION_SOFTWARE_TIMER
  ******************************************************************************
 **/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _VFDCO_TIME
#define _VFDCO_TIME

#include <stdint.h>

/** Begin of:
 * @tableofcontents SECTION_TIME_FORMAT_DEFINITIONS
**/
// Time format 12h or 24h
typedef enum {
  TIME_FORMAT_24H       = 0,
  TIME_FORMAT_12H       = 1,
  TIME_FORMAT_12H_NO_LZ = 2 // No leading zero between 1-11 pm
} time_format_t;
// Date format DD.MM.YY or MM.DD.YY
typedef enum {
  DATE_FORMAT_DDMMYY = 0,
  DATE_FORMAT_MMDDYY = 1
} date_format_t;


/** Begin of:
 * @tableofcontents SECTION_TIME_CLOCK_HARDWARE_FUNCTIONS
 * @brief Platform dependent clock related functions made independent
**/
// Returns milliseconds (GetTick/millis)
extern uint32_t (*vfdco_time_get_milliseconds)(void);
// Delay in milliseconds
extern void (*vfdco_time_delay_milliseconds)(uint32_t);


/** Begin of:
 * @tableofcontents SECTION_TIME_DATE_TYPE
 * @brief Classes time_t and date_t
**/
typedef struct vfdco_time {
  uint8_t h, m, s;
} vfdco_time_t;
typedef struct vfdco_date {
  uint8_t d, m, y; // Note: Only the two least significant digits of the date is used
} vfdco_date_t;
// Request current date and time from RTC
void vfdco_get_date_time(vfdco_date_t *date, vfdco_time_t *time);
// Set date of time of RTC to parameters date & time
void vfdco_set_date_time(const vfdco_date_t *date, const vfdco_time_t *time);
// Initialize RTC
void vfdco_rtc_init();


/** Begin of:
 * @tableofcontents SECTION_SOFTWARE_TIMER
 * @brief Class time_event. Software timer, used as alternative for hardware timer instances
**/
typedef struct time_event_t{
  uint16_t interval;
  unsigned long previousTime;
} time_event_t;

// Create new timer (updates every p1 milliseconds)
struct time_event_t Time_Event_Init(uint16_t interval);
// Reset timer milliseconds to 0
void Time_Event_Reset(time_event_t *input);
// Check if p1 has elapsed. If yes, return 1, else return 0
unsigned char Time_Event_Update(time_event_t *input);

#endif

#ifdef __cplusplus
}
#endif
