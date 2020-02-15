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
void         (*GUI_Format_F2)(GUI_Format *unsafe_self);
void         (*GUI_Format_F3)(GUI_Format *unsafe_self);
void         (*GUI_Format_F4)(GUI_Format *unsafe_self);
void         (*GUI_Format_F2Var)(GUI_Format *unsafe_self);
void         (*GUI_Format_F3Var)(GUI_Format *unsafe_self);
void         (*GUI_Format_F4Var)(GUI_Format *unsafe_self);
void         (*GUI_Format_Update)(GUI_Format *unsafe_self);
void         (*GUI_Format_Save)(GUI_Format *unsafe_self);

/**
  * @brief  Constructor of GUI_Format class
**/
// void GUI_Format_Init(GUI_Format *self, uint_fast8_t update_timer_interval);

/** Begin of:
  * @tableofcontents SECTION_GUI_FORMAT_TIME
 **/
struct GUI_Format_Time {
  uint8_t           *settings;
  time_event_t      update_timer;
  time_format_t     time_mode;
  uint_fast8_t      dot_mode;
  uint8_t           dot_position;
  uint8_t           dot_direction;
  time_event_t      dot_timer;
};

void GUI_Format_Time_Init(struct GUI_Format_Time *self, uint_fast8_t update_timer_interval, uint8_t *settings);
void GUI_Format_Time_Default(uint8_t *settings);

/** Begin of:
  * @tableofcontents SECTION_GUI_FORMAT_DATE
 **/
struct GUI_Format_Date {
  uint8_t            *settings;
  time_event_t       update_timer;
  date_format_t      date_mode;
};

void GUI_Format_Date_Init(struct GUI_Format_Date *self, uint_fast8_t update_timer_interval, uint8_t *settings);
void GUI_Format_Date_Default(uint8_t *settings);


/** Begin of:
  * @tableofcontents SECTION_GUI_FORMAT_TIME_DATE_SET
 **/
struct GUI_Format_Time_Date_Setter {
  time_event_t      update_timer;
  uint_fast8_t      set_mode; // Set date or time?
  uint_fast8_t      active_digit; // Group with blank digits

  uint_fast8_t      blank_active; // Blank period?
  char              blank_alt_message[CONFIG_NUM_DIGITS]; // Since render time is bad, use alt renderer
  time_event_t      blank_timer; // Blank timer

  vfdco_time_t      new_time;
  vfdco_date_t      new_date;
};

void GUI_Format_Time_Date_Setter_Init(struct GUI_Format_Time_Date_Setter *self, uint_fast8_t update_timer_interval, uint_fast8_t set_mode);


enum {
  GUI_FORMAT_STOPWATCH_STATE_INITIALIZED,
  GUI_FORMAT_STOPWATCH_STATE_RUNNING,
  GUI_FORMAT_STOPWATCH_STATE_PAUSED
};

/** Begin of:
  * @tableofcontents SECTION_GUI_FORMAT_STOPWATCH
 **/
struct GUI_Format_Stopwatch {
  time_event_t      update_timer;

  uint_fast8_t      stopwatch_state;

  vfdco_time_t      initial_time;
  uint8_t           not_initial;
  uint32_t          elapsed_time;
  uint32_t          initial_milliseconds;
  uint32_t          elapsed_milliseconds;
};

void GUI_Format_Stopwatch_Init(struct GUI_Format_Stopwatch *self, uint_fast8_t update_timer_interval);


/** Begin of:
  * @tableofcontents SECTION_CONTAINER_GUI
  * @details The GUI container. C Unions are awesome, the German Union party not so.
  * The container is a single element memory pool consisting of a base member and all used childs
  * with the size of the largest child member. By type-punning them into child types, 
  * static polymorphism is achieved. Use the Clear() function to zero out the GUI Container
**/
typedef union GUI_Format {
  // GUI_Format                   base;
  struct GUI_Format_Time              _gui_time;
  struct GUI_Format_Date              _gui_date;
  struct GUI_Format_Time_Date_Setter  _gui_set;
  struct GUI_Format_Stopwatch         _gui_watch;
} GUI_Format;

/**
  * @brief Zero out the GUI Container
**/
void Container_GUI_Clear(GUI_Format *self);


#endif
