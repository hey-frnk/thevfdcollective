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
#include "../vfdco_color_lib.h"
#include "../vfdco_color_lights.h"

// Globally accessible parameters
vfdco_time_t global_time;
vfdco_date_t global_date;

uint8_t global_button_F1_state;
uint8_t global_button_F2_state;
uint8_t global_button_F3_state;
uint8_t global_button_F4_state;

time_event_t global_time_updater;

struct Light_Pattern *global_light_instance = NULL;
struct Light_Pattern_Static global_static;

void vfdco_clock_initializer() {
  // Initialize display and LEDs
  vfdco_display_init(GLOBAL_NUM_DIGITS_NUM_PIXELS);
  vfdco_clr_init(GLOBAL_NUM_DIGITS_NUM_PIXELS);

  global_time_updater = Time_Event_Init(GLOBAL_TIME_UPDATE_INTERVAL);
}

// Human interface device (Buttons) routine
void vfdco_clock_hid_routine() {
  // Poll button activity
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

// VFD display data render routine
void vfdco_clock_display_routine() {

}

void vfdco_clock_lights_initializer() {
  Light_Pattern_Static_Init(&global_static);
  global_light_instance = (struct Light_Pattern *)&global_static;
}

// VFD LED light illumination routine
void vfdco_clock_lights_routine() {
  global_light_instance->Update(global_light_instance);

  switch(global_button_F3_state) {
    case BUTTON_STATE_SHORTPRESS: global_light_instance->F3(global_light_instance); break;
    case BUTTON_STATE_LONGPRESS:  global_light_instance->F3Var(global_light_instance); break;
    default: break;
  }
}

// Communication (Serial/USB, Serial/Bluetooth) routine
void vfdco_clock_com_routine() {

}
