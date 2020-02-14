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

// GUIs tell what kind of data will shuttle through the shift registers and the display render function(s) will render accordingly.
typedef enum {
  GUI_TIME,             // Display the current time (default of default)
  GUI_DATE,             // Display the current date
  GUI_STOPWATCH,        // Display stopwatch
  GUI_TIME_DATE_SET     // Active when time set or date set is enabled
} gui_instance_t;

typedef enum {
  LIGHT_PATTERN_STATIC,
  LIGHT_PATTERN_SPECTRUM,
  LIGHT_PATTERN_MOMENTSOFBLISS,
  LIGHT_PATTERN_RAINBOW,
  LIGHT_PATTERN_CHASE,
  LIGHT_PATTERN_TIME_CODE,
  LIGHT_PATTERN_COP
} light_pattern_instance_t;

// Initialize function. Run first.
void    vfdco_clock_initializer();

// 
void    vfdco_clock_serialization_initializer();
#ifdef DEBUG
void    vfdco_clock_serialization_routine();
#endif

// Human interface device (Buttons) routine
void    vfdco_clock_hid_initializer();
void    vfdco_clock_hid_routine();

// RTC time refresh routine
void    vfdco_clock_time_initializer();
void    vfdco_clock_time_routine();

// VFD display data render routine
void    vfdco_clock_display_initializer();
void    vfdco_clock_display_routine();

// VFD LED light illumination routine
void    vfdco_clock_lights_initializer();
void    vfdco_clock_lights_routine();

// Communication (Serial/USB, Serial/Bluetooth) routine
// void    vfdco_clock_com_initializer();
// void    vfdco_clock_com_routine();


#include "vfdco_serialization.h"
/*
Serialization Documentation (Bytes)

(8) SERIALIZABLE_CLOCK_ROUTINE: Routine Settings
- (6) 6 * char, welcome
- (1) uint8_t, global_gui_instance_counter
- (1) uint8_t, global_light_instance_counter
(2) SERIALIZABLE_GUI_TIME: Time Display Settings
- (1) uint8_t, time_mode (12/24H/12HLZ)
- (1) uint8_t, dot_mode
(1) SERIALIZABLE_GUI_DATE: Date Display Settings
- (1) uint8_t, date_mode (DDMMYY/MMDDYY)
(1) SERIALIZABLE_LIGHTS_STATIC: Static Color Settings
- (1) uint8_t, position
(1) SERIALIZABLE_LIGHTS_BLISS: MomentsOfBliss Color Settings
- (1) uint8_t, moment
(2) SERIALIZABLE_LIGHTS_SPECTRUM: Spectrum Color Settings
- (1) uint8_t, saturation
- (1) uint8_t, lightness
(2) SERIALIZABLE_LIGHTS_RAINBOW: Rainbow Color Settings
- (1) int8_t, chain_hue_diff
- (1) uint8_t, saturation
(2) SERIALIZABLE_LIGHTS_CHASE: 
- (1) uint8_t, chase_mode
- (1) uint8_t, color_peak_diff
*/

// SERIALIZATION MAPPING, ONLY MODIFY HERE
                                                  //  Index|   Size|  Corresponding enum mapping     | Instance
#define NUM_SERIALIZABLE_GLOBAL 1
#define NUM_SERIALIZABLE_GUI 2
#define NUM_SERIALIZABLE_LIGHTS 5
                                                  //  Index|   Size|  Corresponding enum mapping     | Instance
#define CREATE_SERIALIZED_GLOBAL(ENTRY) \
                                              ENTRY(      0,       8, 0                              , SERIALIZABLE_CLOCK_ROUTINE    )
#define CREATE_SERIALIZED_GUI(ENTRY) \
                                              ENTRY(      1,       2, GUI_TIME                       , SERIALIZABLE_GUI_TIME         ) \
                                              ENTRY(      2,       1, GUI_DATE                       , SERIALIZABLE_GUI_DATE         )
#define CREATE_SERIALIZED_LIGHTS(ENTRY) \
                                              ENTRY(      3,       1, LIGHT_PATTERN_STATIC           , SERIALIZABLE_LIGHTS_STATIC    ) \
                                              ENTRY(      4,       1, LIGHT_PATTERN_MOMENTSOFBLISS   , SERIALIZABLE_LIGHTS_BLISS     ) \
                                              ENTRY(      5,       2, LIGHT_PATTERN_SPECTRUM         , SERIALIZABLE_LIGHTS_SPECTRUM  ) \
                                              ENTRY(      6,       2, LIGHT_PATTERN_RAINBOW          , SERIALIZABLE_LIGHTS_RAINBOW   ) \
                                              ENTRY(      7,       2, LIGHT_PATTERN_CHASE            , SERIALIZABLE_LIGHTS_CHASE     )
// END MODIFY
#define NUM_SERIALIZABLE (NUM_SERIALIZABLE_GLOBAL + NUM_SERIALIZABLE_GUI + NUM_SERIALIZABLE_LIGHTS)

// ######## EVERYTHING FROM HERE IS GENERATED AUTOMATICALLY FROM THE SERIALIZATION MAPPING BY PREPROCESSOR ABUSE, DO NOT CHANGE MANUALLY ########
#define CREATE_SERIALIZED_INDEX(_globalindex, _size, _enum_map, _serializable_identifier) \
  enum { _serializable_identifier ## _INDEX = _globalindex }; \
  uint8_t _serializable_identifier ## _arr[_size];
CREATE_SERIALIZED_GLOBAL(CREATE_SERIALIZED_INDEX)
CREATE_SERIALIZED_GUI(CREATE_SERIALIZED_INDEX)
CREATE_SERIALIZED_LIGHTS(CREATE_SERIALIZED_INDEX)

uint8_t _map_gui_instance_to_serialized_settings_size_index(gui_instance_t instance);
uint8_t _map_lights_instance_to_serialized_settings_size_index(light_pattern_instance_t instance);
// ######## END OF DO NOT TOUCH SECTION ########

#endif

#ifdef __cplusplus
}
#endif
