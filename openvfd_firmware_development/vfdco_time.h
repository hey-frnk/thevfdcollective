/**
  ******************************************************************************
  * @file     vfdco_time.h
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     26-November-2019
  * @brief    This is the dummy driver to simulate time functionality
  ******************************************************************************
  * @toc      Table of contents, enter to navigate:
  ******************************************************************************
 **/

#ifndef _VFDCO_TIME
#define _VFDCO_TIME

#include <stdint.h>

#define DATE_MODE_DDMMYY 0
#define DATE_MODE_MMDDYY 1

// Returns milliseconds (GetTick/millis)
uint32_t (*vfdco_time_get_milliseconds)(void);
// Delay in milliseconds
void (*vfdco_time_delay_milliseconds)(uint32_t);

typedef struct vfdco_time {
  uint8_t h, m, s;
} vfdco_time_t;

typedef struct vfdco_date {
  // Note: Only the two least significant digits of the date is used
  uint8_t d, m, y;
} vfdco_date_t;

// Request current date and time from RTC
void vfdco_get_date_time(vfdco_date_t *date, vfdco_time_t *time);
// Set date of time of RTC to parameters date & time
void vfdco_set_date_time(vfdco_date_t *date, vfdco_time_t *time);

typedef struct time_event_t{
  unsigned long interval;
  unsigned long previousTime;
} time_event_t;

// Create new timer (updates every p1 milliseconds)
struct time_event_t Time_Event_Init(long interval);

// Reset timer milliseconds to 0
void Time_Event_Reset(time_event_t *input);

// Check if p1 has elapsed. If yes, return 1, else return 0
unsigned char Time_Event_Update(time_event_t *input);

#endif
