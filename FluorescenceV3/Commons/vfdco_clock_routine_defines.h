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

#ifndef _VFDCO_CLOCK_ROUTINE_DEFINES_H
#define _VFDCO_CLOCK_ROUTINE_DEFINES_H

// GUIs tell what kind of information will be sent to the dispay
typedef enum {
  GUI_TIME = 0,             // Display the current time (default of default)
  GUI_DATE = 1,             // Display the current date
  GUI_STOPWATCH = 10,       // Display stopwatch
  GUI_TIME_DATE_SET = 20,   // Active when time set or date set is enabled
  GUI_BRIGHTNESS_SET = 21   // Active when brightness (night shift) set is enabled
} gui_instance_t;

// Light Patterns tell in which way or pattern the LED lights glow
typedef enum {
  // Iterable instances, max. 8 instances. Reachable by navigating with F2 in strictly ascending order
  LIGHT_PATTERN_STATIC = 0,           // Corresponds to Light_Pattern_Static class. Single color preset
  LIGHT_PATTERN_MOMENTSOFBLISS = 1,   // Corresponds to Light_Pattern_MomentsOfBliss class
  LIGHT_PATTERN_SPECTRUM = 2,         // Corresponds to Light_Pattern_Spectrum class. Single color fade through the entire spectrum
  LIGHT_PATTERN_RAINBOW = 3,          // Corresponds to Light_Pattern_Rainbow class. Multi-color fade through the spectrum
  LIGHT_PATTERN_CHASE = 4,            // Corresponds to Light_Pattern_Chase class. One color chasing another
  LIGHT_PATTERN_MUSIC = 5,            // Corresponds to Light_Pattern_Music class. Beat reactive pattern
  LIGHT_PATTERN_TIME_CODE = 6,        // Corresponds to Light_Pattern_Time_Code class. Pattern according to time digit value
  LIGHT_PATTERN_COP = 7,              // Corresponds to Light_Pattern_Cop. Police lights

  // Non-iterable instances
  LIGHT_PATTERN_SERIAL0 = 20,         // Corresponds to Light_Pattern_Serial0. Optimized for fast response time from incoming serial color data
  LIGHT_PATTERN_SERIAL1 = 21          // Corresponds to Light_Pattern_Serial1. Optimized for smooth color setting from incoming data
} light_pattern_instance_t;

typedef enum {
  NIGHT_SHIFT_OFF,
  NIGHT_SHIFT_ON
} night_shift_state_t;

typedef enum {
  CONFIG_RANDOM_SPEED_1_ONE_MINUTE = 0,
  CONFIG_RANDOM_SPEED_2_TWO_MINUTES = 1,
  CONFIG_RANDOM_SPEED_3_TEN_MINUTES = 2,
  CONFIG_RANDOM_SPEED_4_HALF_HOUR = 3,
  CONFIG_RANDOM_SPEED_5_HOUR = 4
} random_speed_t;

typedef enum {
    COM_PROTOCOL_HW_VERSION_REQ = 0,
    COM_PROTOCOL_FW_VERSION_REQ = 1,
    COM_PROTOCOL_SETTINGS_SAVE_REQ = 2,
    COM_PROTOCOL_DEFAULT_REQ = 3,
    COM_PROTOCOL_ENABLED_INSTANCES_REQ = 4
} com_protocol_clock_control_t;

// Protocol parameters: Common
#define COM_PROTOCOL_COMMAND_OFFSET 1
#define COM_PROTOCOL_DATA_OFFSET 2
#define COM_PROTOCOL_CONTROL_OFFSET 2
#define COM_PROTOCOL_PARAM0_OFFSET 3
#define COM_PROTOCOL_PARAM1_OFFSET 4

// Protocol parameters: Specific
#define COM_PROTOCOL_TIMEDATE_SET_OFFSET_FLAG 8

#define INSTANCE_NO_SETTINGS 255 // If instance has no settings, the setting to instance mapper will return 255

#endif

#ifdef __cplusplus
}
#endif
