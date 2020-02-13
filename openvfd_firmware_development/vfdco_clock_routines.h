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

// 
void    vfdco_clock_serialization_initializer();
#ifdef DEBUG
void    vfdco_clock_serialization_routine();
#endif

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
// void    vfdco_clock_com_initializer();
// void    vfdco_clock_com_routine();


#include "vfdco_serialization.h"
#define NUM_SERIALIZABLE 8
extern struct Serialized_Data *settings_serialized[NUM_SERIALIZABLE];

/*
Serialization Documentation (Bytes)

(8) SERIALIZABLE_CLOCK_ROUTINE: Routine Settings
- (6) 6 * char, welcome
- (1) uint8_t, global_gui_instance_counter
- (1) uint8_t, global_light_instance_counter
(2) SERIALIZABLE_GUI_TIME: Time Display Settings
- (1) uint8_t, time_mode (12/24H/12HLZ)
- (1) uint8_t, dot_mode
(1) SERIALIZABLE_GUI_DATE: Date Display Settings
- (1) uint8_t, date_mode (DDMMYY/MMDDYY)
(1) SERIALIZABLE_LIGHTS_STATIC: Static Color Settings
- (1) uint8_t, position
(1) SERIALIZABLE_LIGHTS_BLISS: MomentsOfBliss Color Settings
- (1) uint8_t, moment
(2) SERIALIZABLE_LIGHTS_SPECTRUM: Spectrum Color Settings
- (1) uint8_t, saturation
- (1) uint8_t, lightness
(2) SERIALIZABLE_LIGHTS_RAINBOW: Rainbow Color Settings
- (1) int8_t, chain_hue_diff
- (1) uint8_t, saturation
(2) SERIALIZABLE_LIGHTS_CHASE: 
- (1) uint8_t, chase_mode
- (1) uint8_t, color_peak_diff
*/

#define CREATE_SERIALIZED_ENTRIES(ENTRY) \
  ENTRY(0, SERIALIZABLE_CLOCK_ROUTINE, 8) \
  ENTRY(1, SERIALIZABLE_GUI_TIME, 2) \
  ENTRY(2, SERIALIZABLE_GUI_DATE, 1) \
  ENTRY(3, SERIALIZABLE_LIGHTS_STATIC, 1) \
  ENTRY(4, SERIALIZABLE_LIGHTS_BLISS, 1) \
  ENTRY(5, SERIALIZABLE_LIGHTS_SPECTRUM, 2) \
  ENTRY(6, SERIALIZABLE_LIGHTS_RAINBOW, 2) \
  ENTRY(7, SERIALIZABLE_LIGHTS_CHASE, 2)

#endif

#ifdef __cplusplus
}
#endif
