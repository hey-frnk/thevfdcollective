/*
 * Being part of something special makes you special
 *
 * Created winter 2020
 * Author: Copyright (C) The VFD Collective, Frank Zheng
 *
 */

#include <stdlib.h>
#include "../vfdco_gui.h"
#include "../vfdco_display.h"

extern vfdco_time_t global_time;
extern vfdco_date_t global_date;

static const uint16_t GUI_Format_Time_Dot_Intervals[4] = {
  800,
  450,
  80,
  800
};

// Virtual function implementation
static inline void _GUI_Format_F2(struct GUI_Format *unsafe_self) { if(!unsafe_self->VTable.F2) return; unsafe_self->VTable.F2(unsafe_self); }
static inline void _GUI_Format_F3(struct GUI_Format *unsafe_self) { if(!unsafe_self->VTable.F3) return; unsafe_self->VTable.F3(unsafe_self); }
static inline void _GUI_Format_F4(struct GUI_Format *unsafe_self) { if(!unsafe_self->VTable.F4) return; unsafe_self->VTable.F4(unsafe_self); }
static inline void _GUI_Format_F2Var(struct GUI_Format *unsafe_self) { if(!unsafe_self->VTable.F2Var) return; unsafe_self->VTable.F2Var(unsafe_self); }
static inline void _GUI_Format_F3Var(struct GUI_Format *unsafe_self) { if(!unsafe_self->VTable.F3Var) return; unsafe_self->VTable.F3Var(unsafe_self); }
static inline void _GUI_Format_F4Var(struct GUI_Format *unsafe_self) { if(!unsafe_self->VTable.F4Var) return; unsafe_self->VTable.F4Var(unsafe_self); }
static inline void _GUI_Format_Update(struct GUI_Format *unsafe_self) { unsafe_self->VTable.Update(unsafe_self); }
static inline void _GUI_Format_Delete(struct GUI_Format *unsafe_self) { unsafe_self->VTable.Delete(unsafe_self); }

void GUI_Format_Init(struct GUI_Format *self, uint_fast8_t update_timer_interval) {
  self->F2 = _GUI_Format_F2;
  self->F3 = _GUI_Format_F3;
  self->F4 = _GUI_Format_F4;
  self->F2Var = _GUI_Format_F2Var;
  self->F3Var = _GUI_Format_F3Var;
  self->F4Var = _GUI_Format_F4Var;
  self->Update = _GUI_Format_Update;
  self->Delete = _GUI_Format_Delete;

  self->update_timer = Time_Event_Init(update_timer_interval);
}

// Time
static void _GUI_Format_Time_Update(struct GUI_Format *unsafe_self) {
  struct GUI_Format_Time *self = (struct GUI_Format_Time *)unsafe_self;

  if(Time_Event_Update(&self->dot_timer)) self->dot_position++;

  if(Time_Event_Update(&unsafe_self->update_timer)) {
    if(self->dot_mode == 0) { // Standard
      if      (self->dot_position == 0) vfdco_display_render_time(&global_time, 0b00010100);
      else if (self->dot_position == 1) vfdco_display_render_time(&global_time, 0b00000000);
      else    self->dot_position = 0; // Reset
    }
    else if(self->dot_mode == 1) { // Converge
      switch (self->dot_position) {
        case 0: vfdco_display_render_time(&global_time, 0b00100001); break;
        case 1: vfdco_display_render_time(&global_time, 0b00010010); break;
        case 2: vfdco_display_render_time(&global_time, 0b00001100); break;
        case 3: vfdco_display_render_time(&global_time, 0b00010010); break;
        default: self->dot_position = 0;
      }
    }
    else if(self->dot_mode == 2) { // Flip
      // This function is damn lit. Once it detects a change in second,
      // the decimal dot will slide over the displays.
      // Get the current time and compare it with the previous timestamp
      if(self->dot_direction != global_time.s) {
        self->dot_position = 0;
        self->dot_direction = global_time.s;
      }

      if(self->dot_direction & 0x01) { // odd: r->l
        if(self->dot_position < 5) vfdco_display_render_time(&global_time, 1 << self->dot_position);
        else                       vfdco_display_render_time(&global_time, 0b00100000); // Leftmost static
      } else {
        if(self->dot_position < 5) vfdco_display_render_time(&global_time, 0b00100000 >> self->dot_position);
        else                       vfdco_display_render_time(&global_time, 0b00000001); // Rightmost static
      }
    }
    else if(self->dot_mode == 3) {
      vfdco_display_render_time(&global_time, 0);
    }
  }
}

static void _GUI_Format_Time_F4(struct GUI_Format *unsafe_self) {
  struct GUI_Format_Time *self = (struct GUI_Format_Time *)unsafe_self;

  // Change dot mode
  ++self->dot_mode;
  if(self->dot_mode > 3) self->dot_mode = 0;

  // Reset parameters
  self->dot_position = 0;
  self->dot_timer = Time_Event_Init(GUI_Format_Time_Dot_Intervals[self->dot_mode]);
}

void _GUI_Format_Time_Delete(struct GUI_Format *unsafe_self) {
  struct GUI_Format_Time *self = (struct GUI_Format_Time *)unsafe_self;
  free(self);
}

void GUI_Format_Time_Init(struct GUI_Format_Time *self, uint_fast8_t update_timer_interval, uint8_t dot_mode) {
  GUI_Format_Init(&self->super, update_timer_interval);

  self->dot_mode = dot_mode;
  self->dot_position = 0;
  self->dot_direction = 0;
  self->dot_timer = Time_Event_Init(GUI_Format_Time_Dot_Intervals[dot_mode]);

  struct GUI_Format_VTable _time_vtable = {
    .F2 = NULL,
    .F3 = NULL,
    .F4 = _GUI_Format_Time_F4,
    .F2Var = NULL,
    .F3Var = NULL,
    .F4Var = NULL,
    .Update = _GUI_Format_Time_Update,
    .Delete = _GUI_Format_Time_Delete
  };
  self->super.VTable = _time_vtable;
}







// Rip a line from a page watch it fade up in smoke hope rise through the rain higher than we'd ever go
