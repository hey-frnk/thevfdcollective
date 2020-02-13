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


// Not sure if it makes things clearer or more fuzzy.
#define GLOBAL_SET_NEXT_LIGHT_INSTANCE(_counter) {global_light_instance_counter = _counter;}
#define GLOBAL_SET_NEXT_GUI_INSTANCE(_counter) {global_gui_instance_counter = _counter;}
#define GLOBAL_CLEAR_BUTTON(_button) _button = BUTTON_STATE_OFF

void vfdco_welcome(char *message) {
  uint8_t spaces = 0;                                                   // Empty spaces
  for(uint_fast8_t i = 0; i < CONFIG_NUM_DIGITS; i++) if(message[i] == ' ') spaces++;   // Count all spaces

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
  vfdco_display_init(CONFIG_NUM_DIGITS);
  vfdco_clr_init(CONFIG_NUM_PIXELS);

  global_time_updater = Time_Event_Init(CONFIG_RTC_UPDATE_INTERVAL);
  display_updater = Time_Event_Init(100);

  vfdco_clock_lights_initializer();
  vfdco_clock_display_initializer();

  char welcome[] = {'V', 'F', 'D', '.', 'C', 'O'};
  vfdco_welcome(welcome);
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


void vfdco_clock_display_initializer() {
  // Start by creating a time instance
  GUI_Format_Time_Init((struct GUI_Format_Time *)&global_gui_instance, CONFIG_GUI_TIME_UPDATE_INTERVAL, TIME_FORMAT_24H, 0);
  GLOBAL_SET_NEXT_GUI_INSTANCE(0);
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

    Container_GUI_Clear(&global_gui_instance);
    switch(global_gui_instance_counter) {
      case GUI_TIME: {
        GUI_Format_Date_Init((struct GUI_Format_Date*)&global_gui_instance, CONFIG_GUI_DATE_UPDATE_INTERVAL, DATE_FORMAT_DDMMYY);
        GLOBAL_SET_NEXT_GUI_INSTANCE(GUI_DATE);
        break;
      }
      case GUI_DATE: {
        GUI_Format_Stopwatch_Init((struct GUI_Format_Stopwatch*)&global_gui_instance, CONFIG_GUI_TIME_UPDATE_INTERVAL);
        GLOBAL_SET_NEXT_GUI_INSTANCE(GUI_STOPWATCH);
        break;
      }
      case GUI_STOPWATCH: {
        GUI_Format_Time_Init((struct GUI_Format_Time*)&global_gui_instance, CONFIG_GUI_TIME_UPDATE_INTERVAL, TIME_FORMAT_24H, 0);
        GLOBAL_SET_NEXT_GUI_INSTANCE(GUI_TIME);
        break;
      }
      case GUI_TIME_DATE_SET: {
        GUI_Format_Time_Init((struct GUI_Format_Time*)&global_gui_instance, CONFIG_GUI_TIME_UPDATE_INTERVAL, TIME_FORMAT_24H, 0);
        GLOBAL_SET_NEXT_GUI_INSTANCE(GUI_TIME);
        break;
      }
      default: break;
    }
    GLOBAL_CLEAR_BUTTON(global_button_F1_state); // Priority clear
  } else if(global_button_F1_state == BUTTON_STATE_LONGPRESS) {
    // To time set menu
    Container_GUI_Clear(&global_gui_instance);
    switch(global_gui_instance_counter) {
      case GUI_TIME: {
        GUI_Format_Time_Date_Setter_Init((struct GUI_Format_Time_Date_Setter*)&global_gui_instance, CONFIG_GUI_DATE_UPDATE_INTERVAL, 0);
        GLOBAL_SET_NEXT_GUI_INSTANCE(GUI_TIME_DATE_SET);
        break;
      }
      case GUI_DATE: {
        GUI_Format_Time_Date_Setter_Init((struct GUI_Format_Time_Date_Setter*)&global_gui_instance, CONFIG_GUI_TIME_UPDATE_INTERVAL, 1);
        GLOBAL_SET_NEXT_GUI_INSTANCE(GUI_TIME_DATE_SET);
        break;
      }
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
    case BUTTON_STATE_SHORTPRESS:
      if(GUI_Format_F4) {GUI_Format_F4(&global_gui_instance); GLOBAL_CLEAR_BUTTON(global_button_F4_state);}
      break;
    case BUTTON_STATE_LONGPRESS:
      if(GUI_Format_F4Var) {GUI_Format_F4Var(&global_gui_instance); GLOBAL_CLEAR_BUTTON(global_button_F4_state);}
      break;
  }
}

void vfdco_clock_lights_initializer() {
  vfdco_clr_set_all_RGBW(0, 0, 0, 0);
	vfdco_time_delay_milliseconds(2);
  vfdco_clr_render();

  Light_Pattern_Static_Init((struct Light_Pattern_Static *)&global_light_instance);
  GLOBAL_SET_NEXT_LIGHT_INSTANCE(LIGHT_PATTERN_STATIC);
}

// VFD LED light illumination routine
void vfdco_clock_lights_routine() {
  Light_Pattern_Update(&global_light_instance);
  if(global_button_F2_state == BUTTON_STATE_SHORTPRESS) {
    Container_Light_Pattern_Clear(&global_light_instance);
    switch(global_light_instance_counter) {
      case LIGHT_PATTERN_STATIC: {
        Light_Pattern_MomentsOfBliss_Init((struct Light_Pattern_MomentsOfBliss *)&global_light_instance, 0);
        GLOBAL_SET_NEXT_LIGHT_INSTANCE(LIGHT_PATTERN_MOMENTSOFBLISS);
        break;
      }
      case LIGHT_PATTERN_MOMENTSOFBLISS: {
        Light_Pattern_Spectrum_Init((struct Light_Pattern_Spectrum *)&global_light_instance);
        GLOBAL_SET_NEXT_LIGHT_INSTANCE(LIGHT_PATTERN_SPECTRUM);
        break;
      }
      case LIGHT_PATTERN_SPECTRUM: {
        Light_Pattern_Rainbow_Init((struct Light_Pattern_Rainbow *)&global_light_instance);
        GLOBAL_SET_NEXT_LIGHT_INSTANCE(LIGHT_PATTERN_RAINBOW);
        break;
      }
      case LIGHT_PATTERN_RAINBOW: {
        Light_Pattern_Chase_Init((struct Light_Pattern_Chase *)&global_light_instance, &global_time, 0);
        GLOBAL_SET_NEXT_LIGHT_INSTANCE(LIGHT_PATTERN_CHASE);
        break;
      }
      case LIGHT_PATTERN_CHASE: {
        Light_Pattern_Time_Code_Init((struct Light_Pattern_Time_Code *)&global_light_instance, &global_time);
        GLOBAL_SET_NEXT_LIGHT_INSTANCE(LIGHT_PATTERN_TIME_CODE);
        break;
      }
      case LIGHT_PATTERN_TIME_CODE: {
        Light_Pattern_Cop_Init((struct Light_Pattern_Cop *)&global_light_instance);
        GLOBAL_SET_NEXT_LIGHT_INSTANCE(LIGHT_PATTERN_COP);
        break;
      }
      case LIGHT_PATTERN_COP: {
        Light_Pattern_Static_Init((struct Light_Pattern_Static *)&global_light_instance);
        GLOBAL_SET_NEXT_LIGHT_INSTANCE(LIGHT_PATTERN_STATIC);
        break;
      }
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
void vfdco_clock_com_routine() {

}
