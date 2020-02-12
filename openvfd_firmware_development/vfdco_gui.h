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
/**
  * @brief  Virtual table for Light_Pattern
**/
struct GUI_Format;
struct GUI_Format_VTable {
  void        (*F2)(struct GUI_Format *self);
  void        (*F3)(struct GUI_Format *self);
  void        (*F4)(struct GUI_Format *self);

  void        (*F2Var)(struct GUI_Format *self);
  void        (*F3Var)(struct GUI_Format *self);
  void        (*F4Var)(struct GUI_Format *self);

  void        (*Update)(struct GUI_Format *self);
  // void        (*Delete)(struct GUI_Format *self);
};

struct GUI_Format {
  vfdco_hid_action_status_t        (*F2)(struct GUI_Format *self);
  vfdco_hid_action_status_t        (*F3)(struct GUI_Format *self);
  vfdco_hid_action_status_t        (*F4)(struct GUI_Format *self);

  vfdco_hid_action_status_t        (*F2Var)(struct GUI_Format *self);
  vfdco_hid_action_status_t        (*F3Var)(struct GUI_Format *self);
  vfdco_hid_action_status_t        (*F4Var)(struct GUI_Format *self);

  void        (*Update)(struct GUI_Format *self);
  // void        (*Delete)(struct GUI_Format *self);

  struct GUI_Format_VTable VTable;
  time_event_t update_timer;
};
/**
  * @brief  Constructor of GUI_Format class
**/
void GUI_Format_Init(struct GUI_Format *self, uint_fast8_t update_timer_interval);

/** Begin of:
  * @tableofcontents SECTION_GUI_FORMAT_TIME
 **/
struct GUI_Format_Time {
  struct GUI_Format super;
  time_format_t     time_mode;
  uint_fast8_t      dot_mode;
  uint8_t           dot_position;
  uint8_t           dot_direction;
  time_event_t      dot_timer;
};

void GUI_Format_Time_Init(struct GUI_Format_Time *self, uint_fast8_t update_timer_interval, time_format_t time_mode, uint8_t dot_mode);

/** Begin of:
  * @tableofcontents SECTION_GUI_FORMAT_DATE
 **/
struct GUI_Format_Date {
  struct GUI_Format super;
  date_format_t      date_mode;
};

void GUI_Format_Date_Init(struct GUI_Format_Date *self, uint_fast8_t update_timer_interval, date_format_t date_mode);


/** Begin of:
  * @tableofcontents SECTION_GUI_FORMAT_TIME_DATE_SET
 **/
struct GUI_Format_Time_Date_Setter {
  struct GUI_Format super;
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
  struct GUI_Format super;

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
typedef union Container_GUI_t {
  struct GUI_Format                   base;
  struct GUI_Format_Time              _gui_time;
  struct GUI_Format_Date              _gui_date;
  struct GUI_Format_Time_Date_Setter  _gui_set;
  struct GUI_Format_Stopwatch         _gui_watch;
} Container_GUI_t;

/**
  * @brief Zero out the GUI Container
**/
void Container_GUI_Clear(Container_GUI_t *self);


#endif
