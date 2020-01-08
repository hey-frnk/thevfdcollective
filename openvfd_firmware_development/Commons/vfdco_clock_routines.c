/*
 * Being part of something special makes you special
 *
 * Created winter 2020
 * Author: Copyright (C) The VFD Collective, Frank Zheng
 *
 */

// Here everything comes together!
#include <stdint.h>
#include <stdlib.h>
#include "../vfdco_clock_routines.h"
#include "../vfdco_hid.h"
#include "../vfdco_time.h"
#include "../vfdco_display.h"
#include "../vfdco_gui.h"
#include "../vfdco_sk6812.h"
#include "../vfdco_color_lib.h"
#include "../vfdco_lights.h"

#ifndef DEBUG
#include "stm32f0xx_hal.h"
#endif

// Globally accessible parameters
vfdco_time_t global_time;
vfdco_date_t global_date;

uint8_t global_button_F1_state = BUTTON_STATE_OFF;
uint8_t global_button_F2_state = BUTTON_STATE_OFF;
uint8_t global_button_F3_state = BUTTON_STATE_OFF;
uint8_t global_button_F4_state = BUTTON_STATE_OFF;

time_event_t global_time_updater;
time_event_t display_updater;

struct Light_Pattern *global_light_instance = NULL;
struct Light_Pattern_Static global_static;

struct GUI_Format *global_gui_instance = NULL;
gui_instance_t global_gui_instance_counter;

void vfdco_welcome(char *message) {
  uint8_t spaces = 0;                                                   // Empty spaces
  for(uint_fast8_t i = 0; i < GLOBAL_NUM_DIGITS_NUM_PIXELS; i++) if(message[i] == ' ') spaces++;   // Count all spaces

  uint8_t delayMatrix[][6] = {{30, 15, 15, 15, 15, 255},
                              {30, 15, 15, 15, 255, 0},
                              {30, 15, 15, 255, 0, 0},
                              {30, 15, 255, 0, 0, 0},
                              {30, 255, 0, 0, 0, 0},
                              {255, 0, 0, 0, 0, 0}};

  for(int k = 0; k < (GLOBAL_NUM_DIGITS_NUM_PIXELS - spaces); k++) {                         // k-th letter of message
    for(int i = 0; i < (GLOBAL_NUM_DIGITS_NUM_PIXELS - k); i++) {                            // Let the letter slide in from the right to the next available position
      char dPattern[GLOBAL_NUM_DIGITS_NUM_PIXELS];                                           // Define empty pattern
      for(int j = 0; j < GLOBAL_NUM_DIGITS_NUM_PIXELS; j++) {
        if(j >= k) dPattern[j] = ' ';                             // All j's larger than current k will be filled with empty spaces
        else dPattern[j] = message[j];                            // If k has increased, fill letters already slided in in advance
      }
      dPattern[5 - i] = message[k];                               // Manipulate i-th filled empty pattern element with k-th letter of message
      vfdco_display_render_message(dPattern, 0, delayMatrix[k][i]);   // Render the message with delay information
    }
  }

  char empty[] = {' ', ' ', ' ', ' ', ' ', ' '};
  vfdco_display_render_message(empty, 0, 400);
  vfdco_display_render_message(message, 0, 1000);
}

void vfdco_clock_initializer() {
  // Initialize display and LEDs
  vfdco_display_init(GLOBAL_NUM_DIGITS_NUM_PIXELS);
  vfdco_clr_init(GLOBAL_NUM_DIGITS_NUM_PIXELS);

  global_time_updater = Time_Event_Init(GLOBAL_TIME_UPDATE_INTERVAL);
  display_updater = Time_Event_Init(100);

  vfdco_clock_lights_initializer();
  vfdco_clock_display_initializer();

  char welcome[] = {'H', 'E', 'L', 'L', 'O', ' '};
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
  struct GUI_Format_Time *initial_time = (struct GUI_Format_Time *)calloc(1, sizeof(struct GUI_Format_Time));
  GUI_Format_Time_Init(initial_time, GLOBAL_GUI_TIME_UPDATE_INTERVAL, TIME_FORMAT_24H, 0);

  global_gui_instance = (struct GUI_Format *)initial_time;
  global_gui_instance_counter = 0;
}

// VFD display data render routine
void vfdco_clock_display_routine() {
	global_gui_instance->Update(global_gui_instance);

  if(global_button_F1_state == BUTTON_STATE_SHORTPRESS) {
    global_gui_instance->Delete(global_gui_instance);
    switch(global_gui_instance_counter) {
      case GUI_TIME: {
        struct GUI_Format_Date *gui_instance = (struct GUI_Format_Date *)calloc(1, sizeof(struct GUI_Format_Date));
        GUI_Format_Date_Init(gui_instance, GLOBAL_GUI_DATE_UPDATE_INTERVAL, DATE_FORMAT_DDMMYY);
        global_gui_instance = (struct GUI_Format *)gui_instance;
        global_gui_instance_counter = GUI_DATE;
        break;
      }
      case GUI_DATE: {
        struct GUI_Format_Stopwatch *gui_instance = (struct GUI_Format_Stopwatch *)calloc(1, sizeof(struct GUI_Format_Stopwatch));
        GUI_Format_Stopwatch_Init(gui_instance, GLOBAL_GUI_TIME_UPDATE_INTERVAL);
        global_gui_instance = (struct GUI_Format *)gui_instance;
        global_gui_instance_counter = GUI_STOPWATCH;
        break;
      }
      case GUI_STOPWATCH: {
        struct GUI_Format_Time *gui_instance = (struct GUI_Format_Time *)calloc(1, sizeof(struct GUI_Format_Time));
        GUI_Format_Time_Init(gui_instance, GLOBAL_GUI_TIME_UPDATE_INTERVAL, TIME_FORMAT_24H, 0);
        global_gui_instance = (struct GUI_Format *)gui_instance;
        global_gui_instance_counter = GUI_TIME;
        break;
      }
      case GUI_TIME_DATE_SET: {
        struct GUI_Format_Time *gui_instance = (struct GUI_Format_Time *)calloc(1, sizeof(struct GUI_Format_Time));
        GUI_Format_Time_Init(gui_instance, GLOBAL_GUI_TIME_UPDATE_INTERVAL, TIME_FORMAT_24H, 0);
        global_gui_instance = (struct GUI_Format *)gui_instance;
        global_gui_instance_counter = GUI_TIME;
        break;
      }
      default: break;
    }
    global_button_F1_state = BUTTON_STATE_OFF; // Priority clear
  } else if(global_button_F1_state == BUTTON_STATE_LONGPRESS) {
    // To time set menu
    global_gui_instance->Delete(global_gui_instance);

    switch(global_gui_instance_counter) {
      case GUI_TIME: {
        struct GUI_Format_Time_Date_Setter *gui_instance = (struct GUI_Format_Time_Date_Setter *)calloc(1, sizeof(struct GUI_Format_Time_Date_Setter));
        GUI_Format_Time_Date_Setter_Init(gui_instance, GLOBAL_GUI_DATE_UPDATE_INTERVAL, 0);
        global_gui_instance = (struct GUI_Format *)gui_instance;
        global_gui_instance_counter = GUI_TIME_DATE_SET;
        break;
      }
      case GUI_DATE: {
        struct GUI_Format_Time_Date_Setter *gui_instance = (struct GUI_Format_Time_Date_Setter *)calloc(1, sizeof(struct GUI_Format_Time_Date_Setter));
        GUI_Format_Time_Date_Setter_Init(gui_instance, GLOBAL_GUI_TIME_UPDATE_INTERVAL, 1);
        global_gui_instance = (struct GUI_Format *)gui_instance;
        global_gui_instance_counter = GUI_TIME_DATE_SET;
        break;
      }
      default: break;
    }
    global_button_F1_state = BUTTON_STATE_OFF; // Priority clear
  }

  switch(global_button_F2_state) {
    case BUTTON_STATE_SHORTPRESS:
      if(global_gui_instance->F2(global_gui_instance) == BUTTON_ACTION_PERFORMED) global_button_F2_state = BUTTON_STATE_OFF;
      break;
    case BUTTON_STATE_LONGPRESS:
      if(global_gui_instance->F2Var(global_gui_instance) == BUTTON_ACTION_PERFORMED) global_button_F2_state = BUTTON_STATE_OFF;
      break;
  }
  switch(global_button_F3_state) {
    case BUTTON_STATE_SHORTPRESS:
      if(global_gui_instance->F3(global_gui_instance) == BUTTON_ACTION_PERFORMED) global_button_F3_state = BUTTON_STATE_OFF;
      break;
    case BUTTON_STATE_LONGPRESS:
      if(global_gui_instance->F3Var(global_gui_instance) == BUTTON_ACTION_PERFORMED) global_button_F3_state = BUTTON_STATE_OFF;
      break;
  }
  switch(global_button_F4_state) {
    case BUTTON_STATE_SHORTPRESS:
      if(global_gui_instance->F4(global_gui_instance) == BUTTON_ACTION_PERFORMED) global_button_F3_state = BUTTON_STATE_OFF;
      break;
    case BUTTON_STATE_LONGPRESS:
      if(global_gui_instance->F4Var(global_gui_instance) == BUTTON_ACTION_PERFORMED) global_button_F3_state = BUTTON_STATE_OFF;
      break;
  }
}

void vfdco_clock_lights_initializer() {
  vfdco_clr_set_all_RGBW(0, 0, 0, 0);
	vfdco_time_delay_milliseconds(2);
  vfdco_clr_render();

  Light_Pattern_Static_Init(&global_static);
  global_light_instance = (struct Light_Pattern *)&global_static;
}

// VFD LED light illumination routine
void vfdco_clock_lights_routine() {
  global_light_instance->Update(global_light_instance);

  switch(global_button_F3_state) {
    case BUTTON_STATE_SHORTPRESS:
      if(global_light_instance->F3(global_light_instance) == BUTTON_ACTION_PERFORMED) global_button_F3_state = BUTTON_STATE_OFF;
      break;
    case BUTTON_STATE_LONGPRESS:
      if(global_light_instance->F3Var(global_light_instance) == BUTTON_ACTION_PERFORMED) global_button_F3_state = BUTTON_STATE_OFF;
      break;
    default:
      break;
  }
}

// Communication (Serial/USB, Serial/Bluetooth) routine
void vfdco_clock_com_routine() {

}
