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
  * @file     vfdco_gui.h
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     12-February-2020
  * @brief    This file contains declarations for the GUI
  *           Designed to be used with Fluorescence by The VFD Collective
  ******************************************************************************
  * @tableofcontents Table of contents, enter to navigate:
  * SECTION_GUI_FORMAT
  * SECTION_GUI_FORMAT_TIME
  * SECTION_GUI_FORMAT_DATE
  * SECTION_GUI_FORMAT_TIME_DATE_SET
  * SECTION_GUI_FORMAT_STOPWATCH
  * SECTION_GUI_BRIGHTNESS_SETTER
  * SECTION_GUI_DIGIT_FADE_SETTER
  * SECTION_CONTAINER_GUI
  ******************************************************************************
 **/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _VFDCO_GUI_H
#define _VFDCO_GUI_H

#include <stdint.h>
#include "vfdco_time.h"
#include "vfdco_hid.h"

/** Begin of:
 * @tableofcontents SECTION_GUI_FORMAT
 * @details GUI_Format is the virtual base class. Its children perform operations to the display.
 * Each child class must overwrite the following virtual methods:
 * - F2: This method is triggered upon an HID event, typically sets a configuration. Can be set to NULL
 * - F3: This method is triggered upon an HID event, typically sets a configuration. Can be set to NULL
 * - F4: This method is triggered upon an HID event, typically sets a configuration. Can be set to NULL
 * - F2Var: This method is triggered upon an HID event, typically sets a configuration. Can be set to NULL
 * - F3Var: This method is triggered upon an HID event, typically sets a configuration. Can be set to NULL
 * - F4Var: This method is triggered upon an HID event, typically sets a configuration. Can be set to NULL
 * - Update: This method is called periodically in object lifetime. Typically used to update FSMs and render. Must not be NULL
**/
typedef union GUI_Format GUI_Format;
extern void         (*GUI_Format_F2)(GUI_Format *unsafe_self);
extern void         (*GUI_Format_F3)(GUI_Format *unsafe_self);
extern void         (*GUI_Format_F4)(GUI_Format *unsafe_self);
extern void         (*GUI_Format_F2Var)(GUI_Format *unsafe_self);
extern void         (*GUI_Format_F3Var)(GUI_Format *unsafe_self);
extern void         (*GUI_Format_F4Var)(GUI_Format *unsafe_self);
extern void         (*GUI_Format_Update)(GUI_Format *unsafe_self);
extern void         (*GUI_Format_Save)(GUI_Format *unsafe_self);

// Documentation see config.h::CONFIG_SAVED_SETTINGS_TABLE. Anonymous enums for setting offsets
#define CREATE_SETTINGS_OFFSET_GUI(_entry, _offset, _size, _setting_identifier, _defaultval, _description) \
  enum { _setting_identifier = _offset };
CREATE_SERIALIZED_GUI_POSITIONS(CREATE_SETTINGS_OFFSET_GUI)


/** Begin of:
  * @tableofcontents SECTION_GUI_FORMAT_TIME
 **/
struct GUI_Format_Time {
  uint8_t           *settings;
  vfdco_time_t      *time_instance;
  time_format_t     time_mode;
  uint_fast8_t      dot_mode;
  uint8_t           dot_position;
  uint8_t           dot_direction;
  time_event_t      dot_timer;
};

void GUI_Format_Time_Init(struct GUI_Format_Time *self, vfdco_time_t *time_instance, uint8_t *settings);


/** Begin of:
  * @tableofcontents SECTION_GUI_FORMAT_DATE
 **/
struct GUI_Format_Date {
  uint8_t            *settings;
  vfdco_date_t       *date_instance;
  date_format_t      date_mode;
};

void GUI_Format_Date_Init(struct GUI_Format_Date *self, vfdco_date_t *date_instance, uint8_t *settings);


/** Begin of:
  * @tableofcontents SECTION_GUI_FORMAT_TIME_DATE_SET
 **/
struct GUI_Format_Time_Date_Setter {
  uint_fast8_t      set_mode; // Set date or time?
  uint_fast8_t      active_digit; // Group with blank digits

  uint_fast8_t      blank_active; // Blank period?
  char              blank_alt_message[CONFIG_NUM_DIGITS]; // Since render time is bad, use alt renderer
  time_event_t      blank_timer; // Blank timer

  vfdco_time_t      new_time;
  vfdco_date_t      new_date;
};

void GUI_Format_Time_Date_Setter_Init(struct GUI_Format_Time_Date_Setter *self, vfdco_time_t *time_instance, vfdco_date_t *date_instance, uint_fast8_t set_mode);


enum {
  GUI_FORMAT_STOPWATCH_STATE_INITIALIZED,
  GUI_FORMAT_STOPWATCH_STATE_RUNNING,
  GUI_FORMAT_STOPWATCH_STATE_PAUSED
};

/** Begin of:
  * @tableofcontents SECTION_GUI_FORMAT_STOPWATCH
  * F2 to start, F3 to pause and resume. During pause, F2 to reset
 **/
struct GUI_Format_Stopwatch {
  uint_fast8_t      stopwatch_state;
  vfdco_time_t      *time_instance;
  vfdco_time_t      initial_time;
  uint8_t           not_initial;
  uint32_t          elapsed_time;
  uint32_t          initial_milliseconds;
  uint32_t          elapsed_milliseconds;
};

void GUI_Format_Stopwatch_Init(struct GUI_Format_Stopwatch *self, vfdco_time_t *time_instance);

/**
 * @tableofcontents SECTION_GUI_BRIGHTNESS_SETTER
 * F2 to set display brightness, immediate effect
 * F3 to set LED brightness, immediate effect
 * F4 to enable night shift set menu
 *    - Step 1: Set starting time. F2: digit switch, F3: --, F4: ++. F4Var to next step
 *    - Setp 2: Set ending time.   F2: digit switch, F3: --, F4: ++. F4Var to save
 * F4Var to disable night shift, if enabled
 */

struct GUI_Format_Brightness_Setter {
  time_event_t      message_timer; // Message timer
  uint8_t           message_counter;

  uint8_t           dim_factor_display, dim_factor_led;
  
  uint8_t           night_shift_new_start_h;
  uint8_t           night_shift_new_end_h;
  uint8_t           night_shift_state;
  uint8_t           night_shift_changed;
};

void GUI_Format_Brightness_Setter_Init(struct GUI_Format_Brightness_Setter *self, const uint8_t *shared_initializer);


/**
 * @tableofcontents SECTION_GUI_DIGIT_FADE_SETTER
 * F1 to leave
 * F2 to change digit fade mode
 */

struct GUI_Format_Digit_Fade_Setter {
  time_event_t      message_timer; // Message timer
  uint8_t           message_counter;

  uint8_t           digit_fade_mode;
};

void GUI_Format_Digit_Fade_Setter_Init(struct GUI_Format_Digit_Fade_Setter *self, const uint8_t *shared_initializer);


/** Begin of:
  * @tableofcontents SECTION_CONTAINER_GUI
  * @details The GUI container. C Unions are awesome, the German Union party not so.
  * The container is a single element memory pool consisting of a base member and all used childs
  * with the size of the largest child member. By type-punning them into child types, 
  * static polymorphism is achieved. Use the Clear() function to zero out the GUI Container
**/
typedef union GUI_Format {
  // GUI_Format                   base;
  struct GUI_Format_Time                _gui_time;
  struct GUI_Format_Date                _gui_date;
  struct GUI_Format_Time_Date_Setter    _gui_set;
  struct GUI_Format_Stopwatch           _gui_watch;
  struct GUI_Format_Brightness_Setter   _gui_bset;
} GUI_Format;

/**
  * @brief Zero out the GUI Container
**/
void Container_GUI_Clear(GUI_Format *self);


#endif

#ifdef __cplusplus
}
#endif
