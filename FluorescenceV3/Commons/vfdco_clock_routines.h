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
#ifndef __AVR__
  #include "../vfdco_config.h"
	#include "../vfdco_serialization.h"
#else 
  #include "vfdco_config.h"
	#include "vfdco_serialization.h"
#endif
#include "vfdco_clock_routine_defines.h"

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

// Microphone initializer
void    vfdco_clock_mic_initializer();

// ######## EVERYTHING FROM HERE IS GENERATED AUTOMATICALLY FROM THE SERIALIZATION MAPPING BY PREPROCESSOR ABUSE in config.h, DO NOT CHANGE MANUALLY ########
#define CREATE_SERIALIZED_INDEX(_globalindex, _size, _enum_map, _serializable_identifier) \
  enum { _serializable_identifier ## _INDEX = _globalindex }; \
  extern uint8_t _serializable_identifier ## _arr[_size];
CREATE_SERIALIZED_GLOBAL(CREATE_SERIALIZED_INDEX)
CREATE_SERIALIZED_GUI(CREATE_SERIALIZED_INDEX)
CREATE_SERIALIZED_LIGHTS(CREATE_SERIALIZED_INDEX)
#undef CREATE_SERIALIZED_INDEX

// Anonymous enums for global settings
#define CREATE_SETTINGS_OFFSET_GLOBAL(_entry, _offset, _size, _setting_identifier, _defaultval, _description) \
  enum { _setting_identifier = _offset };
CREATE_SERIALIZED_GLOBAL_POSITIONS(CREATE_SETTINGS_OFFSET_GLOBAL)
#undef CREATE_SETTINGS_OFFSET_GLOBAL

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
