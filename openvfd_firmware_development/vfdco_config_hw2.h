/**
  ******************************************************************************
  * @file     vfdco_config_hw2.h
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     21-March-2020
  * @brief    This file contains the global configuration parameters optimized for 
  *           Fluorescence running on Arduino (HW 2.0/2.1)
  ******************************************************************************
  * @tableofcontents Table of contents, enter to navigate:
  * CONFIG_VERSION
  * SECTION_CONFIG_DISPLAY
  * SECTION_CONFIG_LEDS
  * SECTION_CONFIG_POWER
  * SECTION_CONFIG_GUI
  * SECTION_CONFIG_LIGHTS
  * SECTION_CONFIG_COM
  * SECTION_CONFIG_CLOCK_ROUTINE
  * CONFIG_SAVED_SETTINGS_TABLE
  ******************************************************************************
 **/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _VFDCO_CONFIG_HW2_H
#define _VFDCO_CONFIG_HW2_H

/**
 * @tableofcontents CONFIG_VERSION
 * @brief Version information
 */
// Edit software version here
#define CONFIG_SW_STRING_LENGTH 6
#define CONFIG_SW_STRING {'3', '.', '0', 'a', ' ', ' '}
// Edit hardware version here
#define CONFIG_HW_STRING_LENGTH 4
#define CONFIG_HW_STRING {'2', '.', 'x', ' '}

/**
 * @tableofcontents SECTION_CONFIG_DISPLAY
 * @brief Configuration parameters of the display driver
 */
// Number of digits. How many displays? Default: 6
#define CONFIG_NUM_DIGITS ((uint8_t) 6)

/**
 * @tableofcontents SECTION_CONFIG_LEDS
 * @brief Configuration parameters of the LED driver
 */
// Number of LEDs. Default: 6
#define CONFIG_NUM_PIXELS ((uint8_t) 6)
// Select the LED driver. If set to 1, a separate white LED can be used, when the LEDs are of type SK6812
#define CONFIG_USE_RGBW 0
// Enable gamma correction for more even brightness mapping
#define CONFIG_ENABLE_GAMMACORRECTION 0
// Enable color correction, so that R, G and B are perceived equally bright when set to the same intensity value
#define CONFIG_ENABLE_COLORCORRECTION 1

/**
 * @tableofcontents SECTION_CONFIG_POWER
 * @brief Configuration parameters for power
 */
// Dim factors for both display and light dimming. Corresponds directly to a shift factor
#define CONFIG_BRIGHTNESS_MAX   0 // 1/1 brightness(no dimming)
#define CONFIG_BRIGHTNESS_HALF  2 // 1/4 brightness
#define CONFIG_BRIGHTNESS_MIN   4 // 1/8 brightness

/**
 * @tableofcontents SECTION_CONFIG_GUI
 * @brief Configuration parameters for the GUI
 */
// How many milliseconds should a short notice last?
#define CONFIG_MESSAGE_SHORT      500
// How many milliseconds should a regular message last?
#define CONFIG_MESSAGE_LONG       1000
// What should be the default welcome message? (Default: {'H', 'E', 'L', 'L', 'O', ' '})
#define CONFIG_WELCOME_MESSAGE_DEFAULT {'H', 'E', 'L', 'L', 'O', ' '}

/**
 * @tableofcontents SECTION_CONFIG_LIGHTS
 * @brief Configuration parameters for the high level Light_Pattern class
 */
// Each iterable light pattern is represented as a number n according to clock_routines. The n-th bit in the 
// default register below enables or disables a light pattern from being switched to by F2 pressing or in the randomizer
// 0b[COP | TIME_CODE | MUSIC | CHASE | RAINBOW | SPECTRUM | MOMENTSOFBLISS | STATIC]
#define CONFIG_ITERABLE_ENABLED_INSTANCES_DEFAULT 0b11111111 // All enabled
#define CONFIG_RANDOM_ENABLED_INSTANCES_DEFAULT 0b01011111 // All except mic & cop
// Definition for different lightess & saturation settings
#define CONFIG_LIGHTNESS_HIGH     127
#define CONFIG_LIGHTNESS_MEDIUM   90
#define CONFIG_LIGHTNESS_LOW      50
#define CONFIG_SATURATION_HIGH    255
#define CONFIG_SATURATION_MEDIUM  196
#define CONFIG_SATURATION_LOW     127
// Fade speeds
#define CONFIG_SINGLE_COLOR_FADE_SPEED     2
#define CONFIG_SPECTRUM_FADE_SPEED         60
#define CONFIG_CHASE_FADE_SPEED            60
#define CONFIG_COP_FADE_SPEED              25
#define CONFIG_MOMENTSOFBLISS_FADE_SPEED   10

/**
 * @tableofcontents SECTION_CONFIG_COM
 * @brief Configuration parameters for COM
 */
// Receive and Transmit buffer size, according to protocol
#define CONFIG_COM_RX_BUF_MAX 27
#define CONFIG_COM_TX_BUF_MAX 10

/**
 * @tableofcontents SECTION_CONFIG_CLOCK_ROUTINE
 * @brief Configuration parameters for the high level clock routine
 */
// How often should we ask the RTC what time it is? (Milliseconds)
#define CONFIG_RTC_UPDATE_INTERVAL 850
// How quickly do we want to update the display?
#define CONFIG_DISPLAY_UPDATE_INTERVAL 11



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
#define NUM_SERIALIZABLE_LIGHTS 8
     // ENTRY:  Index|    Size| Corresponding enum mapping     | Instance
#define CREATE_SERIALIZED_GLOBAL(ENTRY) \
        ENTRY(      0,      16, 0                              , SERIALIZABLE_CLOCK_ROUTINE    )
               // SUBENTRY: Offset| Size| Setting identifier                                    | Description, just for documentation Has no effect on anything
#define CREATE_SERIALIZED_GLOBAL_POSITIONS(SUBENTRY) \
          /*-->*/ SUBENTRY(      0,    6, CLOCK_ROUTINE_SETTING_welcome                         , "6 * char, welcome message") \
          /*-->*/ SUBENTRY(      6,    1, CLOCK_ROUTINE_SETTING_global_light_instance_counter   , "uint8_t, global_light_instance_counter, saved light instance") \
          /*-->*/ SUBENTRY(      7,    1, CLOCK_ROUTINE_SETTING_global_light_instance_random    , "uint8_t, global_light_instance_random, random instance") \
          /*-->*/ SUBENTRY(      8,    1, CLOCK_ROUTINE_SETTING_global_light_it_register        , "uint8_t, global_light_it_register, iterable instance enable register") \
          /*-->*/ SUBENTRY(      9,    1, CLOCK_ROUTINE_SETTING_global_light_rnd_register       , "uint8_t, global_light_rnd_register, random instance enable register") \
          /*-->*/ SUBENTRY(     10,    1, CLOCK_ROUTINE_SETTING_dim_factor_display              , "uint8_t, dim_factor_display") \
          /*-->*/ SUBENTRY(     11,    1, CLOCK_ROUTINE_SETTING_dim_factor_led                  , "uint8_t, dim_factor_led") \
          /*-->*/ SUBENTRY(     12,    1, CLOCK_ROUTINE_SETTING_night_shift_start_h             , "uint8_t, night_shift_start_h") \
          /*-->*/ SUBENTRY(     13,    1, CLOCK_ROUTINE_SETTING_night_shift_start_m             , "uint8_t, night_shift_start_m") \
          /*-->*/ SUBENTRY(     14,    1, CLOCK_ROUTINE_SETTING_night_shift_end_h               , "uint8_t, night_shift_end_h") \
          /*-->*/ SUBENTRY(     15,    1, CLOCK_ROUTINE_SETTING_night_shift_end_m               , "uint8_t, night_shift_end_m")

#define CREATE_SERIALIZED_GUI(ENTRY) \
        ENTRY(      1,       2, GUI_TIME                       , SERIALIZABLE_GUI_TIME         ) \
        ENTRY(      2,       1, GUI_DATE                       , SERIALIZABLE_GUI_DATE         )
#define CREATE_SERIALIZED_GUI_POSITIONS(SUBENTRY) \
          /*-->*/ SUBENTRY(      0,    1, GUI_FORMAT_SETTING_TIME_time_mode                     , "uint8_t, time_mode (12/24H/12HLZ)") \
          /*-->*/ SUBENTRY(      1,    1, GUI_FORMAT_SETTING_TIME_dot_mode                      , "uint8_t, dot_mode (blink/flip/off") \
          /*-->*/ SUBENTRY(      0,    1, GUI_FORMAT_SETTING_DATE_date_mode                     , "uint8_t, date_mode (DDMMYY/MMDDYY)")

#define CREATE_SERIALIZED_LIGHTS(ENTRY) \
        ENTRY(      3,       1, LIGHT_PATTERN_STATIC           , SERIALIZABLE_LIGHTS_STATIC    ) \
        ENTRY(      4,       1, LIGHT_PATTERN_MOMENTSOFBLISS   , SERIALIZABLE_LIGHTS_BLISS     ) \
        ENTRY(      5,       2, LIGHT_PATTERN_SPECTRUM         , SERIALIZABLE_LIGHTS_SPECTRUM  ) \
        ENTRY(      6,       2, LIGHT_PATTERN_RAINBOW          , SERIALIZABLE_LIGHTS_RAINBOW   ) \
        ENTRY(      7,       2, LIGHT_PATTERN_CHASE            , SERIALIZABLE_LIGHTS_CHASE     ) \
        ENTRY(      8,       2, LIGHT_PATTERN_MUSIC            , SERIALIZABLE_LIGHTS_MUSIC     ) \
        ENTRY(      9,      24, LIGHT_PATTERN_SERIAL0          , SERIALIZABLE_LIGHTS_SERIAL0   ) \
        ENTRY(     10,      24, LIGHT_PATTERN_SERIAL1          , SERIALIZABLE_LIGHTS_SERIAL1   )
#define CREATE_SERIALIZED_LIGHTS_POSITIONS(SUBENTRY) \
          /*-->*/ SUBENTRY(      0,    1, LIGHT_PATTERN_SETTING_STATIC_position                 , "uint8_t, position") \
          /*-->*/ SUBENTRY(      0,    1, LIGHT_PATTERN_SETTING_BLISS_moment                    , "uint8_t, moment") \
          /*-->*/ SUBENTRY(      0,    1, LIGHT_PATTERN_SETTING_SPECTRUM_saturation             , "uint8_t, saturation") \
          /*-->*/ SUBENTRY(      1,    1, LIGHT_PATTERN_SETTING_SPECTRUM_lightness              , "uint8_t, lightness") \
          /*-->*/ SUBENTRY(      0,    1, LIGHT_PATTERN_SETTING_RAINBOW_chain_hue_diff          , "int8_t, chain_hue_diff") \
          /*-->*/ SUBENTRY(      1,    1, LIGHT_PATTERN_SETTING_RAINBOW_saturation              , "uint8_t, saturation") \
          /*-->*/ SUBENTRY(      0,    1, LIGHT_PATTERN_SETTING_CHASE_chase_mode                , "uint8_t, chase_mode") \
          /*-->*/ SUBENTRY(      1,    1, LIGHT_PATTERN_SETTING_CHASE_color_peak_diff           , "uint8_t, color_peak_diff") \
          /*-->*/ SUBENTRY(      0,    1, LIGHT_PATTERN_SETTING_MUSIC_color_peak_diff           , "uint8_t, color_peak_diff") \
          /*-->*/ SUBENTRY(      1,    1, LIGHT_PATTERN_SETTING_MUSIC_saturation                , "uint8_t, saturation") \
          /*-->*/ SUBENTRY(      0,   24, LIGHT_PATTERN_SETTING_SERIAL0_colors                  , "24 * uint8_t, colors") \
          /*-->*/ SUBENTRY(      0,   24, LIGHT_PATTERN_SETTING_SERIAL1_colors                  , "24 * uint8_t, colors")

// END MODIFY
#define NUM_SERIALIZABLE (NUM_SERIALIZABLE_GLOBAL + NUM_SERIALIZABLE_GUI + NUM_SERIALIZABLE_LIGHTS)


#endif

#ifdef __cplusplus
}
#endif
