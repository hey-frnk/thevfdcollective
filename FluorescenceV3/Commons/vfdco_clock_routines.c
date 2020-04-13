/**
  ******************************************************************************
  * @file     vfdco_routines.c
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     12-February-2020
  * @brief    Each (peripheral) component has an initializer and a routine function
  *           The routine function is desiged to be called periodically in main()
  *           The initializers and routines are defined here
  *           Designed to be used with Fluorescence by The VFD Collective
  ******************************************************************************
  * @tableofcontents Table of contents, enter to navigate:
  * TODO
  ******************************************************************************
 **/

// Here everything comes together!
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifndef __AVR__
  // Configuration header
  #include "../vfdco_config.h"
  // Low Level/Primitive libraries
  #include "../vfdco_serialization.h"		// Physical serialization driver
  #include "../vfdco_led.h"             // Physical LED driver
  #include "../vfdco_display.h"         // Physical display driver
  #include "../vfdco_time.h"            // Physical timing & RTC driver
  #include "../vfdco_hid.h"             // Physical HID driver
  #include "../vfdco_com.h"             // Physical Communication driver
  #include "../vfdco_mic.h"             // Physical Microphone driver

#else 
  // Configuration header
  #include "vfdco_config.h"
  // Low Level/Primitive libraries
  #include "vfdco_serialization.h"	 // Physical serialization driver
  #include "vfdco_led.h"             // Physical LED driver
  #include "vfdco_display.h"         // Physical display driver
  #include "vfdco_time.h"            // Physical timing & RTC driver
  #include "vfdco_hid.h"             // Physical HID driver
  #include "vfdco_com.h"             // Physical Communication driver
  #include "vfdco_mic.h"             // Physical Microphone driver
#endif

// Application specific libraries
#include "vfdco_lights.h"          // Library of light patterns
#include "vfdco_gui.h"             // Library of different user interfaces
// Clock routine scheduler
#include "vfdco_clock_routines.h"  // Clock routine

// Globally accessible parameters
static time_event_t display_updater;

static vfdco_time_t global_time;
static vfdco_date_t global_date;
static time_event_t global_time_updater;

// HID variables
static uint8_t global_button_F1_state = BUTTON_STATE_OFF;
static uint8_t global_button_F2_state = BUTTON_STATE_OFF;
static uint8_t global_button_F3_state = BUTTON_STATE_OFF;
static uint8_t global_button_F4_state = BUTTON_STATE_OFF;
#define GLOBAL_CLEAR_BUTTON(_button) _button = BUTTON_STATE_OFF

// GUI variables
GUI_Format global_gui_instance;
gui_instance_t global_gui_instance_counter;
#define GLOBAL_SET_NEXT_GUI_INSTANCE(_counter) {global_gui_instance_counter = (_counter);}

// Light Pattern variables
Light_Pattern global_light_instance;
light_pattern_instance_t global_light_instance_counter;
light_pattern_instance_t global_light_instance_random;
#define GLOBAL_SET_NEXT_LIGHT_INSTANCE(_counter) {global_light_instance_counter = (_counter);}
#define GLOBAL_SET_NEXT_RANDOM_INSTANCE(_counter) {global_light_instance_random = (_counter);}

// Random definitions
#define GLOBAL_LIGHT_INSTANCE_RANDOM_OFF 255
#define GLOBAL_LIGHT_INSTANCE_RANDOM_IS_ON (global_light_instance_random != GLOBAL_LIGHT_INSTANCE_RANDOM_OFF)

#define GLOBAL_ITERABLE_INSTANCE_IS_ENABLED(_instance)     ((SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_global_light_it_register]) &   (1 << (_instance)))
#define GLOBAL_ITERABLE_INSTANCE_ENABLE(_instance)         ((SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_global_light_it_register]) |=  (1 << (_instance)))
#define GLOBAL_ITERABLE_INSTANCE_DISABLE(_instance)        ((SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_global_light_it_register]) &= ~(1 << (_instance)))
#define GLOBAL_RANDOM_INSTANCE_IS_ENABLED(_instance)     ((SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_global_light_rnd_register])  &   (1 << (_instance)))
#define GLOBAL_RANDOM_INSTANCE_ENABLE(_instance)         ((SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_global_light_rnd_register])  |=  (1 << (_instance)))
#define GLOBAL_RANDOM_INSTANCE_DISABLE(_instance)        ((SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_global_light_rnd_register])  &= ~(1 << (_instance)))

// Clock power variables
static night_shift_state_t global_night_shift_state;

// COM variables
struct COM_Data global_com_data;

// Messaging constants
static const char Messages_Routine_Settings[CONFIG_NUM_DIGITS]   = {'S', 'E', 'T', 'I', 'N', 'G'};
static const char Messages_Routine_Default1[CONFIG_NUM_DIGITS]   = {'D', 'E', 'F', 'A', 'U', 'L'};
static const char Messages_Routine_Default2[CONFIG_NUM_DIGITS]   = {'R', 'E', 'T', 'O', 'R', 'D'};
static const char Messages_Routine_Saved1[CONFIG_NUM_DIGITS]     = {'A', 'L', 'L', ' ', ' ', ' '};
static const char Messages_Routine_Saved2[CONFIG_NUM_DIGITS]     = {'S', 'A', 'V', 'E', 'D', ' '};
static const char Messages_Routine_RandomOn[CONFIG_NUM_DIGITS]   = {'R', 'N', 'D', ' ', 'O', 'N'};
static const char Messages_Routine_RandomOff[CONFIG_NUM_DIGITS]  = {'R', 'N', 'D', 'O', 'F', 'F'};
static const char Messages_Routine_Inst[CONFIG_NUM_DIGITS]       = {'E', 'N', 'A', 'B', 'L', 'E'};
static const char Messages_Routine_Rnd[CONFIG_NUM_DIGITS]        = {'R', 'A', 'N', 'D', ' ', ' '};
static const char Messages_Routine_Set[CONFIG_NUM_DIGITS]        = {'S', 'E', 'T', ' ', ' ', ' '};
static const char Messages_Routine_Night[CONFIG_NUM_DIGITS]      = {' ', 'N', 'I', 'G', 'H', 'T'};
static const char Messages_Routine_Shift[CONFIG_NUM_DIGITS]      = {' ', 'S', 'H', 'I', 'F', 'T'};

/** Begin of:
  * @tableofcontents SECTION_SUPPORTING_FUNCTIONS
  * @brief Prototypes. Implementation see very end
 **/
static void vfdco_welcome(char *message);
static void set_next_gui_instance_timeset(uint8_t set_mode); // Fancy special case
static void set_next_gui_instance(gui_instance_t next_instance);
static void find_next_lights_instance();
static void set_next_lights_instance(light_pattern_instance_t next_instance);

static void com_decoder(uint8_t *input_buffer, void (*legacy_com_encoder)(struct COM_Data *));


/** Begin of:
  * @tableofcontents SECTION_GLOBAL_INITIALIZER
 **/
void vfdco_clock_initializer() {
  global_time_updater = Time_Event_Init(CONFIG_RTC_UPDATE_INTERVAL);
  display_updater = Time_Event_Init(CONFIG_DISPLAY_UPDATE_INTERVAL);

  // Read in all saved settings from (de)serializer
  vfdco_clock_serialization_initializer();
  vfdco_clock_hid_initializer();
  vfdco_clock_time_initializer();
  vfdco_clock_power_initializer();
  vfdco_clock_gui_initializer();
  vfdco_clock_lights_initializer();
  vfdco_clock_com_initializer();
  vfdco_clock_mic_initializer();
  
  // All good? All good! Fluorescence, say hello!
  vfdco_welcome((char *)SERIALIZABLE_CLOCK_ROUTINE_arr + CLOCK_ROUTINE_SETTING_welcome);

  // Random seed
  for(uint8_t i = 0; i < 7; ++i) vfdco_util_random(i);
}

/** Begin of:
  * @tableofcontents SECTION_PERIPHERAL_INITIALIZERS
 **/
inline void vfdco_clock_serialization_initializer() {
  // Load all clock parameters
  SERIALIZATION_HEADER_STATUS_t read_status = vfdco_serialization_read(serialized_settings, serialized_settings_sizes, NUM_SERIALIZABLE);
  if(read_status != SERIALIZATION_HEADER_STATUS_OK) {
    // Something went wrong. Load default settings!
    vfdco_clock_settings_default(1);
  }
}

inline void vfdco_clock_hid_initializer() {
  // Init HID configuration
  vfdco_hid_init();
}

inline void vfdco_clock_time_initializer() {
  // Real time clock or time initializer 
  vfdco_rtc_init();
}

inline void vfdco_clock_power_initializer() {
  // Start by night shift off
  global_night_shift_state = NIGHT_SHIFT_OFF;
}

inline void vfdco_clock_gui_initializer() {
  // Initialize display driver first, then GUI
  vfdco_display_init(SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_dim_factor_display]);
  // Start by creating a time instance
  set_next_gui_instance(GUI_TIME);
}

inline void vfdco_clock_lights_initializer() {
  // Initialize LED driver first, then Light Patterns
  vfdco_clr_init(SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_dim_factor_led]);
  // Start by loading the saved lights instance
  GLOBAL_SET_NEXT_RANDOM_INSTANCE(SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_global_light_instance_random]);
  set_next_lights_instance((light_pattern_instance_t)SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_global_light_instance_counter]);
}

inline void vfdco_clock_com_initializer() {
  // USB virtual com port init
  COM_Handler_USB_Init();

  global_com_data.rx_buffer_data_present = 0;
  global_com_data.rx_buffer_length = CONFIG_COM_RX_BUF_MAX;
  global_com_data.tx_buffer_length = CONFIG_COM_TX_BUF_MAX;
  memset(global_com_data.rx_buffer, 0x00, global_com_data.rx_buffer_length);
  global_com_data.tx_buffer = NULL;
}

inline void vfdco_clock_mic_initializer() {
  // Initialize driver
  vfdco_mic_init();
}

/** Begin of:
  * @tableofcontents SECTION_GLOBAL_ROUTINE
 **/
inline void vfdco_clock_routine() {
  vfdco_clock_hid_routine();
  vfdco_clock_time_routine();
  vfdco_clock_power_routine();
  vfdco_clock_gui_routine();
  vfdco_clock_lights_routine();
  vfdco_clock_com_routine();
}

/** Begin of:
  * @tableofcontents SECTION_PERIPHRAL_ROUTINES
 **/
// Human interface device (Buttons) routine
inline void vfdco_clock_hid_routine() {
  vfdco_hid_button_retrieve_all(
    &global_button_F1_state,
    &global_button_F2_state,
    &global_button_F3_state,
    &global_button_F4_state
  );
}

// RTC time refresh routine
inline void vfdco_clock_time_routine() {
  if(Time_Event_Update(&global_time_updater)) {
    vfdco_get_date_time(&global_date, &global_time);
  }
}

inline void vfdco_clock_power_routine() {
  // If night shift (power saving mode) is scheduled to start at some time
  if(!( (SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_night_shift_start_h] == 0) &&
        (SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_night_shift_start_m] == 0) &&
        (SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_night_shift_end_h] == 0)   &&
        (SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_night_shift_end_m] == 0)
  )) {
    // Check if night shift can be set
    if(global_night_shift_state == NIGHT_SHIFT_ON) {
      // Night shift is on
      // Look for implicit wake trigger
      if(global_time.h == SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_night_shift_end_h]) { // if statement reduction
        if(global_time.m == SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_night_shift_end_m]) {
          if(global_time.s < 3) {                   // Safe switching time span. Bug (which is ok): Waking when s < 3!
            // Implicit wake: Restore previos brightness values
            vfdco_display_set_dim_factor(SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_dim_factor_display]);
            vfdco_clr_set_dim_factor(SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_dim_factor_led]);
            global_night_shift_state = NIGHT_SHIFT_OFF;
          }
        }
      }
      // F1 long press: Explicit wake trigger
      if(global_button_F1_state == BUTTON_STATE_LONGPRESS) {
        vfdco_display_set_dim_factor(SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_dim_factor_display]);
        vfdco_clr_set_dim_factor(SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_dim_factor_led]);
        global_night_shift_state = NIGHT_SHIFT_OFF;
        GLOBAL_CLEAR_BUTTON(global_button_F1_state); // Priority clear
      }
    } else {
      // Night shift is off
      // Look for enable trigger
      if(global_time.h == SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_night_shift_start_h]) { // if statement reduction
        if(global_time.m == SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_night_shift_start_m]) {
          if(global_time.s < 3) {                   // Safe switching time span. Bug (which is ok): Waking when s < 3!
            // Implicit wake: Restore previos brightness values
            vfdco_display_set_dim_factor(CONFIG_BRIGHTNESS_MIN);
            vfdco_clr_set_dim_factor(CONFIG_BRIGHTNESS_MIN);
            global_night_shift_state = NIGHT_SHIFT_ON;
          }
        }
      }
    }
  }
} 

// VFD display data render routine
inline void vfdco_clock_gui_routine() {
  if(Time_Event_Update(&display_updater)) GUI_Format_Update(&global_gui_instance);
  if(global_button_F1_state == BUTTON_STATE_SHORTPRESS) {
    /* F1: Change GUI, done in 4 steps:
        - (1) Save settings (internal destructor call)
        - (2) Apply set parameters (external destructor call)
        - (3) Dealloc
        - (4) Initialize (internal constructor call)
    */
    // (1) Save settings (internal destructor call)
    if(GUI_Format_Save) GUI_Format_Save(&global_gui_instance);
    // (2) Apply set parameters (external destructor call)
    switch(global_gui_instance_counter) {
      case GUI_TIME_DATE_SET: {
        struct GUI_Format_Time_Date_Setter *self = (struct GUI_Format_Time_Date_Setter*)&global_gui_instance;
        if(self->set_mode == 0) vfdco_set_date_time(&global_date,    &self->new_time);
        else                    vfdco_set_date_time(&self->new_date, &global_time   );
        break;
      }
      case GUI_BRIGHTNESS_SET: {
        struct GUI_Format_Brightness_Setter *self = (struct GUI_Format_Brightness_Setter*)&global_gui_instance;
        vfdco_display_set_dim_factor(self->dim_factor_display);
        vfdco_clr_set_dim_factor(self->dim_factor_led);
        SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_dim_factor_display] = self->dim_factor_display;
        SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_dim_factor_led] = self->dim_factor_led;
        if(self->night_shift_changed) {
          SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_night_shift_start_h] = self->night_shift_new_start_h;
          SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_night_shift_start_m] = 0;
          SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_night_shift_end_h] = self->night_shift_new_end_h;
          SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_night_shift_end_m] = 0;
        }
        break;
      }
      default: break;
    }
    // (3) Dealloc
    Container_GUI_Clear(&global_gui_instance);
    // (4) Initialize (internal constructor call)
    switch(global_gui_instance_counter) {
      case GUI_TIME:            { set_next_gui_instance(GUI_DATE);            break; }
      case GUI_DATE:            { set_next_gui_instance(GUI_STOPWATCH);       break; }
      case GUI_STOPWATCH:       { set_next_gui_instance(GUI_BRIGHTNESS_SET);  break; }
      case GUI_BRIGHTNESS_SET:  { set_next_gui_instance(GUI_TIME);            break; }
      case GUI_TIME_DATE_SET:   { set_next_gui_instance(GUI_TIME);            break; }
      default: break;
    }
    GLOBAL_CLEAR_BUTTON(global_button_F1_state); // Priority clear
  } else if(global_button_F1_state == BUTTON_STATE_LONGPRESS) {
    if((global_gui_instance_counter == GUI_TIME) || (global_gui_instance_counter == GUI_DATE)) {
      // To time set menu: Save settings
      if(GUI_Format_Save) GUI_Format_Save(&global_gui_instance);
      Container_GUI_Clear(&global_gui_instance);
      if      (global_gui_instance_counter ==  GUI_TIME) set_next_gui_instance_timeset(0); 
      else if (global_gui_instance_counter ==  GUI_DATE) set_next_gui_instance_timeset(1); 
    }
    GLOBAL_CLEAR_BUTTON(global_button_F1_state); // Priority clear
  }

  switch(global_button_F2_state) {
    case BUTTON_STATE_SHORTPRESS:   if(GUI_Format_F2)         {GUI_Format_F2(&global_gui_instance);     GLOBAL_CLEAR_BUTTON(global_button_F2_state);} break;
    case BUTTON_STATE_LONGPRESS:    if(GUI_Format_F2Var)      {GUI_Format_F2Var(&global_gui_instance);  GLOBAL_CLEAR_BUTTON(global_button_F2_state);} break;
  }
  switch(global_button_F3_state) {
    case BUTTON_STATE_SHORTPRESS:   if(GUI_Format_F3)         {GUI_Format_F3(&global_gui_instance);     GLOBAL_CLEAR_BUTTON(global_button_F3_state);} break;
    case BUTTON_STATE_LONGPRESS:    if(GUI_Format_F3Var)      {GUI_Format_F3Var(&global_gui_instance);  GLOBAL_CLEAR_BUTTON(global_button_F3_state);} break;
  }
  switch(global_button_F4_state) {
    case BUTTON_STATE_SHORTPRESS: {
      if(global_gui_instance_counter == GUI_STOPWATCH) vfdco_clock_settings_save(0);
      else if(GUI_Format_F4) GUI_Format_F4(&global_gui_instance);
      GLOBAL_CLEAR_BUTTON(global_button_F4_state); 
      break;
    }
    case BUTTON_STATE_LONGPRESS: {
      if(global_gui_instance_counter == GUI_STOPWATCH) vfdco_clock_settings_default(0);
      else if(GUI_Format_F4Var) GUI_Format_F4Var(&global_gui_instance);
      GLOBAL_CLEAR_BUTTON(global_button_F4_state); 
      break;
    }
  }
}

// VFD LED light illumination routine
inline void vfdco_clock_lights_routine() {
  Light_Pattern_Update(&global_light_instance);
  if(global_button_F2_state == BUTTON_STATE_SHORTPRESS) {
    if(!GLOBAL_LIGHT_INSTANCE_RANDOM_IS_ON) {
      find_next_lights_instance();
      if(Light_Pattern_Hello) Light_Pattern_Hello();
    }
    GLOBAL_CLEAR_BUTTON(global_button_F2_state); // Priority clear
  } else if(global_button_F2_state == BUTTON_STATE_LONGPRESS) {
    if(GLOBAL_LIGHT_INSTANCE_RANDOM_IS_ON) { // If random state is on, turn off
      GLOBAL_SET_NEXT_RANDOM_INSTANCE(GLOBAL_LIGHT_INSTANCE_RANDOM_OFF);
      vfdco_display_render_message(Messages_Routine_RandomOff, 0, CONFIG_MESSAGE_LONG);
      find_next_lights_instance();
    } else { // If random stoff is off, turn on!
      GLOBAL_SET_NEXT_RANDOM_INSTANCE(LIGHT_PATTERN_STATIC);
      find_next_lights_instance();
      vfdco_display_render_message(Messages_Routine_RandomOn, 0, CONFIG_MESSAGE_LONG);
    }
    GLOBAL_CLEAR_BUTTON(global_button_F2_state); // Priority clear
  }
  switch(global_button_F3_state) {
    case BUTTON_STATE_SHORTPRESS:
      if(Light_Pattern_F3) { Light_Pattern_F3(&global_light_instance); GLOBAL_CLEAR_BUTTON(global_button_F3_state); } 
      break;
    case BUTTON_STATE_LONGPRESS:
      if(Light_Pattern_F3Var) { Light_Pattern_F3Var(&global_light_instance); GLOBAL_CLEAR_BUTTON(global_button_F3_state); }
      break;
    default: break;
  }

  // Randomizer on? Randomize next instance!
  if(GLOBAL_LIGHT_INSTANCE_RANDOM_IS_ON) {
    static uint8_t prev_s = 0; // Last second
    uint8_t trigger_rnd = 0; // Random trigger
    if((prev_s == 58) && (global_time.s == 59)) {
      switch(SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_global_light_rnd_speed]) {
        case CONFIG_RANDOM_SPEED_1_ONE_MINUTE: {                              trigger_rnd = 1; break; }
        case CONFIG_RANDOM_SPEED_2_TWO_MINUTES: { if(!(global_time.m & 0x01)) trigger_rnd = 1; break; }
        case CONFIG_RANDOM_SPEED_3_TEN_MINUTES: { if(global_time.m % 10 == 0) trigger_rnd = 1; break; }
        case CONFIG_RANDOM_SPEED_4_HALF_HOUR: {   if(global_time.m % 30 == 0) trigger_rnd = 1; break; }
        case CONFIG_RANDOM_SPEED_5_HOUR: {        if(global_time.m == 0)      trigger_rnd = 1; break; }
      }
    }
    if(trigger_rnd) find_next_lights_instance();
    prev_s = global_time.s;
  }
}

// Communication (Serial/USB, Serial/Bluetooth) routine
inline void vfdco_clock_com_routine() {
  // If data is present
  if(global_com_data.rx_buffer_data_present == RX_BUFFER_DATA_USB_BUSY) {
    // Perform decoding, transfer back using USB transfer
    com_decoder(global_com_data.rx_buffer, COM_Handler_USB_Transfer);
    global_com_data.rx_buffer_data_present = RX_BUFFER_DATA_IDLE;
  } else if(global_com_data.rx_buffer_data_present == RX_BUFFER_DATA_BT_BUSY) {
    // Perform decoding, transfer back using BT transfer
    // legacy_com_decoder(global_com_data.rx_buffer, COM_Handler_BT_Transfer);
    global_com_data.rx_buffer_data_present = RX_BUFFER_DATA_IDLE;
  }
}

/** Begin of:
  * @tableofcontents SECTION_SETTING FUNCTIONS
 **/
void _vfdco_clock_settings_default_internal();
void vfdco_clock_settings_default(uint8_t silent) {
  // Restore settings for all class members according to settings
  _vfdco_clock_settings_default_internal();

  // Message about this
  if(!silent) {
    vfdco_display_render_message(Messages_Routine_Default1, 0, CONFIG_MESSAGE_SHORT); 
    vfdco_display_render_message(Messages_Routine_Settings, 0, CONFIG_MESSAGE_SHORT);
    vfdco_display_render_message(Messages_Routine_Default2, 0, CONFIG_MESSAGE_SHORT);
  }
}

void vfdco_clock_settings_save(uint8_t silent) {
  // Global (routine) settings
  SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_global_light_instance_counter] = global_light_instance_counter;
  SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_global_light_instance_random] = global_light_instance_random;
  // Save current instance settings
  if(GUI_Format_Save) GUI_Format_Save(&global_gui_instance);
  if(Light_Pattern_Save) Light_Pattern_Save(&global_light_instance);

  vfdco_serialization_write(serialized_settings, serialized_settings_sizes, NUM_SERIALIZABLE);
  if(!silent) {
    vfdco_display_render_message(Messages_Routine_Saved1, 0, CONFIG_MESSAGE_SHORT);
    vfdco_display_render_message(Messages_Routine_Settings, 0, CONFIG_MESSAGE_SHORT);
    vfdco_display_render_message(Messages_Routine_Saved2, 0, CONFIG_MESSAGE_SHORT); 
  }
}




/** Begin of:
  * @tableofcontents SECTION_SUPPORTING_FUNCTIONS
  * @brief Prototypes. Implementation see very end
 **/
static inline void vfdco_welcome(char *message) {
  uint8_t spaces = 0; // Empty spaces
  for(uint_fast8_t i = 0; i < CONFIG_NUM_DIGITS; i++) if(message[i] == ' ') spaces++; // Count all spaces

  const uint8_t delayMatrix[][6] = {
    { 30, 15,  15,  15,  15, 255},
    { 30, 15,  15,  15, 255,   0},
    { 30, 15,  15, 255,   0,   0},
    { 30, 15, 255,   0,   0,   0},
    { 30, 255,  0,   0,   0,   0},
    {255, 0,    0,   0,   0,   0}
  };

  for(uint8_t k = 0; k < (CONFIG_NUM_DIGITS - spaces); k++) {     // k-th letter of message
    for(uint8_t i = 0; i < (CONFIG_NUM_DIGITS - k); i++) {        // Let the letter slide in from the right to the next available position
      char delay_pattern[CONFIG_NUM_DIGITS];                      // Define empty pattern
      for(uint8_t j = 0; j < CONFIG_NUM_DIGITS; j++) {
        if(j >= k) delay_pattern[j] = ' ';                        // All j's larger than current k will be filled with empty spaces
        else delay_pattern[j] = message[j];                       // If k has increased, fill letters already slided in in advance
      }
      delay_pattern[5 - i] = message[k];                          // Manipulate i-th filled empty pattern element with k-th letter of message
      vfdco_display_render_message(delay_pattern, 0, delayMatrix[k][i]);   // Render the message with delay information
    }
  }

  char empty[] = {' ', ' ', ' ', ' ', ' ', ' '};
  vfdco_display_render_message(empty,   0, CONFIG_MESSAGE_SHORT);
  vfdco_display_render_message(message, 0, CONFIG_MESSAGE_LONG );
}

static void set_next_gui_instance_timeset(uint8_t set_mode) {
  GUI_Format_Time_Date_Setter_Init((struct GUI_Format_Time_Date_Setter*)&global_gui_instance, &global_time, &global_date, set_mode);
  GLOBAL_SET_NEXT_GUI_INSTANCE(GUI_TIME_DATE_SET);
}
/**
 * @brief Set the next GUI instance
 * @param next_instance any regular GUI without special parameters (TIME/DATE_STOPWATCH)
 */
static void set_next_gui_instance(gui_instance_t next_instance) {
  uint8_t mapped_instance = _map_gui_instance_to_serialized_settings_index(next_instance), *instance_settings = NULL;
  if(mapped_instance != INSTANCE_NO_SETTINGS) instance_settings = serialized_settings[mapped_instance];
  switch(next_instance) {
    case GUI_TIME:      { GUI_Format_Time_Init(     (struct GUI_Format_Time*)&global_gui_instance, &global_time, instance_settings); break; }
    case GUI_DATE:      { GUI_Format_Date_Init(     (struct GUI_Format_Date*)&global_gui_instance, &global_date, instance_settings); break; }
    case GUI_STOPWATCH: { GUI_Format_Stopwatch_Init((struct GUI_Format_Stopwatch*)&global_gui_instance, &global_time);               break; }
    case GUI_BRIGHTNESS_SET: {
      GUI_Format_Brightness_Setter_Init(
        (struct GUI_Format_Brightness_Setter *)&global_gui_instance,
        SERIALIZABLE_CLOCK_ROUTINE_arr + CLOCK_ROUTINE_SETTING_dim_factor_display
      );
      break;
    }
    default: break;
  }
  GLOBAL_SET_NEXT_GUI_INSTANCE(next_instance);
}

static void find_next_lights_instance() {
  if(Light_Pattern_Save) Light_Pattern_Save(&global_light_instance);
  Container_Light_Pattern_Clear(&global_light_instance);
  if(global_light_instance_counter < 8) { // Iterable instance
    light_pattern_instance_t next_iterable_instance;
    if(!GLOBAL_LIGHT_INSTANCE_RANDOM_IS_ON) {
      next_iterable_instance = global_light_instance_counter;
      do { // Look for the next enabled iterable instance in the enable register
        ++next_iterable_instance;
        if(next_iterable_instance == 8) next_iterable_instance = LIGHT_PATTERN_STATIC;
      } while(!GLOBAL_ITERABLE_INSTANCE_IS_ENABLED(next_iterable_instance));
    } else {
      next_iterable_instance = vfdco_util_random(3);
      do { // Look for the next enabled iterable instance in the enable register
        ++next_iterable_instance;
        if(next_iterable_instance == 8) next_iterable_instance = LIGHT_PATTERN_STATIC;
      } while(!GLOBAL_RANDOM_INSTANCE_IS_ENABLED(next_iterable_instance));
    }
    set_next_lights_instance(next_iterable_instance);
  } else set_next_lights_instance(LIGHT_PATTERN_STATIC);
}

static uint8_t _reduce(uint8_t x, uint8_t N) { return ((uint16_t)x * (uint16_t)N) >> 8; }
static void set_next_lights_instance(light_pattern_instance_t next_instance) {
  uint8_t mapped_instance = _map_lights_instance_to_serialized_settings_index(next_instance), *instance_settings = NULL;
  if(!GLOBAL_LIGHT_INSTANCE_RANDOM_IS_ON) {
    if(mapped_instance != INSTANCE_NO_SETTINGS) instance_settings = serialized_settings[mapped_instance]; // Saved settings
  } else {
    static uint8_t random_unsaved_settings[2];    // Unsaved settings
    instance_settings = random_unsaved_settings;  // Or any random settings. Shuffle it for real
    instance_settings[0] = (vfdco_util_random(7) << 1) | (global_time.s & 0x01);
    instance_settings[1] = (vfdco_util_random(7) << 1) | (global_time.s & 0x01);
    switch(next_instance) {
      case LIGHT_PATTERN_STATIC: {
        instance_settings[0] &= 0x0F; // 16 presets
        break;
      }
      case LIGHT_PATTERN_MOMENTSOFBLISS: {
        instance_settings[0] = _reduce(instance_settings[0], 7); // Mode: Max 6 modes
        break;
      }
      case LIGHT_PATTERN_SPECTRUM: {
        instance_settings[0] |= 0xC0; // Sat: at least 192
        instance_settings[1] = CONFIG_LIGHTNESS_MEDIUM + _reduce(instance_settings[0], 38); // Li: max 128
        break;
      }
      case LIGHT_PATTERN_RAINBOW: {
        instance_settings[0] = 10 + _reduce(instance_settings[0], 33); // Diff: at least 10, max 42
        instance_settings[1] |= 0xC0; // Sat: at least 192
        break;
      }
      case LIGHT_PATTERN_CHASE: {
        instance_settings[0] = _reduce(instance_settings[0], 3); // Mode: Max 2
        instance_settings[1] = _reduce(instance_settings[1], 7); // Diff: Max 6
        break;
      }
      case LIGHT_PATTERN_MUSIC: {
        instance_settings[0] = 10 + _reduce(instance_settings[0], 33); // Diff: at least 10, max 42
        instance_settings[1] &= 0x7F; // Li: max 128
        break;
      }
      default: break;
    }
  }
  switch(next_instance) {
    case LIGHT_PATTERN_STATIC: {
      Light_Pattern_Static_Init((struct Light_Pattern_Static *)&global_light_instance, instance_settings);
      break;
    }
    case LIGHT_PATTERN_MOMENTSOFBLISS: {
      Light_Pattern_MomentsOfBliss_Init((struct Light_Pattern_MomentsOfBliss *)&global_light_instance, instance_settings);
      break;
    }
    case LIGHT_PATTERN_SPECTRUM: {
      Light_Pattern_Spectrum_Init((struct Light_Pattern_Spectrum *)&global_light_instance, instance_settings);
      break;
    }
    case LIGHT_PATTERN_RAINBOW: {
      Light_Pattern_Rainbow_Init((struct Light_Pattern_Rainbow *)&global_light_instance, instance_settings);
      break;
    }
    case LIGHT_PATTERN_CHASE: {
      Light_Pattern_Chase_Init((struct Light_Pattern_Chase *)&global_light_instance, &global_time, instance_settings);
      break;
    }
    case LIGHT_PATTERN_MUSIC: {
      Light_Pattern_Music_Init((struct Light_Pattern_Music *)&global_light_instance, instance_settings);
      break;
    }
    case LIGHT_PATTERN_TIME_CODE: {
      Light_Pattern_Time_Code_Init((struct Light_Pattern_Time_Code *)&global_light_instance, &global_time);
      break;
    }
    case LIGHT_PATTERN_COP: {
      Light_Pattern_Cop_Init((struct Light_Pattern_Cop *)&global_light_instance);
      break;
    }
    case LIGHT_PATTERN_SERIAL0: {
      Light_Pattern_Serial0_Init((struct Light_Pattern_Serial0 *)&global_light_instance, instance_settings);
      break;
    }
    case LIGHT_PATTERN_SERIAL1: {
      Light_Pattern_Serial1_Init((struct Light_Pattern_Serial1 *)&global_light_instance, instance_settings);
      break;
    }
  }
  if(!GLOBAL_LIGHT_INSTANCE_RANDOM_IS_ON) {
    GLOBAL_SET_NEXT_LIGHT_INSTANCE(next_instance);
  } else {
    GLOBAL_SET_NEXT_RANDOM_INSTANCE(next_instance);
  }
}

static void com_decoder(uint8_t *input_buffer, void (*com_encoder)(struct COM_Data *)) {
  if((input_buffer[0] == 0x24) && (input_buffer[26] == 0x25)) {
    const uint8_t command_byte = input_buffer[COM_PROTOCOL_COMMAND_OFFSET];

    // LED set
    if(command_byte == 0x00) {
      GLOBAL_SET_NEXT_RANDOM_INSTANCE(GLOBAL_LIGHT_INSTANCE_RANDOM_OFF); // Immediately turn off
      if(Light_Pattern_Save) Light_Pattern_Save(&global_light_instance);
      memcpy(
        serialized_settings[_map_lights_instance_to_serialized_settings_index(LIGHT_PATTERN_SERIAL0)], 
        input_buffer + COM_PROTOCOL_DATA_OFFSET, 4 * CONFIG_NUM_PIXELS * sizeof(uint8_t)
      );
      set_next_lights_instance(LIGHT_PATTERN_SERIAL0);
    }
    // LED smooth set 
    else if(command_byte == 0x01) {
      GLOBAL_SET_NEXT_RANDOM_INSTANCE(GLOBAL_LIGHT_INSTANCE_RANDOM_OFF); // Immediately turn off
      if(Light_Pattern_Save) Light_Pattern_Save(&global_light_instance);
      memcpy(
        serialized_settings[_map_lights_instance_to_serialized_settings_index(LIGHT_PATTERN_SERIAL1)], 
        input_buffer + COM_PROTOCOL_DATA_OFFSET, 4 * CONFIG_NUM_PIXELS * sizeof(uint8_t)
      );
      set_next_lights_instance(LIGHT_PATTERN_SERIAL1);
    }

    // LED presets 
    else if(command_byte == 0x04) {
      GLOBAL_SET_NEXT_RANDOM_INSTANCE(GLOBAL_LIGHT_INSTANCE_RANDOM_OFF); // Immediately turn off
      light_pattern_instance_t instance = input_buffer[COM_PROTOCOL_CONTROL_OFFSET];
      if(Light_Pattern_Save) Light_Pattern_Save(&global_light_instance);
      // If it has saved settings
      uint8_t mapped_settings_index = _map_lights_instance_to_serialized_settings_index(instance);
      if(mapped_settings_index != 255) {
        if(serialized_settings_sizes[mapped_settings_index] >= 1) { // One setting parameter
          serialized_settings[mapped_settings_index][0] = input_buffer[COM_PROTOCOL_PARAM0_OFFSET];
          if(serialized_settings_sizes[mapped_settings_index] == 2) { // Two setting parameters
            serialized_settings[mapped_settings_index][1] = input_buffer[COM_PROTOCOL_PARAM1_OFFSET];
          }
        }
      }
      // Switch to instance
      light_pattern_instance_t prev_instance_counter = global_light_instance_counter;
      set_next_lights_instance(instance);
      if(global_light_instance_counter != prev_instance_counter) if(Light_Pattern_Hello) Light_Pattern_Hello();
    }

    // Enabled iterable instances set
    else if(command_byte == 0x05) {
      SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_global_light_it_register] = input_buffer[COM_PROTOCOL_DATA_OFFSET];
      vfdco_display_render_message(Messages_Routine_Inst, 0, CONFIG_MESSAGE_SHORT);
      vfdco_display_render_message(Messages_Routine_Set, 0, CONFIG_MESSAGE_SHORT); 
    }
    // Random set
    else if(command_byte == 0x06) {
      // Apply settings
      SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_global_light_rnd_register] = input_buffer[COM_PROTOCOL_DATA_OFFSET];
      SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_global_light_rnd_speed] = input_buffer[COM_PROTOCOL_DATA_OFFSET + 1];
      vfdco_display_render_message(Messages_Routine_Rnd, 0, CONFIG_MESSAGE_SHORT);
      vfdco_display_render_message(Messages_Routine_Set, 0, CONFIG_MESSAGE_SHORT); 
      // Turn on
      GLOBAL_SET_NEXT_RANDOM_INSTANCE(LIGHT_PATTERN_STATIC);
      find_next_lights_instance();
      vfdco_display_render_message(Messages_Routine_RandomOn, 0, CONFIG_MESSAGE_LONG);
    }


    // GUI setting set
    else if(command_byte == 0x10) {
      gui_instance_t instance = input_buffer[COM_PROTOCOL_CONTROL_OFFSET];
      uint8_t mapped_settings_index = _map_gui_instance_to_serialized_settings_index(instance);
      if(mapped_settings_index != 255) {
        serialized_settings[mapped_settings_index][0] = input_buffer[COM_PROTOCOL_PARAM0_OFFSET];
      }
      if(instance == global_gui_instance_counter) vfdco_display_render_message(Messages_Routine_Set, 0, CONFIG_MESSAGE_LONG);
    }

    // If time set command is detected
    else if(command_byte == 0x20) {
      if(input_buffer[COM_PROTOCOL_TIMEDATE_SET_OFFSET_FLAG] == 0x23 /* Make sure flag */) {
        // Send to RTC
        global_time.s = input_buffer[COM_PROTOCOL_DATA_OFFSET + 0];
        global_time.m = input_buffer[COM_PROTOCOL_DATA_OFFSET + 1];
        global_time.h = input_buffer[COM_PROTOCOL_DATA_OFFSET + 2];
        global_date.d = input_buffer[COM_PROTOCOL_DATA_OFFSET + 3];
        global_date.m = input_buffer[COM_PROTOCOL_DATA_OFFSET + 4];
        global_date.y = input_buffer[COM_PROTOCOL_DATA_OFFSET + 5];
        vfdco_set_date_time(&global_date, &global_time);
        // Say that time and date is synced now.
        const char _msg_tsync1[CONFIG_NUM_DIGITS] = {'T', '-', 'D', ' ', ' ', ' '};
        const char _msg_tsync2[CONFIG_NUM_DIGITS] = {'S', 'Y', 'N', 'C', 'E', 'D'};
        vfdco_display_render_message(_msg_tsync1, 0x00, CONFIG_MESSAGE_LONG);
        vfdco_display_render_message(_msg_tsync2, 0x00, CONFIG_MESSAGE_LONG);
      }
      // Answer with a beginning of a message. If it's all good, the PC controller will complete the message :p
      uint8_t transfer_buffer[10] = {0x24, 0x30, 'T', 'i', 'm', 'e', ' ', 'S', 'y', 0x25};
      global_com_data.tx_buffer = transfer_buffer;
      com_encoder(&global_com_data);
      global_com_data.tx_buffer = NULL;
    }

    // If brightness set command is detected
    else if(command_byte == 0x21) {
      if(input_buffer[COM_PROTOCOL_CONTROL_OFFSET] == 0) { // 0: Set display brightness
        SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_dim_factor_display] = input_buffer[COM_PROTOCOL_PARAM0_OFFSET];
        vfdco_display_set_dim_factor(input_buffer[COM_PROTOCOL_PARAM0_OFFSET]);
      } else if (input_buffer[COM_PROTOCOL_CONTROL_OFFSET] == 1) { // 1: Set LED brightness
        SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_dim_factor_led] = input_buffer[COM_PROTOCOL_PARAM0_OFFSET];
        vfdco_clr_set_dim_factor(input_buffer[COM_PROTOCOL_PARAM0_OFFSET]);
      }
    }

    // If night shift set is detected
    else if(command_byte == 0x22) {
      SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_night_shift_start_h] = input_buffer[COM_PROTOCOL_DATA_OFFSET];
      SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_night_shift_start_m] = input_buffer[COM_PROTOCOL_DATA_OFFSET + 1];
      SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_night_shift_end_h] = input_buffer[COM_PROTOCOL_DATA_OFFSET + 2];
      SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_night_shift_end_m] = input_buffer[COM_PROTOCOL_DATA_OFFSET + 3];
      vfdco_display_render_message(Messages_Routine_Night, 0, CONFIG_MESSAGE_LONG);
      vfdco_display_render_message(Messages_Routine_Shift, 0, CONFIG_MESSAGE_LONG);
      vfdco_display_render_message(Messages_Routine_Set, 0, CONFIG_MESSAGE_LONG);
    }

    // If welcome message set is detected
    else if(command_byte == 0x25) {
      for(uint8_t i = COM_PROTOCOL_DATA_OFFSET; i < (COM_PROTOCOL_DATA_OFFSET + CONFIG_NUM_DIGITS); ++i)
        SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_welcome + i - COM_PROTOCOL_DATA_OFFSET] = (char)input_buffer[i];
      vfdco_clock_settings_save(1);
      vfdco_display_render_message((const char *)SERIALIZABLE_CLOCK_ROUTINE_arr + CLOCK_ROUTINE_SETTING_welcome, 0x00, CONFIG_MESSAGE_LONG);
    }

    // If message display set is detected
    else if(command_byte == 0x26) {
      // Get message delay time. It's the incoming value in seconds
      uint8_t offset = COM_PROTOCOL_DATA_OFFSET;
      for(uint8_t i = 0; i < 4; ++i) {
        if(input_buffer[offset] == 0) break;
        vfdco_display_render_message((char *)input_buffer + offset, 0x00, CONFIG_MESSAGE_LONG);
        offset += CONFIG_NUM_DIGITS;
      }
    }

    // If clock control is detected
    else if(command_byte == 0x30) {
      const com_protocol_clock_control_t control_byte = input_buffer[COM_PROTOCOL_CONTROL_OFFSET];
      switch(control_byte) {
        // HW Version Request
        case COM_PROTOCOL_HW_VERSION_REQ: {
          uint8_t transfer_buffer[10] = CONFIG_HW_STRING;
          for(uint8_t i = 0; i < CONFIG_HW_STRING_LENGTH; ++i) 
            transfer_buffer[2 + (CONFIG_HW_STRING_LENGTH - i - 1)] = transfer_buffer[(CONFIG_HW_STRING_LENGTH - i - 1)];
          transfer_buffer[0] = 0x24; transfer_buffer[1] = 0x32; transfer_buffer[9] = 0x25; // [start][command] .. [stop]
          global_com_data.tx_buffer = transfer_buffer;
          com_encoder(&global_com_data);
          global_com_data.tx_buffer = NULL;
          break;
        }
        // SW Version Request
        case COM_PROTOCOL_FW_VERSION_REQ: {
          uint8_t transfer_buffer[10] = CONFIG_SW_STRING;
          for(uint8_t i = 0; i < CONFIG_SW_STRING_LENGTH; ++i) 
            transfer_buffer[2 + (CONFIG_SW_STRING_LENGTH - i - 1)] = transfer_buffer[(CONFIG_SW_STRING_LENGTH - i - 1)];
          transfer_buffer[0] = 0x24; transfer_buffer[1] = 0x31; transfer_buffer[9] = 0x25; // [start][command] .. [stop]
          global_com_data.tx_buffer = transfer_buffer;
          com_encoder(&global_com_data);
          global_com_data.tx_buffer = NULL;
          break;
        }
        // Save request
        case COM_PROTOCOL_SETTINGS_SAVE_REQ: vfdco_clock_settings_save(0); break;
        // Default load request 
        case COM_PROTOCOL_DEFAULT_REQ: vfdco_clock_settings_default(0); break;
        default: break;
        // Enabled instances request
        case COM_PROTOCOL_ENABLED_INSTANCES_REQ: {
          uint8_t transfer_buffer[10] = {
            0x24, 0x33, 
            SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_global_light_it_register], 
            SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_global_light_rnd_register], 
            0, 0, 0, 0, 0, 0x25
          };
          global_com_data.tx_buffer = transfer_buffer;
          com_encoder(&global_com_data);
          global_com_data.tx_buffer = NULL;
          break;
        }
      }
    }
  }
  // Bad command or bit error :( Some random return otherwise. You should never ever get to this point.
  else {}
}

// ######## EVERYTHING FROM HERE IS GENERATED AUTOMATICALLY FROM THE SERIALIZATION MAPPING BY PREPROCESSOR ABUSE, only change in designated section in vfdco_clock_routines.h! ########
// Creates setting array for each saved class
#define CREATE_SERIALIZED_ARR(_globalindex, _size, _enum_map, _serializable_identifier) \
  uint8_t _serializable_identifier ## _arr[_size] = {0};
CREATE_SERIALIZED_GLOBAL(CREATE_SERIALIZED_ARR)
CREATE_SERIALIZED_GUI(CREATE_SERIALIZED_ARR)
CREATE_SERIALIZED_LIGHTS(CREATE_SERIALIZED_ARR)
#undef CREATE_SERIALIZED_ARR

// Creates pair of array of setting arrays and corresponding size map
uint8_t *const serialized_settings[NUM_SERIALIZABLE] = {
  #define CREATE_SERIALIZED_SETTING_ENTRIES(_globalindex, _size, _enum_map, _serializable_identifier) _serializable_identifier ## _arr,
  CREATE_SERIALIZED_GLOBAL(CREATE_SERIALIZED_SETTING_ENTRIES)
  CREATE_SERIALIZED_GUI(CREATE_SERIALIZED_SETTING_ENTRIES)
  CREATE_SERIALIZED_LIGHTS(CREATE_SERIALIZED_SETTING_ENTRIES)
  #undef CREATE_SERIALIZED_SETTING_ENTRIES
};
const uint8_t serialized_settings_sizes[NUM_SERIALIZABLE] = {
  #define CREATE_SERIALIZED_GLOBAL_SIZES(_globalindex, _size, _enum_map, _serializable_identifier) _size,
  CREATE_SERIALIZED_GLOBAL(CREATE_SERIALIZED_GLOBAL_SIZES)
  CREATE_SERIALIZED_GUI(CREATE_SERIALIZED_GLOBAL_SIZES)
  CREATE_SERIALIZED_LIGHTS(CREATE_SERIALIZED_GLOBAL_SIZES)
  #undef CREATE_SERIALIZED_GLOBAL_SIZES
};

inline void _vfdco_clock_settings_default_internal() {
  // Global (routine) settings
  #define CREATE_SETTINGS_DEFAULT(_entry, _offset, _size, _setting_identifier, _defaultval, _description) \
    const uint8_t _setting_identifier ## _default[_size] = { _defaultval }; \
    memcpy(serialized_settings[_entry] + _offset, _setting_identifier ## _default, _size);
  CREATE_SERIALIZED_GLOBAL_POSITIONS(CREATE_SETTINGS_DEFAULT)
  CREATE_SERIALIZED_GUI_POSITIONS(CREATE_SETTINGS_DEFAULT)
  CREATE_SERIALIZED_LIGHTS_POSITIONS(CREATE_SETTINGS_DEFAULT)
  #undef CREATE_SETTINGS_DEFAULT_GLOBAL
}

uint8_t _map_gui_instance_to_serialized_settings_index(gui_instance_t instance) {
  switch(instance) {
    #define CREATE_SERIALIZED_GUI_SWITCHES(_globalindex, _size, _enum_map, _serializable_identifier) case _enum_map: return _serializable_identifier ## _INDEX;
    CREATE_SERIALIZED_GUI(CREATE_SERIALIZED_GUI_SWITCHES)
    #undef CREATE_SERIALIZED_GUI_SWITCHES
    default: return INSTANCE_NO_SETTINGS;
  }
}
uint8_t _map_lights_instance_to_serialized_settings_index(light_pattern_instance_t instance) {
  switch(instance) {
    #define CREATE_SERIALIZED_LIGHTS_SWITCHES(_globalindex, _size, _enum_map, _serializable_identifier) case _enum_map: return _serializable_identifier ## _INDEX;
    CREATE_SERIALIZED_LIGHTS(CREATE_SERIALIZED_LIGHTS_SWITCHES)
    #undef CREATE_SERIALIZED_LIGHTS_SWITCHES
    default: return INSTANCE_NO_SETTINGS;
  }
}
// ######## END OF DO NOT TOUCH SECTION ########
