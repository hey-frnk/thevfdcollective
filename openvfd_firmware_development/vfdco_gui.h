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

#ifndef _VFDCO_GUI_H
#define _VFDCO_GUI_H

#include <stdint.h>
#include "vfdco_time.h"
#include "vfdco_hid.h"

struct GUI_Format;
struct GUI_Format_VTable {
  void        (*F2)(struct GUI_Format *self);
  void        (*F3)(struct GUI_Format *self);
  void        (*F4)(struct GUI_Format *self);

  void        (*F2Var)(struct GUI_Format *self);
  void        (*F3Var)(struct GUI_Format *self);
  void        (*F4Var)(struct GUI_Format *self);

  void        (*Update)(struct GUI_Format *self);
  void        (*Delete)(struct GUI_Format *self);
};

struct GUI_Format {
  vfdco_hid_action_status_t        (*F2)(struct GUI_Format *self);
  vfdco_hid_action_status_t        (*F3)(struct GUI_Format *self);
  vfdco_hid_action_status_t        (*F4)(struct GUI_Format *self);

  vfdco_hid_action_status_t        (*F2Var)(struct GUI_Format *self);
  vfdco_hid_action_status_t        (*F3Var)(struct GUI_Format *self);
  vfdco_hid_action_status_t        (*F4Var)(struct GUI_Format *self);

  void        (*Update)(struct GUI_Format *self);
  void        (*Delete)(struct GUI_Format *self);

  struct GUI_Format_VTable VTable;
  time_event_t update_timer;
};

void GUI_Format_Init(struct GUI_Format *self, uint_fast8_t update_timer_interval);

// Time Class
struct GUI_Format_Time {
  struct GUI_Format super;

  // Internal
  uint_fast8_t      dot_mode;
  uint8_t           dot_position;
  uint8_t           dot_direction;
  time_event_t      dot_timer;
};

void GUI_Format_Time_Init(struct GUI_Format_Time *self, uint_fast8_t update_timer_interval, uint8_t dot_mode);

// Date Class
struct GUI_Format_Date {
  struct GUI_Format super;

  uint_fast8_t      date_mode;
};

void GUI_Format_Date_Init(struct GUI_Format_Date *self, uint_fast8_t update_timer_interval, uint_fast8_t date_mode);


// Time Setter
struct GUI_Format_Time_Date_Setter {
  struct GUI_Format super;
  uint_fast8_t      set_mode; // Set date or time?

  uint_fast8_t      blank_active; // Blank period?
  uint_fast8_t      active_digit; // Group with blank digits

  char              *blank_alt_message; // Since render time is bad, use alt renderer

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

// Stopwatch Class
struct GUI_Format_Stopwatch {
  struct GUI_Format super;

  uint_fast8_t      stopwatch_state;


  vfdco_time_t      *initial_time;
  uint32_t          elapsed_time;
  uint32_t          initial_milliseconds;
  uint32_t          elapsed_milliseconds;
};

void GUI_Format_Stopwatch_Init(struct GUI_Format_Stopwatch *self, uint_fast8_t update_timer_interval);


#endif
