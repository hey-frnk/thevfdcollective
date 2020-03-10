/**
  ******************************************************************************
  * @file     vfdco_gui.c
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     12-February-2020
  * @brief    This file contains definitions for the GUI
  *           Designed to be used with Fluorescence by The VFD Collective
  ******************************************************************************
  * @tableofcontents Table of contents, enter to navigate:
  * SECTION_GUI_FORMAT_CONSTANTS
  * SECTION_GUI_FORMAT
  * SECTION_GUI_FORMAT_TIME
  * SECTION_GUI_FORMAT_DATE
  * SECTION_GUI_FORMAT_TIME_DATE_SET
  * SECTION_GUI_FORMAT_STOPWATCH
  * SECTION_CONTAINER_GUI
  ******************************************************************************
 **/

#include <stdlib.h>
#include <string.h>
#include "../vfdco_config.h"
#include "../vfdco_gui.h"
#include "../vfdco_hid.h"
#include "../vfdco_display.h"

// vfdco_clock_routines.c dependence. Baad thing, but...
extern vfdco_time_t global_time;
extern vfdco_date_t global_date;


/** Begin of:
 * @tableofcontents SECTION_GUI_FORMAT_CONSTANTS
 * @brief Definition of all constants used in the GUI. Customizable constants are defined in vfdco_config.h
**/
static const uint16_t GUI_Format_Time_Dot_Intervals[4] = {
  800,
  450,
  80,
  800
};

#define _SHARED_dim_factor_display              0
#define _SHARED_dim_factor_led                  1
#define _SHARED_dim_factor_night_shift_start_h  2
#define _SHARED_dim_factor_night_shift_start_m  3
#define _SHARED_dim_factor_night_shift_end_h    4
#define _SHARED_dim_factor_night_shift_end_m    5


#define MESSAGES_BRIGHTNESS_SET_IDLE_MAX 11
#define MESSAGES_BRIGHTNESS_SET_IDLE_REPEAT 2
#define MESSAGES_BRIGHTNESS_SET_CONTINUE 4
static const char Messages_Brightness_Set_Idle[MESSAGES_BRIGHTNESS_SET_IDLE_MAX - MESSAGES_BRIGHTNESS_SET_IDLE_REPEAT][CONFIG_NUM_DIGITS] = {
  {'B', 'R', 'I', 'S', 'E', 'T'}, // 0
  {'F',  2 , ' ', 'V', 'F', 'D'}, // 1
  {'F',  3 , ' ', 'L', 'E', 'D'}, // 2
  {'F',  4 , ' ', 'S', 'E', 'T'}, // 3
  {' ', 'N', 'I', 'G', 'H', 'T'}, // 4, 9
  {' ', 'S', 'H', 'I', 'F', 'T'}, // 5, 10
  {'F',  4 , ' ', ' ', ' ', ' '}, // 6
  {' ', ' ', 'L', 'O', 'N', 'G'}, // 7
  {'D', 'I', 'S', 'A', 'B', 'L'}, // 8
};
static const uint8_t Messages_Brightness_Set_Idle_Dots[MESSAGES_BRIGHTNESS_SET_IDLE_MAX - MESSAGES_BRIGHTNESS_SET_IDLE_REPEAT] = {
  0b00000100, 0b00001000, 0b00001000, 0b00001000, 0, 0, 0b00001000, 0, 0
};
static const char Messages_Brightness_Set_Continue[MESSAGES_BRIGHTNESS_SET_CONTINUE][2] = {
  {'F',  4 },
  {'T', 'O'},
  {'S', 'A'},
  {'V', 'E'}
};

/** Begin of:
 * @tableofcontents SECTION_GUI_FORMAT
**/

/** Begin of:
  * @tableofcontents SECTION_GUI_FORMAT_TIME
 **/
void _GUI_Format_Time_Update(GUI_Format *unsafe_self) {
  struct GUI_Format_Time *self = (struct GUI_Format_Time *)unsafe_self;

  if(Time_Event_Update(&self->dot_timer)) self->dot_position++;
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

void _GUI_Format_Time_F4(GUI_Format *unsafe_self) {
  struct GUI_Format_Time *self = (struct GUI_Format_Time *)unsafe_self;

  // Change dot mode
  ++self->dot_mode;
  if(self->dot_mode > 3) self->dot_mode = 0;

  // Reset parameters
  self->dot_position = 0;
  self->dot_timer = Time_Event_Init(GUI_Format_Time_Dot_Intervals[self->dot_mode]);
}

void _GUI_Format_Time_F4Var(GUI_Format *unsafe_self) {
  struct GUI_Format_Time *self = (struct GUI_Format_Time *)unsafe_self;

  if(self->time_mode == TIME_FORMAT_24H) {
    self->time_mode = TIME_FORMAT_12H;
    char _message[CONFIG_NUM_DIGITS] = {1, 2, 'H', ' ', ' ', ' '};
    vfdco_display_render_message(_message, 0, CONFIG_MESSAGE_LONG);
  }
  else if(self->time_mode == TIME_FORMAT_12H) {
    self->time_mode = TIME_FORMAT_12H_NO_LZ;
    char _message[CONFIG_NUM_DIGITS] = {1, 2, 'H', ' ', 'N', 0};
    vfdco_display_render_message(_message, 0, CONFIG_MESSAGE_LONG);
  }
  else {
    self->time_mode = TIME_FORMAT_24H;
    char _message[CONFIG_NUM_DIGITS] = {2, 4, 'H', ' ', ' ', ' '};
    vfdco_display_render_message(_message, 0, CONFIG_MESSAGE_LONG);
  }
}

void _GUI_Format_Time_Save(GUI_Format *unsafe_self) {
  struct GUI_Format_Time *self = (struct GUI_Format_Time *)unsafe_self;
  self->settings[GUI_FORMAT_SETTING_TIME_time_mode] = self->time_mode;
  self->settings[GUI_FORMAT_SETTING_TIME_dot_mode] = self->dot_mode;
}

void GUI_Format_Time_Init(struct GUI_Format_Time *self, uint8_t *settings) {

  // Default loading if saved value is litter, then load by assignment
  time_format_t _chk_time_mode = settings[GUI_FORMAT_SETTING_TIME_time_mode];
  if(((_chk_time_mode != TIME_FORMAT_12H) && (_chk_time_mode != TIME_FORMAT_24H) && (_chk_time_mode != TIME_FORMAT_12H_NO_LZ))
      || (settings[GUI_FORMAT_SETTING_TIME_dot_mode] > 3)) GUI_Format_Time_Default(settings);
  self->time_mode = settings[GUI_FORMAT_SETTING_TIME_time_mode];
  self->dot_mode = settings[GUI_FORMAT_SETTING_TIME_dot_mode];

  self->dot_position = 0;
  self->dot_direction = 0;
  self->dot_timer = Time_Event_Init(GUI_Format_Time_Dot_Intervals[self->dot_mode]);
  self->settings = settings;

  GUI_Format_F2 = NULL;
  GUI_Format_F3 = NULL;
  GUI_Format_F4 = _GUI_Format_Time_F4;
  GUI_Format_F2Var = NULL;
  GUI_Format_F3Var = NULL;
  GUI_Format_F4Var = _GUI_Format_Time_F4Var;
  GUI_Format_Update = _GUI_Format_Time_Update;
  GUI_Format_Save = _GUI_Format_Time_Save;
}
void GUI_Format_Time_Default(uint8_t *settings) {
  settings[GUI_FORMAT_SETTING_TIME_time_mode] = TIME_FORMAT_24H;
  settings[GUI_FORMAT_SETTING_TIME_dot_mode] = 0;
}


/** Begin of:
  * @tableofcontents SECTION_GUI_FORMAT_DATE
 **/
void _GUI_Format_Date_Update(GUI_Format *unsafe_self) {
  struct GUI_Format_Date *self = (struct GUI_Format_Date *)unsafe_self;
  vfdco_display_render_date(&global_date, self->date_mode);
}

void _GUI_Format_Date_F4Var(GUI_Format *unsafe_self) {
  struct GUI_Format_Date *self = (struct GUI_Format_Date *)unsafe_self;
  if(self->date_mode == DATE_FORMAT_DDMMYY) {
    self->date_mode = DATE_FORMAT_MMDDYY;
    char _message[CONFIG_NUM_DIGITS] = {'D', ' ', ' ',  1, 'S', 'T'};
    vfdco_display_render_message(_message, 0, CONFIG_MESSAGE_LONG);
  }
  else {
    self->date_mode = DATE_FORMAT_DDMMYY;
    char _message[CONFIG_NUM_DIGITS] = {'D', ' ', ' ', '2', 'N', 'D'};
    vfdco_display_render_message(_message, 0, CONFIG_MESSAGE_LONG);
  }
}

void _GUI_Format_Date_Save(GUI_Format *unsafe_self) {
  struct GUI_Format_Date *self = (struct GUI_Format_Date *)unsafe_self;
  self->settings[GUI_FORMAT_SETTING_DATE_date_mode] = self->date_mode;
}

void GUI_Format_Date_Init(struct GUI_Format_Date *self, uint8_t *settings) {
  // Default loading if saved value is crap, then load by assignment
  date_format_t _chk_date_mode = settings[GUI_FORMAT_SETTING_DATE_date_mode];
  if((_chk_date_mode != DATE_FORMAT_DDMMYY) && (_chk_date_mode != DATE_FORMAT_MMDDYY)) 
    GUI_Format_Date_Default(settings);
  
  self->date_mode = settings[GUI_FORMAT_SETTING_DATE_date_mode];
  self->settings = settings;

  GUI_Format_F2 = NULL;
  GUI_Format_F3 = NULL;
  GUI_Format_F4 = NULL;
  GUI_Format_F2Var = NULL;
  GUI_Format_F3Var = NULL;
  GUI_Format_F4Var = _GUI_Format_Date_F4Var;
  GUI_Format_Update = _GUI_Format_Date_Update;
  GUI_Format_Save = _GUI_Format_Date_Save;
}

void GUI_Format_Date_Default(uint8_t *settings) {
  settings[GUI_FORMAT_SETTING_DATE_date_mode] = DATE_FORMAT_DDMMYY;
}


/** Begin of:
  * @tableofcontents SECTION_GUI_FORMAT_TIME_DATE_SET
 **/
void _GUI_Format_Time_Date_Setter_Update(GUI_Format *unsafe_self) {
  struct GUI_Format_Time_Date_Setter *self = (struct GUI_Format_Time_Date_Setter *)unsafe_self;

  if(Time_Event_Update(&self->blank_timer)) self->blank_active = !self->blank_active;
  uint8_t _c1, _c2, _c3;
  if(self->set_mode == 0) {
    _c1 = self->new_time.h;
    _c2 = self->new_time.m;
    _c3 = self->new_time.s;
  } else {
    _c1 = self->new_date.d;
    _c2 = self->new_date.m;
    _c3 = self->new_date.y;
  }
  
  self->blank_alt_message[0] = _c1 / 10;
  self->blank_alt_message[1] = _c1 % 10;
  self->blank_alt_message[2] = _c2 / 10;
  self->blank_alt_message[3] = _c2 % 10;
  self->blank_alt_message[4] = _c3 / 10;
  self->blank_alt_message[5] = _c3 % 10;

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

void _GUI_Format_Time_Date_Setter_F2(GUI_Format *unsafe_self) {
  struct GUI_Format_Time_Date_Setter *self = (struct GUI_Format_Time_Date_Setter *)unsafe_self;
  // Short press on F2 changes the active parameter (h/m/s)
  ++self->active_digit;
  if(self->active_digit == 3) self->active_digit = 0;
}

void _GUI_Format_Time_Date_Setter_F3(GUI_Format *unsafe_self) {
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

void _GUI_Format_Time_Date_Setter_F4(GUI_Format *unsafe_self) {
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

void GUI_Format_Time_Date_Setter_Init(struct GUI_Format_Time_Date_Setter *self, uint_fast8_t set_mode) {
  self->set_mode = set_mode;
  self->active_digit = 0;
  self->blank_active = 0;
  self->blank_timer = Time_Event_Init(500);

  self->new_time = global_time;
  self->new_date = global_date;

  GUI_Format_F2 = _GUI_Format_Time_Date_Setter_F2; // sw active
  GUI_Format_F3 = _GUI_Format_Time_Date_Setter_F3; // --
  GUI_Format_F4 = _GUI_Format_Time_Date_Setter_F4; // ++
  GUI_Format_F2Var = NULL;
  GUI_Format_F3Var = NULL;
  GUI_Format_F4Var = NULL;
  GUI_Format_Update = _GUI_Format_Time_Date_Setter_Update;
  GUI_Format_Save = NULL;
}



/** Begin of:
  * @tableofcontents SECTION_GUI_FORMAT_STOPWATCH
 **/
void _GUI_Format_Stopwatch_Update(GUI_Format *unsafe_self) {
  struct GUI_Format_Stopwatch *self = (struct GUI_Format_Stopwatch *)unsafe_self;
  if(self->stopwatch_state == GUI_FORMAT_STOPWATCH_STATE_INITIALIZED) {
    char zeros[CONFIG_NUM_DIGITS] = {0};
    vfdco_display_render_message(zeros, 0b000010100, 0);
  }
  else if(self->stopwatch_state == GUI_FORMAT_STOPWATCH_STATE_RUNNING) {
    // running time: elapsed base + current time - start timestamp
    uint32_t running_time = self->elapsed_time +
      (        global_time.h * 3600 +         global_time.m * 60 +         global_time.s) -
      ( self->initial_time.h * 3600 +  self->initial_time.m * 60 +  self->initial_time.s);

    if(running_time > 3599) {
      vfdco_time_t new_time = {
        .h = running_time / 3600 % 24,
        .m = running_time / 60 % 60,
        .s = running_time % 60
      };
      vfdco_display_render_time(&new_time, 0, TIME_FORMAT_24H);
    } else {
      char digits[CONFIG_NUM_DIGITS];
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
    uint8_t elapsed_m = elapsed_time / 60 % 60;
    uint8_t elapsed_s = elapsed_time % 60;
    if(elapsed_time > 3599) {
      vfdco_time_t new_time = {
        .h = elapsed_time / 3600 % 24,
        .m = elapsed_m,
        .s = elapsed_s
      };
      vfdco_display_render_time(&new_time, 0, TIME_FORMAT_24H);
    } else {
      char digits[CONFIG_NUM_DIGITS];
      uint8_t running_millis = (self->elapsed_milliseconds % 1000) / 10;
      digits[0] = elapsed_m / 10;
      digits[1] = elapsed_m % 10;
      digits[2] = elapsed_s / 10;
      digits[3] = elapsed_s % 10;
      digits[4] = running_millis / 10;
      digits[5] = running_millis % 10;
      vfdco_display_render_message(digits, 0, 0);
    }
  }
}

void _GUI_Format_Stopwatch_F2(GUI_Format *unsafe_self) {
  struct GUI_Format_Stopwatch *self = (struct GUI_Format_Stopwatch *)unsafe_self;
  // Initialize
  if(self->stopwatch_state == GUI_FORMAT_STOPWATCH_STATE_INITIALIZED) {
    if(!self->not_initial) {
      self->initial_time.h = global_time.h;
      self->initial_time.m = global_time.m;
      self->initial_time.s = global_time.s;
      self->not_initial = 1;
    }
    #ifndef DEBUG // TODO: platform independency
    self->initial_milliseconds = vfdco_time_get_milliseconds();
    #endif
    self->elapsed_time = 0;
    self->elapsed_milliseconds = 0;
    self->stopwatch_state = GUI_FORMAT_STOPWATCH_STATE_RUNNING;
  }
  // Reset
  else if(self->stopwatch_state == GUI_FORMAT_STOPWATCH_STATE_PAUSED) {
    self->not_initial = 0;
    self->elapsed_time = 0;
    self->initial_milliseconds = 0;
    self->elapsed_milliseconds = 0;
    self->stopwatch_state = GUI_FORMAT_STOPWATCH_STATE_INITIALIZED;
  }
}

void _GUI_Format_Stopwatch_F3(GUI_Format *unsafe_self) {
  struct GUI_Format_Stopwatch *self = (struct GUI_Format_Stopwatch *)unsafe_self;
  // Pause
  if(self->stopwatch_state == GUI_FORMAT_STOPWATCH_STATE_RUNNING) {
    // Save seconds
    self->elapsed_time +=
      (        global_time.h * 3600 +         global_time.m * 60 +         global_time.s) -
      ( self->initial_time.h * 3600 +  self->initial_time.m * 60 +  self->initial_time.s);

    // Save milliseconds
    self->elapsed_milliseconds += vfdco_time_get_milliseconds() - self->initial_milliseconds;
    self->stopwatch_state = GUI_FORMAT_STOPWATCH_STATE_PAUSED;

  }
  // Resume
  else if(self->stopwatch_state == GUI_FORMAT_STOPWATCH_STATE_PAUSED) {
    self->initial_time.h = global_time.h;
    self->initial_time.m = global_time.m;
    self->initial_time.s = global_time.s;
    #ifndef DEBUG
    self->initial_milliseconds = vfdco_time_get_milliseconds();
    #endif
    self->stopwatch_state = GUI_FORMAT_STOPWATCH_STATE_RUNNING;
  }
}

void GUI_Format_Stopwatch_Init(struct GUI_Format_Stopwatch *self) {
  self->stopwatch_state = GUI_FORMAT_STOPWATCH_STATE_INITIALIZED;
  // self->initial_time = NULL;
  self->not_initial = 0;
  self->elapsed_time = 0;
  self->initial_milliseconds = 0;
  self->elapsed_milliseconds = 0;

  GUI_Format_F2 = _GUI_Format_Stopwatch_F2;
  GUI_Format_F3 = _GUI_Format_Stopwatch_F3;
  GUI_Format_F4 = NULL;
  GUI_Format_F2Var = NULL;
  GUI_Format_F3Var = NULL;
  GUI_Format_F4Var = NULL;
  GUI_Format_Update = _GUI_Format_Stopwatch_Update;
  GUI_Format_Save = NULL;
}



/**
 * @tableofcontents SECTION_GUI_BRIGHTNESS_SETTER
 * F2 to set display brightness, immediate effect
 * F3 to set LED brightness, immediate effect
 * F4 to enable night shift set menu
 *    - Step 1: Set starting time. F2: digit switch, F3: --, F4: ++. F4Var to next step
 *    - Setp 2: Set ending time.   F2: digit switch, F3: --, F4: ++. F4Var to save
 * F4Var to disable night shift, if enabled
 */

enum {
  GUI_FORMAT_BRIGHTNESS_SETTER_STATE_INITIALIZED = 0,
  GUI_FORMAT_BRIGHTNESS_SETTER_STATE_SET_NSH_START = 1,
  GUI_FORMAT_BRIGHTNESS_SETTER_STATE_SET_NSH_END = 2
}; 

void _GUI_Format_Brightness_Setter_Update(GUI_Format *unsafe_self) {
  struct GUI_Format_Brightness_Setter *self = (struct GUI_Format_Brightness_Setter *)unsafe_self;
  if(self->menu_state == GUI_FORMAT_BRIGHTNESS_SETTER_STATE_INITIALIZED) {
    // Idle. Waiting for F2/F3/F4/F4Var
    if(Time_Event_Update(&self->blank_timer)) {
      // Just cycle through all the messages
      uint8_t message_counter = self->message_counter;
      if(message_counter < (MESSAGES_BRIGHTNESS_SET_IDLE_MAX - MESSAGES_BRIGHTNESS_SET_IDLE_REPEAT)) {
        vfdco_display_render_message(
          Messages_Brightness_Set_Idle[message_counter], 
          Messages_Brightness_Set_Idle_Dots[message_counter], 
          0
        );
      } else {
        vfdco_display_render_message(
          Messages_Brightness_Set_Idle[message_counter - MESSAGES_BRIGHTNESS_SET_IDLE_REPEAT], 
          Messages_Brightness_Set_Idle_Dots[message_counter - MESSAGES_BRIGHTNESS_SET_IDLE_REPEAT], 
          0
        );
      }
      ++self->message_counter;
      if(message_counter == MESSAGES_BRIGHTNESS_SET_IDLE_MAX) self->message_counter = 0;
    }
  } else {
    // Show NSH start time or NSH stop time with blinking active digit
    char _render_msg[CONFIG_NUM_DIGITS];
    if(Time_Event_Update(&self->blank_timer)) {
      self->blank_active = !self->blank_active;
      // T4 TO SA VE
      _render_msg[4] = Messages_Brightness_Set_Continue[self->message_counter][0];
      _render_msg[5] = Messages_Brightness_Set_Continue[self->message_counter][1];
      ++self->message_counter;
      if(self->message_counter == MESSAGES_BRIGHTNESS_SET_CONTINUE) self->message_counter = 0;
    }
    uint8_t _h = 0, _m = 0;
    if(self->menu_state == GUI_FORMAT_BRIGHTNESS_SETTER_STATE_SET_NSH_START) {
      _h = self->night_shift_new_start_h;
      _m = self->night_shift_new_start_m;
    } else {
      _h = self->night_shift_new_end_h;
      _m = self->night_shift_new_end_m;
    }
    _render_msg[0] = _h / 10;
    _render_msg[1] = _h % 10;
    _render_msg[2] = _m / 10;
    _render_msg[3] = _m % 10;

    if(self->blank_active) {
      if(self->active_digit == 0) {
        _render_msg[0] = ' ';
        _render_msg[1] = ' ';
      } else if(self->active_digit == 1) {
        _render_msg[2] = ' ';
        _render_msg[3] = ' ';
      }
    }
    vfdco_display_render_message(_render_msg, 0, 0);
  }
}

void _GUI_Format_Brightness_Setter_F2(GUI_Format *unsafe_self) {
  struct GUI_Format_Brightness_Setter *self = (struct GUI_Format_Brightness_Setter *)unsafe_self;
  if(self->menu_state == GUI_FORMAT_BRIGHTNESS_SETTER_STATE_INITIALIZED) {
    char _msg[CONFIG_NUM_DIGITS] = {'D', ' ', 'B', 'R', 'I',  3 };
    if(self->dim_factor_display == CONFIG_BRIGHTNESS_MAX) {
      self->dim_factor_display = CONFIG_BRIGHTNESS_HALF;
      _msg[5] = 2;
    } else if(self->dim_factor_display == CONFIG_BRIGHTNESS_HALF) {
      self->dim_factor_display = CONFIG_BRIGHTNESS_MIN;
      _msg[5] = 1;
    } else {
      self->dim_factor_display = CONFIG_BRIGHTNESS_MAX;
    }
    vfdco_display_render_message(_msg, 0x00, CONFIG_MESSAGE_SHORT);
    self->message_counter = 0; // Resume message cycling
  } else {
    ++self->active_digit;
    if(self->active_digit == 2) self->active_digit = 0;
  }
}

void _GUI_Format_Brightness_Setter_F3(GUI_Format *unsafe_self) {
  struct GUI_Format_Brightness_Setter *self = (struct GUI_Format_Brightness_Setter *)unsafe_self;
  if(self->menu_state == GUI_FORMAT_BRIGHTNESS_SETTER_STATE_INITIALIZED) {
    char _msg[CONFIG_NUM_DIGITS] = {'L', ' ', 'B', 'R', 'I', 3};
    if(self->dim_factor_led == CONFIG_BRIGHTNESS_MAX) {
      self->dim_factor_led = CONFIG_BRIGHTNESS_HALF;
      _msg[5] = 2;
    } else if(self->dim_factor_led == CONFIG_BRIGHTNESS_HALF) {
      self->dim_factor_led = CONFIG_BRIGHTNESS_MIN;
      _msg[5] = 1;
    } else {
      self->dim_factor_led = CONFIG_BRIGHTNESS_MAX;
    }
    vfdco_display_render_message(_msg, 0x00, CONFIG_MESSAGE_SHORT);
    self->message_counter = 0; // Resume message cycling
  } else if(self->menu_state == GUI_FORMAT_BRIGHTNESS_SETTER_STATE_SET_NSH_START) {
    // Decrease NSH start digit
    if      (self->active_digit == 0) { // Set hour
      if       (self->night_shift_new_start_h  > 0)    self->night_shift_new_start_h--;
      else if  (self->night_shift_new_start_h == 0)    self->night_shift_new_start_h = 23;
    }
    else if (self->active_digit == 1) { // Set minute
      if       (self->night_shift_new_start_m  > 0)    self->night_shift_new_start_m--;
      else if  (self->night_shift_new_start_m == 0)    self->night_shift_new_start_m = 59;
    }
  } else if(self->menu_state == GUI_FORMAT_BRIGHTNESS_SETTER_STATE_SET_NSH_END) {
    // Decrease NSH end digit
    if      (self->active_digit == 0) { // Set hour
      if       (self->night_shift_new_end_m  > 0)    self->night_shift_new_end_m--;
      else if  (self->night_shift_new_end_m == 0)    self->night_shift_new_end_m = 23;
    }
    else if (self->active_digit == 1) { // Set minute
      if       (self->night_shift_new_end_m  > 0)    self->night_shift_new_end_m--;
      else if  (self->night_shift_new_end_m == 0)    self->night_shift_new_end_m = 59;
    }
  }
}

void _GUI_Format_Brightness_Setter_F4(GUI_Format *unsafe_self) {
  struct GUI_Format_Brightness_Setter *self = (struct GUI_Format_Brightness_Setter *)unsafe_self;
  if(self->menu_state == GUI_FORMAT_BRIGHTNESS_SETTER_STATE_INITIALIZED) {
    self->menu_state = GUI_FORMAT_BRIGHTNESS_SETTER_STATE_SET_NSH_START;
    // "NIGHT" "SHIFT"
    vfdco_display_render_message(Messages_Brightness_Set_Idle[4], 0x00, CONFIG_MESSAGE_SHORT);
    vfdco_display_render_message(Messages_Brightness_Set_Idle[5], 0x00, CONFIG_MESSAGE_SHORT);
    self->message_counter = 0;
  } else if(self->menu_state == GUI_FORMAT_BRIGHTNESS_SETTER_STATE_SET_NSH_START) {
    // Increase NSH start digit
    if      (self->active_digit == 0) { // Set hour
      if       (self->night_shift_new_start_h  < 23)    self->night_shift_new_start_h++;
      else if  (self->night_shift_new_start_h == 23)    self->night_shift_new_start_h = 0;
    }
    else if (self->active_digit == 1) { // Set minute
      if       (self->night_shift_new_start_m  < 59)    self->night_shift_new_start_m++;
      else if  (self->night_shift_new_start_m == 59)    self->night_shift_new_start_m = 0;
    }
  } else if(self->menu_state == GUI_FORMAT_BRIGHTNESS_SETTER_STATE_SET_NSH_END) {
    // Increase NSH end digit
    if      (self->active_digit == 0) { // Set hour
      if       (self->night_shift_new_end_h  < 23)    self->night_shift_new_end_h++;
      else if  (self->night_shift_new_end_h == 23)    self->night_shift_new_end_h = 0;
    }
    else if (self->active_digit == 1) { // Set minute
      if       (self->night_shift_new_end_m  < 59)    self->night_shift_new_end_m++;
      else if  (self->night_shift_new_end_m == 59)    self->night_shift_new_end_m = 0;
    }
  }
}

void _GUI_Format_Brightness_Setter_F4Var(GUI_Format *unsafe_self) {
  struct GUI_Format_Brightness_Setter *self = (struct GUI_Format_Brightness_Setter *)unsafe_self;
  if(self->menu_state == GUI_FORMAT_BRIGHTNESS_SETTER_STATE_INITIALIZED) {
    // Disable/clear NSH
    self->night_shift_new_start_h = 0;
    self->night_shift_new_start_m = 0;
    self->night_shift_new_end_h = 0;
    self->night_shift_new_end_m = 0;
    vfdco_display_render_message(Messages_Brightness_Set_Idle[4], 0x00, CONFIG_MESSAGE_SHORT);
    vfdco_display_render_message(Messages_Brightness_Set_Idle[5], 0x00, CONFIG_MESSAGE_SHORT);
    const char _msg[CONFIG_NUM_DIGITS] = {' ', ' ', ' ', 'O', 'F', 'F'};
    vfdco_display_render_message(_msg, 0x00, CONFIG_MESSAGE_SHORT);
  } else if(self->menu_state == GUI_FORMAT_BRIGHTNESS_SETTER_STATE_SET_NSH_START) {
    // Go to set NSH end
    self->menu_state = GUI_FORMAT_BRIGHTNESS_SETTER_STATE_SET_NSH_END;
    self->message_counter = 0;
  } else if(self->menu_state == GUI_FORMAT_BRIGHTNESS_SETTER_STATE_SET_NSH_END) {
    // Go back to menu idle
    self->menu_state = GUI_FORMAT_BRIGHTNESS_SETTER_STATE_INITIALIZED;
    self->message_counter = 0;
    vfdco_display_render_message(Messages_Brightness_Set_Idle[4], 0x00, CONFIG_MESSAGE_SHORT);
    vfdco_display_render_message(Messages_Brightness_Set_Idle[5], 0x00, CONFIG_MESSAGE_SHORT);
    const char _msg[CONFIG_NUM_DIGITS] = {' ', 'S', 'A', 'V', 'E', 'D'};
    vfdco_display_render_message(_msg, 0x00, CONFIG_MESSAGE_SHORT);
  }
}

void GUI_Format_Brightness_Setter_Init(struct GUI_Format_Brightness_Setter *self, const uint8_t *shared_initializer) {
  self->menu_state = GUI_FORMAT_BRIGHTNESS_SETTER_STATE_INITIALIZED;
  // Initialize potential new NSH times with existing NSH times
  self->dim_factor_display = shared_initializer[_SHARED_dim_factor_display];
  self->dim_factor_led = shared_initializer[_SHARED_dim_factor_led];
  self->night_shift_new_start_h = shared_initializer[_SHARED_dim_factor_night_shift_start_h];
  self->night_shift_new_start_m = shared_initializer[_SHARED_dim_factor_night_shift_start_m];
  self->night_shift_new_end_h = shared_initializer[_SHARED_dim_factor_night_shift_end_h];
  self->night_shift_new_end_m = shared_initializer[_SHARED_dim_factor_night_shift_end_m];
  // Digit blanking at set
  self->message_counter = 0;
  self->blank_active = 0;
  self->active_digit = 0;
  self->blank_timer = Time_Event_Init(800);

  GUI_Format_F2 = _GUI_Format_Brightness_Setter_F2;
  GUI_Format_F3 = _GUI_Format_Brightness_Setter_F3;
  GUI_Format_F4 = _GUI_Format_Brightness_Setter_F4;
  GUI_Format_F2Var = NULL;
  GUI_Format_F3Var = NULL;
  GUI_Format_F4Var = _GUI_Format_Brightness_Setter_F4;
  GUI_Format_Update = _GUI_Format_Brightness_Setter_Update;
  GUI_Format_Save = NULL;
}


/** Begin of:
  * @tableofcontents SECTION_CONTAINER_GUI
**/
void Container_GUI_Clear(GUI_Format *self) {
  memset(self, 0, sizeof(GUI_Format));
}


// Rip a line from a page watch it fade up in smoke hope rise through the rain higher than we'd ever go
