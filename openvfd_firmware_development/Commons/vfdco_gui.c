/*
 * Being part of something special makes you special
 *
 * Created winter 2020
 * Author: Copyright (C) The VFD Collective, Frank Zheng
 *
 */

#include <stdlib.h>
#include "../vfdco_gui.h"
#include "../vfdco_hid.h"
#include "../vfdco_display.h"

#ifndef DEBUG
#include "stm32f0xx_hal.h"
#endif

extern vfdco_time_t global_time;
extern vfdco_date_t global_date;

/*extern uint8_t global_button_F1_state;
extern uint8_t global_button_F2_state;
extern uint8_t global_button_F3_state;
extern uint8_t global_button_F4_state;*/

static const uint16_t GUI_Format_Time_Dot_Intervals[4] = {
  800,
  450,
  80,
  800
};

// Virtual function implementation
static inline vfdco_hid_action_status_t _GUI_Format_F2(struct GUI_Format *unsafe_self) { if(!unsafe_self->VTable.F2) return BUTTON_ACTION_NOT_PERFORMED; unsafe_self->VTable.F2(unsafe_self); return BUTTON_ACTION_PERFORMED;}
static inline vfdco_hid_action_status_t _GUI_Format_F3(struct GUI_Format *unsafe_self) { if(!unsafe_self->VTable.F3) return BUTTON_ACTION_NOT_PERFORMED; unsafe_self->VTable.F3(unsafe_self); return BUTTON_ACTION_PERFORMED;}
static inline vfdco_hid_action_status_t _GUI_Format_F4(struct GUI_Format *unsafe_self) { if(!unsafe_self->VTable.F4) return BUTTON_ACTION_NOT_PERFORMED; unsafe_self->VTable.F4(unsafe_self); return BUTTON_ACTION_PERFORMED;}
static inline vfdco_hid_action_status_t _GUI_Format_F2Var(struct GUI_Format *unsafe_self) { if(!unsafe_self->VTable.F2Var) return BUTTON_ACTION_NOT_PERFORMED; unsafe_self->VTable.F2Var(unsafe_self); return BUTTON_ACTION_PERFORMED;}
static inline vfdco_hid_action_status_t _GUI_Format_F3Var(struct GUI_Format *unsafe_self) { if(!unsafe_self->VTable.F3Var) return BUTTON_ACTION_NOT_PERFORMED; unsafe_self->VTable.F3Var(unsafe_self); return BUTTON_ACTION_PERFORMED;}
static inline vfdco_hid_action_status_t _GUI_Format_F4Var(struct GUI_Format *unsafe_self) { if(!unsafe_self->VTable.F4Var) return BUTTON_ACTION_NOT_PERFORMED; unsafe_self->VTable.F4Var(unsafe_self); return BUTTON_ACTION_PERFORMED;}
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
      if      (self->dot_position == 0) vfdco_display_render_time(&global_time, 0b00010100, self->time_mode);
      else if (self->dot_position == 1) vfdco_display_render_time(&global_time, 0b00000000, self->time_mode);
      else    self->dot_position = 0; // Reset
    }
    else if(self->dot_mode == 1) { // Converge
      switch (self->dot_position) {
        case 0: vfdco_display_render_time(&global_time, 0b00100001, self->time_mode); break;
        case 1: vfdco_display_render_time(&global_time, 0b00010010, self->time_mode); break;
        case 2: vfdco_display_render_time(&global_time, 0b00001100, self->time_mode); break;
        case 3: vfdco_display_render_time(&global_time, 0b00010010, self->time_mode); break;
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
        if(self->dot_position < 5) vfdco_display_render_time(&global_time, 1 << self->dot_position, self->time_mode);
        else                       vfdco_display_render_time(&global_time, 0b00100000, self->time_mode); // Leftmost static
      } else {
        if(self->dot_position < 5) vfdco_display_render_time(&global_time, 0b00100000 >> self->dot_position, self->time_mode);
        else                       vfdco_display_render_time(&global_time, 0b00000001, self->time_mode); // Rightmost static
      }
    }
    else if(self->dot_mode == 3) {
      vfdco_display_render_time(&global_time, 0, self->time_mode);
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

static void _GUI_Format_Time_F4Var(struct GUI_Format *unsafe_self) {
  struct GUI_Format_Time *self = (struct GUI_Format_Time *)unsafe_self;

  if(self->time_mode == TIME_FORMAT_24H) {
    self->time_mode = TIME_FORMAT_12H;
    char _message[GUI_MESSAGE_LENGTH] = {1, 2, 'H', ' ', ' ', ' '};
    vfdco_display_render_message(_message, 0, GUI_MESSAGE_LONG);
  }
  else if(self->time_mode == TIME_FORMAT_12H) {
    self->time_mode = TIME_FORMAT_12H_NO_LZ;
    char _message[GUI_MESSAGE_LENGTH] = {1, 2, 'H', ' ', 'N', 0};
    vfdco_display_render_message(_message, 0, GUI_MESSAGE_LONG);
  }
  else {
    self->time_mode = TIME_FORMAT_24H;
    char _message[GUI_MESSAGE_LENGTH] = {2, 4, 'H', ' ', ' ', ' '};
    vfdco_display_render_message(_message, 0, GUI_MESSAGE_LONG);
  }
}

void _GUI_Format_Time_Delete(struct GUI_Format *unsafe_self) {
  struct GUI_Format_Time *self = (struct GUI_Format_Time *)unsafe_self;
  free(self);
}

void GUI_Format_Time_Init(struct GUI_Format_Time *self, uint_fast8_t update_timer_interval, time_format_t time_mode, uint8_t dot_mode) {
  GUI_Format_Init(&self->super, update_timer_interval);

  self->time_mode = time_mode;

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
    .F4Var = _GUI_Format_Time_F4Var,
    .Update = _GUI_Format_Time_Update,
    .Delete = _GUI_Format_Time_Delete
  };
  self->super.VTable = _time_vtable;
}


// Date
static void _GUI_Format_Date_Update(struct GUI_Format *unsafe_self) {
  struct GUI_Format_Date *self = (struct GUI_Format_Date *)unsafe_self;
  if(Time_Event_Update(&unsafe_self->update_timer)) {
    vfdco_display_render_date(&global_date, 0b00010100, self->date_mode);
  }
}

static void _GUI_Format_Date_F4Var(struct GUI_Format *unsafe_self) {
  struct GUI_Format_Date *self = (struct GUI_Format_Date *)unsafe_self;

  if(self->date_mode == DATE_FORMAT_DDMMYY) {
    self->date_mode = DATE_FORMAT_MMDDYY;
    char _message[GUI_MESSAGE_LENGTH] = {'D', ' ', ' ', 'G', 'E', 'R'};
    vfdco_display_render_message(_message, 0, GUI_MESSAGE_LONG);
  }
  else {
    self->date_mode = DATE_FORMAT_DDMMYY;
    char _message[GUI_MESSAGE_LENGTH] = {'D', ' ', 'I', 'N', 'T', 'L'};
    vfdco_display_render_message(_message, 0, GUI_MESSAGE_LONG);
  }
}

void _GUI_Format_Date_Delete(struct GUI_Format *unsafe_self) {
  struct GUI_Format_Date *self = (struct GUI_Format_Date *)unsafe_self;
  free(self);
}

void GUI_Format_Date_Init(struct GUI_Format_Date *self, uint_fast8_t update_timer_interval, date_format_t date_mode) {
  GUI_Format_Init(&self->super, update_timer_interval);

  self->date_mode = date_mode;

  struct GUI_Format_VTable _date_vtable = {
    .F2 = NULL,
    .F3 = NULL,
    .F4 = NULL,
    .F2Var = NULL,
    .F3Var = NULL,
    .F4Var = _GUI_Format_Date_F4Var,
    .Update = _GUI_Format_Date_Update,
    .Delete = _GUI_Format_Date_Delete
  };
  self->super.VTable = _date_vtable;
}




// Time_Set
static void _GUI_Format_Time_Date_Setter_Update(struct GUI_Format *unsafe_self) {
  struct GUI_Format_Time_Date_Setter *self = (struct GUI_Format_Time_Date_Setter *)unsafe_self;

  if(Time_Event_Update(&self->blank_timer)) self->blank_active = !self->blank_active;

  if(self->set_mode == 0) {
    self->blank_alt_message[0] = self->new_time.h / 10;
    self->blank_alt_message[1] = self->new_time.h % 10;
    self->blank_alt_message[2] = self->new_time.m / 10;
    self->blank_alt_message[3] = self->new_time.m % 10;
    self->blank_alt_message[4] = self->new_time.s / 10;
    self->blank_alt_message[5] = self->new_time.s % 10;
  } else {
    self->blank_alt_message[0] = self->new_date.d / 10;
    self->blank_alt_message[1] = self->new_date.d % 10;
    self->blank_alt_message[2] = self->new_date.m / 10;
    self->blank_alt_message[3] = self->new_date.m % 10;
    self->blank_alt_message[4] = self->new_date.y / 10;
    self->blank_alt_message[5] = self->new_date.y % 10;
  }

  if(self->blank_active) {
    if(self->active_digit == 0) {
      self->blank_alt_message[0] = ' ';
      self->blank_alt_message[1] = ' ';
    } else if(self->active_digit == 1) {
      self->blank_alt_message[2] = ' ';
      self->blank_alt_message[3] = ' ';
    } else if(self->active_digit == 2) {
      self->blank_alt_message[4] = ' ';
      self->blank_alt_message[5] = ' ';
    }
  }
  vfdco_display_render_message(self->blank_alt_message, 0, 0);
}


static void _GUI_Format_Time_Date_Setter_F2(struct GUI_Format *unsafe_self) {
  struct GUI_Format_Time_Date_Setter *self = (struct GUI_Format_Time_Date_Setter *)unsafe_self;

  // Short press on F2 changes the active parameter (h/m/s)
  ++self->active_digit;
  if(self->active_digit == 3) self->active_digit = 0;
}

static void _GUI_Format_Time_Date_Setter_F3(struct GUI_Format *unsafe_self) {
  struct GUI_Format_Time_Date_Setter *self = (struct GUI_Format_Time_Date_Setter *)unsafe_self;

  // Short press on F3 decreases the active parameter (h/m/s)
  if(self->set_mode == 0) { // Time Set
    if      (self->active_digit == 0) { // Set hour
      if       (self->new_time.h  > 0)    self->new_time.h--;
      else if  (self->new_time.h == 0)    self->new_time.h = 23;
    }
    else if (self->active_digit == 1) { // Set minute
      if       (self->new_time.m  > 0)    self->new_time.m--;
      else if  (self->new_time.m == 0)    self->new_time.m = 59;
    }
    else if (self->active_digit == 2) { // Set second
      if       (self->new_time.s  > 0)    self->new_time.s--;
      else if  (self->new_time.s == 0)    self->new_time.s = 59;
    }
  } else { // Date Set
    if      (self->active_digit == 0) { // Set date
      uint8_t number_of_days = 31;
      // Leap year detection
      if(self->new_date.m == 2) number_of_days = ((self->new_date.y % 400 == 0) || ((self->new_date.y % 4 == 0) && (self->new_date.y % 100 != 0))) ? 29 : 28;
      // Short month detection
      else if((self->new_date.m == 4) || (self->new_date.m == 6) || (self->new_date.m == 9) || (self->new_date.m == 11)) number_of_days = 30;
      if       (self->new_date.d  > 1)    self->new_date.d--;
      else if  (self->new_date.d == 1)    self->new_date.d = number_of_days;
    }
    else if (self->active_digit == 1) { // Set month
      if       (self->new_date.m  > 1)    self->new_date.m--;
      else if  (self->new_date.m == 1)    self->new_date.m = 12;
    }
    else if (self->active_digit == 2) { // Set year
      if       (self->new_date.y  > 0)    self->new_date.y--;
      else if  (self->new_date.y == 0)    self->new_date.y = 40;
    }
  }
}

static void _GUI_Format_Time_Date_Setter_F4(struct GUI_Format *unsafe_self) {
  struct GUI_Format_Time_Date_Setter *self = (struct GUI_Format_Time_Date_Setter *)unsafe_self;

  // Short press on F4 increases the active parameter (h/m/s)
  if(self->set_mode == 0) { // Time Set
    if      (self->active_digit == 0) { // Set hour
      if       (self->new_time.h  < 23)    self->new_time.h++;
      else if  (self->new_time.h == 23)    self->new_time.h = 0;
    }
    else if (self->active_digit == 1) { // Set minute
      if       (self->new_time.m  < 59)    self->new_time.m++;
      else if  (self->new_time.m == 59)    self->new_time.m = 0;
    }
    else if (self->active_digit == 2) { // Set second
      if       (self->new_time.s  < 59)    self->new_time.s++;
      else if  (self->new_time.s == 59)    self->new_time.s = 0;
    }
  } else { // Date Set
    if      (self->active_digit == 0) { // Set date
      uint8_t number_of_days = 31;
      // Leap year detection
      if(self->new_date.m == 2) number_of_days = ((self->new_date.y % 400 == 0) || ((self->new_date.y % 4 == 0) && (self->new_date.y % 100 != 0))) ? 29 : 28;
      // Short month detection
      else if((self->new_date.m == 4) || (self->new_date.m == 6) || (self->new_date.m == 9) || (self->new_date.m == 11)) number_of_days = 30;
      if       (self->new_date.d  <  number_of_days)    self->new_date.d++;
      else if  (self->new_date.d ==  number_of_days)    self->new_date.d = 1;
    }
    else if (self->active_digit == 1) { // Set month
      if       (self->new_date.m  < 12)    self->new_date.m++;
      else if  (self->new_date.m == 12)    self->new_date.m = 1;
    }
    else if (self->active_digit == 2) { // Set year
      if       (self->new_date.y  < 40)    self->new_date.y++;
      else if  (self->new_date.y == 40)    self->new_date.y = 0;
    }
  }
}

void _GUI_Format_Time_Date_Setter_Delete(struct GUI_Format *unsafe_self) {
  struct GUI_Format_Time_Date_Setter *self = (struct GUI_Format_Time_Date_Setter *)unsafe_self;
  // Set time before we leave
  if(self->set_mode == 0) vfdco_set_date_time(&global_date,    &self->new_time);
  else                    vfdco_set_date_time(&self->new_date, &global_time   );

  free(self->blank_alt_message);
  free(self);
}

void GUI_Format_Time_Date_Setter_Init(struct GUI_Format_Time_Date_Setter *self, uint_fast8_t update_timer_interval, uint_fast8_t set_mode) {
  GUI_Format_Init(&self->super, update_timer_interval);

  self->set_mode = set_mode;

  self->blank_active = 0;
  self->active_digit = 0;

  self->blank_alt_message = calloc(GUI_MESSAGE_LENGTH, sizeof(char));
  self->blank_timer = Time_Event_Init(500);

  self->new_time = global_time;
  self->new_date = global_date;

  struct GUI_Format_VTable _time_date_setter_vtable = {
    .F2 = _GUI_Format_Time_Date_Setter_F2, // sw active
    .F3 = _GUI_Format_Time_Date_Setter_F3, // --
    .F4 = _GUI_Format_Time_Date_Setter_F4, // ++
    .F2Var = NULL,
    .F3Var = NULL,
    .F4Var = NULL,
    .Update = _GUI_Format_Time_Date_Setter_Update,
    .Delete = _GUI_Format_Time_Date_Setter_Delete
  };
  self->super.VTable = _time_date_setter_vtable;
}



// Stopwatch
static void _GUI_Format_Stopwatch_Update(struct GUI_Format *unsafe_self) {
  struct GUI_Format_Stopwatch *self = (struct GUI_Format_Stopwatch *)unsafe_self;

  if(Time_Event_Update(&unsafe_self->update_timer)) {
    if(self->stopwatch_state == GUI_FORMAT_STOPWATCH_STATE_INITIALIZED) {
      char zeros[GUI_MESSAGE_LENGTH] = {0};
      vfdco_display_render_message(zeros, 0b000010100, 0);
    }

    else if(self->stopwatch_state == GUI_FORMAT_STOPWATCH_STATE_RUNNING) {
      // running time: elapsed base + current time - start timestamp
      uint32_t running_time = self->elapsed_time +
        (        global_time.h * 3600 +         global_time.m * 60 +         global_time.s) -
        (self->initial_time->h * 3600 + self->initial_time->m * 60 + self->initial_time->s);

      if(running_time > 3599) {
        vfdco_time_t new_time = {
          .h = running_time / 3600 % 24,
          .m = running_time / 60 % 60,
          .s = running_time % 60
        };
        vfdco_display_render_time(&new_time, 0, TIME_FORMAT_24H);
      } else {
        char digits[GUI_MESSAGE_LENGTH];
        uint8_t running_millis = ((self->elapsed_milliseconds +
          vfdco_time_get_milliseconds() -
          self->initial_milliseconds) % 1000) / 10;
        digits[0] = (running_time / 60 % 60) / 10;
        digits[1] = (running_time / 60 % 60) % 10;
        digits[2] = (running_time % 60) / 10;
        digits[3] = (running_time % 60) % 10;
        digits[4] = running_millis / 10;
        digits[5] = running_millis % 10;
        vfdco_display_render_message(digits, 0, 0);
      }
    }

    else if(self->stopwatch_state == GUI_FORMAT_STOPWATCH_STATE_PAUSED) {
      // running time: elapsed base
      uint32_t elapsed_time = self->elapsed_time;

      if(elapsed_time > 3599) {
        vfdco_time_t new_time = {
          .h = elapsed_time / 3600 % 24,
          .m = elapsed_time / 60 % 60,
          .s = elapsed_time % 60
        };

        vfdco_display_render_time(&new_time, 0, TIME_FORMAT_24H);
      } else {
        char digits[GUI_MESSAGE_LENGTH];
        uint8_t running_millis = (self->elapsed_milliseconds % 1000) / 10;
        digits[0] = (elapsed_time / 60 % 60) / 10;
        digits[1] = (elapsed_time / 60 % 60) % 10;
        digits[2] = (elapsed_time % 60) / 10;
        digits[3] = (elapsed_time % 60) % 10;
        digits[4] = running_millis / 10;
        digits[5] = running_millis % 10;

        vfdco_display_render_message(digits, 0, 0);
      }
    }
  }
}

static void _GUI_Format_Stopwatch_F2(struct GUI_Format *unsafe_self) {
  struct GUI_Format_Stopwatch *self = (struct GUI_Format_Stopwatch *)unsafe_self;

  // Initialize
  if(self->stopwatch_state == GUI_FORMAT_STOPWATCH_STATE_INITIALIZED) {
    if(!self->initial_time) {
      self->initial_time = (vfdco_time_t *)malloc(sizeof(vfdco_time_t));
      self->initial_time->h = global_time.h;
      self->initial_time->m = global_time.m;
      self->initial_time->s = global_time.s;
    }
    #ifndef DEBUG // Needs platform independency
    self->initial_milliseconds = vfdco_time_get_milliseconds();
    #endif

    self->elapsed_time = 0;
    self->elapsed_milliseconds = 0;

    self->stopwatch_state = GUI_FORMAT_STOPWATCH_STATE_RUNNING;
  }

  // Reset
  else if(self->stopwatch_state == GUI_FORMAT_STOPWATCH_STATE_PAUSED) {
    free(self->initial_time);
    self->initial_time = NULL;
    self->elapsed_time = 0;
    self->initial_milliseconds = 0;
    self->elapsed_milliseconds = 0;

    self->stopwatch_state = GUI_FORMAT_STOPWATCH_STATE_INITIALIZED;
  }
}

static void _GUI_Format_Stopwatch_F3(struct GUI_Format *unsafe_self) {
  struct GUI_Format_Stopwatch *self = (struct GUI_Format_Stopwatch *)unsafe_self;

  // Pause
  if(self->stopwatch_state == GUI_FORMAT_STOPWATCH_STATE_RUNNING) {
    // Save seconds
    self->elapsed_time +=
      (        global_time.h * 3600 +         global_time.m * 60 +         global_time.s) -
      (self->initial_time->h * 3600 + self->initial_time->m * 60 + self->initial_time->s);

    // Save milliseconds
    self->elapsed_milliseconds += vfdco_time_get_milliseconds() - self->initial_milliseconds;
    self->stopwatch_state = GUI_FORMAT_STOPWATCH_STATE_PAUSED;

  }

  // Resume
  else if(self->stopwatch_state == GUI_FORMAT_STOPWATCH_STATE_PAUSED) {
    self->initial_time->h = global_time.h;
    self->initial_time->m = global_time.m;
    self->initial_time->s = global_time.s;
    #ifndef DEBUG
    self->initial_milliseconds = vfdco_time_get_milliseconds();
    #endif
    self->stopwatch_state = GUI_FORMAT_STOPWATCH_STATE_RUNNING;
  }
}

void _GUI_Format_Stopwatch_Delete(struct GUI_Format *unsafe_self) {
  struct GUI_Format_Stopwatch *self = (struct GUI_Format_Stopwatch *)unsafe_self;
  free(self);
}

void GUI_Format_Stopwatch_Init(struct GUI_Format_Stopwatch *self, uint_fast8_t update_timer_interval) {
  GUI_Format_Init(&self->super, update_timer_interval);

  self->stopwatch_state = GUI_FORMAT_STOPWATCH_STATE_INITIALIZED;

  self->initial_time = NULL;
  self->elapsed_time = 0;
  self->initial_milliseconds = 0;
  self->elapsed_milliseconds = 0;

  struct GUI_Format_VTable _stopwatch_vtable = {
    .F2 = _GUI_Format_Stopwatch_F2,
    .F3 = _GUI_Format_Stopwatch_F3,
    .F4 = NULL,
    .F2Var = NULL,
    .F3Var = NULL,
    .F4Var = NULL,
    .Update = _GUI_Format_Stopwatch_Update,
    .Delete = _GUI_Format_Stopwatch_Delete
  };
  self->super.VTable = _stopwatch_vtable;
}




// Rip a line from a page watch it fade up in smoke hope rise through the rain higher than we'd ever go
