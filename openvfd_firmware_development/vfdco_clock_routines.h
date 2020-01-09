/*
 * Being part of something special makes you special
 *
 * Created winter 2020
 * Author: Copyright (C) The VFD Collective, Frank Zheng
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _VFDCO_CLOCK_ROUTINES_H
#define _VFDCO_CLOCK_ROUTINES_H

// Number of digits and pixels
#define GLOBAL_NUM_DIGITS_NUM_PIXELS 6

// How often should we ask the RTC what time it is? (Milliseconds)
#define GLOBAL_TIME_UPDATE_INTERVAL 111
#define GLOBAL_GUI_TIME_UPDATE_INTERVAL 18
#define GLOBAL_GUI_DATE_UPDATE_INTERVAL 240

// GUIs tell what kind of data will shuttle through the shift registers and the display render function(s) will render accordingly.
typedef enum {
  GUI_TIME,             // Display the current time (default of default)
  GUI_DATE,             // Display the current date

  GUI_STOPWATCH,        // Display stopwatch

  GUI_TIME_DATE_SET     // Active when time set or date set is enabled
} gui_instance_t;

typedef enum {
  LIGHT_PATTERN_STATIC,
  LIGHT_PATTERN_SPECTRUM,
  LIGHT_PATTERN_RAINBOW,
  LIGHT_PATTERN_CHASE,
  LIGHT_PATTERN_TIME_CODE
} light_pattern_instance_t;

// Initialize function. Run first.
void    vfdco_clock_initializer();

// Human interface device (Buttons) routine
void    vfdco_clock_hid_initializer();
void    vfdco_clock_hid_routine();

// RTC time refresh routine
void    vfdco_clock_time_initializer();
void    vfdco_clock_time_routine();

// VFD display data render routine
void    vfdco_clock_display_initializer();
void    vfdco_clock_display_routine();

// VFD LED light illumination routine
void    vfdco_clock_lights_initializer();
void    vfdco_clock_lights_routine();

// Communication (Serial/USB, Serial/Bluetooth) routine
void    vfdco_clock_com_initializer();
void    vfdco_clock_com_routine();

#endif

#ifdef __cplusplus
}
#endif
