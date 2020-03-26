/**
  ******************************************************************************
  * @file     vfdco_routines.c
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     12-February-2020
  * @brief    Each (peripheral) component has an initializer and a routine function
  *           The routine function is desiged to be called periodically in main()
  *           The initializers and routines are declared here
  *           Designed to be used with Fluorescence by The VFD Collective
  ******************************************************************************
  * @tableofcontents Table of contents, enter to navigate:
  * TODO
  ******************************************************************************
 **/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _VFDCO_CLOCK_ROUTINES_H
#define _VFDCO_CLOCK_ROUTINES_H

#include <stdint.h>
#include "vfdco_config.h"

// GUIs tell what kind of information will be sent to the dispay
typedef enum {
  GUI_TIME = 0,             // Display the current time (default of default)
  GUI_DATE = 1,             // Display the current date
  GUI_STOPWATCH = 10,       // Display stopwatch
  GUI_TIME_DATE_SET = 20,   // Active when time set or date set is enabled
  GUI_BRIGHTNESS_SET = 21   // Active when brightness (night shift) set is enabled
} gui_instance_t;

// Light Patterns tell in which way or pattern the LED lights glow
typedef enum {
  // Iterable instances, max. 8 instances. Reachable by navigating with F2 in strictly ascending order
  LIGHT_PATTERN_STATIC = 0,           // Corresponds to Light_Pattern_Static class. Single color preset
  LIGHT_PATTERN_MOMENTSOFBLISS = 1,   // Corresponds to Light_Pattern_MomentsOfBliss class
  LIGHT_PATTERN_SPECTRUM = 2,         // Corresponds to Light_Pattern_Spectrum class. Single color fade through the entire spectrum
  LIGHT_PATTERN_RAINBOW = 3,          // Corresponds to Light_Pattern_Rainbow class. Multi-color fade through the spectrum
  LIGHT_PATTERN_CHASE = 4,            // Corresponds to Light_Pattern_Chase class. One color chasing another
  LIGHT_PATTERN_MUSIC = 5,            // Corresponds to Light_Pattern_Music class. Beat reactive pattern
  LIGHT_PATTERN_TIME_CODE = 6,        // Corresponds to Light_Pattern_Time_Code class. Pattern according to time digit value
  LIGHT_PATTERN_COP = 7,              // Corresponds to Light_Pattern_Cop. Police lights

  // Non-iterable instances
  LIGHT_PATTERN_SERIAL0 = 20,         // Corresponds to Light_Pattern_Serial0. Optimized for fast response time from incoming serial color data
  LIGHT_PATTERN_SERIAL1 = 21          // Corresponds to Light_Pattern_Serial1. Optimized for smooth color setting from incoming data
} light_pattern_instance_t;

enum {
  NIGHT_SHIFT_OFF,
  NIGHT_SHIFT_ON
};

// Initialize function. Run first.
void    vfdco_clock_initializer();
void    vfdco_clock_routine();

// Serialization initializer
void    vfdco_clock_serialization_initializer();

// Human interface device (Buttons) routine
void    vfdco_clock_hid_initializer();
void    vfdco_clock_hid_routine();

// RTC time refresh routine
void    vfdco_clock_time_initializer();
void    vfdco_clock_time_routine();

// VFD display data render routine
void    vfdco_clock_gui_initializer();
void    vfdco_clock_gui_routine();

// VFD LED light illumination routine
void    vfdco_clock_lights_initializer();
void    vfdco_clock_lights_routine();

// Power management routine (Brightness, Night Shift/Energy saving)
void    vfdco_clock_power_initializer();
void    vfdco_clock_power_routine();

void    vfdco_clock_settings_default();
void    vfdco_clock_settings_save(uint8_t silent);

// Communication (Serial/USB, Serial/Bluetooth) routine
void    vfdco_clock_com_initializer();
void    vfdco_clock_com_routine();

// Protocol parameters 
#define COM_PROTOCOL_SER0_OFFSET 2
#define COM_PROTOCOL_SER1_OFFSET 2
#define COM_PROTOCOL_LIGHT_SET_OFFSET_INSTANCE 2
#define COM_PROTOCOL_LIGHT_SET_OFFSET_SETTING0 3
#define COM_PROTOCOL_LIGHT_SET_OFFSET_SETTING1 4
#define COM_PROTOCOL_GUI_SET_OFFSET_INSTANCE 2
#define COM_PROTOCOL_GUI_SET_OFFSET_SETTING0 3
#define COM_PROTOCOL_TIMEDATE_SET_OFFSET_DATA 2
#define COM_PROTOCOL_TIMEDATE_SET_OFFSET_FLAG 8
#define COM_PROTOCOL_BRIGHTNESS_SET_OFFSET_DISPLED 2
#define COM_PROTOCOL_BRIGHTNESS_SET_OFFSET_DIMFACTOR 3
#define COM_PROTOCOL_NSH_SET_OFFSET 2
#define COM_PROTOCOL_WELCOME_SET_OFFSET 2
#define COM_PROTOCOL_MESSAGE_DISPLAY_OFFSET 2
#define COM_PROTOCOL_CLOCK_CONTROL_OFFSET 2

#include "vfdco_serialization.h"

#define INSTANCE_NO_SETTINGS 255 // If instance has no settings, the setting to instance mapper will return 255

// ######## EVERYTHING FROM HERE IS GENERATED AUTOMATICALLY FROM THE SERIALIZATION MAPPING BY PREPROCESSOR ABUSE in config.h, DO NOT CHANGE MANUALLY ########
#define CREATE_SERIALIZED_INDEX(_globalindex, _size, _enum_map, _serializable_identifier) \
  enum { _serializable_identifier ## _INDEX = _globalindex }; \
  extern uint8_t _serializable_identifier ## _arr[_size];
CREATE_SERIALIZED_GLOBAL(CREATE_SERIALIZED_INDEX)
CREATE_SERIALIZED_GUI(CREATE_SERIALIZED_INDEX)
CREATE_SERIALIZED_LIGHTS(CREATE_SERIALIZED_INDEX)

uint8_t _map_gui_instance_to_serialized_settings_index(gui_instance_t instance);
uint8_t _map_lights_instance_to_serialized_settings_index(light_pattern_instance_t instance);

// All settings are tracked in serialized_settings
extern uint8_t *const serialized_settings[NUM_SERIALIZABLE];
// The size of each serialized setting is tracked in here
extern const uint8_t serialized_settings_sizes[NUM_SERIALIZABLE];
// ######## END OF DO NOT TOUCH SECTION ########

#endif

#ifdef __cplusplus
}
#endif
