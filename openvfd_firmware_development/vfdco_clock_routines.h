/**
  ******************************************************************************
  * @file     vfdco_routines.c
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     12-February-2020
  * @brief    Each (peripheral) component has an initializer and a routine function
  *           The routine function is desiged to be called periodically in main()
  *           The initializers and routines are declared here
  *           Designed to be used with Fluorescence by The VFD Collective
  ******************************************************************************
  * @tableofcontents Table of contents, enter to navigate:
  * TODO
  ******************************************************************************
 **/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _VFDCO_CLOCK_ROUTINES_H
#define _VFDCO_CLOCK_ROUTINES_H

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
  LIGHT_PATTERN_MOMENTSOFBLISS,
  LIGHT_PATTERN_RAINBOW,
  LIGHT_PATTERN_CHASE,
  LIGHT_PATTERN_TIME_CODE,
  LIGHT_PATTERN_COP
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
