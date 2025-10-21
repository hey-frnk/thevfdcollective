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
  * @file     vfdco_gui.c
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     12-February-2020
  * @brief    This file contains definitions for the GUI
  *           Designed to be used with Fluorescence by The VFD Collective
  ******************************************************************************
  * @tableofcontents Table of contents, enter to navigate:
  * SECTION_SUPPORTING_FUNCTIONS
  * SECTION_GUI_FORMAT_CONSTANTS
  * SECTION_GUI_FORMAT
  * SECTION_GUI_FORMAT_TIME
  * SECTION_GUI_FORMAT_DATE
  * SECTION_GUI_FORMAT_SENSORS
  * SECTION_GUI_FORMAT_TIME_DATE_SET
  * SECTION_GUI_FORMAT_ALARM_SET
  * SECTION_GUI_FORMAT_STOPWATCH
  * SECTION_CONTAINER_GUI
  * SECTION_SUPPORTING_FUNCTIONS_IMPLEMENTATION
  ******************************************************************************
 **/

#include <stdlib.h>
#include <string.h>

#include "vfdco_config.h"
#include "vfdco_hid.h"
#include "vfdco_display.h"
#include "vfdco_gui.h"
#include "vfdco_audio.h"

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

void GUI_Format_FOverride(GUI_Format *unsafe_self) {
  // Do nothing. Just override color.
  return;
}

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

// These shared setting indices must be provided as they are dependent on routine.c
#define _SHARED_dim_factor_display              0
#define _SHARED_dim_factor_led                  1
#define _SHARED_dim_factor_night_shift_start_h  2
#define _SHARED_dim_factor_night_shift_start_m  3
#define _SHARED_dim_factor_night_shift_end_h    4
#define _SHARED_dim_factor_night_shift_end_m    5
#define _SHARED_digit_fade_mode                 0
#define _SHARED_alarm_h                         0
#define _SHARED_alarm_m                         1
#define _SHARED_alarm_snooze_option             2


static const char Messages_Sensors_Set_Mode[][CONFIG_NUM_DIGITS] = {
    { 'T', 'E', 'P', ' ', ' ', ' ' },     // TEMP
    { 'H', 'U', 'D', ' ', ' ', ' ' },     // HUMI (Humidity)
    { 'P', 'R', 'E', 'S', 'S', ' ' },     // PRESS
    { 'T', 'I', 'L', 'T', ' ', ' ' }      // TILT
};
static const char Messages_Sensors_Set_Temp_Units[][CONFIG_NUM_DIGITS] = {
    { 'U', ' ', 'C', 'E', 'L', 'S' },    // U CELS
    { 'U', ' ', 'F', 'A', 'H', 'R' }     // U FAHR
};
static const char Messages_Sensors_Set_Pressure_Units[][CONFIG_NUM_DIGITS] = {
    { 'U', ' ', 'H', 'P', 'A', ' ' },    // U HPA
    { 'U', ' ', 'I', 'N', 'H', 'G' }     // U INHG
};

// Alarm
// Snooze times
static const uint8_t Alarm_Snooze_Options[] = {0, 1, 7, 9, 10, 15 };
// Auto-off times
static const uint8_t Alarm_Auto_Off_Options[] = {2, 1, 0, 5};

#define MESSAGES_BRIGHTNESS_SET_IDLE_MAX 6
static const char Messages_Brightness_Set_Idle[MESSAGES_BRIGHTNESS_SET_IDLE_MAX][CONFIG_NUM_DIGITS] = {
  {'B', 'R', 'I', 'S', 'E', 'T'}, // 0
  {'F',  2 , ' ', 'V', 'F', 'D'}, // 1
  {'F',  3 , ' ', 'L', 'E', 'D'}, // 2
  {'F',  4 , ' ', 'S', 'E', 'T'}, // 3
  {' ', 'N', 'I', 'G', 'H', 'T'}, // 4
  {' ', 'S', 'H', 'I', 'F', 'T'}, // 5
};
static const uint8_t Messages_Brightness_Set_Idle_Dots[MESSAGES_BRIGHTNESS_SET_IDLE_MAX] = {
  0b00000100, 0b00001000, 0b00001000, 0b00001000, 0, 0
};

#define MESSAGES_BRIGHTNESS_SET_NSH 6
static const uint8_t Brightness_Set_Night_Shift_Times[MESSAGES_BRIGHTNESS_SET_NSH][2] = {
  { 0,  0}, // Off
  {20,  5}, // Yogi: 8 PM -> 5 AM
  {21,  6}, // Early bird: 9 PM -> 6 AM
  {22,  8}, // Average joe: 10 PM -> 8 AM
  { 0,  9}, // Night owl: Midnight -> 9 AM
  { 2, 10}  // Student: 2 AM -> 10 AM
};
static const char Messages_Brightness_Set_Nsh[MESSAGES_BRIGHTNESS_SET_NSH][CONFIG_NUM_DIGITS] = {
  {'O', 'F', 'F', ' ', ' ', ' '}, // Off
  {'E', 'R', 'L', 'I', 'S', 'T'}, // Earliest
  {'E', 'A', 'R', 'L', 'Y', ' '}, // Early bird
  {'A', 'V', 'E', 'R', 'A', 'G'}, // Average Joe
  {'O', 'V', 'L', ' ', ' ', ' '}, // Owl
  {'S', 'T', 'U', 'D', 'N', 'T'}  // Student
};

#define MESSAGES_DIGIT_FADE_SET_IDLE_MAX 5
static const char Messages_Digit_Fade_Set_Idle[MESSAGES_DIGIT_FADE_SET_IDLE_MAX][CONFIG_NUM_DIGITS] = {
  {'D', 'I', 'G', 'I', 'T', ' '}, // Digit
  {'F', 'A', 'D', 'E', ' ', ' '}, // Fade
  {'S', 'E', 'T', ' ', ' ', ' '}, // Set
  {'F', 2,   ' ', 'T', 'O', ' '}, // F2 to
  {'C', 'H', 'A', 'N', 'G', 'E'}, // Change
};
static const char Messages_Digit_Fade_Set[3][CONFIG_NUM_DIGITS] = {
  {'N', 'O', 'N', 'E', ' ', ' '}, // None
  {'F', 'A', 'D', 'I', 'N', 'G'}, // Fade-out and fade-in digits
  {'B', 'L', 'E', 'N', 'D', ' '}  // Blend digits
};


/** Begin of:
  * @tableofcontents SECTION_SUPPORTING_FUNCTIONS
  * @brief Prototypes. Implementation see very end
 **/
// HSL to RGB fast calculation
uint32_t vfdco_time_epoch_seconds(const vfdco_date_t *date, const vfdco_time_t *time);

/** Begin of:
 * @tableofcontents SECTION_GUI_FORMAT
**/

/** Begin of:
  * @tableofcontents SECTION_GUI_FORMAT_TIME
 **/

static inline void _GUI_Format_Time_Allocate_Special(struct GUI_Format_Time *self);
static inline void _GUI_Format_Time_Deallocate_Special(struct GUI_Format_Time *self);

static void _GUI_Format_Time_Update(GUI_Format *unsafe_self) {
  struct GUI_Format_Time *self = (struct GUI_Format_Time *)unsafe_self;
  if(self->alarm_state == ALARM_STATE_STARTED) {
    _GUI_Format_Time_Allocate_Special(self);
  }
  // If it is snoozed, check if we should ring again
  if(self->alarm_state == ALARM_STATE_SNOOZED) {
    uint32_t now = vfdco_time_epoch_seconds(self->date_instance, self->time_instance);
    if(now >= self->alarm_trigger_epoch) {
      _GUI_Format_Time_Allocate_Special(self);
    }
  }
  // If it has been dismissed, exit alarm overlay
  if(self->alarm_state == ALARM_STATE_DISMISSED) {
    _GUI_Format_Time_Deallocate_Special(self);
  }

  // During ringing state, occasionally
  if(self->alarm_state == ALARM_STATE_RINGING) {
    if(!vfdco_audio_speaker_is_active()) vfdco_audio_speaker_play(CONFIG_ALARM_FREQ);
    if(Time_Event_Update(&self->alarm_flash_timer))
      self->alarm_flash_toggle ^= 1;
    if(self->alarm_flash_toggle) { // Display AL instead of seconds
      vfdco_display_render_message((char[]){
        self->time_instance->h / 10, self->time_instance->h % 10,
        self->time_instance->m / 10, self->time_instance->m % 10, 'A','L'
      }, 0, 0);
      return;
    }
  }

  // Fall through
  if(Time_Event_Update(&self->dot_timer)) self->dot_position++;
  if(self->dot_mode == 0) { // Standard
    if      (self->dot_position == 0) vfdco_display_render_time(self->time_instance, 0b00010100, self->time_mode);
    else if (self->dot_position == 1) vfdco_display_render_time(self->time_instance, 0b00000000, self->time_mode);
    else    self->dot_position = 0; // Reset
  }
  else if(self->dot_mode == 1) { // Converge
    switch (self->dot_position) {
      case 0: vfdco_display_render_time(self->time_instance, 0b00100001, self->time_mode); break;
      case 1: vfdco_display_render_time(self->time_instance, 0b00010010, self->time_mode); break;
      case 2: vfdco_display_render_time(self->time_instance, 0b00001100, self->time_mode); break;
      case 3: vfdco_display_render_time(self->time_instance, 0b00010010, self->time_mode); break;
      default: self->dot_position = 0;
    }
  }
  else if(self->dot_mode == 2) { // Flip
    // This function is damn lit. Once it detects a change in second,
    // the decimal dot will slide over the displays.
    // Get the current time and compare it with the previous timestamp
    if(self->dot_direction != self->time_instance->s) {
      self->dot_position = 0;
      self->dot_direction = self->time_instance->s;
    }

    if(self->dot_direction & 0x01) { // odd: r->l
      if(self->dot_position < 5) vfdco_display_render_time(self->time_instance, 1 << self->dot_position, self->time_mode);
      else                       vfdco_display_render_time(self->time_instance, 0b00100000, self->time_mode); // Leftmost static
    } else {
      if(self->dot_position < 5) vfdco_display_render_time(self->time_instance, 0b00100000 >> self->dot_position, self->time_mode);
      else                       vfdco_display_render_time(self->time_instance, 0b00000001, self->time_mode); // Rightmost static
    }
  }
  else if(self->dot_mode == 3) {
    vfdco_display_render_time(self->time_instance, 0, self->time_mode);
  }
}

static void _GUI_Format_Time_F2(GUI_Format *unsafe_self) {
  struct GUI_Format_Time *self = (struct GUI_Format_Time *)unsafe_self;
  if(self->alarm_state == ALARM_STATE_RINGING) {
    if(self->alarm_snooze_option == 0) {
      self->alarm_state = ALARM_STATE_DISMISSED;
      vfdco_display_render_message((char[]) {'A','L',' ','O','F','F'}, 0, CONFIG_MESSAGE_SHORT);
    } else {
      self->alarm_trigger_epoch = vfdco_time_epoch_seconds(self->date_instance, self->time_instance) + Alarm_Snooze_Options[self->alarm_snooze_option] * 60;
      vfdco_display_render_message((char[]) {'A','L','S','N', Alarm_Snooze_Options[self->alarm_snooze_option] / 10, Alarm_Snooze_Options[self->alarm_snooze_option] % 10}, 0, CONFIG_MESSAGE_SHORT);
      self->alarm_state = ALARM_STATE_SNOOZED;
    }
  }
}

static void _GUI_Format_Time_F3(GUI_Format *unsafe_self) {
  struct GUI_Format_Time *self = (struct GUI_Format_Time *)unsafe_self;
  if(self->alarm_state == ALARM_STATE_RINGING) {
    self->alarm_state = ALARM_STATE_DISMISSED;
    vfdco_display_render_message((char[]) {'A','L',' ','O','F','F'}, 0, CONFIG_MESSAGE_SHORT);
  }
}

static void _GUI_Format_Time_F4(GUI_Format *unsafe_self) {
  struct GUI_Format_Time *self = (struct GUI_Format_Time *)unsafe_self;

  // Change dot mode
  ++self->dot_mode;
  if(self->dot_mode > 3) self->dot_mode = 0;

  // Reset parameters
  self->dot_position = 0;
  self->dot_timer = Time_Event_Init(GUI_Format_Time_Dot_Intervals[self->dot_mode]);
}

static void _GUI_Format_Time_F4Var(GUI_Format *unsafe_self) {
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

static void _GUI_Format_Time_Save(GUI_Format *unsafe_self) {
  struct GUI_Format_Time *self = (struct GUI_Format_Time *)unsafe_self;
  self->settings[GUI_FORMAT_SETTING_TIME_time_mode] = self->time_mode;
  self->settings[GUI_FORMAT_SETTING_TIME_dot_mode] = self->dot_mode;
}

void GUI_Format_Time_Init(struct GUI_Format_Time *self, vfdco_time_t *time_instance, vfdco_date_t *date_instance, uint8_t alarm_snooze_option, uint8_t *settings) {

  // Default loading if saved value is litter, then load by assignment
  /* time_format_t _chk_time_mode = settings[GUI_FORMAT_SETTING_TIME_time_mode];
  if(((_chk_time_mode != TIME_FORMAT_12H) && (_chk_time_mode != TIME_FORMAT_24H) && (_chk_time_mode != TIME_FORMAT_12H_NO_LZ))
      || (settings[GUI_FORMAT_SETTING_TIME_dot_mode] > 3)) GUI_Format_Time_Default(settings); */

  self->time_instance = time_instance;
  self->date_instance = date_instance;
  self->time_mode = settings[GUI_FORMAT_SETTING_TIME_time_mode];
  self->dot_mode = settings[GUI_FORMAT_SETTING_TIME_dot_mode];

  self->dot_position = 0;
  self->dot_direction = 0;
  self->dot_timer = Time_Event_Init(GUI_Format_Time_Dot_Intervals[self->dot_mode]);
  self->settings = settings;

  self->alarm_snooze_option = alarm_snooze_option;

  GUI_Format_F2 = NULL;
  GUI_Format_F3 = NULL;
  GUI_Format_F4 = _GUI_Format_Time_F4;
  GUI_Format_F2Var = NULL;
  GUI_Format_F3Var = NULL;
  GUI_Format_F4Var = _GUI_Format_Time_F4Var;
  GUI_Format_Update = _GUI_Format_Time_Update;
  GUI_Format_Save = _GUI_Format_Time_Save;
}

static inline void _GUI_Format_Time_Allocate_Special(struct GUI_Format_Time *self) {
  self->alarm_state = ALARM_STATE_RINGING;
  self->alarm_flash_timer = Time_Event_Init(622);
  self->alarm_flash_toggle = 0;
  GUI_Format_F2 = _GUI_Format_Time_F2;
  GUI_Format_F3 = _GUI_Format_Time_F3;
}
static inline void _GUI_Format_Time_Deallocate_Special(struct GUI_Format_Time *self) {
  self->alarm_state = ALARM_STATE_IDLE;
  Time_Event_Reset(&self->alarm_flash_timer);
  self->alarm_flash_toggle = 0;
  GUI_Format_F2 = NULL;
  GUI_Format_F3 = NULL;
  if(vfdco_audio_speaker_is_active()) vfdco_audio_speaker_stop();
}


/** Begin of:
  * @tableofcontents SECTION_GUI_FORMAT_DATE
 **/
static void _GUI_Format_Date_Update(GUI_Format *unsafe_self) {
  struct GUI_Format_Date *self = (struct GUI_Format_Date *)unsafe_self;
  vfdco_display_render_date(self->date_instance, self->date_mode);
}

static void _GUI_Format_Date_F4Var(GUI_Format *unsafe_self) {
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

static void _GUI_Format_Date_Save(GUI_Format *unsafe_self) {
  struct GUI_Format_Date *self = (struct GUI_Format_Date *)unsafe_self;
  self->settings[GUI_FORMAT_SETTING_DATE_date_mode] = self->date_mode;
}

void GUI_Format_Date_Init(struct GUI_Format_Date *self, vfdco_date_t *date_instance, uint8_t *settings) {
  // Default loading if saved value is crap, then load by assignment
  /* date_format_t _chk_date_mode = settings[GUI_FORMAT_SETTING_DATE_date_mode];
  if((_chk_date_mode != DATE_FORMAT_DDMMYY) && (_chk_date_mode != DATE_FORMAT_MMDDYY))
    GUI_Format_Date_Default(settings); */

  self->date_instance = date_instance;
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


/** Begin of:
  * @tableofcontents SECTION_GUI_FORMAT_SENSORS
 **/

#define GUI_FORMAT_SENSOR_MODE_TEMP 0
#define GUI_FORMAT_SENSOR_MODE_HUMI 1
#define GUI_FORMAT_SENSOR_MODE_PRES 2
#define GUI_FORMAT_SENSOR_MODE_TILT 3

static void _GUI_Format_Sensors_Update(GUI_Format *unsafe_self) {
  struct GUI_Format_Sensors *self = (struct GUI_Format_Sensors *)unsafe_self;
  char message[CONFIG_NUM_DIGITS];
  uint8_t dot_overlay = 0;
  switch(self->sensor_mode) {
    case GUI_FORMAT_SENSOR_MODE_TEMP: {
      // Constants in this case
      message[4] = '.';
      dot_overlay = 0b000001000; // Decimal point
      // Slight variations in this case
      int16_t val;
      if(self->unit_temperature == TEMPERATURE_UNIT_CELSIUS) {
        val = self->temperature_instance->celsius_x10;
        message[5] = 'C';
      } else {
        val = self->temperature_instance->fahrenheit_x10;
        message[5] = 'F';
      }
      // Calculated in this case
      uint16_t absval = (val < 0) ? -val : val;
      // Thousands possible due to climate change (F), lower than -999 not handled!!
      uint8_t thousands = absval / 1000;
      uint16_t decimal = absval - thousands * 1000;
      uint8_t hundreds = decimal / 100;
      decimal -= hundreds * 100;
      uint8_t tens = decimal / 10;
      decimal -= tens * 10;
      // Constant in display
      message[2] = tens;
      message[3] = decimal;
      // Special cases
      if(val < 0) {
        message[0] = absval < 100 ? ' ' : '-';
        message[1] = absval < 100 ? '-' : hundreds;
      } else if(val > 1000) {
        message[0] = thousands;
        message[1] = hundreds;
      } else {
        message[0] = ' ';
        message[1] = absval < 100 ? ' ' : hundreds;
      }
      break;
    }
    case GUI_FORMAT_SENSOR_MODE_HUMI: {
      // Read from humidity storage
      uint8_t val = self->humidity_instance->humidity_pct;
      // Decompose h into digits
      uint8_t hundreds = val / 100;
      uint8_t rem = val - hundreds * 100;
      uint8_t tens = rem / 10;
      uint8_t ones = rem - tens * 10;
      // Fill message buffer
      message[0] = 'R';
      message[1] = 'H';
      message[2] = ' ';
      message[3] = (hundreds == 1) ? 1 : ' '; // '0' if < 100
      message[4] = tens;
      message[5] = ones;
      break;
    }
    case GUI_FORMAT_SENSOR_MODE_PRES: {
      // Read from pressure storage
      if (self->unit_pressure == PRESSURE_UNIT_HPA) {
        uint16_t val = self->pressure_instance->hpa_x10; // e.g. 10132 for 1013.2 hPa
        // Wan is 10000 in Chinese and easier to read than tenthousands
        uint8_t wan = val / 10000;
        message[0] = val < 10000 ? ' ' : wan; // Remove LZ
        val -= wan * 10000;
        uint8_t thousands = val / 1000;
        val -= thousands * 1000;
        uint8_t hundreds = val / 100;
        val -= hundreds * 100;
        uint8_t tens = val / 10;
        val -= tens * 10;
        message[1] = thousands;
        message[2] = hundreds;
        message[3] = tens;
        message[4] = val;
        message[5] = 'H'; // 'H' for hPa
        dot_overlay = 0b00000100; // Decimal point
      } else {
        uint16_t val = self->pressure_instance->inhg_x100;
        // Clamp to max 9999, min 0
        if(val > 9999) val = 9999;
        uint8_t thousands = val / 1000;
        val -= thousands * 1000;
        uint8_t hundreds = val / 100;
        val -= hundreds * 100;
        uint8_t tens = val / 10;
        message[0] = thousands;
        message[1] = hundreds;
        message[2] = tens;
        message[3] = val - tens * 10;
        message[4] = 'I';
        message[5] = 'N';
        dot_overlay = 0b00010000; // Decimal point
      }
      break;
    }
    case GUI_FORMAT_SENSOR_MODE_TILT:
    default: {
      int8_t pitch = self->tilt_instance->pitch_deg;
      int8_t roll = self->tilt_instance->roll_deg;
      // Clamp to max +19, min -9
      if(pitch > 19) pitch = 19;
      if(pitch < -9) pitch = -9;
      if(roll > 19) roll = 19;
      if(roll < -9) roll = -9;
      // --- PITCH ---
      message[0] = 'T';
      if(pitch > 9) {
          message[1] = 1;                       // Show '1' for 10–19
          message[2] = pitch - 10;              // e.g. 12 → '2'
      } else if(pitch < 0) {
          message[1] = '-';
          message[2] = (-pitch);                // e.g. -3 → '3'
      } else {
          message[1] = ' ';
          message[2] = pitch;                   // 0–9
      }
      // --- ROLL ---
      message[3] = 'R';
      if(roll > 9) {
          message[4] = 1;                       // 10–19
          message[5] = roll - 10;
      } else if(roll < 0) {
          message[4] = '-';
          message[5] = -roll;
      } else {
          message[4] = ' ';
          message[5] = roll;
      }
      break;
    }
  }
  vfdco_display_render_message(message, dot_overlay, 0);
}

static void _GUI_Format_Sensors_F4(GUI_Format *unsafe_self) {
  struct GUI_Format_Sensors *self = (struct GUI_Format_Sensors *)unsafe_self;
  // Short press on F4 changes the active sensor
  ++self->sensor_mode;
  if(self->sensor_mode > GUI_FORMAT_SENSOR_MODE_TILT) self->sensor_mode = GUI_FORMAT_SENSOR_MODE_TEMP;
  // Message the new sensor
  vfdco_display_render_message(Messages_Sensors_Set_Mode[self->sensor_mode], 0, CONFIG_MESSAGE_SHORT);
}

static void _GUI_Format_Sensors_F4Var(GUI_Format *unsafe_self) {
  struct GUI_Format_Sensors *self = (struct GUI_Format_Sensors *)unsafe_self;
  // Long press on F4 changes the sensor unit when applicable
  if(self->sensor_mode == GUI_FORMAT_SENSOR_MODE_TEMP) {
    if(self->unit_temperature == TEMPERATURE_UNIT_CELSIUS)  self->unit_temperature = TEMPERATURE_UNIT_FAHRENHEIT;
    else                                                      self->unit_temperature = TEMPERATURE_UNIT_CELSIUS;
    vfdco_display_render_message(Messages_Sensors_Set_Temp_Units[(uint8_t)self->unit_temperature], 0, CONFIG_MESSAGE_LONG);
  } else if(self->sensor_mode == GUI_FORMAT_SENSOR_MODE_PRES) {
    if(self->unit_pressure == PRESSURE_UNIT_HPA)            self->unit_pressure = PRESSURE_UNIT_INHG;
    else                                                      self->unit_pressure = PRESSURE_UNIT_HPA;
    vfdco_display_render_message(Messages_Sensors_Set_Pressure_Units[(uint8_t)self->unit_pressure], 0, CONFIG_MESSAGE_LONG);
  }
}

static void _GUI_Format_Sensors_Save(GUI_Format *unsafe_self) {
  struct GUI_Format_Sensors *self = (struct GUI_Format_Sensors *)unsafe_self;
  self->settings[GUI_FORMAT_SETTING_SENSORS_unit_temperature] = self->unit_temperature;
  self->settings[GUI_FORMAT_SETTING_SENSORS_unit_pressure] = self->unit_pressure;
}

void GUI_Format_Sensors_Init(
  struct GUI_Format_Sensors *self,
  vfdco_temperature_t *temperature_instance,
  vfdco_humidity_t *humidity_instance,
  vfdco_pressure_t *pressure_instance,
  vfdco_tilt_t *tilt_instance,
  uint8_t *settings
) {
  self->temperature_instance = temperature_instance;
  self->humidity_instance = humidity_instance;
  self->pressure_instance = pressure_instance;
  self->tilt_instance = tilt_instance;
  self->settings = settings;
  self->unit_temperature = (sensors_temperature_unit_t)self->settings[GUI_FORMAT_SETTING_SENSORS_unit_temperature];
  self->unit_pressure = (sensors_pressure_unit_t)self->settings[GUI_FORMAT_SETTING_SENSORS_unit_pressure];

  GUI_Format_F2 = GUI_Format_FOverride;
  GUI_Format_F3 = GUI_Format_FOverride;
  GUI_Format_F4 = _GUI_Format_Sensors_F4;
  GUI_Format_F2Var = GUI_Format_FOverride;
  GUI_Format_F3Var = GUI_Format_FOverride;
  GUI_Format_F4Var = _GUI_Format_Sensors_F4Var;
  GUI_Format_Update = _GUI_Format_Sensors_Update;
  GUI_Format_Save = _GUI_Format_Sensors_Save;
}


/** Begin of:
  * @tableofcontents SECTION_GUI_FORMAT_TIME_DATE_SET
 **/
static void _GUI_Format_Time_Date_Setter_Update(GUI_Format *unsafe_self) {
  struct GUI_Format_Time_Date_Setter *self = (struct GUI_Format_Time_Date_Setter *)unsafe_self;

  if(Time_Event_Update(&self->blank_timer)) self->blank_active = !self->blank_active;
  uint8_t new_hour, new_minute, new_second;
  if(self->set_mode == 0) {
    new_hour = self->new_time.h;
    new_minute = self->new_time.m;
    new_second = self->new_time.s;
  } else {
    new_hour = self->new_date.d;
    new_minute = self->new_date.m;
    new_second = self->new_date.y;
  }

  self->blank_alt_message[0] = new_hour / 10;
  self->blank_alt_message[1] = new_hour % 10;
  self->blank_alt_message[2] = new_minute / 10;
  self->blank_alt_message[3] = new_minute % 10;
  self->blank_alt_message[4] = new_second / 10;
  self->blank_alt_message[5] = new_second % 10;

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

static void _GUI_Format_Time_Date_Setter_F2(GUI_Format *unsafe_self) {
  struct GUI_Format_Time_Date_Setter *self = (struct GUI_Format_Time_Date_Setter *)unsafe_self;
  // Short press on F2 changes the active parameter (h/m/s)
  ++self->active_digit;
  if(self->active_digit == 3) self->active_digit = 0;
}

static void _GUI_Format_Time_Date_Setter_F3(GUI_Format *unsafe_self) {
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

static void _GUI_Format_Time_Date_Setter_F4(GUI_Format *unsafe_self) {
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

void GUI_Format_Time_Date_Setter_Init(struct GUI_Format_Time_Date_Setter *self, vfdco_time_t *time_instance, vfdco_date_t *date_instance, uint_fast8_t set_mode) {
  self->set_mode = set_mode;
  self->active_digit = 0;
  self->blank_active = 0;
  self->blank_timer = Time_Event_Init(500);

  self->new_time = *time_instance;
  self->new_date = *date_instance;

  GUI_Format_F2 = _GUI_Format_Time_Date_Setter_F2; // sw active
  GUI_Format_F3 = _GUI_Format_Time_Date_Setter_F3; // --
  GUI_Format_F4 = _GUI_Format_Time_Date_Setter_F4; // ++
  GUI_Format_F2Var = GUI_Format_FOverride;
  GUI_Format_F3Var = GUI_Format_FOverride;
  GUI_Format_F4Var = GUI_Format_FOverride;
  GUI_Format_Update = _GUI_Format_Time_Date_Setter_Update;
  GUI_Format_Save = NULL;
}



/** Begin of:
  * @tableofcontents SECTION_GUI_FORMAT_ALARM_SET
 **/
static void _GUI_Format_Alarm_Setter_Update(GUI_Format *unsafe_self) {
  struct GUI_Format_Alarm_Setter *self = (struct GUI_Format_Alarm_Setter *)unsafe_self;

  if(Time_Event_Update(&self->blank_timer)) self->blank_active = !self->blank_active;
  uint8_t digits_hour, digits_minute;
  digits_hour = self->new_time.h;
  digits_minute = self->new_time.m;
  self->blank_alt_message[0] = 'A';
  self->blank_alt_message[1] = 'L';
  self->blank_alt_message[2] = digits_hour / 10;
  self->blank_alt_message[3] = digits_hour % 10;
  self->blank_alt_message[4] = digits_minute / 10;
  self->blank_alt_message[5] = digits_minute % 10;

  if(self->blank_active) {
    if(self->active_digit == 0) {
      self->blank_alt_message[2] = ' ';
      self->blank_alt_message[3] = ' ';
    } else if(self->active_digit == 1) {
      self->blank_alt_message[4] = ' ';
      self->blank_alt_message[5] = ' ';
    }
  }
  vfdco_display_render_message(self->blank_alt_message, 0, 0);
}

static void _GUI_Format_Alarm_Setter_F2(GUI_Format *unsafe_self) {
  struct GUI_Format_Alarm_Setter *self = (struct GUI_Format_Alarm_Setter *)unsafe_self;
  // Short press on F2 changes the active parameter (h/m/s)
  ++self->active_digit;
  if(self->active_digit == 2) self->active_digit = 0;
}

static void _GUI_Format_Alarm_Setter_F3(GUI_Format *unsafe_self) {
  struct GUI_Format_Alarm_Setter *self = (struct GUI_Format_Alarm_Setter *)unsafe_self;
  // Short press on F3 decreases the active parameter (h/m/s)
  if      (self->active_digit == 0) { // Set hour
    if       (self->new_time.h  > 0)    self->new_time.h--;
    else if  (self->new_time.h == 0)    self->new_time.h = 23;
  }
  else if (self->active_digit == 1) { // Set minute
    if       (self->new_time.m  > 0)    self->new_time.m--;
    else if  (self->new_time.m == 0)    self->new_time.m = 59;
  }
}

static void _GUI_Format_Alarm_Setter_F3Var(GUI_Format *unsafe_self) {
  struct GUI_Format_Alarm_Setter *self = (struct GUI_Format_Alarm_Setter *)unsafe_self;
  // Long press on F4 changes alarm auto-off
  ++self->auto_off_option;
}

static void _GUI_Format_Alarm_Setter_F4(GUI_Format *unsafe_self) {
  struct GUI_Format_Alarm_Setter *self = (struct GUI_Format_Alarm_Setter *)unsafe_self;
  // Short press on F4 increases the active parameter (h/m/s)
  if      (self->active_digit == 0) { // Set hour
    if       (self->new_time.h  < 23)    self->new_time.h++;
    else if  (self->new_time.h == 23)    self->new_time.h = 0;
  }
  else if (self->active_digit == 1) { // Set minute
    if       (self->new_time.m  < 59)    self->new_time.m++;
    else if  (self->new_time.m == 59)    self->new_time.m = 0;
  }
}

static void _GUI_Format_Alarm_Setter_F4Var(GUI_Format *unsafe_self) {
  struct GUI_Format_Alarm_Setter *self = (struct GUI_Format_Alarm_Setter *)unsafe_self;
  ++self->snooze_option;
  if(self->snooze_option == 0) {
    vfdco_display_render_message((char[]) {'S','N',' ','O','F','F'}, 0, CONFIG_MESSAGE_LONG);
  } else if(self->snooze_option == 6) {
    self->snooze_option = 0;
  } else {
    vfdco_display_render_message((char[]) {'A','L','S','N', Alarm_Snooze_Options[self->snooze_option] / 10, Alarm_Snooze_Options[self->snooze_option] % 10}, 0, CONFIG_MESSAGE_LONG);
  }
}

void GUI_Format_Alarm_Setter_Init(struct GUI_Format_Alarm_Setter *self, uint8_t *settings) {
  self->active_digit = 0;
  self->blank_active = 0;
  self->blank_timer = Time_Event_Init(500);

  self->settings = settings;
  self->new_time.h = self->settings[_SHARED_alarm_h];
  self->new_time.m = self->settings[_SHARED_alarm_m];
  self->snooze_option = self->settings[_SHARED_alarm_snooze_option]; // TODO snooze times
  self->auto_off_option = self->settings[_SHARED_alarm_auto_off_option];

  GUI_Format_F2 = _GUI_Format_Alarm_Setter_F2; // sw active
  GUI_Format_F3 = _GUI_Format_Alarm_Setter_F3; // --
  GUI_Format_F4 = _GUI_Format_Alarm_Setter_F4; // ++
  GUI_Format_F2Var = GUI_Format_FOverride;
  GUI_Format_F3Var = GUI_Format_FOverride;
  GUI_Format_F4Var = _GUI_Format_Alarm_Setter_F4Var; // Snooze
  GUI_Format_Update = _GUI_Format_Alarm_Setter_Update;
  GUI_Format_Save = NULL;
}




/** Begin of:
  * @tableofcontents SECTION_GUI_FORMAT_STOPWATCH
 **/
static void _GUI_Format_Stopwatch_Update(GUI_Format *unsafe_self) {
  struct GUI_Format_Stopwatch *self = (struct GUI_Format_Stopwatch *)unsafe_self;
  if(self->stopwatch_state == GUI_FORMAT_STOPWATCH_STATE_INITIALIZED) {
    char zeros[CONFIG_NUM_DIGITS] = {0};
    vfdco_display_render_message(zeros, 0b000010100, 0);
  }
  else if(self->stopwatch_state == GUI_FORMAT_STOPWATCH_STATE_RUNNING) {
    // running time: elapsed base + current time - start timestamp
    uint32_t running_time = self->elapsed_time +
      ( self->time_instance->h * 3600 + self->time_instance->m * 60 + self->time_instance->s) -
      ( self->initial_time.h * 3600   + self->initial_time.m * 60   + self->initial_time.s);

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

static void _GUI_Format_Stopwatch_F2(GUI_Format *unsafe_self) {
  struct GUI_Format_Stopwatch *self = (struct GUI_Format_Stopwatch *)unsafe_self;
  // Initialize
  if(self->stopwatch_state == GUI_FORMAT_STOPWATCH_STATE_INITIALIZED) {
    if(!self->not_initial) {
      self->initial_time.h = self->time_instance->h;
      self->initial_time.m = self->time_instance->m;
      self->initial_time.s = self->time_instance->s;
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

static void _GUI_Format_Stopwatch_F3(GUI_Format *unsafe_self) {
  struct GUI_Format_Stopwatch *self = (struct GUI_Format_Stopwatch *)unsafe_self;
  // Pause
  if(self->stopwatch_state == GUI_FORMAT_STOPWATCH_STATE_RUNNING) {
    // Save seconds
    self->elapsed_time +=
      (        self->time_instance->h * 3600 +         self->time_instance->m * 60 +         self->time_instance->s) -
      ( self->initial_time.h * 3600 +  self->initial_time.m * 60 +  self->initial_time.s);

    // Save milliseconds
    self->elapsed_milliseconds += vfdco_time_get_milliseconds() - self->initial_milliseconds;
    self->stopwatch_state = GUI_FORMAT_STOPWATCH_STATE_PAUSED;

  }
  // Resume
  else if(self->stopwatch_state == GUI_FORMAT_STOPWATCH_STATE_PAUSED) {
    self->initial_time.h = self->time_instance->h;
    self->initial_time.m = self->time_instance->m;
    self->initial_time.s = self->time_instance->s;
    #ifndef DEBUG
    self->initial_milliseconds = vfdco_time_get_milliseconds();
    #endif
    self->stopwatch_state = GUI_FORMAT_STOPWATCH_STATE_RUNNING;
  }
}

void GUI_Format_Stopwatch_Init(struct GUI_Format_Stopwatch *self, vfdco_time_t *time_instance) {
  self->stopwatch_state = GUI_FORMAT_STOPWATCH_STATE_INITIALIZED;
  self->time_instance = time_instance;
  self->not_initial = 0;
  self->elapsed_time = 0;
  self->initial_milliseconds = 0;
  self->elapsed_milliseconds = 0;

  GUI_Format_F2 = _GUI_Format_Stopwatch_F2;
  GUI_Format_F3 = _GUI_Format_Stopwatch_F3;
  GUI_Format_F4 = GUI_Format_FOverride;
  GUI_Format_F2Var = GUI_Format_FOverride;
  GUI_Format_F3Var = GUI_Format_FOverride;
  GUI_Format_F4Var = GUI_Format_FOverride;
  GUI_Format_Update = _GUI_Format_Stopwatch_Update;
  GUI_Format_Save = NULL;
}



/**
 * @tableofcontents SECTION_GUI_BRIGHTNESS_SETTER
 * F2 to set display brightness
 * F3 to set LED brightness
 * F4 to set night shift options
 * F4Var to disable night shift, if enabled
 */

static void _GUI_Format_Brightness_Setter_Update(GUI_Format *unsafe_self) {
  struct GUI_Format_Brightness_Setter *self = (struct GUI_Format_Brightness_Setter *)unsafe_self;
  if(Time_Event_Update(&self->message_timer)) {
    // Idle. Waiting for F2/F3/F4/F4Var
    // Just cycle through all the messages
    uint8_t message_counter = self->message_counter;
    vfdco_display_render_message(
      Messages_Brightness_Set_Idle[message_counter],
      Messages_Brightness_Set_Idle_Dots[message_counter],
      0
    );
    ++self->message_counter;
    if(self->message_counter == MESSAGES_BRIGHTNESS_SET_IDLE_MAX) self->message_counter = 0;
  }
}

static void _GUI_Format_Brightness_Setter_F2(GUI_Format *unsafe_self) {
  struct GUI_Format_Brightness_Setter *self = (struct GUI_Format_Brightness_Setter *)unsafe_self;
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
}

static void _GUI_Format_Brightness_Setter_F3(GUI_Format *unsafe_self) {
  struct GUI_Format_Brightness_Setter *self = (struct GUI_Format_Brightness_Setter *)unsafe_self;
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
}

static void _GUI_Format_Brightness_Setter_F4(GUI_Format *unsafe_self) {
  struct GUI_Format_Brightness_Setter *self = (struct GUI_Format_Brightness_Setter *)unsafe_self;
  self->night_shift_changed = 1;

  ++self->night_shift_state;
  if(self->night_shift_state == MESSAGES_BRIGHTNESS_SET_NSH) self->night_shift_state = 0;

  self->night_shift_new_start_h = Brightness_Set_Night_Shift_Times[self->night_shift_state][0]; // Get start hour
  self->night_shift_new_end_h   = Brightness_Set_Night_Shift_Times[self->night_shift_state][1]; // Get stop hour

  // "NIGHT" "SHIFT", parameter
  vfdco_display_render_message(Messages_Brightness_Set_Idle[4], 0x00, CONFIG_MESSAGE_LONG);
  vfdco_display_render_message(Messages_Brightness_Set_Idle[5], 0x00, CONFIG_MESSAGE_LONG);
  vfdco_display_render_message(Messages_Brightness_Set_Nsh[self->night_shift_state], 0x00, CONFIG_MESSAGE_LONG);

  // If not off, tell when it starts
  if(self->night_shift_state) {
    char _msg[CONFIG_NUM_DIGITS] = {
      Brightness_Set_Night_Shift_Times[self->night_shift_state][0] / 10, // Start hour dec
      Brightness_Set_Night_Shift_Times[self->night_shift_state][0] % 10, // Start hour
      '-', ' ',
      Brightness_Set_Night_Shift_Times[self->night_shift_state][1] / 10, // Stop hour dec
      Brightness_Set_Night_Shift_Times[self->night_shift_state][1] % 10, // Stop hour
    };
    vfdco_display_render_message(_msg, 0x00, CONFIG_MESSAGE_LONG);
  }
}

void GUI_Format_Brightness_Setter_Init(struct GUI_Format_Brightness_Setter *self, const uint8_t *shared_initializer) {
  // Initialize potential new NSH times with existing NSH times
  self->dim_factor_display = shared_initializer[_SHARED_dim_factor_display];
  self->dim_factor_led = shared_initializer[_SHARED_dim_factor_led];

  self->night_shift_new_start_h = shared_initializer[_SHARED_dim_factor_night_shift_start_h];
  self->night_shift_new_end_h = shared_initializer[_SHARED_dim_factor_night_shift_end_h];
  self->night_shift_state = 0;
  self->night_shift_changed = 0;

  self->message_timer = Time_Event_Init(800);
  self->message_counter = 0;

  GUI_Format_F2 = _GUI_Format_Brightness_Setter_F2;
  GUI_Format_F3 = _GUI_Format_Brightness_Setter_F3;
  GUI_Format_F4 = _GUI_Format_Brightness_Setter_F4;
  GUI_Format_F2Var = GUI_Format_FOverride;
  GUI_Format_F3Var = GUI_Format_FOverride;
  GUI_Format_F4Var = GUI_Format_FOverride;
  GUI_Format_Update = _GUI_Format_Brightness_Setter_Update;
  GUI_Format_Save = NULL;
}


/**
 * @tableofcontents SECTION_GUI_DIGIT_FADE_SETTER
 * F2 to set
 */

 static void _GUI_Format_Digit_Fade_Setter_Update(GUI_Format *unsafe_self) {
  struct GUI_Format_Digit_Fade_Setter *self = (struct GUI_Format_Digit_Fade_Setter *)unsafe_self;
  if(Time_Event_Update(&self->message_timer)) {
    // Idle. Waiting for F2/F3/F4/F4Var
    // Just cycle through all the messages
    uint8_t message_counter = self->message_counter;
    vfdco_display_render_message( Messages_Digit_Fade_Set_Idle[message_counter], 0x00, 0);
    ++self->message_counter;
    if(self->message_counter == MESSAGES_DIGIT_FADE_SET_IDLE_MAX) self->message_counter = 0;
  }
}

static void _GUI_Format_Digit_Fade_Setter_F2(GUI_Format *unsafe_self) {
  struct GUI_Format_Digit_Fade_Setter *self = (struct GUI_Format_Digit_Fade_Setter *)unsafe_self;
  if(self->digit_fade_mode == CONFIG_DIGIT_FADE_NONE) {
    self->digit_fade_mode = CONFIG_DIGIT_FADE_CROSS;
  } else if(self->digit_fade_mode == CONFIG_DIGIT_FADE_CROSS) {
    self->digit_fade_mode = CONFIG_DIGIT_FADE_BLEND;
  } else {
    self->digit_fade_mode = CONFIG_DIGIT_FADE_NONE;
  }
  vfdco_display_render_message(Messages_Digit_Fade_Set[self->digit_fade_mode], 0x00, CONFIG_MESSAGE_SHORT);
  self->message_counter = 0; // Resume message cycling
}

void GUI_Format_Digit_Fade_Setter_Init(struct GUI_Format_Digit_Fade_Setter *self, const uint8_t *shared_initializer) {
  self->digit_fade_mode = shared_initializer[_SHARED_digit_fade_mode];

  self->message_timer = Time_Event_Init(800);
  self->message_counter = 0;

  GUI_Format_F2 = _GUI_Format_Digit_Fade_Setter_F2;
  GUI_Format_F3 = GUI_Format_FOverride;
  GUI_Format_F4 = GUI_Format_FOverride;
  GUI_Format_F2Var = GUI_Format_FOverride;
  GUI_Format_F3Var = GUI_Format_FOverride;
  GUI_Format_F4Var = GUI_Format_FOverride;
  GUI_Format_Update = _GUI_Format_Digit_Fade_Setter_Update;
  GUI_Format_Save = NULL;
}


/** Begin of:
  * @tableofcontents SECTION_CONTAINER_GUI
**/
void Container_GUI_Clear(GUI_Format *self) {
  memset(self, 0, sizeof(GUI_Format));
}



/** Begin of:
 * @tableofcontents SECTION_SUPPORTING_FUNCTIONS_IMPLEMENTATION
 **/
/**
 * @brief Returns singular uint32_t expression of time and date
 */
static const uint8_t days_in_month[12]= {
  31, 28, 31, 30, 31, 30,
  31, 31, 30, 31, 30, 31
};

static inline uint8_t is_leap_year(uint8_t year) {
  return (year % 4 == 0); // Valid for 2000–2099
}

uint32_t vfdco_time_epoch_seconds(const vfdco_date_t *date, const vfdco_time_t *time) {
  uint32_t total_days = 0;
  for(uint_fast8_t y = 0; y < date->y; ++y) {
    total_days += is_leap_year(y) ? 366 : 365;
  }
  for(uint8_t m = 0; m < date->m - 1; ++m) {
    total_days += days_in_month[m];
    if(m == 1 && is_leap_year(date->y)) total_days += 1;
  }
  total_days += (date->d - 1); // d is 1-based
  // Convert to seconds
  uint32_t total_seconds = total_days * 86400UL;
  total_seconds += (uint32_t)time->h * 3600UL;
  total_seconds += (uint32_t)time->m * 60UL;
  total_seconds += time->s;
  return total_seconds;
}


// Rip a line from a page watch it fade up in smoke hope rise through the rain higher than we'd ever go
