/*
 * Being part of something special makes you special
 *
 * Created winter 2020
 * Author: Copyright (C) The VFD Collective, Frank Zheng
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _VFDCO_CONFIG_H
#define _VFDCO_CONFIG_H

#define CONFIG_SW_STRING_LENGTH 6
#define CONFIG_SW_STRING {'3', '.', '0', 'f', ' ', ' '}
#define CONFIG_HW_STRING_LENGTH 4
#define CONFIG_HW_STRING {'3', '.', '0', ' '}

// GENERAL SETTINGS
// Number of digits. How many displays?
#define CONFIG_NUM_DIGITS ((uint8_t) 6)
// Number of LEDs.
#define CONFIG_NUM_PIXELS ((uint8_t) 6)
#define CONFIG_NUM_BPP ((uint8_t) 4)
// Do not touch this.
#define CONFIG_NUM_BYTES ((uint8_t)(CONFIG_NUM_PIXELS * CONFIG_NUM_BPP))

// #define CONFIG_ENABLE_GAMMACORRECTION
#define CONFIG_ENABLE_COLORCORRECTION

// ROUTINE SETTINGS
// How often should we ask the RTC what time it is? (Milliseconds)
#define CONFIG_RTC_UPDATE_INTERVAL 111
// How quickly do we want to update the display?
#define CONFIG_DISPLAY_UPDATE_INTERVAL 6

// DISPLAY SETTINGS
// How many milliseconds should a short notice last?
#define CONFIG_MESSAGE_SHORT      500
// How many milliseconds should a regular message last?
#define CONFIG_MESSAGE_LONG       1000

// LIGHT PATTERN SETTINGS
// If legacy patterns are enabled, the color modes are just like in v2.x
#define CONFIG_LEGACY_LIGHT_PATTERNS       1
// Definition for different lightess & saturation settings
#define CONFIG_LIGHTNESS_HIGH     127
#define CONFIG_LIGHTNESS_MEDIUM   90
#define CONFIG_LIGHTNESS_LOW      50
#define CONFIG_SATURATION_HIGH    255
#define CONFIG_SATURATION_MEDIUM  196
#define CONFIG_SATURATION_LOW     127
// Legacy fade speeds
#define CONFIG_SINGLE_COLOR_FADE_SPEED     2
#define CONFIG_SPECTRUM_FADE_SPEED         60
#define CONFIG_CHASE_FADE_SPEED            60
#define CONFIG_COP_FADE_SPEED              25
#define CONFIG_MOMENTSOFBLISS_FADE_SPEED   10





/**
 * @tableofcontents CONFIG_SAVED_SETTINGS_TABLE
 * SAVED SETTINGS TABLE
 * This is damn cool. Every instance and every class GUI/Light_Pattern can have its saved settings
 * Instead of managing them in their corresponding source code files, you can use the tables below, that will generate setting entries for use automatically
 * So that they are always in sync and you don't need to deal with low level serialization (EEPROM read/write) and stuff
 * Here's how it works:
 * - Every instance/class can have saved settings. The script ENTRY will generate:
 *    -> ONE entry for all settings of ONE instance
 *    -> Index: Must be STRICTLY ASCENDING throughout ALL ENTRY elements. No consistency is checked, so be careful. Might throw warnings at compile if inconsistent
 *    -> Size: Must be the SAME AS THE SUM OF THE SIZES of the SUBENTRY-(ies) of the corresponding component. No consistency is checked here, so be careful
 *    -> Corresponding enum mapping: MUST have the same name as the enum definitions in clock_routines.h. Will throw compile error if not
 *    -> Instance: Name can be freely chosen, but it is advisory to keep a consistent naming
 * - Every ENTRY of saved settings for ONE instance contain #Size amount of bytes of SUBENTRY-(ies)
 *    -> Offset: Must be STRICTLY ASCENDING for each instance. No consistency is checked here, be careful
 *    -> Size: Only for documentation/reference. Must be kept consistent to the size of the settings type of the instance
 *    -> Setting identifier: Give it a name, but it is advisory to keep a consistent naming
 *    -> Description: Only for documentation/reference. Should be meaningful
 */
#define NUM_SERIALIZABLE_GLOBAL 1
#define NUM_SERIALIZABLE_GUI 2
#define NUM_SERIALIZABLE_LIGHTS 7
     // ENTRY:  Index|    Size| Corresponding enum mapping     | Instance
#define CREATE_SERIALIZED_GLOBAL(ENTRY) \
        ENTRY(      0,       7, 0                              , SERIALIZABLE_CLOCK_ROUTINE    )
               // SUBENTRY: Offset| Size| Setting identifier                                    | Description, just for documentation Has no effect on anything
#define CREATE_SERIALIZED_GLOBAL_POSITIONS(SUBENTRY) \
          /*-->*/ SUBENTRY(      0,    6, CLOCK_ROUTINE_SETTING_welcome                         ,"6 * char, welcome message") \
          /*-->*/ SUBENTRY(      6,    1, CLOCK_ROUTINE_SETTING_global_light_instance_counter   ,"uint8_t, global_light_instance_counter") 

#define CREATE_SERIALIZED_GUI(ENTRY) \
        ENTRY(      1,       2, GUI_TIME                       , SERIALIZABLE_GUI_TIME         ) \
        ENTRY(      2,       1, GUI_DATE                       , SERIALIZABLE_GUI_DATE         )
#define CREATE_SERIALIZED_GUI_POSITIONS(SUBENTRY) \
          /*-->*/ SUBENTRY(      0,    1, GUI_FORMAT_SETTING_TIME_time_mode                     ,"uint8_t, time_mode (12/24H/12HLZ)") \
          /*-->*/ SUBENTRY(      1,    1, GUI_FORMAT_SETTING_TIME_dot_mode                      ,"uint8_t, dot_mode (blink/flip/off") \
          /*-->*/ SUBENTRY(      1,    1, GUI_FORMAT_SETTING_DATE_date_mode                     ,"uint8_t, date_mode (DDMMYY/MMDDYY)")

#define CREATE_SERIALIZED_LIGHTS(ENTRY) \
        ENTRY(      3,       1, LIGHT_PATTERN_STATIC           , SERIALIZABLE_LIGHTS_STATIC    ) \
        ENTRY(      4,       1, LIGHT_PATTERN_MOMENTSOFBLISS   , SERIALIZABLE_LIGHTS_BLISS     ) \
        ENTRY(      5,       2, LIGHT_PATTERN_SPECTRUM         , SERIALIZABLE_LIGHTS_SPECTRUM  ) \
        ENTRY(      6,       2, LIGHT_PATTERN_RAINBOW          , SERIALIZABLE_LIGHTS_RAINBOW   ) \
        ENTRY(      7,       2, LIGHT_PATTERN_CHASE            , SERIALIZABLE_LIGHTS_CHASE     ) \
        ENTRY(      8,      24, LIGHT_PATTERN_SERIAL0          , SERIALIZABLE_LIGHTS_SERIAL0   ) \
        ENTRY(      9,      24, LIGHT_PATTERN_SERIAL1          , SERIALIZABLE_LIGHTS_SERIAL1   )
#define CREATE_SERIALIZED_LIGHTS_POSITIONS(SUBENTRY) \
          /*-->*/ SUBENTRY(      0,    1, LIGHT_PATTERN_SETTING_STATIC_position                 ,"uint8_t, position") \
          /*-->*/ SUBENTRY(      0,    1, LIGHT_PATTERN_SETTING_BLISS_moment                    ,"uint8_t, moment") \
          /*-->*/ SUBENTRY(      0,    1, LIGHT_PATTERN_SETTING_SPECTRUM_saturation             ,"uint8_t, saturation") \
          /*-->*/ SUBENTRY(      1,    1, LIGHT_PATTERN_SETTING_SPECTRUM_lightness              ,"uint8_t, lightness") \
          /*-->*/ SUBENTRY(      0,    1, LIGHT_PATTERN_SETTING_RAINBOW_chain_hue_diff          ,"int8_t, chain_hue_diff") \
          /*-->*/ SUBENTRY(      1,    1, LIGHT_PATTERN_SETTING_RAINBOW_saturation              ,"uint8_t, saturation") \
          /*-->*/ SUBENTRY(      0,    1, LIGHT_PATTERN_SETTING_CHASE_chase_mode                ,"uint8_t, chase_mode") \
          /*-->*/ SUBENTRY(      1,    1, LIGHT_PATTERN_SETTING_CHASE_color_peak_diff           ,"uint8_t, color_peak_diff") \
          /*-->*/ SUBENTRY(      0,   24, LIGHT_PATTERN_SETTING_SERIAL0_colors                  ,"24 * uint8_t, colors") \
          /*-->*/ SUBENTRY(      0,   24, LIGHT_PATTERN_SETTING_SERIAL1_colors                  ,"24 * uint8_t, colors")

// END MODIFY
#define NUM_SERIALIZABLE (NUM_SERIALIZABLE_GLOBAL + NUM_SERIALIZABLE_GUI + NUM_SERIALIZABLE_LIGHTS)


#endif

#ifdef __cplusplus
}
#endif
