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
  void        (*F2)(struct GUI_Format *self);
  void        (*F3)(struct GUI_Format *self);
  void        (*F4)(struct GUI_Format *self);

  void        (*F2Var)(struct GUI_Format *self);
  void        (*F3Var)(struct GUI_Format *self);
  void        (*F4Var)(struct GUI_Format *self);

  void        (*Update)(struct GUI_Format *self);
  void        (*Delete)(struct GUI_Format *self);

  struct GUI_Format_VTable VTable;
  time_event_t update_timer;
};

void GUI_Format_Init(struct GUI_Format *self, uint_fast8_t update_timer_interval);


struct GUI_Format_Time {
  struct GUI_Format super;

  // Internal
  uint_fast8_t      dot_mode;
  uint8_t           dot_position;
  uint8_t           dot_direction;
  time_event_t      dot_timer;

  uint_fast8_t      pwm_enabled;
  uint_fast8_t      pwm_state;
  vfdco_time_t
};

void GUI_Format_Time_Init(struct GUI_Format_Time *self, uint_fast8_t update_timer_interval, uint8_t dot_mode);


#endif
