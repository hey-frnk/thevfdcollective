/**
  ******************************************************************************
  * @file     vfdco_config.h
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     21-March-2020
  * @brief    This header links the selected configuration header
  *           and performs a parameter validity check
  ************************************************************************************************************************************************************
  * @tableofcontents Table of contents, enter to navigate:
  * CONFIG_PLATFORM
  * CONFIG_SAVED_SETTINGS_TABLE
  * SHARED_UTILITIES
 **/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _VFDCO_CONFIG_H
#define _VFDCO_CONFIG_H

/**
 * @tableofcontents CONFIG_PLATFORM
 * @brief By including either one of the platform headers, its corresponding (optimal) configuration
 *        for the platform is used. Only uncomment one header and leave the rest commented
 */
// #include "vfdco_config_hw2.h" // Arduino/AVR (HW Version < 3.0)
#include "vfdco_config_stm.h" // STM32F0 (HW Version >= 3.0)
// #include "vfdco_config_sim.h" // Simulation platform



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
 *    -> Entry: Corresponding entry for subentry. Must be STRICTLY ASCENDING throughout all corresponsing ENTRY elements. No consistency is checked, so be careful.
 *    -> Offset: Must be STRICTLY ASCENDING for each instance. No consistency is checked here, be careful
 *    -> Size: Size of setting in bytes. Must be kept consistent to the size of the settings type of the instance
 *    -> Setting identifier: Give it a name, but it is advisory to keep a consistent naming
 *    -> Description: Only for documentation/reference. Should be meaningful
 */
#define NUM_SERIALIZABLE_GLOBAL 1
#define NUM_SERIALIZABLE_GUI 2
#define NUM_SERIALIZABLE_LIGHTS 8
     // ENTRY:  Index|    Size| Corresponding enum mapping     | Instance
#define CREATE_SERIALIZED_GLOBAL(ENTRY) \
        ENTRY(      0,      16, 0                              , SERIALIZABLE_CLOCK_ROUTINE    )
               // SUBENTRY: Entry|Offset| Size| Setting identifier                                    | Default value                            | Description, just for documentation Has no effect on anything
#define CREATE_SERIALIZED_GLOBAL_POSITIONS(SUBENTRY) \
          /*-->*/ SUBENTRY(      0,    0,    6, CLOCK_ROUTINE_SETTING_welcome                         , CONFIG_WELCOME_MESSAGE_DEFAULT           , "6 * char, welcome message") \
          /*-->*/ SUBENTRY(      0,    6,    1, CLOCK_ROUTINE_SETTING_global_light_instance_counter   , 0 /* Static */                           , "uint8_t, global_light_instance_counter, saved light instance") \
          /*-->*/ SUBENTRY(      0,    7,    1, CLOCK_ROUTINE_SETTING_global_light_instance_random    , 255 /* Off */                            , "uint8_t, global_light_instance_random, random instance") \
          /*-->*/ SUBENTRY(      0,    8,    1, CLOCK_ROUTINE_SETTING_global_light_it_register        , CONFIG_ITERABLE_ENABLED_INSTANCES_DEFAULT, "uint8_t, global_light_it_register, iterable instance enable register") \
          /*-->*/ SUBENTRY(      0,    9,    1, CLOCK_ROUTINE_SETTING_global_light_rnd_register       , CONFIG_RANDOM_ENABLED_INSTANCES_DEFAULT  , "uint8_t, global_light_rnd_register, random instance enable register") \
          /*-->*/ SUBENTRY(      0,   10,    1, CLOCK_ROUTINE_SETTING_dim_factor_display              , CONFIG_BRIGHTNESS_MAX                    , "uint8_t, dim_factor_display") \
          /*-->*/ SUBENTRY(      0,   11,    1, CLOCK_ROUTINE_SETTING_dim_factor_led                  , CONFIG_BRIGHTNESS_MAX                    , "uint8_t, dim_factor_led") \
          /*-->*/ SUBENTRY(      0,   12,    1, CLOCK_ROUTINE_SETTING_night_shift_start_h             , 0                                        , "uint8_t, night_shift_start_h") \
          /*-->*/ SUBENTRY(      0,   13,    1, CLOCK_ROUTINE_SETTING_night_shift_start_m             , 0                                        , "uint8_t, night_shift_start_m") \
          /*-->*/ SUBENTRY(      0,   14,    1, CLOCK_ROUTINE_SETTING_night_shift_end_h               , 0                                        , "uint8_t, night_shift_end_h") \
          /*-->*/ SUBENTRY(      0,   15,    1, CLOCK_ROUTINE_SETTING_night_shift_end_m               , 0                                        , "uint8_t, night_shift_end_m")

#define CREATE_SERIALIZED_GUI(ENTRY) \
        ENTRY(      1,       2, GUI_TIME                       , SERIALIZABLE_GUI_TIME         ) \
        ENTRY(      2,       1, GUI_DATE                       , SERIALIZABLE_GUI_DATE         )
#define CREATE_SERIALIZED_GUI_POSITIONS(SUBENTRY) \
          /*-->*/ SUBENTRY(      1,    0,    1, GUI_FORMAT_SETTING_TIME_time_mode                     , 0 /* 24H */                              , "uint8_t, time_mode (12/24H/12HLZ)") \
          /*-->*/ SUBENTRY(      1,    1,    1, GUI_FORMAT_SETTING_TIME_dot_mode                      , 0 /* Standard */                         , "uint8_t, dot_mode (blink/flip/off") \
          /*-->*/ SUBENTRY(      2,    0,    1, GUI_FORMAT_SETTING_DATE_date_mode                     , 0 /* DDMMYY */                           , "uint8_t, date_mode (DDMMYY/MMDDYY)")

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
          /*-->*/ SUBENTRY(      3,    0,    1, LIGHT_PATTERN_SETTING_STATIC_position                 , 11 /* Rainbow */                        , "uint8_t, position") \
          /*-->*/ SUBENTRY(      4,    0,    1, LIGHT_PATTERN_SETTING_BLISS_moment                    , 0 /* Nordlicht */                       , "uint8_t, moment") \
          /*-->*/ SUBENTRY(      5,    0,    1, LIGHT_PATTERN_SETTING_SPECTRUM_saturation             , CONFIG_SATURATION_HIGH                  , "uint8_t, saturation") \
          /*-->*/ SUBENTRY(      5,    1,    1, LIGHT_PATTERN_SETTING_SPECTRUM_lightness              , CONFIG_LIGHTNESS_HIGH                   , "uint8_t, lightness") \
          /*-->*/ SUBENTRY(      6,    0,    1, LIGHT_PATTERN_SETTING_RAINBOW_chain_hue_diff          , 10 /* Chill */                          , "int8_t, chain_hue_diff") \
          /*-->*/ SUBENTRY(      6,    1,    1, LIGHT_PATTERN_SETTING_RAINBOW_saturation              , CONFIG_SATURATION_HIGH                  , "uint8_t, saturation") \
          /*-->*/ SUBENTRY(      7,    0,    1, LIGHT_PATTERN_SETTING_CHASE_chase_mode                , 2 /* Flip */                            , "uint8_t, chase_mode") \
          /*-->*/ SUBENTRY(      7,    1,    1, LIGHT_PATTERN_SETTING_CHASE_color_peak_diff           , 0 /* No diff */                         , "uint8_t, color_peak_diff") \
          /*-->*/ SUBENTRY(      8,    0,    1, LIGHT_PATTERN_SETTING_MUSIC_color_peak_diff           , 10 /* Chill */                          , "uint8_t, color_peak_diff") \
          /*-->*/ SUBENTRY(      8,    1,    1, LIGHT_PATTERN_SETTING_MUSIC_saturation                , CONFIG_SATURATION_HIGH                  , "uint8_t, saturation") \
          /*-->*/ SUBENTRY(      9,    0,   24, LIGHT_PATTERN_SETTING_SERIAL0_colors                  , 0 /* Off */                             , "24 * uint8_t, colors") \
          /*-->*/ SUBENTRY(     10,    0,   24, LIGHT_PATTERN_SETTING_SERIAL1_colors                  , 0 /* Off */                             , "24 * uint8_t, colors")

// END MODIFY




/**
 * @tableofcontents SHARED_UTILITIES
 * @brief Shared functions
 */
#include <stdint.h>
static volatile uint8_t _rrx = 0;
static volatile uint8_t _rry = 0;
static volatile uint8_t _rrz = 0;
static volatile uint8_t _rra = 1;
/**
 * @brief <= 7 Bit Xorshift randomizer between [0 .. 2^(bits) - 1]]
 * @param bits Max. 7 bit!
 * @return uint8_t Random number
 */
__attribute__((unused)) static uint8_t vfdco_util_random(uint8_t bits) {
  if(!bits) return 0;
  uint8_t _rrt = _rrx ^ (_rrx >> 1);
  _rrx = _rry;
  _rry = _rrz;
  _rrz = _rra;
  _rra = _rrz ^ _rrt ^ (_rrz >> 3) ^ (_rrt << 1);
  return _rra & ((1 << bits) - 1);
}





// Do not touch from here
// ******** VALIDITY CHECK ********
// Platform validation 
#if (defined(_VFDCO_CONFIG_STM_H) + defined(_VFDCO_CONFIG_HW2_H) + defined(_VFDCO_CONFIG_SIM_H)) != 1
#error "Multiple or no configuration headers! Please specify the platform by only including one configuration header!"
#endif

// Color/gamma correction should not be enabled both
#if (CONFIG_ENABLE_GAMMACORRECTION + CONFIG_ENABLE_COLORCORRECTION) > 1
#warning "It is not advised to enable both color and gamma correction"
#endif

// At least one iterable/random instance should be enabled
#if (CONFIG_ITERABLE_ENABLED_INSTANCES_DEFAULT <= 1)
#error "At least two iterable instances need to be enabled"
#endif
#if (CONFIG_RANDOM_ENABLED_INSTANCES_DEFAULT <= 1)
#error "At least two random instances need to be enabled"
#endif

// ******** Joint configuration parameters  ********
// NUM_BPP & NUM_BYTES from LED configuration
#if CONFIG_USE_RGBW == 0
  #define CONFIG_NUM_BPP ((uint8_t) 3)
#else 
  #define CONFIG_NUM_BPP ((uint8_t) 4)
#endif
#define CONFIG_NUM_BYTES ((uint8_t)(CONFIG_NUM_PIXELS * CONFIG_NUM_BPP))

// NUM_SERIALIZABLE
#define NUM_SERIALIZABLE (NUM_SERIALIZABLE_GLOBAL + NUM_SERIALIZABLE_GUI + NUM_SERIALIZABLE_LIGHTS)

#endif

#ifdef __cplusplus
}
#endif
