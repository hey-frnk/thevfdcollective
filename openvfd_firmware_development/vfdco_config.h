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

// GENERAL SETTINGS
// Number of digits. How many displays?
#define CONFIG_NUM_DIGITS ((uint8_t) 6)
// Number of LEDs.
#define CONFIG_NUM_PIXELS ((uint8_t) 6)
#define CONFIG_NUM_BPP ((uint8_t) 4)
#define CONFIG_NUM_BYTES ((uint8_t)(CONFIG_NUM_PIXELS * CONFIG_NUM_BPP))

// ROUTINE SETTINGS
// How often should we ask the RTC what time it is? (Milliseconds)
#define CONFIG_RTC_UPDATE_INTERVAL 111
// How quickly do we want to update the clock display?
#define CONFIG_GUI_TIME_UPDATE_INTERVAL 18
// How quickly do we want to update the date display?
#define CONFIG_GUI_DATE_UPDATE_INTERVAL 240

// DISPLAY SETTINGS
// How many milliseconds should a short notice last?
#define CONFIG_MESSAGE_SHORT      200
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
#define CONFIG_COP_FADE_SPEED              25
#define CONFIG_MOMENTSOFBLISS_FADE_SPEED   3

#endif

#ifdef __cplusplus
}
#endif
