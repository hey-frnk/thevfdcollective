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
// Configuration header
#include "../vfdco_config.h"
// Low Level/Primitive libraries
#include "../vfdco_serialization.h"		// Physical serialization driver
#include "../vfdco_led.h"             // Physical LED driver
#include "../vfdco_display.h"         // Physical display driver
#include "../vfdco_time.h"            // Physical timing & RTC driver
#include "../vfdco_hid.h"             // Physical HID driver
// Application specific libraries
#include "../vfdco_lights.h"          // Library of light patterns
#include "../vfdco_gui.h"             // Library of different user interfaces
// Clock routine scheduler
#include "../vfdco_clock_routines.h"  // Clock routine

// Globally accessible parameters
vfdco_time_t global_time;
vfdco_date_t global_date;

uint8_t global_button_F1_state = BUTTON_STATE_OFF;
uint8_t global_button_F2_state = BUTTON_STATE_OFF;
uint8_t global_button_F3_state = BUTTON_STATE_OFF;
uint8_t global_button_F4_state = BUTTON_STATE_OFF;

time_event_t global_time_updater;
time_event_t display_updater;

Light_Pattern global_light_instance;
light_pattern_instance_t global_light_instance_counter;

GUI_Format global_gui_instance;
gui_instance_t global_gui_instance_counter;

#define GLOBAL_SET_NEXT_LIGHT_INSTANCE(_counter) {global_light_instance_counter = _counter;}
#define GLOBAL_SET_NEXT_GUI_INSTANCE(_counter) {global_gui_instance_counter = _counter;}

#define GLOBAL_CLEAR_BUTTON(_button) _button = BUTTON_STATE_OFF

// Saved clock routine settings
#define CLOCK_ROUTINE_SETTING_welcome                       0
#define CLOCK_ROUTINE_SETTING_global_light_instance_counter 6

const light_pattern_instance_t global_light_instance_default = LIGHT_PATTERN_STATIC;
const char Messages_Welcome_Default[CONFIG_NUM_DIGITS]  = {'H', 'E', 'L', 'L', 'O', ' '};
const char Messages_Routine_Settings[CONFIG_NUM_DIGITS] = {'S', 'E', 'T', 'I', 'N', 'G'};
const char Messages_Routine_Default1[CONFIG_NUM_DIGITS] = {'D', 'E', 'F', 'A', 'U', 'L'};
const char Messages_Routine_Default2[CONFIG_NUM_DIGITS] = {'R', 'E', 'T', 'O', 'R', 'D'};
const char Messages_Routine_Saved1[CONFIG_NUM_DIGITS]   = {'A', 'L', 'L', ' ', ' ', ' '};
const char Messages_Routine_Saved2[CONFIG_NUM_DIGITS]   = {'S', 'A', 'V', 'E', 'D', ' '};

void vfdco_welcome(char *message) {
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

void vfdco_clock_initializer() {
  // Initialize display and LEDs
  vfdco_display_init();
  vfdco_clr_init();

  global_time_updater = Time_Event_Init(CONFIG_RTC_UPDATE_INTERVAL);
  display_updater = Time_Event_Init(100);

  vfdco_clock_serialization_initializer();
  vfdco_clock_lights_initializer();
  vfdco_clock_display_initializer();

  char welcome[CONFIG_NUM_DIGITS] = {0};
  for(uint8_t i = 0; i < CONFIG_NUM_DIGITS; ++i) welcome[i] = SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_welcome + i];
  vfdco_welcome(welcome);
}

void vfdco_clock_serialization_initializer() {
  // Load all clock parameters
  SERIALIZATION_HEADER_STATUS_t read_status = vfdco_serialization_read(serialized_settings, serialized_settings_sizes, NUM_SERIALIZABLE);
  if(read_status) {

    // Something went wrong. Load default settings!
    vfdco_clock_settings_default();
  }
}

// Human interface device (Buttons) routine
void vfdco_clock_hid_routine() {
  vfdco_hid_button_retrieve_all(
    &global_button_F1_state,
    &global_button_F2_state,
    &global_button_F3_state,
    &global_button_F4_state
  );
}

// RTC time refresh routine
void vfdco_clock_time_routine() {
  if(Time_Event_Update(&global_time_updater)) {
    vfdco_get_date_time(&global_date, &global_time);
  }
}


void set_next_gui_instance_timeset(uint8_t set_mode) {
  GUI_Format_Time_Date_Setter_Init((struct GUI_Format_Time_Date_Setter*)&global_gui_instance, CONFIG_GUI_DATE_UPDATE_INTERVAL, set_mode);
  GLOBAL_SET_NEXT_GUI_INSTANCE(GUI_TIME_DATE_SET);
}
/**
 * @brief Set the next GUI instance
 * @param next_instance any regular GUI without special parameters (TIME/DATE_STOPWATCH)
 */
void set_next_gui_instance(gui_instance_t next_instance) {
  switch(next_instance) {
    case GUI_TIME: {
      // GLOBAL_READ_GUI_INSTANCE(GUI_TIME);
      GUI_Format_Time_Init(
        (struct GUI_Format_Time*)&global_gui_instance, 
        CONFIG_GUI_TIME_UPDATE_INTERVAL, 
        serialized_settings[_map_gui_instance_to_serialized_settings_size_index(next_instance)]
      );
      break;
    }
    case GUI_DATE: {
      // GLOBAL_READ_GUI_INSTANCE(GUI_DATE);
      GUI_Format_Date_Init(
        (struct GUI_Format_Date*)&global_gui_instance, 
        CONFIG_GUI_DATE_UPDATE_INTERVAL,
        serialized_settings[_map_gui_instance_to_serialized_settings_size_index(next_instance)]
      );
      break;
    }
    case GUI_STOPWATCH: {
      GUI_Format_Stopwatch_Init((struct GUI_Format_Stopwatch*)&global_gui_instance, CONFIG_GUI_TIME_UPDATE_INTERVAL);
      break;
    }
    default: exit(-42); // Oops
  }
  GLOBAL_SET_NEXT_GUI_INSTANCE(next_instance);
}

void vfdco_clock_display_initializer() {
  // Start by creating a time instance
  set_next_gui_instance(GUI_TIME);
}

// VFD display data render routine
void vfdco_clock_display_routine() {
  GUI_Format_Update(&global_gui_instance);
  if(global_button_F1_state == BUTTON_STATE_SHORTPRESS) {
    if(global_gui_instance_counter == GUI_TIME_DATE_SET) {
      // Sorry ugly AF but saves a truck load of code memory
      struct GUI_Format_Time_Date_Setter *self = (struct GUI_Format_Time_Date_Setter*)&global_gui_instance;
      if(self->set_mode == 0) vfdco_set_date_time(&global_date,    &self->new_time);
      else                    vfdco_set_date_time(&self->new_date, &global_time   );
    }
    if(GUI_Format_Save) GUI_Format_Save(&global_gui_instance);
    Container_GUI_Clear(&global_gui_instance);
    switch(global_gui_instance_counter) {
      case GUI_TIME:          { set_next_gui_instance(GUI_DATE);      break; }
      case GUI_DATE:          { set_next_gui_instance(GUI_STOPWATCH); break; }
      case GUI_STOPWATCH:     { set_next_gui_instance(GUI_TIME);      break; }
      case GUI_TIME_DATE_SET: { set_next_gui_instance(GUI_TIME);      break; }
      default: break;
    }
    GLOBAL_CLEAR_BUTTON(global_button_F1_state); // Priority clear
  } else if(global_button_F1_state == BUTTON_STATE_LONGPRESS) {
    // To time set menu
    if(GUI_Format_Save) GUI_Format_Save(&global_gui_instance);
    Container_GUI_Clear(&global_gui_instance);
    switch(global_gui_instance_counter) {
      case GUI_TIME: { set_next_gui_instance_timeset(0); break; }
      case GUI_DATE: { set_next_gui_instance_timeset(1); break; }
      default: break;
    }
    GLOBAL_CLEAR_BUTTON(global_button_F1_state); // Priority clear
  }

  switch(global_button_F2_state) {
    case BUTTON_STATE_SHORTPRESS:
      if(GUI_Format_F2) {GUI_Format_F2(&global_gui_instance); GLOBAL_CLEAR_BUTTON(global_button_F2_state);}
      break;
    case BUTTON_STATE_LONGPRESS:
      if(GUI_Format_F2Var) {GUI_Format_F2Var(&global_gui_instance); GLOBAL_CLEAR_BUTTON(global_button_F2_state);}
      break;
  }
  switch(global_button_F3_state) {
    case BUTTON_STATE_SHORTPRESS:
      if(GUI_Format_F3) {GUI_Format_F3(&global_gui_instance); GLOBAL_CLEAR_BUTTON(global_button_F3_state);}
      break;
    case BUTTON_STATE_LONGPRESS:
      if(GUI_Format_F3Var) {GUI_Format_F3Var(&global_gui_instance); GLOBAL_CLEAR_BUTTON(global_button_F3_state);}
      break;
  }
  switch(global_button_F4_state) {
    case BUTTON_STATE_SHORTPRESS: {
      if(global_gui_instance_counter == GUI_STOPWATCH) {
        // In stopwatch interface, F4 short press corresponds to settings saving!
        vfdco_clock_settings_save(0);
      } else {
        if(GUI_Format_F4) GUI_Format_F4(&global_gui_instance);
      }
      GLOBAL_CLEAR_BUTTON(global_button_F4_state); 
      break;
    }
    case BUTTON_STATE_LONGPRESS: {
      if(global_gui_instance_counter == GUI_STOPWATCH) {
        // In stopwatch interface, F4 long press corresponds to default loading!
        vfdco_clock_settings_default();
      } else {
        if(GUI_Format_F4Var) GUI_Format_F4Var(&global_gui_instance);
      }
      GLOBAL_CLEAR_BUTTON(global_button_F4_state); 
      break;
    }
  }
}

void set_next_lights_instance(light_pattern_instance_t next_instance) {
  uint8_t *instance_settings = NULL;
  if(next_instance != LIGHT_PATTERN_TIME_CODE && next_instance != LIGHT_PATTERN_COP)
    instance_settings = serialized_settings[_map_lights_instance_to_serialized_settings_size_index(next_instance)];
  switch(next_instance) {
    case LIGHT_PATTERN_STATIC: {
      Light_Pattern_Static_Init((struct Light_Pattern_Static *)&global_light_instance, instance_settings);
      GLOBAL_SET_NEXT_LIGHT_INSTANCE(LIGHT_PATTERN_STATIC);
      break;
    }
    case LIGHT_PATTERN_SPECTRUM: {
      Light_Pattern_Spectrum_Init((struct Light_Pattern_Spectrum *)&global_light_instance, instance_settings);
      GLOBAL_SET_NEXT_LIGHT_INSTANCE(LIGHT_PATTERN_SPECTRUM);
      break;
    }
    case LIGHT_PATTERN_RAINBOW: {
      Light_Pattern_Rainbow_Init((struct Light_Pattern_Rainbow *)&global_light_instance, instance_settings);
      GLOBAL_SET_NEXT_LIGHT_INSTANCE(LIGHT_PATTERN_RAINBOW);
      break;
    }
    case LIGHT_PATTERN_CHASE: {
      Light_Pattern_Chase_Init((struct Light_Pattern_Chase *)&global_light_instance, &global_time, instance_settings);
      GLOBAL_SET_NEXT_LIGHT_INSTANCE(LIGHT_PATTERN_CHASE);
      break;
    }
    case LIGHT_PATTERN_TIME_CODE: {
      Light_Pattern_Time_Code_Init((struct Light_Pattern_Time_Code *)&global_light_instance, &global_time);
      GLOBAL_SET_NEXT_LIGHT_INSTANCE(LIGHT_PATTERN_TIME_CODE);
      break;
    }
    case LIGHT_PATTERN_COP: {
      Light_Pattern_Cop_Init((struct Light_Pattern_Cop *)&global_light_instance);
      GLOBAL_SET_NEXT_LIGHT_INSTANCE(LIGHT_PATTERN_COP);
      break;
    }
    case LIGHT_PATTERN_MOMENTSOFBLISS: {
      Light_Pattern_MomentsOfBliss_Init((struct Light_Pattern_MomentsOfBliss *)&global_light_instance, instance_settings);
      GLOBAL_SET_NEXT_LIGHT_INSTANCE(LIGHT_PATTERN_MOMENTSOFBLISS);
      break;
    }
    case LIGHT_PATTERN_SERIAL0: {
      Light_Pattern_Serial0_Init((struct Light_Pattern_Serial0 *)&global_light_instance, instance_settings);
      GLOBAL_SET_NEXT_LIGHT_INSTANCE(LIGHT_PATTERN_SERIAL0);
      break;
    }
    case LIGHT_PATTERN_SERIAL1: {
      Light_Pattern_Serial1_Init((struct Light_Pattern_Serial1 *)&global_light_instance, instance_settings);
      GLOBAL_SET_NEXT_LIGHT_INSTANCE(LIGHT_PATTERN_SERIAL1);
      break;
    }
  }
}

void vfdco_clock_lights_initializer() {
  vfdco_clr_set_all_RGBW(0, 0, 0, 0);
  vfdco_time_delay_milliseconds(2);
  vfdco_clr_render();

  set_next_lights_instance((light_pattern_instance_t)SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_global_light_instance_counter]);
}

// VFD LED light illumination routine
void vfdco_clock_lights_routine() {
  Light_Pattern_Update(&global_light_instance);
  if(global_button_F2_state == BUTTON_STATE_SHORTPRESS) {
    if(Light_Pattern_Save) Light_Pattern_Save(&global_light_instance);
    Container_Light_Pattern_Clear(&global_light_instance);
    switch(global_light_instance_counter) {
      case LIGHT_PATTERN_STATIC:          { set_next_lights_instance(LIGHT_PATTERN_MOMENTSOFBLISS); break; }
      case LIGHT_PATTERN_MOMENTSOFBLISS:  { set_next_lights_instance(LIGHT_PATTERN_SPECTRUM);       break; }
      case LIGHT_PATTERN_SPECTRUM:        { set_next_lights_instance(LIGHT_PATTERN_RAINBOW);        break; }
      case LIGHT_PATTERN_RAINBOW:         { set_next_lights_instance(LIGHT_PATTERN_CHASE);          break; }
      case LIGHT_PATTERN_CHASE:           { set_next_lights_instance(LIGHT_PATTERN_TIME_CODE);      break; }
      case LIGHT_PATTERN_TIME_CODE:       { set_next_lights_instance(LIGHT_PATTERN_COP);            break; }
      case LIGHT_PATTERN_COP:             { set_next_lights_instance(LIGHT_PATTERN_STATIC);         break; }
      case LIGHT_PATTERN_SERIAL0:         { set_next_lights_instance(LIGHT_PATTERN_STATIC);         break; }
      case LIGHT_PATTERN_SERIAL1:         { set_next_lights_instance(LIGHT_PATTERN_STATIC);         break; }
      default: break;
    }
    if(Light_Pattern_Hello) Light_Pattern_Hello();
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
}

// Communication (Serial/USB, Serial/Bluetooth) routine
/* void vfdco_clock_com_routine() {
  // TODO
} */

void vfdco_clock_settings_default() {
  // Global (routine) settings
  for(uint8_t i = 0; i < CONFIG_NUM_DIGITS; ++i) 
    SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_welcome + i] = Messages_Welcome_Default[i];
  SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_global_light_instance_counter] = global_light_instance_default;

  // Class settings
  GUI_Format_Time_Default(SERIALIZABLE_GUI_TIME_arr);
  GUI_Format_Date_Default(SERIALIZABLE_GUI_DATE_arr);
  Light_Pattern_Static_Default(SERIALIZABLE_LIGHTS_STATIC_arr);
  Light_Pattern_Spectrum_Default(SERIALIZABLE_LIGHTS_SPECTRUM_arr);
  Light_Pattern_Rainbow_Default(SERIALIZABLE_LIGHTS_RAINBOW_arr);
  Light_Pattern_Chase_Default(SERIALIZABLE_LIGHTS_CHASE_arr);
  Light_Pattern_MomentsOfBliss_Default(SERIALIZABLE_LIGHTS_BLISS_arr);

  // Message about this
  vfdco_display_render_message(Messages_Routine_Default1, 0, CONFIG_MESSAGE_SHORT);
  vfdco_display_render_message(Messages_Routine_Settings, 0, CONFIG_MESSAGE_SHORT);
  vfdco_display_render_message(Messages_Routine_Default2, 0, CONFIG_MESSAGE_SHORT);
}

void vfdco_clock_settings_save(uint8_t silent) {
  // Global (routine) settings
  SERIALIZABLE_CLOCK_ROUTINE_arr[CLOCK_ROUTINE_SETTING_global_light_instance_counter] = global_light_instance_counter;

  vfdco_serialization_write(serialized_settings, serialized_settings_sizes, NUM_SERIALIZABLE);
  if(!silent) {
    vfdco_display_render_message(Messages_Routine_Saved1, 0, CONFIG_MESSAGE_SHORT);
    vfdco_display_render_message(Messages_Routine_Settings, 0, CONFIG_MESSAGE_SHORT);
    vfdco_display_render_message(Messages_Routine_Saved2, 0, CONFIG_MESSAGE_SHORT); 
  }
}

// ######## EVERYTHING FROM HERE IS GENERATED AUTOMATICALLY FROM THE SERIALIZATION MAPPING BY PREPROCESSOR ABUSE, only change in designated section in vfdco_clock_routines.h! ########
// Creates setting array for each saved class
#define CREATE_SERIALIZED_ARR(_globalindex, _size, _enum_map, _serializable_identifier) \
  uint8_t _serializable_identifier ## _arr[_size] = {0};
CREATE_SERIALIZED_GLOBAL(CREATE_SERIALIZED_ARR)
CREATE_SERIALIZED_GUI(CREATE_SERIALIZED_ARR)
CREATE_SERIALIZED_LIGHTS(CREATE_SERIALIZED_ARR)

// Creates pair of array of setting arrays and corresponding size map
uint8_t *const serialized_settings[NUM_SERIALIZABLE] = {
  #define CREATE_SERIALIZED_SETTING_ENTRIES(_globalindex, _size, _enum_map, _serializable_identifier) _serializable_identifier ## _arr,
  CREATE_SERIALIZED_GLOBAL(CREATE_SERIALIZED_SETTING_ENTRIES)
  CREATE_SERIALIZED_GUI(CREATE_SERIALIZED_SETTING_ENTRIES)
  CREATE_SERIALIZED_LIGHTS(CREATE_SERIALIZED_SETTING_ENTRIES)
};
const uint8_t serialized_settings_sizes[NUM_SERIALIZABLE] = {
  #define CREATE_SERIALIZED_GLOBAL_SIZES(_globalindex, _size, _enum_map, _serializable_identifier) _size,
  CREATE_SERIALIZED_GLOBAL(CREATE_SERIALIZED_GLOBAL_SIZES)
  CREATE_SERIALIZED_GUI(CREATE_SERIALIZED_GLOBAL_SIZES)
  CREATE_SERIALIZED_LIGHTS(CREATE_SERIALIZED_GLOBAL_SIZES)
};

uint8_t _map_gui_instance_to_serialized_settings_size_index(gui_instance_t instance) {
  switch(instance) {
    #define CREATE_SERIALIZED_GUI_SWITCHES(_globalindex, _size, _enum_map, _serializable_identifier) case _enum_map: return _serializable_identifier ## _INDEX;
    CREATE_SERIALIZED_GUI(CREATE_SERIALIZED_GUI_SWITCHES)
    default: exit(-80);
  }
}
uint8_t _map_lights_instance_to_serialized_settings_size_index(light_pattern_instance_t instance) {
  switch(instance) {
    #define CREATE_SERIALIZED_LIGHTS_SWITCHES(_globalindex, _size, _enum_map, _serializable_identifier) case _enum_map: return _serializable_identifier ## _INDEX;
    CREATE_SERIALIZED_LIGHTS(CREATE_SERIALIZED_LIGHTS_SWITCHES)
    default: exit(-81);
  }
}
// ######## END OF DO NOT TOUCH SECTION ########
