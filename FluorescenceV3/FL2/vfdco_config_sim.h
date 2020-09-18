/*MIT License

Copyright (c) The VFD Collective, Frank from The VFD Collective (Fu Zheng)
Date: 04/25/2020

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

/**
  ******************************************************************************
  * @file     vfdco_config_sim.h
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     21-March-2020
  * @brief    This file contains the global configuration parameters optimized for 
  *           Fluorescence running for PC simulation
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
  ******************************************************************************
 **/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _VFDCO_CONFIG_SIM_H
#define _VFDCO_CONFIG_SIM_H

/**
 * @tableofcontents CONFIG_VERSION
 * @brief Version information
 */
// Edit software version here
#define CONFIG_SW_STRING_LENGTH 6
#define CONFIG_SW_STRING {'3', '.', '0', 's', 'i', 'm'}
// Edit hardware version here
#define CONFIG_HW_STRING_LENGTH 4
#define CONFIG_HW_STRING {'3', '.', '0', '0'}

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
#define CONFIG_USE_RGBW 1
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
#define CONFIG_BRIGHTNESS_HALF  1 // 1/4 brightness
#define CONFIG_BRIGHTNESS_MIN   2 // 1/8 brightness

/**
 * @tableofcontents SECTION_CONFIG_GUI
 * @brief Configuration parameters for the GUI
 */
// How many milliseconds should a short notice last?
#define CONFIG_MESSAGE_SHORT      500
// How many milliseconds should a regular message last?
#define CONFIG_MESSAGE_LONG       1000
// What should be the default welcome message? (Default: {'H', 'E', 'L', 'L', 'O', ' '})
#define CONFIG_WELCOME_MESSAGE_DEFAULT 'H', 'E', 'L', 'L', 'O', ' '

/**
 * @tableofcontents SECTION_CONFIG_LIGHTS
 * @brief Configuration parameters for the high level Light_Pattern class
 */
// Each iterable light pattern is represented as a number n according to clock_routines. The n-th bit in the 
// default register below enables or disables a light pattern from being switched to by F2 pressing or in the randomizer
// 0b[COP | TIME_CODE | MUSIC | CHASE | RAINBOW | SPECTRUM | MOMENTSOFBLISS | STATIC]
#define CONFIG_ITERABLE_ENABLED_INSTANCES_DEFAULT 0b01011111 // Mic & cop disabled like in STM
#define CONFIG_RANDOM_ENABLED_INSTANCES_DEFAULT 0b01011111 // Mic & cop disabled
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
#define CONFIG_MOMENTSOFBLISS_FADE_SPEED   15

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
#define CONFIG_RTC_UPDATE_INTERVAL 156
// How quickly do we want to update the display?
#define CONFIG_DISPLAY_UPDATE_INTERVAL 37

#endif
#ifdef __cplusplus
}
#endif