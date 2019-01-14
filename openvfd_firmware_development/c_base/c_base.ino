/*MIT License

Copyright (c) 2019 Frank F. Zheng, Date: 01/05/2019

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


// --------- Libraries Used ---------
#include <avr/pgmspace.h>
#include <RTClib.h>              // RTC Clock Library
#include <Wire.h>                // RTC Clock Communication Library (Wire)
#include <digitalWriteFast.h>    // Clock Cycle Optimized Output
#include <EEPROM.h>              // EEPROM Access
#include <SoftwareSerial.h>      // Bluetooth Serial Communication

// Remove comment to enable temperature sensor reading at STEM pin (DS18B20)
// #define    TEMPERATURE_ENABLE

#ifdef TEMPERATURE_ENABLE
  #include <OneWire.h>          // One Wire DS18B20 Temperature Sensor Access - FLUORESCENCE: No TEMP
#endif

// Running firmware for the first time? Remove comment to configure OpenVFD for the first time (EEPROM reset)!
// #define FIRSTCONFIG

// Hardware Version Control
#define HW_VERSION_22

// --------- Pin Mapping Defines ---------

//         Pin Name  | A | ATMEGA Mapping | Comment, Schematics Signal Name
// ------------------------------------------------------------------------------------------
#define    CLOCK_PIN   2    // ATMEGA:  4   74HC595 SPI Clock Pin, SCK
#define    LATCH_PIN   3    // ATMEGA:  5   74HC595 SPI Latch Pin, RCK
#define    DATA_PIN    4    // ATMEGA:  6   74HC595 SPI Data Pin, SER

#define    B_F1_PIN   5     // ATMEGA: 11   F1 Button
#define    B_F2_PIN   6     // ATMEGA: 12   F2 Button
#define    B_F3_PIN   7     // ATMEGA: 13   F3 Button
#define    B_F4_PIN   8     // ATMEGA: 14   F4 Button

#define    LED_PIN    13    // ATMEGA: 19   LED Pin, LEDPIN

#define    MIC_PIN    A0    // ATMEGA: 23   Microphone Input Pin, MIN
const uint8_t micPin = MIC_PIN;

#ifdef TEMPERATURE_ENABLE
  #define    STEM_PIN   A1  // ATMEGA: 24   Temperature Sensor Input Pin, STEM
#endif

#define    B_GROUND   9     // Bluetooth Ground Pin
#define    B_TXD     10     // Bluetooth TXD Pin
#define    B_RXD     11     // Bluetooth RXD Pin


// ------------------------------------------------------------------------------------------
#define    SHORTPRESS  1    // Short press is 1
#define    LONGPRESS   2    // Long press is 2

#define    NUM_RGB_V   6    // 6 LEDs for OpenVFD
#define    NUM_BYTES_V (NUM_RGB_V * 3)    // 3 * 6 = 18 bytes
const uint8_t NUM_DIGITS_V = 6;

#define    PORT        (PORTB)          // Digital pin's port
#define    PORT_PIN    (PORTB5)         // Digital pin's bit position
#define    NUM_BITS    (8)              // Const 8

// Used for LED crossfade phase value
#define    PI85        0.0369599135716446263348546280

// Boolean values
#define    FALSE       0
#define    TRUE        1


// --------- Firmware Information ---------

// FIRMWARE VERSION STRING
// Version 2.3 fluorescence, Date: 01/05/2019
char fwString[7] = {'v', '2', '.', '3', 'f', ' ', ' '};

/* Changelog
 * 2.3f (01/05/2019):
 * - New mode: Stopwatch! Use F2 to start/reset stopwatch, F3 to pause/resume!
 * - New settings for spectrum fade: Saturation & brightness control
 * - New settings for cross fade and chase fade: brightness control
 * - Significantly faster color wheel algorithm using fixed point arithmetic HSL transform enables adjustment of saturation and lightness besides color (hue)
 * - (Finally) fixed a pesk bugs that caused night shift to turn on/off incorrectly or at unexpected time
 * - Hardware Version 2.2b (experimental) support
 *
 * 2.21rf (04/21/2018):
 * - Fixed an issue in 12h mode
 *
 * 2.2rf (03/21/2018):
 * - Clock can now be fully controlled via Bluetooth!
 * - Significant performance and code efficiency improvements
 * - Dynamic memory optimization, moving less frequently used constant variables into PROGMEM
 * - Night shift is now available. The clock will enter low brightness when time is reached. Time setting using serial command (app or myOpenVFD)
 * - 12h/24h, international date format and leading zero now configurable using serial command
 * - Resolved an issue that would cause significant lag when flip dot mode is active in night shift
 * - First firmware config can now be enabled in code to load default settings correctly
 *
 * 2.2f (02/25/2018):
 * - Serial Bluetooth enables Bluetooth communication between clock and HM-10 module
 *    -> Serial protocol is the same
 * - Minor enhancements on color distance for USB communication
 *
 * 2.1f (01/11/2018):
 * - Removed manual sensitivity setting of VU meter LED mode
 *    -> Software 'sort-of' AGC implementation enables dynamic range detection
 *    -> Parameter is now the color distance
 * - Color distance is now called "Chill", "Regular" and "Tight" instead of delta values of 10, 21 and 42
 * - New clock mode: Silent. In silent mode, the clock segments can be purely software PWM dimmed to a duty cycle of 6.25%
 *    -> Measurements show that this can reduce power consumption up to around 40%!
 * - Date format is now customizable between DDMMYY (default) and MMDDYY
 * - Time format is now customizable between 12h and 24h
 * - Welcome message is now customizable
 * - Temperature sensor can now be turned on simply by removing comment on define
 * - Fixed an issue where the user won't be able to set 29th February when leap year
*/

// --------- Component Initializer ---------

RTC_DS1307 rtc;
uint8_t global_s, global_m, global_h;
#ifdef TEMPERATURE_ENABLE
  OneWire ds(STEM_PIN);
#endif
SoftwareSerial BTSerial(B_TXD, B_RXD);

// --------- Global Variable Initializer ---------

// Interfaces tell what kind of data will shuttle through the shift registers and the display render function(s) will render accordingly.
typedef enum {
  INTERFACE_TIME = 0,             // Display the current time (default of default)
  INTERFACE_DATE = 1,             // Display the current date

  #ifdef TEMPERATURE_ENABLE
    INTERFACE_TEMPERATURE = 2,    // Display the current temperature measured by the clock
  #endif

  INTERFACE_STOPWATCH = 3,        // Display stopwatch

  INTERFACE_TIMEDATE_SET = 128    // Active when time set or date set is enabled
} INTERFACE_MODE_t;

// Global Display Mode
INTERFACE_MODE_t    interface   = INTERFACE_TIME;
// Global LED Mode
// LED_MODE_t          led         = LED_STATIC;

// Clock flags
uint8_t clockFlags = 0;           // Global clock flags
#define B_12H       0             // Bit 0: 12h/24h option
#define B_INTD      1             // Bit 1: DDMMYY or MMDDYY
#define B_NSHIFT    2             // Bit 2: Night shift
#define B_LZERO     3
// End Clock Flags

char welcomeText[NUM_DIGITS_V];

#ifdef TEMPERATURE_ENABLE
  uint32_t ts;                    // Mean temperature value
#endif
uint8_t isFahrenheit = 0;         // Fahrenheit flag

uint8_t INTF0_DM = 0;             // Interface 0 dot mode counter
uint8_t INTF0_DP = 0;             // Interface 0 dot position
uint8_t INTF0_ds = 0;             // Interface 0 dot mode: second flip time delta flag
uint8_t INTF0_dr = FALSE;         // Interface 0 dot mode: second flip direction

uint8_t dateSet = FALSE;          // Date set flag
uint8_t setOnceFlag = FALSE;      // Set once flag. Is used to prevent the clock from ticking on when entered time/date set mode

DateTime *INTF3_TB = nullptr;     // Stopwatch backup time
TimeSpan *INTF3_TS = nullptr;     // Stopwatch time span
unsigned long INTF3_MILLIS = 0;   // Stopwatch milliseconds
unsigned long INTF3_MILLIE = 0;   // Stopwatch elapsed millis

enum {INTF3_INITIAL, INTF3_RUNNING, INTF3_PAUSED};
uint8_t   INTF3_st = INTF3_INITIAL;           // Stopwatch FSM state

// ---- Menu/Interface selector variables
long p_t[4] = {0, 0, 0, 0};    // Button press timer
const long lp_t = 500;         // Long press threshold
uint8_t p[4] = {FALSE, FALSE, FALSE, FALSE};    // Button enable
uint8_t lp[4] = {FALSE, FALSE, FALSE, FALSE};   // Long press enable
uint8_t cF1, cF2, cF3, cF4 = 0;             // Check state variable

// Display Render function pointer
void displayWrite_REG(uint8_t, uint8_t, uint32_t, const char *);
void displayWrite_DIM(uint8_t, uint8_t, uint32_t, const char *);
void (*displayWrite)(uint8_t, uint8_t, uint32_t, const char *) = displayWrite_REG;

typedef struct intervalEvent{
    unsigned long interval;
    unsigned long previousMillis;
} intervalEvent;

struct intervalEvent newiE(long p1) {
  struct intervalEvent iE;
  iE.interval = p1;
  iE.previousMillis = 0;
  return iE;
}

void resetiE(intervalEvent *input) {
  input->previousMillis = 0;
}

uint8_t updateIntervalEvent(intervalEvent *input) {
  unsigned long currentMillis = millis();
  if((currentMillis - input->previousMillis) > input->interval) {
    input->previousMillis = currentMillis;
    return TRUE;
  }
  else return FALSE;
  return FALSE;
}

intervalEvent dotUpdater, jdotUpdater, sdotUpdater, cfUpdater, chUpdater, vuUpdater, vu2Updater, nShiftUpdater;
#ifdef TEMPERATURE_ENABLE
  intervalEvent tsUpdater;
#endif

/**
  ******************************************************************************
  * @file     LED.h
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     13-January-2019
  * @brief    This file contains declarations of a WS2812B library written for Arduino
  *           and the OpenVFD platform, providing expandable LED patterns
  ******************************************************************************
  * @toc      Table of contents, enter to navigate:
  *           SECTION_SHARED_DEFINES
  *           SECTION_LED_COLOR
  *           SECTION_LED_COLOR_MODE
  *           SECTION_LED_COLOR_STATIC
  *           SECTION_LED_COLOR_SPECTRUM
  *           SECTION_LED_COLOR_CROSSFADE
  *           SECTION_LED_COLOR_CHASEFADE
  *           SECTION_LED_COLOR_RESISTOR
  *           SECTION_LED_COLOR_COP
  *           SECTION_LED_COLOR_MUSIC
  *           SECTION_LED_COLOR_SERIAL
  *           SECTION_LED_MODE_MANAGER
  ******************************************************************************
 **/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _LED_H
#define _LED_H

#include <stdint.h>

/** Begin of:
  * @toc SECTION_SHARED_DEFINES
 **/

#ifdef DEBUG
  #include "ArduinoSIM.h"

  #define _debug_static_identifier_ static

  #define NUM_DIGITS_V 6

  #define    FALSE       0
  #define    TRUE        1

  #define    SHORTPRESS  1
  #define    LONGPRESS   2

  extern uint8_t cF2, cF3;
  extern const uint8_t micPin;
  extern uint8_t global_h, global_m, global_s;

  typedef struct intervalEvent{
    unsigned long interval;
    unsigned long previousMillis;
  } intervalEvent;
  
  extern struct intervalEvent newiE(long p1);
  extern void resetiE(intervalEvent *input);
  extern uint8_t updateIntervalEvent(intervalEvent *input);
#else
  #define _debug_static_identifier_
#endif

// Each LED mode is assigned to a number. All number configurations are listed in this enum.
typedef enum {
  LED_STATIC = 0,                 // Static color preset mode. Takes colors out of a predifined constant array
  LED_SERIAL_0 = 2,               // Serial mode 0, only used for instantly writing colors from the color wheel and mood lighting
  LED_SERIAL_1 = 3,               // Serial mode 1, used to write colors smoothly over serial communication
  LED_FADE = 6,                   // Dynamic color preset mode: Single color fade through HSL outer radius
  LED_CROSSFADE = 7,              // Dynamic color preset mode: Cross fade. Each LED has its own color phase with adjustable phase
  LED_CHASEFADE = 8,              // Dynamic color preset mode: Chase fade. One color chases another along the HSL wheel with adjustable direction
  LED_RESISTOR = 10,              // Dynamic color preset mode: Resistor colors. Nerd alert! LEDs have the colors of resistors ring codes according to the current time
  LED_COP = 11,                   // Dynamic color preset mode: Police car lights!
  LED_MUSIC = 20,                 // Dynamic color preset mode: Number of the LEDs turned on equal to loudness. Colors same as cross fade with adjustable phase
  LED_SILENT = 21                 // Static color preset mode with all LEDs off and brightness control down to 6% brightness
} LED_MODE_t;

// Progmem messages (less used, optimized for RAM availability)
_debug_static_identifier_ const char MSG_COLOR[NUM_DIGITS_V] PROGMEM = {' ', 'C', 'O', 'L', 'O', 'R'};
_debug_static_identifier_ const char MSG_FADE[NUM_DIGITS_V] PROGMEM = {'C', ' ', 'F', 'A', 'D', 'E'};
_debug_static_identifier_ const char MSG_CROSSFADE[NUM_DIGITS_V] PROGMEM = {'C', ' ', 'C', 'R', 'F', 'D'};
_debug_static_identifier_ const char MSG_CHASEFADE[NUM_DIGITS_V] PROGMEM = {'C', ' ', 'C', 'H', 'F', 'D'};
_debug_static_identifier_ const char MSG_CRCODE[NUM_DIGITS_V] PROGMEM = {'C', 'R', 'C', 'O', 'D', 'E'};
_debug_static_identifier_ const char MSG_CSOUND[NUM_DIGITS_V] PROGMEM = {'C', 'S', 'O', 'U', 'N', 'D'};
_debug_static_identifier_ const char MSG_CCOP[NUM_DIGITS_V] PROGMEM = {'C', ' ', ' ', 'C', 'O', 'P'};
_debug_static_identifier_ const char MSG_CSILENT[NUM_DIGITS_V] PROGMEM = {'C', ' ', 'S', 'L', 'N', 'T'};
_debug_static_identifier_ const char MSG_LED6_L0[NUM_DIGITS_V] PROGMEM = {' ', ' ', 'H', 'I', 'G', 'H'};
_debug_static_identifier_ const char MSG_LED6_L1[NUM_DIGITS_V] PROGMEM = {' ', ' ', ' ', 'R', 'E', 'G'};
_debug_static_identifier_ const char MSG_LED6_L2[NUM_DIGITS_V] PROGMEM = {' ', ' ', ' ', ' ', 'L', 'O'};
_debug_static_identifier_ const char MSG_INTLDATESET[NUM_DIGITS_V] PROGMEM = {'O', 'O', 'D', 'D', 'Y', 'Y'};
_debug_static_identifier_ const char MSG_INTLDATERESET[NUM_DIGITS_V] PROGMEM = {'D', 'D', 'O', 'O', 'Y', 'Y'};
_debug_static_identifier_ const char MSG_LEADINGZERO_ON[NUM_DIGITS_V] PROGMEM = {0, ' ', ' ', ' ', 'O', 'N'};
_debug_static_identifier_ const char MSG_LEADINGZERO_OFF[NUM_DIGITS_V] PROGMEM = {0, ' ', ' ', 'O', 'F', 'F'};
_debug_static_identifier_ const char MSG_DEFAULT1[NUM_DIGITS_V] PROGMEM= {'D', 'E', 'F', 'A', 'U', 'L'};
_debug_static_identifier_ const char MSG_DEFAULT2[NUM_DIGITS_V] PROGMEM = {'S', 'E', 'T', 'I', 'N', 'G'};
_debug_static_identifier_ const char MSG_DEFAULT3[NUM_DIGITS_V] PROGMEM = {'R', 'E', 'T', 'O', 'R', 'D'};
_debug_static_identifier_ const char MSG_TIMESYNC1[NUM_DIGITS_V] PROGMEM = {'T', '-', 'D', ' ', ' ', ' '};
_debug_static_identifier_ const char MSG_TIMESYNC2[NUM_DIGITS_V] PROGMEM = {'S', 'Y', 'N', 'C', 'E', 'D'};
_debug_static_identifier_ const char MSG_SAVESETTINGS1[NUM_DIGITS_V] PROGMEM = {'A', 'L', 'L', ' ', ' ', ' '};
_debug_static_identifier_ const char MSG_SAVESETTINGS2[NUM_DIGITS_V] PROGMEM = {'S', 'E', 'T', 'I', 'N', 'G'};
_debug_static_identifier_ const char MSG_SAVESETTINGS3[NUM_DIGITS_V] PROGMEM = {'S', 'A', 'V', 'E', 'D', ' '};
_debug_static_identifier_ const char MSG_SILENT1[NUM_DIGITS_V] PROGMEM = {'F', 'U', 'L', 'L', ' ', ' '};
_debug_static_identifier_ const char MSG_SILENT2[NUM_DIGITS_V] PROGMEM = {'N', 'I', 'G', 'H', 'T', ' '};
_debug_static_identifier_ const char MSG_SILENT3[NUM_DIGITS_V] PROGMEM = {'S', 'H', 'I', 'F', 'T', ' '};
_debug_static_identifier_ const char MSG_ON[NUM_DIGITS_V] PROGMEM = {' ', ' ', ' ', ' ', 'O', 'N'};
_debug_static_identifier_ const char MSG_OFF[NUM_DIGITS_V] PROGMEM = {' ', ' ', ' ', 'O', 'F', 'F'};
_debug_static_identifier_ const char MSG_ERROR[NUM_DIGITS_V] PROGMEM = {'E', 'R', 'R', 'O', 'R', ' '};

_debug_static_identifier_ const char LED0PM[][4] = {         {' ', 'O', 'F', 'F'},
                                   {' ', ' ', 'O', 'N'},
                                   {' ', 'L', 'O', 'N'},
                                   {' ', 'R', 'E', 'D'},
                                   {' ', 'G', 'R', 'N'},
                                   {'B', 'L', 'U', 'E'},
                                   {'Y', 'E', 'L', 'O'},
                                   {'O', 'R', 'N', 'G'},
                                   {'C', 'Y', 'A', 'N'},
                                   {'P', 'R', 'E', 'D'},
                                   {'P', 'R', 'P', 'L'},
                                   {' ', 'R', 'N', 'B'},
                                   {'P', 'R', 'N', 'B'},
                                   {' ', 'G',   2, 'B'},
                                   {' ', 'R',   2, 'B'},
                                   {' ', 'R',   2, 'G'}};

_debug_static_identifier_ const char LED7PM[][NUM_DIGITS_V] = {{'C', 'H', 'I', 'L', 'L', ' '},
                                  {'R', 'E', 'G', 'U', 'L', 'A'},
                                  {'T', 'I', 'G', 'H', 'T', ' '}};

_debug_static_identifier_ const char LED8PM[][4] = {        {' ', 'R', '-', 'L'},
                                  {' ', 'L', '-', 'R'},
                                  {'F', 'L', 'I', 'P'},
                                  {'C', 'L', 'A', 'P'}};

#define LED0_cOffset 11                               // # Single color presets
#define LED0_mcOffset (LED0_cOffset - 1)              // # Single color presets - 1

_debug_static_identifier_ const uint8_t led_scPresets[][3] =  {{  0,   0,   0}, // Off! ("Off")
                                     {255, 255, 255}, // White ("On")
                                     {200, 255,  32}, // Warm White ("LON := Light On")
                                     {  0, 255,   0}, // Red ("Red")
                                     {255,   0,   0}, // Green ("GRN := Green")
                                     {  0,   0, 255}, // Blue ("Blue")
                                     {125, 255,   0}, // Yellow ("YELO = Yellow")
                                     { 30, 255,   0}, // Orange ("ORNG = Orange")
                                     {255,   0, 128}, // Cyan ("Cyan")
                                     {  0, 255, 170}, // Magenta ("PRED := Purple Red")
                                     {  0, 200, 255}  // Purple ("PRPL := Purple")
                                                    };

_debug_static_identifier_ const uint8_t led_Presets[][18] = {
                                    {  0, 200, 255,  // Rainbow colors!
                                       0,   0, 255,
                                     255,   0,   0,
                                     128, 255,   0,
                                      30, 255,   0,
                                       0, 255,   0},

                                    {128, 255, 255,  // Pastel rainbow!
                                     128,  50, 255,
                                     255,   0, 128,
                                     255, 128, 128,
                                     255, 255, 128,
                                     100, 255, 128},

                                    {255,   0,   0,  // Green to blue!
                                     240,   0,  64,
                                     216,   0, 128,
                                     128,   0, 216,
                                      64,   0, 240,
                                       0,   0, 255},

                                    {  0, 255,   3,  // Red to blue!
                                       0, 255,  10,
                                       0, 240,  25,
                                       0, 200,  80,
                                       0, 100, 150,
                                       0,  50, 255},

                                    {  3, 255,   0,  // Red to green!
                                      30, 255,   0,
                                      60, 240,   0,
                                     100, 180,   0,
                                     180, 180,   0,
                                     255,  20,   0}
                                     };

// ---- LED Resistor preset GRB     0: Off     1: Brown        2: Red       3: Orange     4: Yellow      5: Green     6: Blue      7: Purple      8: Gray       9: White
_debug_static_identifier_ const uint8_t led_Resistor[][3] =  {{0, 0, 0}, {128, 255, 64}, {0, 255, 0}, {30, 255, 0}, {125, 255, 0}, {255, 0, 0}, {0, 0, 255}, {0, 200, 255}, {40, 40, 60}, {255, 255, 255}};

_debug_static_identifier_ const uint8_t LED11_colors[][3] = {{  0, 255,  10},       // Cop red
                                   {  0,  15, 255}};      // Cop blue

typedef enum {LED_L_HIGH = 127,   LED_L_REG = 90,   LED_L_LOW = 50    } LED_L_t;
typedef enum {LED_S_HIGH = 255,   LED_S_REG = 196,  LED_S_LOW = 127   } LED_S_t;

uint32_t ledPhase(uint8_t h, uint8_t s, uint8_t l);
uint8_t getMicData(uint8_t MicPin);

/** Begin of:
  * @toc SECTION_LED_COLOR
 **/
/**
  * @brief  Definition of LED_Color class
 **/
struct LED_Color {
  #ifndef DEBUG
    uint32_t        t_f;
  #endif
  uint8_t         NUM_RGB,
                  NUM_BYTES,
                  *rgb_arr,
                  *target_arr;
  void            (*render)(const struct LED_Color *self);
};

/**
  * @brief  Constructor of LED_Color class
 **/
void LED_Color_Init(struct LED_Color *self, uint8_t num_pixel, uint8_t bpp, uint8_t *rgb, uint8_t *target);

/** Begin of:
  * @toc SECTION_LED_COLOR_MODE
 **/
/**
  * @brief  Virtual table for LED_Color_Mode
 **/
struct LED_Color_Mode_VTable {
  void                  (*F3)               (const void *unsafe_self);
  void                  (*F3Var)            (const void *unsafe_self);
  void                  (*Update)           (const void *unsafe_self);
  void                  (*Hello)            (const void *unsafe_self);
};

/**
  * @brief  Definition of LED_Color_Mode class
 **/
struct LED_Color_Mode {
  struct LED_Color      *l;
  uint8_t               NUM_RGB,      // Copy of l->NUM_RGB
                        NUM_BYTES,    // Copy of l->NUM_BYTES
                        *rgb_arr,     // Copy of l->rgb_arr
                        *target_arr;  // Copy of l->target_arr

  void                  (*ledSmoothWrite)   (struct LED_Color_Mode *self);
  void                  (*ledDirectWrite)   (struct LED_Color_Mode *self, const uint8_t *ledTarget);

  // VTable (virtual) functions
  void                  (*F3)               (const void *unsafe_self);
  void                  (*F3Var)            (const void *unsafe_self);
  void                  (*Update)           (const void *unsafe_self);
  void                  (*Hello)            (const void *unsafe_self);

  struct LED_Color_Mode_VTable VTable;
};

/**
  * @brief  Constructor of LED_Color_Mode class
 **/
void LED_Color_Mode_Init(struct LED_Color_Mode *self, struct LED_Color *l);

/** Begin of:
  * @toc SECTION_LED_COLOR_STATIC
 **/
/**
  * @brief  Definition of LED_Color_Static class
 **/
struct LED_Color_Static {
  struct LED_Color_Mode super;

  uint8_t               position;   // Color lookup array index
};

/**
  * @brief  Constructor of LED_Color_Static class
 **/
void LED_Color_Static_Init(struct LED_Color_Static *self, struct LED_Color *l, uint8_t position);

/** Begin of:
  * @toc SECTION_LED_COLOR_SPECTRUM
 **/
/**
  * @brief  Definition of LED_Color_Spectrum class
 **/
struct LED_Color_Spectrum {
  struct LED_Color_Mode super;

  uint8_t               angle;                 // LED regular fade position
  LED_L_t               lightness;             // LED regular fade lightness
  LED_S_t               saturation;            // LED regular fade saturation
  intervalEvent         angleUpdater;          // Timer for position
};

/**
  * @brief  Constructor of LED_Color_Spectrum class
 **/
void LED_Color_Spectrum_Init(struct LED_Color_Spectrum *self, struct LED_Color *l);

/** Begin of:
  * @toc SECTION_LED_COLOR_CROSSFADE
 **/
/**
  * @brief  Definition of LED_Color_Cross class
 **/
struct LED_Color_Cross {
  struct LED_Color_Mode super;

  uint8_t               angle,                 // LED cross fade position
                        delta;                 // (LED7_delta, shared, saved)
  LED_L_t               lightness;             // LED cross fade lightness
  intervalEvent         angleUpdater;          // Timer for position
};

/**
  * @brief  Constructor of LED_Color_Cross class
 **/
void LED_Color_Cross_Init(struct LED_Color_Cross *self, struct LED_Color *l, uint8_t delta);

/** Begin of:
  * @toc SECTION_LED_COLOR_CHASEFADE
 **/
/**
  * @brief  Definition of LED_Color_Chase class
 **/
struct LED_Color_Chase {
  struct LED_Color_Mode super;

  uint8_t               direction,             // LED chase fade direction state (LED8_dp, shared, saved)
                        angle,                 // LED chase fade position
                        secondFlip,            // LED chase fade second flip (LED8_ds)
                        directionFlag,         // LED chase fade direction flag (LED8_dr)
                        state,                 // LED chase fade FSM position (LED8_st)
                        MicPin,
                        flipdir,               // Based on interface 0 dot flip
                        *s;                    // Global second
  LED_L_t               lightness;             // LED chase fade lightness
  intervalEvent         stateUpdater,          // FSM timer
                        delayUpdater;          // Mic read in peak delay timer
};

/**
  * @brief  Constructor of LED_Color_Chase class
 **/
void LED_Color_Chase_Init(
  struct LED_Color_Chase *self,
  struct LED_Color *l,
  uint8_t MicPin,               // Pin number of microphone
  uint8_t *Second,              // Reference of a globally updated second variable
  uint8_t FlipSync,             // Flag to sync L-R <-> R-L chase flip with dot flip of display
  uint8_t ChaseMode             // Saved parameter chase mode (L-R/R-L/flip/mic)
);

/** Begin of:
  * @toc SECTION_LED_COLOR_RESISTOR
 **/
/**
  * @brief  Definition of LED_Color_Resistor class
 **/
struct LED_Color_Resistor {
  struct LED_Color_Mode super;

  uint8_t               *h, *m, *s;            // Globally updated h/m/s
};

/**
  * @brief  Constructor of LED_Color_Resistor class
 **/
void LED_Color_Resistor_Init(
  struct LED_Color_Resistor *self,
  struct LED_Color *l,
  // Reference of globally updated hour/minute/second variable
  uint8_t *h, uint8_t *m, uint8_t *s
);

/** Begin of:
  * @toc SECTION_LED_COLOR_COP
 **/
/**
  * @brief  Definition of LED_Color_Cop class
 **/
struct LED_Color_Cop {
  struct LED_Color_Mode super;

  uint8_t               pattern,               // LED cop mode pattern (LED11_pt, shared, saved)
                        state;                 // Cop FSM position
  intervalEvent         stateUpdater;          // FSM timer

  // Internal method generating cop pattern
  void                  (*copHalfRender)(struct LED_Color_Mode *rself, uint8_t left, uint8_t right);
};

/**
  * @brief  Constructor of LED_Color_Cop class
 **/
void LED_Color_Cop_Init(struct LED_Color_Cop *self, struct LED_Color *l, uint8_t pattern);

/** Begin of:
  * @toc SECTION_LED_COLOR_MUSIC
 **/
/**
  * @brief  Definition of LED_Color_Music class
 **/
struct LED_Color_Music {
  struct LED_Color_Mode super;

  uint8_t               angle,                 // LED music cross fade position
                        delta,                 // LED music cross fade delta
                        state,                 // LED microphone mode off fader state
                        delayState,            // LED microphone mode blink delay state
                        MicPin;
  LED_L_t               lightness;             // LED lightness
  intervalEvent         angleUpdater,          // LED music cross fade position update timer
                        stateUpdater,          // FSM timer
                        delayUpdater;          // Blink delay timer
};

/**
  * @brief  Constructor of LED_Color_Music class
 **/
void LED_Color_Music_Init(struct LED_Color_Music *self, struct LED_Color *l, uint8_t MicPin);

/** Begin of:
  * @toc SECTION_LED_COLOR_SERIAL
 **/
/**
  * @brief  Definition of classes LED_Color_Serial0 and LED_Color_Serial1
 **/
struct LED_Color_Serial0 { struct LED_Color_Mode super; };
struct LED_Color_Serial1 { struct LED_Color_Mode super; };

/**
  * @brief  Constructor of LED_Color_Serial0/1 class
 **/
void LED_Color_Serial0_Init(struct LED_Color_Serial0 *self, struct LED_Color *l);
void LED_Color_Serial1_Init(struct LED_Color_Serial1 *self, struct LED_Color *l);

/** Begin of:
  * @toc SECTION_LED_MODE_MANAGER
 **/
/**
  * @brief  This struct keeps all saved data
 **/
struct LED_SavedParam_Serialization {
  uint8_t led,
          LED0P,
          *SER0,
          *SER1,
          LED7_delta,
          LED8_dp,
          LED11_pt,
          LED21_DF,
          LED21_hEN,
          LED21_mEN,
          LED21_hDS,
          LED21_mDS;
};

/**
  * @brief  Internal array mapping
 **/
#define COLORPOS_STATIC      0
#define COLORPOS_FADE        1
#define COLORPOS_CROSSFADE   2
#define COLORPOS_CHASEFADE   3
#define COLORPOS_RESISTOR    4
#define COLORPOS_COP         5
#define COLORPOS_MUSIC       6

#define COLORPOS_SERIAL0     7
#define COLORPOS_SERIAL1     8

#define COLORPOS_MAXCNT      9

/**
  * @brief  Definition of LED_Mode_Manager class. LED manager instance tracks all LED modes
 **/
struct LED_Mode_Manager {
  LED_MODE_t            LED;                   // Current active instance (quick n dirty 'dynamic_cast')
  struct LED_Color      *LED_Hardware;         // Hardware mapping
  struct LED_SavedParam_Serialization s;       // Saved parameters

  struct LED_Color_Mode **LED_Instance;        // Array of all instances!

  uint8_t               LED_Instance_Position; // Index in LED_Instance array

  // Methods
  void (*LED_Manager_Routine)(struct LED_Mode_Manager *self);
  struct LED_SavedParam_Serialization (*EEPGenerate)(struct LED_Mode_Manager *self);
  void (*EEPReadIn)(struct LED_Mode_Manager *self, struct LED_SavedParam_Serialization s);
};

/**
  * @brief  Constructor of LED_Mode_Manager
 **/
void LED_Mode_Manager_Init(
  struct LED_Mode_Manager *self,
  struct LED_Color *l,                    // Hardware instance
  struct LED_SavedParam_Serialization s   // Saved parameters
);

static struct LED_Color_Static led_static;
static struct LED_Color_Serial0 led_s0;
static struct LED_Color_Serial1 led_s1;
static struct LED_Color_Spectrum led_spectrum;
static struct LED_Color_Cross led_cfade;
static struct LED_Color_Chase led_chfd;
static struct LED_Color_Resistor led_resistor;
static struct LED_Color_Cop led_cop;
static struct LED_Color_Music led_music;


#endif

#ifdef __cplusplus
}
#endif

/**
  ******************************************************************************
  * @end      END OF FILE LED.h
  ******************************************************************************
 **/
/**
  ******************************************************************************
  * @file     LED.h
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     13-January-2019
  * @brief    This file contains definitions of a WS2812B library written for Arduino
  *           and the OpenVFD platform, providing expandable LED patterns
  ******************************************************************************
  * @toc      Table of contents, enter to navigate:
  *           SECTION_SHARED_DEFINES
  *           SECTION_LED_COLOR
  *           SECTION_LED_COLOR_MODE
  *           SECTION_LED_COLOR_STATIC
  *           SECTION_LED_COLOR_SPECTRUM
  *           SECTION_LED_COLOR_CROSSFADE
  *           SECTION_LED_COLOR_CHASEFADE
  *           SECTION_LED_COLOR_RESISTOR
  *           SECTION_LED_COLOR_COP
  *           SECTION_LED_COLOR_MUSIC
  *           SECTION_LED_COLOR_SERIAL
  *           SECTION_LED_MODE_MANAGER
  ******************************************************************************
 **/

#ifdef DEBUG
#include <math.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "LED.h"
#include "ArduinoSIM.h"
unsigned long programruntime = 0;

void clearInterface() {
  cF2 = 0;
  cF3 = 0;
}
#endif

/** Begin of:
  * @toc SECTION_SHARED_DEFINES
 **/

// Faster color wheel with HSL transform and fixed point arithmetics
// https://stackoverflow.com/questions/2353211
static inline int32_t _mdisplay_fpmul(int32_t x, int32_t y) { return ((int64_t)x * (int64_t)y) / 0x01000000; }
static int32_t _mdisplay_hslp_hue2rgb(int32_t p, int32_t q, int32_t t) {
  if(t < 0x00000000) t += 0x01000000;
  if(t > 0x01000000) t -= 0x01000000;
  if(t < 0x002AAAAB) return p + _mdisplay_fpmul(0x06000000, _mdisplay_fpmul((q - p), t));
  if(t < 0x00800000) return q;
  if(t < 0x00AAAAAB) return p + _mdisplay_fpmul(0x06000000, _mdisplay_fpmul((q - p), 0x00AAAAAB - t));
  return p;
}

uint32_t ledPhase(uint8_t h, uint8_t s, uint8_t l) {
  int32_t hFP = _mdisplay_fpmul((int32_t)h << 23, 0x20202);
  int32_t sFP = _mdisplay_fpmul((int32_t)s << 23, 0x20202);
  int32_t lFP = _mdisplay_fpmul((int32_t)l << 23, 0x20202);

  int32_t rFP, gFP, bFP;
  if(s == 0) return ((uint32_t)l) | (((uint32_t)l) << 8) | (((uint32_t)l) << 16);
  else{
    int32_t q = (lFP < 0x00800000) ? _mdisplay_fpmul(lFP, 0x01000000 + sFP) : (lFP + sFP - _mdisplay_fpmul(lFP, sFP));
    int32_t p = _mdisplay_fpmul(0x02000000, lFP) - q;
    rFP = _mdisplay_hslp_hue2rgb(p, q, hFP + 0x00555555);
    gFP = _mdisplay_hslp_hue2rgb(p, q, hFP);
    bFP = _mdisplay_hslp_hue2rgb(p, q, hFP - 0x00555555);
  }

  return ((uint32_t)(_mdisplay_fpmul(0x3FC00000, gFP) >> 6 ) & 0x00FF0000) | \
         ((uint32_t)(_mdisplay_fpmul(0x3FC00000, rFP) >> 14) & 0x0000FF00) | \
         ((uint32_t)(_mdisplay_fpmul(0x3FC00000, bFP) >> 22) & 0x000000FF);
}

const uint8_t LED20_samplesCount = 8;                     // Empirical sample acquisition array length
uint16_t LED20_samples[LED20_samplesCount] = {0};         // Empirical sample acquisition array allocation
uint8_t LED20_sampleIndex = 0;                            // Buffer push sample index
const uint8_t LED20_analogSamples = 128;                  // How many samples per reading
const uint8_t LED20_sense = 2;                            // Noise threshold
const float LED20_linFactor = -0.50;                      // VU meter log mapping adjust: linFactor in [-1 ... inf], -1 <= lF < 0: log, 0: lin, > 0: exp

static inline uint16_t LED20_mapUnsigned16(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
  if(in_max - in_min == 0) return 0;
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Dynamically adjust sensitivity according to amplitude (sort-of-AGC)
static uint16_t LED20_lLevelArr_vPush(uint16_t amplitudeIn) {
  // Queue push in
  if(LED20_sampleIndex < LED20_samplesCount) LED20_samples[LED20_sampleIndex++] = amplitudeIn;
  else{
    LED20_samples[LED20_sampleIndex = 0] = amplitudeIn;
    LED20_sampleIndex++;
  }

  // Control loop of maintaining healthy amplitude
  uint16_t lMin = 1023;
  uint16_t lMax = 0;
  for(uint8_t i = 0; i < LED20_samplesCount; i++) {
    if(LED20_samples[i] < lMin) lMin = LED20_samples[i];
    if(LED20_samples[i] > lMax) lMax = LED20_samples[i];
  }

  if(lMax - lMin < LED20_sense) return 0;
  uint16_t amplitudeAmplified = LED20_mapUnsigned16(amplitudeIn, lMin, lMax, 0, 255);
  return amplitudeAmplified;
}

// This function reads the microphone input and returns a value between 0 and 255
// Threshold sets the minimum value the mic is sensitive to. Must not be larger than 49 (not checked in the function, results division by zero otherwise)
uint8_t getMicData(uint8_t MicPin) {
  #ifdef DEBUG
  srand(0);
  return (rand() % 256);
  #endif

  uint16_t dMicRead = 512;
  uint16_t dPrevSample = 512;

  // Obtain amplitude
  uint16_t dMicMax = 0;
  uint16_t dMicMin = 1023;
  for(uint8_t i = 0; i < LED20_analogSamples; i++) {
    dPrevSample = dMicRead;
    dMicRead = analogRead(MicPin);
    // Digital low pass filtering
    dMicRead = (dMicRead >> 2) + (dPrevSample >> 1) + (dPrevSample >> 2);

    // Get minimum and maximum
    if(dMicRead > dMicMax) dMicMax = dMicRead;
    else if(dMicRead < dMicMin) dMicMin = dMicRead;
  }

  float dMicA = (float)(LED20_lLevelArr_vPush(dMicMax - dMicMin));
  // Serial.println(dMicA);
  uint8_t u = dMicA ? (uint8_t)(roundf(powf(dMicA / 255.0, LED20_linFactor) * dMicA)) : 0;
  return u;
}

#ifdef DEBUG
struct intervalEvent newiE(long p1) {
  intervalEvent iE;
  iE.interval = p1;
  iE.previousMillis = 0;
  return iE;
}

void resetiE(intervalEvent *input) {
  input->previousMillis = 0;
}

uint8_t updateIntervalEvent(intervalEvent *input) {
  unsigned long currentMillis = programruntime;
  if((currentMillis - input->previousMillis) > input->interval) {
    input->previousMillis = currentMillis;
    return TRUE;
  }
  else return FALSE;
  return FALSE;
}
#endif

/** Begin of:
  * @toc SECTION_LED_COLOR
 **/

/**
  * @brief  Implementation of void LED_Color::render (static void _LED_Color_render)
 **/
#ifdef DEBUG

static void _LED_Color_render(const struct LED_Color *self) {
  printf("Rendering %hhu bytes of LED data with %hhu pixels and %hhu bytes per pixel.\n", self->NUM_BYTES, self->NUM_RGB, (uint8_t)(self->NUM_BYTES / self->NUM_RGB));
  printf("rgb_arr: ");
  for (uint8_t i = 0; i < self->NUM_RGB; ++i) {
    printf("(%03hhu, %03hhu, %03hhu) ", self->rgb_arr[3 * i], self->rgb_arr[3 * i + 1], self->rgb_arr[3 * i + 2]);
  }
  printf("\n\n");
}

#else

static void _LED_Color_render(const struct LED_Color *self) {
  uint32_t        t_f = self->t_f;
  uint8_t         NUM_RGB = self->NUM_RGB,
                  NUM_BYTES = self->NUM_BYTES,
                  *rgb_arr = self->rgb_arr;
  // This section is writF1 by:
  // Acrobotic - 01/10/2013
  // Author: x1sc0

  /*License:
    Beerware License; if you find the code useful, and we happen to cross
    paths, you're encouraged to buy us a beer. The code is distributed hoping
    that you in fact find it useful, but  without warranty of any kind.*/
  if (!rgb_arr) return;

  while ((micros() - t_f) < 50L); // wait for 50us (data latch)

  cli(); // Disable interrupts so that timing is as precise as possible
  volatile uint8_t
   *p    = rgb_arr,   // Copy the start address of our data array
    val  = *p++,      // Get the current byte value & point to next byte
    high = PORT |  _BV(PORT_PIN), // Bitmask for sending HIGH to pin
    low  = PORT & ~_BV(PORT_PIN), // Bitmask for sending LOW to pin
    tmp  = low,       // Swap variable to adjust duty cycle
    nbits= NUM_BITS;  // Bit counter for inner loop
  volatile uint16_t
    nbytes = NUM_BYTES; // Byte counter for outer loop
  asm volatile(
    // Instruction        CLK     Description                 Phase
   "nextbit:\n\t"         // -    label                       (T =  0)
    "sbi  %0, %1\n\t"     // 2    signal HIGH                 (T =  2)
    "sbrc %4, 7\n\t"      // 1-2  if MSB set                  (T =  ?)
     "mov  %6, %3\n\t"    // 0-1   tmp'll set signal high     (T =  4)
    "dec  %5\n\t"         // 1    decrease bitcount           (T =  5)
    "nop\n\t"             // 1    nop (idle 1 clock cycle)    (T =  6)
    "st   %a2, %6\n\t"    // 2    set PORT to tmp             (T =  8)
    "mov  %6, %7\n\t"     // 1    reset tmp to low (default)  (T =  9)
    "breq nextbyte\n\t"   // 1-2  if bitcount ==0 -> nextbyte (T =  ?)
    "rol  %4\n\t"         // 1    shift MSB leftwards         (T = 11)
    "rjmp .+0\n\t"        // 2    nop nop                     (T = 13)
    "cbi   %0, %1\n\t"    // 2    signal LOW                  (T = 15)
    "rjmp .+0\n\t"        // 2    nop nop                     (T = 17)
    "nop\n\t"             // 1    nop                         (T = 18)
    "rjmp nextbit\n\t"    // 2    bitcount !=0 -> nextbit     (T = 20)
   "nextbyte:\n\t"        // -    label                       -
    "ldi  %5, 8\n\t"      // 1    reset bitcount              (T = 11)
    "ld   %4, %a8+\n\t"   // 2    val = *p++                  (T = 13)
    "cbi   %0, %1\n\t"    // 2    signal LOW                  (T = 15)
    "rjmp .+0\n\t"        // 2    nop nop                     (T = 17)
    "nop\n\t"             // 1    nop                         (T = 18)
    "dec %9\n\t"          // 1    decrease bytecount          (T = 19)
    "brne nextbit\n\t"    // 2    if bytecount !=0 -> nextbit (T = 20)
    ::
    // Input operands         Operand Id (w/ constraint)
    "I" (_SFR_IO_ADDR(PORT)), // %0
    "I" (PORT_PIN),           // %1
    "e" (&PORT),              // %a2
    "r" (high),               // %3
    "r" (val),                // %4
    "r" (nbits),              // %5
    "r" (tmp),                // %6
    "r" (low),                // %7
    "e" (p),                  // %a8
    "w" (nbytes)              // %9
  );
  sei();                          // Enable interrupts
  t_f = micros();                 // t_f will be used to measure the 50us
                                  // latching period in the next call of the
                                  // function.
}
#endif
/**
  * @brief  Constructor of LED_Color class
 **/
void LED_Color_Init(struct LED_Color *self, uint8_t num_pixel, uint8_t bpp, uint8_t *rgb, uint8_t *target) {
  self->NUM_RGB = num_pixel;
  self->NUM_BYTES = num_pixel * bpp;
  self->rgb_arr = rgb;
  self->target_arr = target;

  self->render = _LED_Color_render;
}

/** Begin of:
  * @toc SECTION_LED_COLOR_MODE
 **/
/**
  * @brief  Implementation of void LED_Color_Mode::ledSmoothWrite (static void _LED_Color_Mode_ledSmoothWrite)
 **/
static void _LED_Color_Mode_ledSmoothWrite(struct LED_Color_Mode *self) {
  const struct LED_Color      *l = self->l;
  const uint8_t               NUM_BYTES = self->NUM_BYTES;
  uint8_t * const             rgb_arr = self->rgb_arr;
  uint8_t * const             target_arr = self->target_arr;

  // Obtain equality
  for (uint8_t i = 0; i < NUM_BYTES; i++) {
    if (rgb_arr[i] < target_arr[i]) rgb_arr[i]++;
    else if (rgb_arr[i] > target_arr[i]) rgb_arr[i]--;
  }
  l->render(l);
}

/**
  * @brief  Implementation of void LED_Color_Mode::ledDirectWrite (static void _LED_Color_Mode_ledDirectWrite)
 **/
static void _LED_Color_Mode_ledDirectWrite(struct LED_Color_Mode *self, const uint8_t *ledTarget) {
  const struct LED_Color      *l = self->l;
  const uint8_t               NUM_BYTES = self->NUM_BYTES;

  memcpy(self->rgb_arr, ledTarget, NUM_BYTES);
  memcpy(self->target_arr, ledTarget, NUM_BYTES);
  l->render(l);
}

/**
  * @brief  Implementation of virtual functions LED_Color_Mode::VTable (static void _LED_Color_Mode_F3)
 **/
static inline void _LED_Color_Mode_F3(const void *unsafe_self) {
  struct LED_Color_Mode *self = (struct LED_Color_Mode *)unsafe_self;
  if(!self->VTable.F3) return;
  self->VTable.F3(unsafe_self);
}
static inline void _LED_Color_Mode_F3Var(const void *unsafe_self) {
  struct LED_Color_Mode *self = (struct LED_Color_Mode *)unsafe_self;
  if(!self->VTable.F3Var) return;
  self->VTable.F3Var(unsafe_self);
}
static inline void _LED_Color_Mode_Update(const void *unsafe_self) {
  struct LED_Color_Mode *self = (struct LED_Color_Mode *)unsafe_self;
  // if(!self->VTable.Update) return; Will make sure this never happens. Optimize for loop performance
  self->VTable.Update(unsafe_self);
}
static inline void _LED_Color_Mode_Hello(const void *unsafe_self) {
  struct LED_Color_Mode *self = (struct LED_Color_Mode *)unsafe_self;
  if(!self->VTable.Hello) return;
  self->VTable.Hello(unsafe_self);
}

/**
  * @brief  Constructor of LED_Color_Mode class
 **/
void LED_Color_Mode_Init(struct LED_Color_Mode *self, struct LED_Color *l) {
  self->l = l;

  self->NUM_RGB = l->NUM_RGB;
  self->NUM_BYTES = l->NUM_BYTES;
  self->rgb_arr = l->rgb_arr;
  self->target_arr = l->target_arr;

  self->ledSmoothWrite = _LED_Color_Mode_ledSmoothWrite;
  self->ledDirectWrite = _LED_Color_Mode_ledDirectWrite;

  self->F3 = _LED_Color_Mode_F3;
  self->F3Var = _LED_Color_Mode_F3Var;
  self->Update = _LED_Color_Mode_Update;
  self->Hello = _LED_Color_Mode_Hello;
}


/** Begin of:
  * @toc SECTION_LED_COLOR_STATIC
 **/
/**
  * @brief  Implementation of virtual function LED_Color_Static::Update (static void _LED_Color_Static_Update)
 **/
static void _LED_Color_Static_Update(const void *unsafe_self) {
  struct LED_Color_Static *self = (struct LED_Color_Static *)unsafe_self;

  const uint8_t         position = self->position,
                        NUM_BYTES = self->super.NUM_BYTES;
  uint8_t * const       target_arr = self->super.target_arr;

  // If not single Color
  if(position > LED0_mcOffset) {
    for(uint8_t i = 0; i < NUM_BYTES; i++) target_arr[i] = led_Presets[position - LED0_cOffset][i];
    self->super.ledSmoothWrite(&self->super);
  }
  else{ // Save some RAM
    for(uint8_t offset = 0; offset < NUM_BYTES; offset += 3) {
      target_arr[offset] = led_scPresets[position][0];
      target_arr[offset + 1] = led_scPresets[position][1];
      target_arr[offset + 2] = led_scPresets[position][2];
    }
    self->super.ledSmoothWrite(&self->super);
  }
}

/**
  * @brief  Implementation of virtual function LED_Color_Static::F3 (static void _LED_Color_Static_F3)
 **/
static void _LED_Color_Static_F3(const void *unsafe_self) {
  struct LED_Color_Static *self = (struct LED_Color_Static *)unsafe_self;

  const uint8_t         NUM_RGB = self->super.NUM_RGB;

  self->position++;
  if(self->position == 16) self->position = 0;

  // Dynamic memory saving
  char LED0PMC[NUM_DIGITS_V];
  LED0PMC[0] = 'C';
  LED0PMC[1] = ' ';
  for(uint8_t i = 2; i < NUM_RGB; i++) LED0PMC[i] = LED0PM[self->position][i - 2];

  displayWrite(3, 0x00, 500, LED0PMC);    // Write change message
}

/**
  * @brief  Implementation of virtual function LED_Color_Static::Hello (static void _LED_Color_Static_Hello)
 **/
static inline void _LED_Color_Static_Hello(const void *unsafe_self) {
  char k[NUM_DIGITS_V];
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_COLOR + i);
  displayWrite(3, 0x00, 1000, k);
}

/**
  * @brief  Constructor of LED_Color_Static class
 **/
void LED_Color_Static_Init(struct LED_Color_Static *self, struct LED_Color *l, uint8_t position) {
  LED_Color_Mode_Init(&self->super, l);

  self->position = position;

  struct LED_Color_Mode_VTable _static_vtable = {
    .F3 = _LED_Color_Static_F3,
    .F3Var = NULL,
    .Update = _LED_Color_Static_Update,
    .Hello = _LED_Color_Static_Hello
  };
  self->super.VTable = _static_vtable;
}

/** Begin of:
  * @toc SECTION_LED_COLOR_SPECTRUM
 **/
/**
  * @brief  Implementation of virtual function LED_Color_Spectrum::Update (static void _LED_Color_Spectrum_Update)
 **/
static void _LED_Color_Spectrum_Update(const void *unsafe_self) {
  struct LED_Color_Spectrum *self = (struct LED_Color_Spectrum *)unsafe_self;
  // Dereference const (read only) variables
  const uint8_t         NUM_BYTES = self->super.NUM_BYTES;
  uint8_t * const       target_arr = self->super.target_arr;
  const LED_S_t         saturation = self->saturation;
  const LED_L_t         lightness = self->lightness;

  if(updateIntervalEvent(&self->angleUpdater)) self->angle++;

  uint32_t phase = ledPhase(self->angle, saturation, lightness);

  for(uint8_t offset = 0; offset < NUM_BYTES; offset += 3) {
    target_arr[offset] = (uint8_t)((phase >> 16) & 0xFF);     // G
    target_arr[offset + 1] = (uint8_t)((phase >> 8) & 0xFF);  // R
    target_arr[offset + 2] = (uint8_t)(phase & 0xFF);         // B
  }

  self->super.ledSmoothWrite(&self->super);
}

/**
  * @brief  Implementation of virtual function LED_Color_Spectrum::F3 (static void _LED_Color_Spectrum_F3)
 **/
static void _LED_Color_Spectrum_F3(const void *unsafe_self) {
  struct LED_Color_Spectrum *self = (struct LED_Color_Spectrum *)unsafe_self;

  char k[NUM_DIGITS_V];
  if(self->lightness == LED_L_REG) {
    self->lightness = LED_L_HIGH; // Change to bright
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L0 + i);
    k[0] = 'L';
    displayWrite(3, 0x00, 1000, k);
  }
  else if(self->lightness == LED_L_HIGH) {
    self->lightness = LED_L_LOW; // Change to dim
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L2 + i);
    k[0] = 'L';
    displayWrite(3, 0x00, 1000, k);
  }
  else{
    self->lightness = LED_L_REG; // Back to normal
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L1 + i);
    k[0] = 'L';
    displayWrite(3, 0x00, 1000, k);
  }
}

/**
  * @brief  Implementation of virtual function LED_Color_Spectrum::F3Var (static void _LED_Color_Spectrum_F3Var)
 **/
static void _LED_Color_Spectrum_F3Var(const void *unsafe_self) {
  struct LED_Color_Spectrum *self = (struct LED_Color_Spectrum *)unsafe_self;

  char k[NUM_DIGITS_V];
  if(self->saturation == LED_S_HIGH) {
    self->saturation = LED_S_LOW; // Change to super low sat
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L2 + i);
    k[0] = 'S';
    displayWrite(3, 0x00, 1000, k);
  }
  else if(self->saturation == LED_S_LOW) {
    self->saturation = LED_S_REG; // Change to low sat
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L1 + i);
    k[0] = 'S';
    displayWrite(3, 0x00, 1000, k);
  }
  else{
    self->saturation = LED_S_HIGH; // Change to normal (high) sat
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L0 + i);
    k[0] = 'S';
    displayWrite(3, 0x00, 1000, k);
  }
}

/**
  * @brief  Implementation of virtual function LED_Color_Spectrum::Hello (static void _LED_Color_Spectrum_Hello)
 **/
static inline void _LED_Color_Spectrum_Hello(const void *unsafe_self) {
  char k[NUM_DIGITS_V];
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_FADE + i);
  displayWrite(3, 0x00, 1000, k);
}

/**
  * @brief  Constructor of LED_Color_Spectrum class
 **/
void LED_Color_Spectrum_Init(struct LED_Color_Spectrum *self, struct LED_Color *l) {
  LED_Color_Mode_Init(&self->super, l);

  self->angle = 0;
  self->lightness = LED_L_HIGH;
  self->saturation = LED_S_HIGH;
  self->angleUpdater = newiE(60);

  struct LED_Color_Mode_VTable _spectrum_vtable = {
    .F3 = _LED_Color_Spectrum_F3,
    .F3Var = _LED_Color_Spectrum_F3Var,
    .Update = _LED_Color_Spectrum_Update,
    .Hello = _LED_Color_Spectrum_Hello
  };
  self->super.VTable = _spectrum_vtable;
}

/** Begin of:
  * @toc SECTION_LED_COLOR_CROSSFADE
 **/
/**
  * @brief  Implementation of virtual function LED_Color_Cross::Update (static void _LED_Color_Cross_Update)
 **/
static void _LED_Color_Cross_Update(const void *unsafe_self) {
  struct LED_Color_Cross *self = (struct LED_Color_Cross *)unsafe_self;
  // Dereference const (read only) variables
  const uint8_t         NUM_RGB = self->super.NUM_RGB,
                        delta = self->delta;
  const LED_L_t         lightness = self->lightness;
  uint8_t * const       rgb_arr = self->super.rgb_arr;

  if(updateIntervalEvent(&self->angleUpdater)) self->angle++; // Just let it overflow and begin from 0 :p
  uint8_t offset = 0;

  // Cycle position
  for(uint8_t i = 0; i < NUM_RGB; i++) {
    uint32_t phase = ledPhase(self->angle + (i * delta), 255, lightness);
    rgb_arr[offset] = (uint8_t)((phase >> 16) & 0xFF);     // G
    rgb_arr[offset + 1] = (uint8_t)((phase >> 8) & 0xFF);  // R
    rgb_arr[offset + 2] = (uint8_t)(phase & 0xFF);         // B
    offset += 3;
  }
  self->super.l->render(self->super.l);
}

/**
  * @brief  Implementation of virtual function LED_Color_Cross::F3 (static void _LED_Color_Cross_F3)
 **/
static void _LED_Color_Cross_F3(const void *unsafe_self) {
  struct LED_Color_Cross *self = (struct LED_Color_Cross *)unsafe_self;

  // Higher delta: wider rainbow
  if(self->delta == 42) {
    self->delta = 10;
    displayWrite(3, 0x00, 1000, LED7PM[0]);
  }
  else if(self->delta == 10) {
    self->delta = 21;
    displayWrite(3, 0x00, 1000, LED7PM[1]);
  }
  else if(self->delta == 21) {
    self->delta = 42;
    displayWrite(3, 0x00, 1000, LED7PM[2]);
  }
  else{
    self->delta = 10;
    displayWrite(3, 0x00, 1000, LED7PM[0]);
  }
}

/**
  * @brief  Implementation of virtual function LED_Color_Cross::F3Var (static void _LED_Color_Cross_F3Var)
 **/
static void _LED_Color_Cross_F3Var(const void *unsafe_self) {
  struct LED_Color_Cross *self = (struct LED_Color_Cross *)unsafe_self;

  char k[NUM_DIGITS_V];
  if(self->lightness == LED_L_REG) {
    self->lightness = LED_L_HIGH; // Change to bright
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L0 + i);
    k[0] = 'L';
    displayWrite(3, 0x00, 1000, k);
  }
  else if(self->lightness == LED_L_HIGH) {
    self->lightness = LED_L_LOW; // Change to dim
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L2 + i);
    k[0] = 'L';
    displayWrite(3, 0x00, 1000, k);
  }
  else{
    self->lightness = LED_L_REG; // Back to normal
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L1 + i);
    k[0] = 'L';
    displayWrite(3, 0x00, 1000, k);
  }
}

/**
  * @brief  Implementation of virtual function LED_Color_Cross::Hello (static void _LED_Color_Cross_Hello)
 **/
static inline void _LED_Color_Cross_Hello(const void *unsafe_self) {
  char k[NUM_DIGITS_V];
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_CROSSFADE + i);
  displayWrite(3, 0x00, 1000, k);
}

/**
  * @brief  Constructor of LED_Color_Cross class
 **/
void LED_Color_Cross_Init(struct LED_Color_Cross *self, struct LED_Color *l, uint8_t delta) {
  LED_Color_Mode_Init(&self->super, l);

  self->angle = 0;
  self->lightness = LED_L_HIGH;
  self->delta = delta;
  self->angleUpdater = newiE(60);

  struct LED_Color_Mode_VTable _cross_vtable = {
    .F3 = _LED_Color_Cross_F3,
    .F3Var = _LED_Color_Cross_F3Var,
    .Update = _LED_Color_Cross_Update,
    .Hello = _LED_Color_Cross_Hello
  };
  self->super.VTable = _cross_vtable;
}

/** Begin of:
  * @toc SECTION_LED_COLOR_CHASEFADE
 **/
/**
  * @brief  Implementation of virtual function LED_Color_Chase::Update (static void _LED_Color_Chase_Update)
 **/
static void _LED_Color_Chase_Update(const void *unsafe_self) {
  struct LED_Color_Chase *self = (struct LED_Color_Chase *)unsafe_self;
  // Dereference const (read only) variables
  const uint8_t         NUM_BYTES = self->super.NUM_BYTES,
                        direction = self->direction,
                        MicPin = self->MicPin,
                        s = *(self->s);
  const LED_L_t         lightness = self->lightness;
  uint8_t * const       rgb_arr = self->super.rgb_arr;

  if(direction < 3) {                                         // If reactive to second flip
    if(self->secondFlip != s) {                               // If the second has changed
      // Change chase fade direction
      if(direction == 2) self->directionFlag = !self->directionFlag;
      self->state = 0;                                        // Reset state machine
      self->secondFlip = s;                                   // Overwrite old second with new second
      self->angle += 22;                                      // Let it overflow and get different values.
    }
  }
  else{                                                       // If reactive to sound
    if(getMicData(MicPin) > 196) {                            // If the intensity of the audio samples are higher than 196 - threshold
      if(updateIntervalEvent(&self->delayUpdater)) {          // And some time has elapsed
        self->directionFlag = !self->directionFlag;           // Flip direction
        self->state = 0;                                      // Reset state machine
        self->angle += 29;                                    // And get some different color values!
      }
    }
  }

  if(self->state < 6) {                                       // Only run this code fragment if state is in range (< 6)
    uint32_t phase = ledPhase(self->angle, 255, lightness);   // Get new phase
    uint8_t offset = 0;
    if(!self->directionFlag) offset = self->state * 3;        // Get manipulating position
    else offset = NUM_BYTES - ((self->state * 3) + 3);        // If direction backward, then backward!
    rgb_arr[offset] = (uint8_t)((phase >> 16) & 0xFF);        // Manipulate G
    rgb_arr[offset + 1] = (uint8_t)((phase >> 8) & 0xFF);     // Manipulate R
    rgb_arr[offset + 2] = (uint8_t)(phase & 0xFF);            // Manipulate B
  }
  self->super.l->render(self->super.l);

  if(updateIntervalEvent(&self->stateUpdater)) self->state++;
}

/**
  * @brief  Implementation of virtual function LED_Color_Chase::F3 (static void _LED_Color_Chase_F3)
 **/
static void _LED_Color_Chase_F3(const void *unsafe_self) {
  struct LED_Color_Chase *self = (struct LED_Color_Chase *)unsafe_self;
  const uint8_t         NUM_RGB = self->super.NUM_RGB,
                        flipdir = self->flipdir;

  // Short press results change in direction
  // direction = 0: From right to left (default)
  // direction = 1: From left to right
  // direction = 2: Direction flip
  // direction = 3: Flip on clap or any significant change in microphone input value
  self->direction++;
  if(self->direction == 0) self->directionFlag = FALSE;
  else if(self->direction == 1) self->directionFlag = TRUE;
  else if(self->direction == 2) self->directionFlag = !flipdir;
  else if(self->direction == 4) self->direction = 0;
  char LED8PMC[NUM_DIGITS_V];
  for(uint8_t i = 0; i < 2; i++) LED8PMC[i] = ' ';
  for(uint8_t i = 2; i < NUM_RGB; i++) LED8PMC[i] = LED8PM[self->direction][i - 2];
  displayWrite(3, 0x00, 1000, LED8PMC);
}

/**
  * @brief  Implementation of virtual function LED_Color_Chase::F3Var (static void _LED_Color_Chase_F3Var)
 **/
static void _LED_Color_Chase_F3Var(const void *unsafe_self) {
  struct LED_Color_Chase *self = (struct LED_Color_Chase *)unsafe_self;

  char k[NUM_DIGITS_V];
  if(self->lightness == LED_L_REG) {
    self->lightness = LED_L_HIGH; // Change to bright
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L0 + i);
    k[0] = 'L';
    displayWrite(3, 0x00, 1000, k);
  }
  else if(self->lightness == LED_L_HIGH) {
    self->lightness = LED_L_LOW; // Change to dim
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L2 + i);
    k[0] = 'L';
    displayWrite(3, 0x00, 1000, k);
  }
  else{
    self->lightness = LED_L_REG; // Back to normal
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L1 + i);
    k[0] = 'L';
    displayWrite(3, 0x00, 1000, k);
  }
}

/**
  * @brief  Implementation of virtual function LED_Color_Chase::Hello (static void _LED_Color_Chase_Hello)
 **/
static inline void _LED_Color_Chase_Hello(const void *unsafe_self) {
  char k[NUM_DIGITS_V];
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_CHASEFADE + i);
  displayWrite(3, 0x00, 1000, k);
}

/**
  * @brief  Constructor of LED_Color_Chase class
 **/
void LED_Color_Chase_Init(
  struct LED_Color_Chase *self,
  struct LED_Color *l,
  uint8_t MicPin,               // Pin number of microphone
  uint8_t *Second,              // Reference of a globally updated second variable
  uint8_t FlipSync,             // Flag to sync L-R <-> R-L chase flip with dot flip of display
  uint8_t ChaseMode             // Saved parameter chase mode (L-R/R-L/flip/mic)
) {
  LED_Color_Mode_Init(&self->super, l);

  self->angle = 0;
  self->secondFlip = 0;
  self->directionFlag = 0;
  self->state = 0;
  self->MicPin = MicPin;
  self->flipdir = FlipSync;
  self->s = Second;
  self->lightness = LED_L_HIGH;
  self->direction = ChaseMode;
  self->stateUpdater = newiE(60);
  self->delayUpdater = newiE(250);

  struct LED_Color_Mode_VTable _chase_vtable = {
    .F3 = _LED_Color_Chase_F3,
    .F3Var = _LED_Color_Chase_F3Var,
    .Update = _LED_Color_Chase_Update,
    .Hello = _LED_Color_Chase_Hello
  };
  self->super.VTable = _chase_vtable;
}

/** Begin of:
  * @toc SECTION_LED_COLOR_RESISTOR
 **/
/**
  * @brief  Implementation of virtual function LED_Color_Resistor::Update (static void _LED_Color_Resistor_Update)
 **/
static void _LED_Color_Resistor_Update(const void *unsafe_self) {
  struct LED_Color_Resistor *self = (struct LED_Color_Resistor *)unsafe_self;
  // Dereference const (read only) variables
  const uint8_t         NUM_RGB = self->super.NUM_RGB,
                        h = *(self->h),
                        m = *(self->m),
                        s = *(self->s);
  uint8_t * const       target_arr = self->super.target_arr;

  uint8_t clockData[NUM_RGB];
  clockData[0] = s % 10;
  clockData[1] = s / 10;
  clockData[2] = m % 10;
  clockData[3] = m / 10;
  clockData[4] = h % 10;
  clockData[5] = h / 10;

  uint8_t offset = 0;
  for(uint8_t i = 0; i < NUM_RGB; i++) {
    target_arr[offset] = led_Resistor[clockData[i]][0];        // G
    target_arr[offset + 1] = led_Resistor[clockData[i]][1];    // R
    target_arr[offset + 2] = led_Resistor[clockData[i]][2];    // B
    offset += 3;
  }

  self->super.ledSmoothWrite(&self->super);
}

/**
  * @brief  Implementation of virtual function LED_Color_Resistor::Hello (static void _LED_Color_Resistor_Hello)
 **/
static inline void _LED_Color_Resistor_Hello(const void *unsafe_self) {
  char k[NUM_DIGITS_V];
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_CRCODE + i);
  displayWrite(3, 0x00, 1000, k);
}

/**
  * @brief  Constructor of LED_Color_Resistor class
 **/
void LED_Color_Resistor_Init(
  struct LED_Color_Resistor *self,
  struct LED_Color *l,
  // Reference of globally updated hour/minute/second variable
  uint8_t *h, uint8_t *m, uint8_t *s
) {
  LED_Color_Mode_Init(&self->super, l);

  self->h = h;
  self->m = m;
  self->s = s;

  struct LED_Color_Mode_VTable _resistor_vtable = {
    .F3 = NULL,
    .F3Var = NULL,
    .Update = _LED_Color_Resistor_Update,
    .Hello = _LED_Color_Resistor_Hello
  };
  self->super.VTable = _resistor_vtable;
}

/** Begin of:
  * @toc SECTION_LED_COLOR_COP
 **/
/**
  * @brief  Implementation of virtual function LED_Color_Cop::Update (static void _LED_Color_Cop_Update)
 **/
static void _LED_Color_Cop_Update(const void *unsafe_self) {
  struct LED_Color_Cop *self = (struct LED_Color_Cop *)unsafe_self;
  struct LED_Color_Mode *super = &self->super;
  const uint8_t         pattern = self->pattern,
                        NUM_BYTES = super->NUM_BYTES;
  uint8_t * const       rgb_arr = super->rgb_arr;

  if(updateIntervalEvent(&self->stateUpdater)) {
    if(self->state < 13) self->state++;
    else if(self->state == 13) self->state = 0;
  }

  if(pattern == 0) {
    if(self->state == 0) self->copHalfRender(super, 0, 1);         // b | r fill

    else if(self->state == 5) {
      for(uint8_t i = 0; i < NUM_BYTES; i += 3) for(uint8_t j = 0; j < 3; j++) rgb_arr[i + j] = led_scPresets[0][j];
      super->l->render(super->l);                                  // off fill
    }

    else if(self->state == 6) self->copHalfRender(super, 0, 1);    // b | r fill
    else if(self->state == 7) self->copHalfRender(super, 1, 0);    // r | b fill

    else if(self->state == 12) {
      for(uint8_t i = 0; i < NUM_BYTES; i += 3) for(uint8_t j = 0; j < 3; j++) rgb_arr[i + j] = led_scPresets[1][j];
      super->l->render(super->l);                                  // white fill
    }

    else if(self->state == 13) self->copHalfRender(super, 1, 0);   // r | b fill
  }
}

/**
  * @brief  Implementation of method LED_Color_Cop::copHalfRender (static inline void _LED_Color_Cop_copHalfRender)
 **/
static inline void _LED_Color_Cop_copHalfRender(struct LED_Color_Mode *rself, uint8_t left, uint8_t right) {
  const struct LED_Color      *l = rself->l;
  const uint8_t               NUM_BYTES = rself->NUM_BYTES;
  uint8_t * const             rgb_arr = rself->rgb_arr;

  for(uint8_t i = 0; i < (NUM_BYTES >> 1); i+= 3) for(uint8_t j = 0; j < 3; j++) rgb_arr[i + j] = LED11_colors[right][j];
  for(uint8_t i = (NUM_BYTES >> 1); i < NUM_BYTES; i+= 3) for(uint8_t j = 0; j < 3; j++) rgb_arr[i + j] = LED11_colors[left][j];
  l->render(l);
}

/**
  * @brief  Implementation of virtual function LED_Color_Cop::Hello (static void _LED_Color_Cop_Hello)
 **/
static inline void _LED_Color_Cop_Hello(const void *unsafe_self) {
  char k[NUM_DIGITS_V];
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_CCOP + i);
  displayWrite(3, 0x00, 1000, k);
}

/**
  * @brief  Constructor of LED_Color_Cop class
 **/
void LED_Color_Cop_Init(struct LED_Color_Cop *self, struct LED_Color *l, uint8_t pattern) {
  LED_Color_Mode_Init(&self->super, l);

  self->pattern = pattern;
  self->state = 0;
  self->stateUpdater = newiE(25);

  self->copHalfRender = _LED_Color_Cop_copHalfRender;

  struct LED_Color_Mode_VTable _cop_vtable = {
    .F3 = NULL,
    .F3Var = NULL,
    .Update = _LED_Color_Cop_Update,
    .Hello = _LED_Color_Cop_Hello
  };
  self->super.VTable = _cop_vtable;
}

/** Begin of:
  * @toc SECTION_LED_COLOR_MUSIC
 **/
/**
  * @brief  Implementation of virtual function LED_Color_Music::Update (static void _LED_Color_Music_Update)
 **/
static void _LED_Color_Music_Update(const void *unsafe_self) {
  struct LED_Color_Music *self = (struct LED_Color_Music *)unsafe_self;
  // Dereference const (read only) variables
  const uint8_t         NUM_BYTES = self->super.NUM_BYTES,
                        delta = self->delta,
                        MicPin = self->MicPin;
  const LED_L_t         lightness = self->lightness;
  uint8_t * const       rgb_arr = self->super.rgb_arr;

  // If time interval passed, decrease turned on LEDs by one (regular state update)
  if(updateIntervalEvent(&self->stateUpdater)) if(self->state < 7) self->state++;

  // Cross fade LED color position update
  if(updateIntervalEvent(&self->angleUpdater)) self->angle++; // Just let it overflow and begin from 0 :p

  // Read microphone information, convert value to updateable state
  // Get mic data (log), divide by 36
  uint8_t rLevel = 6 - (uint8_t)(roundf(getMicData(MicPin) / 42.5));
  // Write the less valued LEDs only when the sound is actively changed

  if(self->state >= rLevel) {
    self->state = rLevel;                                        // If the new state is lower than the previous state: Overwrite current state with new rLevel (interrupt state)
    self->delayState = FALSE;                                        // Delay state = 0 (reset)
  }
  else{
    if(updateIntervalEvent(&self->delayUpdater)) self->delayState = TRUE;    // If the time has elapsed, write empty
  }

  if(self->state < 7) {                                           // If new information
    uint8_t offset = 0;

    for(uint8_t i = 0; i < (6 - self->state); i++) {
      uint32_t phase = ledPhase(self->angle + (i * delta), 255, lightness);
      rgb_arr[offset] = (uint8_t)((phase >> 16) & 0xFF);      // G
      rgb_arr[offset + 1] = (uint8_t)((phase >> 8) & 0xFF);   // R
      rgb_arr[offset + 2] = (uint8_t)(phase & 0xFF);          // B
      offset += 3;
    }

    // And set all the others zero
    for(uint8_t lOffset = offset; lOffset < NUM_BYTES; lOffset++) rgb_arr[lOffset] = 0;

    // Black out the inactives
    if(self->delayState) {
      if(self->state < 6) {
        uint8_t tOffset = 0;                                  // Temporary offset variable
        for(uint8_t i = 0; i < (5 - self->state); i++) {
          rgb_arr[tOffset] = 0;
          rgb_arr[tOffset + 1] = 0;
          rgb_arr[tOffset + 2] = 0;
          tOffset += 3;
        }
      }
    }

    self->super.l->render(self->super.l);
  }
}

/**
  * @brief  Implementation of virtual function LED_Color_Music::F3 (static void _LED_Color_Music_F3)
 **/
static void _LED_Color_Music_F3(const void *unsafe_self) {
  struct LED_Color_Music *self = (struct LED_Color_Music *)unsafe_self;

  // Higher delta: wider rainbow
  if(self->delta == 42) {
    self->state = 10;
    displayWrite(3, 0x00, 1000, LED7PM[0]);
  }
  else if(self->state == 10) {
    self->state = 21;
    displayWrite(3, 0x00, 1000, LED7PM[1]);
  }
  else if(self->state == 21) {
    self->state = 42;
    displayWrite(3, 0x00, 1000, LED7PM[2]);
  }
  else{
    self->state = 10;
    displayWrite(3, 0x00, 1000, LED7PM[0]);
  }
}

/**
  * @brief  Implementation of virtual function LED_Color_Music::F3Var (static void _LED_Color_Music_F3Var)
 **/
static void _LED_Color_Music_F3Var(const void *unsafe_self) {
  struct LED_Color_Music *self = (struct LED_Color_Music *)unsafe_self;

  char k[NUM_DIGITS_V];
  if(self->lightness == LED_L_REG) {
    self->lightness = LED_L_HIGH; // Change to bright
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L0 + i);
    k[0] = 'L';
    displayWrite(3, 0x00, 1000, k);
  }
  else if(self->lightness == LED_L_HIGH) {
    self->lightness = LED_L_LOW; // Change to dim
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L2 + i);
    k[0] = 'L';
    displayWrite(3, 0x00, 1000, k);
  }
  else{
    self->lightness = LED_L_REG; // Back to normal
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L1 + i);
    k[0] = 'L';
    displayWrite(3, 0x00, 1000, k);
  }
}

/**
  * @brief  Implementation of virtual function LED_Color_Music::Hello (static void _LED_Color_Music_Hello)
 **/
static inline void _LED_Color_Music_Hello(const void *unsafe_self) {
  char k[NUM_DIGITS_V];
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_CSOUND + i);
  displayWrite(3, 0x00, 1000, k);
}

/**
  * @brief  Constructor of LED_Color_Music class
 **/
void LED_Color_Music_Init(struct LED_Color_Music *self, struct LED_Color *l, uint8_t MicPin) {
  LED_Color_Mode_Init(&self->super, l);

  self->angle = 0;
  self->delta = 21;
  self->state = 0;
  self->delayState = FALSE;
  self->MicPin = MicPin;
  self->lightness = LED_L_HIGH;
  self->angleUpdater = newiE(60);
  self->stateUpdater = newiE(25);
  self->delayUpdater = newiE(250);

  struct LED_Color_Mode_VTable _music_vtable = {
    .F3 = _LED_Color_Music_F3,
    .F3Var = _LED_Color_Music_F3Var,
    .Update = _LED_Color_Music_Update,
    .Hello = _LED_Color_Music_Hello
  };
  self->super.VTable = _music_vtable;
}

/** Begin of:
  * @toc SECTION_LED_COLOR_SERIAL
 **/
/**
  * @brief  Implementation of virtual functions LED_Color_Serial0/1::Update (static void _LED_Color_Serial0/1_Update)
 **/
static void _LED_Color_Serial0_Update(const void *unsafe_self) { return; }
static void _LED_Color_Serial1_Update(const void *unsafe_self) {
  struct LED_Color_Serial1 *self = (struct LED_Color_Serial1 *)unsafe_self;
  self->super.ledSmoothWrite(&self->super);
}

/**
  * @brief  Constructor of LED_Color_Serial0/1 class
 **/
void LED_Color_Serial0_Init(struct LED_Color_Serial0 *self, struct LED_Color *l) {
  LED_Color_Mode_Init(&self->super, l);

  struct LED_Color_Mode_VTable _serial0_vtable = {
    .F3 = NULL,
    .F3Var = NULL,
    .Update = _LED_Color_Serial0_Update,
    .Hello = NULL
  };
  self->super.VTable = _serial0_vtable;
}

void LED_Color_Serial1_Init(struct LED_Color_Serial1 *self, struct LED_Color *l) {
  LED_Color_Mode_Init(&self->super, l);

  struct LED_Color_Mode_VTable _serial1_vtable = {
    .F3 = NULL,
    .F3Var = NULL,
    .Update = _LED_Color_Serial1_Update,
    .Hello = NULL
  };
  self->super.VTable = _serial1_vtable;
}

/** Begin of:
  * @toc SECTION_LED_MODE_MANAGER
 **/
/**
  * @brief  Implementation of method LED_Mode_Manager::LED_Manager_Routine (static void _LED_Mode_Manager_Routine)
 **/
static void _LED_Mode_Manager_Routine(struct LED_Mode_Manager *self) {
  struct LED_Color_Mode *activeInstance = self->LED_Instance[self->LED_Instance_Position];
  activeInstance->Update((void *)activeInstance);

  // F2 was pressed, switch color mode
  if(cF2 == SHORTPRESS) {
    ++self->LED_Instance_Position;
    if(self->LED_Instance_Position >= COLORPOS_SERIAL0) self->LED_Instance_Position = COLORPOS_STATIC;
    activeInstance = self->LED_Instance[self->LED_Instance_Position]; // Re-reference
    activeInstance->Hello((void *)activeInstance);
    clearInterface();
  }

  // F3 was pressed, switch color parameter 1
  if(cF3 == SHORTPRESS) {
    activeInstance->F3((void *)activeInstance);
    clearInterface();
  }

  // F3 was pressed, switch color parameter 2
  if(cF3 == LONGPRESS) {
    activeInstance->F3Var((void *)activeInstance);
    clearInterface();
  }
}

/**
  * @brief  Implementation of method LED_Mode_Manager::EEPGenerate (static struct LED_SavedParam_Serialization _LED_Mode_Manager_EEPGenerate)
 **/
static struct LED_SavedParam_Serialization _LED_Mode_Manager_EEPGenerate(struct LED_Mode_Manager *self) {
  self->s.led = (uint8_t)self->LED;
  struct LED_Color_Static *led_static = (struct LED_Color_Static *)(self->LED_Instance[COLORPOS_STATIC]);
  struct LED_Color_Cross *led_cfade = (struct LED_Color_Cross *)(self->LED_Instance[COLORPOS_CROSSFADE]);
  struct LED_Color_Chase *led_chfd = (struct LED_Color_Chase *)(self->LED_Instance[COLORPOS_CHASEFADE]);
  struct LED_Color_Cop *led_cop = (struct LED_Color_Cop *)(self->LED_Instance[COLORPOS_COP]);

  if(led_static) self->s.LED0P = led_static->position;
  if(led_cfade) self->s.LED7_delta = led_cfade->delta;
  if(led_chfd) self->s.LED8_dp = led_chfd->direction;
  if(led_cop) self->s.LED11_pt = led_cop->pattern;

  return self->s;
}

/**
  * @brief  Implementation of method LED_Mode_Manager::EEPReadIn (static void _LED_Mode_Manager_EEPReadIn)
 **/
static void _LED_Mode_Manager_EEPReadIn(struct LED_Mode_Manager *self, struct LED_SavedParam_Serialization s) {
  self->LED = (LED_MODE_t)s.led;

  struct LED_Color_Static *led_static = (struct LED_Color_Static *)(self->LED_Instance[COLORPOS_STATIC]);
  struct LED_Color_Cross *led_cfade = (struct LED_Color_Cross *)(self->LED_Instance[COLORPOS_CROSSFADE]);
  struct LED_Color_Chase *led_chfd = (struct LED_Color_Chase *)(self->LED_Instance[COLORPOS_CHASEFADE]);
  struct LED_Color_Cop *led_cop = (struct LED_Color_Cop *)(self->LED_Instance[COLORPOS_COP]);

  if(led_static) led_static->position = s.LED0P;
  if(led_cfade) led_cfade->delta = s.LED7_delta;
  if(led_chfd) led_chfd->direction = s.LED8_dp;
  if(led_cop) led_cop->pattern = s.LED11_pt;
}

/**
  * @brief  Constructor of LED_Mode_Manager
 **/
void LED_Mode_Manager_Init(
  struct LED_Mode_Manager *self,
  struct LED_Color *l,                    // Hardware instance
  struct LED_SavedParam_Serialization s   // Saved parameters
) {
  self->LED = (LED_MODE_t)s.led;
  self->LED_Hardware = l;
  self->s = s;

  self->LED_Manager_Routine = _LED_Mode_Manager_Routine;
  self->EEPGenerate         = _LED_Mode_Manager_EEPGenerate;
  self->EEPReadIn           = _LED_Mode_Manager_EEPReadIn;

  LED_Color_Static_Init       (&led_static, l, s.LED0P);
  LED_Color_Serial0_Init      (&led_s0, l);
  LED_Color_Serial1_Init      (&led_s1, l);
  LED_Color_Spectrum_Init     (&led_spectrum, l);
  LED_Color_Cross_Init        (&led_cfade, l, s.LED7_delta);
  LED_Color_Chase_Init        (&led_chfd, l, micPin, &global_s, 0, s.LED8_dp);
  LED_Color_Resistor_Init     (&led_resistor, l, &global_h, &global_m, &global_s);
  LED_Color_Cop_Init          (&led_cop, l, s.LED11_pt);
  LED_Color_Music_Init        (&led_music, l, micPin);

  self->LED_Instance = (struct LED_Color_Mode **)malloc(COLORPOS_MAXCNT * sizeof(struct LED_Color_Mode *));
  self->LED_Instance[COLORPOS_STATIC]     = (struct LED_Color_Mode *) &led_static;
  self->LED_Instance[COLORPOS_SERIAL0]    = (struct LED_Color_Mode *) &led_s0;
  self->LED_Instance[COLORPOS_SERIAL1]    = (struct LED_Color_Mode *) &led_s1;
  self->LED_Instance[COLORPOS_FADE]       = (struct LED_Color_Mode *) &led_spectrum;
  self->LED_Instance[COLORPOS_CROSSFADE]  = (struct LED_Color_Mode *) &led_cfade;
  self->LED_Instance[COLORPOS_CHASEFADE]  = (struct LED_Color_Mode *) &led_chfd;
  self->LED_Instance[COLORPOS_RESISTOR]   = (struct LED_Color_Mode *) &led_resistor;
  self->LED_Instance[COLORPOS_COP]        = (struct LED_Color_Mode *) &led_cop;
  self->LED_Instance[COLORPOS_MUSIC]      = (struct LED_Color_Mode *) &led_music;

  switch (s.led) {
    case LED_STATIC:      self->LED_Instance_Position = COLORPOS_STATIC;      break;
    case LED_FADE:        self->LED_Instance_Position = COLORPOS_FADE;        break;
    case LED_CROSSFADE:   self->LED_Instance_Position = COLORPOS_CROSSFADE;   break;
    case LED_CHASEFADE:   self->LED_Instance_Position = COLORPOS_CHASEFADE;   break;
    case LED_RESISTOR:    self->LED_Instance_Position = COLORPOS_RESISTOR;    break;
    case LED_COP:         self->LED_Instance_Position = COLORPOS_COP;         break;
    case LED_MUSIC:       self->LED_Instance_Position = COLORPOS_MUSIC;       break;

    case LED_SERIAL_0: {
      self->LED_Instance_Position = COLORPOS_SERIAL0;
      for(uint8_t i = 0; i < l->NUM_BYTES; ++i) l->rgb_arr[i] = s.SER0[i];
      l->render(l);
      break;
    }
    case LED_SERIAL_1: {
      self->LED_Instance_Position = COLORPOS_SERIAL1;
      for(uint8_t i = 0; i < l->NUM_BYTES; ++i) l->target_arr[i] = s.SER1[i];
      break;
    }

    default:              self->LED_Instance_Position = COLORPOS_STATIC;      break;
  }
}


/**
  ******************************************************************************
  * @end      END OF FILE LED.c
  ******************************************************************************
 **/

enum {LED21_SILENT, LED21_SLEEP, LED21_SHIFTSLEEP};       // 3 States
uint8_t LED21_st = LED21_SILENT;                          // State: standard silent
LED_MODE_t LED21_shslpst = LED_STATIC;                    // Shift sleep previous state
uint8_t LED21_DF = 1;                                     // Dim factor, 1 = 50% render duty cycle
const uint8_t LED21_DFMAX = 19;                           // Max dim factor
uint8_t LED21_DC = 0;                                     // Dim flag used for switching PWM
const char offs[NUM_DIGITS_V] = {' ', ' ', ' ', ' ', ' ', ' '};      // Digit blanking
uint8_t LED21_hEN = 22;                                   // Begin time of night shift (hour)
uint8_t LED21_mEN = 0;                                    // Begin time of night shift (minute)
uint8_t LED21_hDS = 8;                                    // End time of night shift (hour)
uint8_t LED21_mDS = 0;                                    // End time of night shift (minute)

struct LED_Color LED_Hardware;
struct LED_Mode_Manager m;
uint8_t ra[NUM_BYTES_V] = {0};
uint8_t ta[NUM_BYTES_V] = {0};

// ------------------------------------------------------------------------------------------



void setup() {
  Serial.begin(115200);

  // Output Pin Initializer
  pinMode(LED_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);

  // Bluetooth Configuration
  pinMode(B_GROUND, OUTPUT);                    // Data Ground Pin (yup that's clumsy)
  digitalWrite(B_GROUND, LOW);                  // Permanent Low
  BTSerial.begin(4800);

  analogReference(DEFAULT);

  // Input Pin Initializer
  #ifdef HW_VERSION_22
    pinMode(B_F1_PIN, INPUT_PULLUP);
    pinMode(B_F2_PIN, INPUT_PULLUP);
    pinMode(B_F3_PIN, INPUT_PULLUP);
    pinMode(B_F4_PIN, INPUT_PULLUP);
  #else
    pinMode(B_F1_PIN, INPUT);
    pinMode(B_F2_PIN, INPUT);
    pinMode(B_F3_PIN, INPUT);
    pinMode(B_F4_PIN, INPUT);
  #endif
  pinMode(MIC_PIN, INPUT);
  #ifdef TEMPERATURE_ENABLE
    pinMode(STEM_PIN, INPUT);
  #endif

  // LED initializer
  digitalWriteFast(LED_PIN, LOW);

  // Wire, RTC Initializer
  wrInit();

  #ifdef FIRSTCONFIG  // If first time configuring, load default and save all settings
    firstConfig();
    saveConfig(1);
  #endif
  // Initialize global saved values
  struct LED_SavedParam_Serialization k = loadConfig();
  LED_Color_Init(&LED_Hardware, NUM_RGB_V, 3, ra, ta);
  LED_Mode_Manager_Init(&m, &LED_Hardware, k);

  // Welcome message, read from EEPROM
  welcome(welcomeText);

  #ifdef TEMPERATURE_ENABLE
    tsUpdater = newiE(7500);
  #endif
  dotUpdater = newiE(800);
  jdotUpdater = newiE(500);
  sdotUpdater = newiE(80);
  cfUpdater = newiE(25);
  chUpdater = newiE(60);
  vuUpdater = newiE(90);
  vu2Updater = newiE(250);
  nShiftUpdater = newiE(1000);
}

void loop() {
  // Button check routine
  cButtonRoutine();

  // Interface render routine
  interfaceRoutine();

  // LED render routine
  ledRoutine();

  // Serial routine
  serialRoutine();
}

// This is the main VFD Display interface loop routine
void interfaceRoutine() {
  // This is the launch interface with standard clock ticking
  if(interface == INTERFACE_TIME) {
    // If intervall length exceeded, update dot position

    // BEGIN OF DOT MODE HANDLER
    if(INTF0_DM == 0) {
      if(updateIntervalEvent(&dotUpdater)) INTF0_DP++;

      if(INTF0_DP == 0) displayWrite(0, 0b00010100, 0, 0);
      else if(INTF0_DP == 1) displayWrite(0, 0, 0, 0);
      else INTF0_DP = 0;
    }

    else if(INTF0_DM == 1) {
      if(updateIntervalEvent(&jdotUpdater)) INTF0_DP++;

      if(INTF0_DP == 0) displayWrite(0, 0b00100001, 0, 0);
      else if(INTF0_DP == 1) displayWrite(0, 0b00010010, 0, 0);
      else if(INTF0_DP == 2) displayWrite(0, 0b00001100, 0, 0);
      else if(INTF0_DP == 3) displayWrite(0, 0b00010010, 0, 0);
      else INTF0_DP = 0;
    }

    else if(INTF0_DM == 2) {
      // This function is damn lit. Once it detects a change in second,
      // the decimal dot will slide over the displays.
      // Get the current time and compare it with the previous timestamp
      // DateTime now = rtc.now();
      if(INTF0_ds != global_s) {
        // Time has changed -> Reset dot position, remember timestamp, change direction
        INTF0_DP = 0;
        INTF0_ds = global_s;
        INTF0_dr = !INTF0_dr;
      }

      // Next position
      if(updateIntervalEvent(&sdotUpdater)) INTF0_DP++;

      // From right to left
      if(INTF0_dr) {
        if(INTF0_DP < 5) displayWrite(0, (1 << INTF0_DP), 0, 0);
        else displayWrite(0, 0b00100000, 0, 0);
      }

      // From left to right
      else{
        if(INTF0_DP < 5) displayWrite(0, (0b00100000 >> INTF0_DP), 0, 0);
        else displayWrite(0, 0b00000001, 0, 0);
      }
    }

    else if(INTF0_DM == 3) displayWrite(0, 0, 0, 0);
    else INTF0_DM = 0;

    // BEGIN OF BUTTON HANDLER
    // Short press on F1 will change interface to date display
    if(cF1 == SHORTPRESS) switchInterface(INTERFACE_DATE);  // Enter date interface
    if(cF1 == LONGPRESS) {
      // Enter time set interface
      char message[NUM_DIGITS_V] = {'T', ' ', 'S', 'E', 'T', ' '};
      displayWrite(3, 0x00, 1000, message);
      dateSet = FALSE;
      switchInterface(INTERFACE_TIMEDATE_SET);
    }

    if(cF4 == SHORTPRESS) {
      clearInterface();
      INTF0_DM++;
      // if((INTF0_DM == 2) && (led == LED_SILENT)) INTF0_DM = 3;
    }
    // Long press will save all settings.
    if(cF4 == LONGPRESS) {
      clearInterface();
      saveConfig(0, m.EEPGenerate(&m));
    }
  }

  // This is the date display
  else if(interface == INTERFACE_DATE) {
    displayWrite(1, 0b00010100, 0, 0);

    // Short press on F1 will change interface to temperature display (TEMPERATURE_ENABLE) or clock display (!TEMPERATURE_ENABLE)
    if(cF1 == SHORTPRESS) {
      #ifdef TEMPERATURE_ENABLE
        switchInterface(INTERFACE_TEMPERATURE);
      #else
        switchInterface(INTERFACE_STOPWATCH);
      #endif
    }
    if(cF1 == LONGPRESS) {
      // Enter date set interface
      char message[NUM_DIGITS_V] = {'D', ' ', 'S', 'E', 'T', ' '};
      displayWrite(3, 0x00, 1000, message);
      dateSet = TRUE;
      switchInterface(INTERFACE_TIMEDATE_SET);
    }
  }

  #ifdef TEMPERATURE_ENABLE
    // This is the temperature sensor interface
    else if(interface == INTERFACE_TEMPERATURE) {
      // Create temperature reading collector
      // Check for value update
      if(updateIntervalEvent(&tsUpdater)) {
        // Call temperature update routine to update DS18B20 reading
        updateTemperature();
      }
      if(ts != 0) displayWrite(2 + (isFahrenheit << 1), 0b00010000, 0, 0);

      // Short press on TEN will change interface to standard clock display
      if(cF1 == SHORTPRESS) switchInterface(INTERFACE_STOPWATCH);

      if(cF4 == SHORTPRESS) {
        clearInterface();
        if(isFahrenheit) isFahrenheit = 0;
      }
    }
  #endif

  // 3: Stopwatch
  else if(interface == INTERFACE_STOPWATCH) {
    if(cF1 == SHORTPRESS) switchInterface(INTERFACE_TIME);

    if(INTF3_st == INTF3_INITIAL) {
      // Initial State
      if(cF2 == SHORTPRESS) {
        // Start stopwatch
        if(!INTF3_TB) INTF3_TB = new DateTime(rtc.now()); // Create new backup time
        if(!INTF3_TS) INTF3_TS = new TimeSpan(0);         // No difference
        INTF3_MILLIS = millis();                          // Start milliseconds
        INTF3_MILLIE = 0;
        INTF3_st = INTF3_RUNNING;                         // Set state to 1 (running)
        clearInterface();
      }

      char k[NUM_DIGITS_V] = {0};
      displayWrite(3, 0b000010100, 0, k);
    }

    else if(INTF3_st == INTF3_RUNNING) {
      // Stopwatch running, get elapsed time
      TimeSpan ts = *INTF3_TS + (rtc.now() - *INTF3_TB);
      int8_t    _h = ts.hours(),
                _m = ts.minutes(),
                _s = ts.seconds(),
                _u = ((INTF3_MILLIE + (millis() - INTF3_MILLIS)) % 1000) / 10;
      char k[NUM_DIGITS_V] = {0};
      if(_h > 0) {
        k[0] = _h / 10;
        k[1] = _h % 10;
        k[2] = _m / 10;
        k[3] = _m % 10;
        k[4] = _s / 10;
        k[5] = _s % 10;
      } else {
        k[0] = _m / 10;
        k[1] = _m % 10;
        k[2] = _s / 10;
        k[3] = _s % 10;
        k[4] = _u / 10;
        k[5] = _u % 10;
      }
      displayWrite(3, 0x00, 0, k);

      if(cF3 == SHORTPRESS) {
        // Pause stopwatch
        *INTF3_TS = *INTF3_TS + (rtc.now() - *INTF3_TB);         // Current elapsed time
        INTF3_MILLIE = INTF3_MILLIE + (millis() - INTF3_MILLIS); // Current millis
        INTF3_st = INTF3_PAUSED;
        clearInterface();
      }
    }

    else if(INTF3_st == INTF3_PAUSED) {
      // Stopwatch paused, constant display of current time
      int8_t    _h = INTF3_TS->hours(),
                _m = INTF3_TS->minutes(),
                _s = INTF3_TS->seconds(),
                _u = (INTF3_MILLIE % 1000) / 10;
      char k[NUM_DIGITS_V] = {0};
      if(_h > 0) {
        k[0] = _h / 10;
        k[1] = _h % 10;
        k[2] = _m / 10;
        k[3] = _m % 10;
        k[4] = _s / 10;
        k[5] = _s % 10;
      } else {
        k[0] = _m / 10;
        k[1] = _m % 10;
        k[2] = _s / 10;
        k[3] = _s % 10;
        k[4] = _u / 10;
        k[5] = _u % 10;
      }
      displayWrite(3, 0x00, 0, k);

      if(cF2 == SHORTPRESS) {
        // Reset stopwatch by clearing all time variables
        delete INTF3_TS;
        delete INTF3_TB;
        INTF3_TS = nullptr;
        INTF3_TB = nullptr;
        INTF3_MILLIS = 0;
        INTF3_MILLIE = 0;

        INTF3_st = INTF3_INITIAL;
        clearInterface();
      }

      if(cF3 == SHORTPRESS) {
        // Resume stopwatch
        INTF3_st = INTF3_RUNNING;
        *INTF3_TB = rtc.now();
        INTF3_MILLIS = millis();
        clearInterface();
      }
    }
  }

  // 128: Time/Date set menu!
  else if(interface == INTERFACE_TIMEDATE_SET) {
    // Blink active set in time interval of 800 ms
    // Switch between displayWrite(0) and displayWrite(3) for individual inactive segments
    // Use intervalEvent jdotUpdater which has the same attributes
    static uint8_t offActive = FALSE;
    static uint8_t blinkDisplay = 0;
    static uint8_t tmpHour, tmpMinute, tmpSecond, tmpDay, tmpMonth, tmpYear = 0;

    DateTime now = rtc.now();
    if(setOnceFlag == FALSE) {
        tmpHour = now.hour();
        tmpMinute = now.minute();
        tmpSecond = now.second();
        tmpDay = now.day();
        tmpMonth = now.month();
        tmpYear = now.year() % 100;
        setOnceFlag = TRUE;
    }

    if(updateIntervalEvent(&jdotUpdater)) offActive = !offActive; // Flip uint8_t

    char tRenderArray[NUM_DIGITS_V] = {0, 0, 0, 0, 0, 0};
    if(!dateSet) { // If time set
      tRenderArray[5] = tmpSecond % 10;
      tRenderArray[4] = tmpSecond / 10;
      tRenderArray[3] = tmpMinute % 10;
      tRenderArray[2] = tmpMinute / 10;
      tRenderArray[1] = tmpHour % 10;
      tRenderArray[0] = tmpHour / 10;
    }
    else{ // If date set
      tRenderArray[5] = tmpYear % 10;
      tRenderArray[4] = (tmpYear % 100) / 10;
      if(rBit(clockFlags, B_INTD)) {
        tRenderArray[1] = tmpMonth % 10;
        tRenderArray[0] = tmpMonth / 10;
        tRenderArray[3] = tmpDay % 10;
        tRenderArray[2] = tmpDay / 10;
      }else{
        tRenderArray[3] = tmpMonth % 10;
        tRenderArray[2] = tmpMonth / 10;
        tRenderArray[1] = tmpDay % 10;
        tRenderArray[0] = tmpDay / 10;
      }
    }

    if(offActive) {
      // Blink corresponding display parameter
      if(blinkDisplay == 0) {
        tRenderArray[0] = ' ';
        tRenderArray[1] = ' ';
      }
      else if(blinkDisplay == 1) {
        tRenderArray[2] = ' ';
        tRenderArray[3] = ' ';
      }
      else if(blinkDisplay == 2) {
        tRenderArray[4] = ' ';
        tRenderArray[5] = ' ';
      }
    }

    displayWrite(3, 0x00, 0, tRenderArray);

    // Short press on F1 will leave time set mode and enter time interface again
    if(cF1 == SHORTPRESS) {
      if(!dateSet) switchInterface(INTERFACE_TIME);
      else switchInterface(INTERFACE_DATE);
    }

    // Long press on F1 will change between 12h and 24h display
    if(cF1 == LONGPRESS) {
      if(!dateSet) {
        tBit(&clockFlags, B_12H);  // Flip clockFlags 12h bit
        if(rBit(clockFlags, B_12H)) {
          char k[NUM_DIGITS_V] = {1, 2, 'H'};
          for(uint8_t i = 3; i < NUM_DIGITS_V; i++) k[i] = ' ';
          displayWrite(3, 0x00, 1000, k);
        }else{
          char k[NUM_DIGITS_V] = {2, 4, 'H'};
          for(uint8_t i = 3; i < NUM_DIGITS_V; i++) k[i] = ' ';
          displayWrite(3, 0x00, 1000, k);
        }
      }else{
        tBit(&clockFlags, B_INTD); // Flip clockFlags international date bit
        if(rBit(clockFlags, B_INTD)) {
          char k[NUM_DIGITS_V];
          for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_INTLDATESET + i);
          displayWrite(3, 0x00, 1000, k);
        }else{
          char k[NUM_DIGITS_V];
          for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_INTLDATERESET + i);
          displayWrite(3, 0x00, 1000, k);
        }
      }
    }

    // Short press on F2 changes the active parameter (h/m/s)
    if(cF2 == SHORTPRESS) {
      clearInterface();
      blinkDisplay++;
      if(blinkDisplay == 3) blinkDisplay = 0;
    }

    if(cF3 == SHORTPRESS) {
      clearInterface();
      // parameter--
      if(blinkDisplay == 0) { // Set hour or day
        if(!dateSet) { // Set hour
          if (tmpHour > 0) tmpHour--;
          else if (tmpHour == 0) tmpHour = 23;
        }
        else{ // Set day or month (B_INTD)
          if(rBit(clockFlags, B_INTD)) goto monthsetMinus; // Please don't pick on goto :p
          daysetMinus:
          uint8_t dMax = 31;
          // Leap year detection
          if(tmpMonth == 2) dMax = ((tmpYear % 400 == 0) || ((tmpYear % 4 == 0) && (tmpYear % 100 != 0))) ? 29 : 28;
          // Short month detection
          else if((tmpMonth == 4) || (tmpMonth == 6) || (tmpMonth == 9) || (tmpMonth == 11)) dMax = 30;
          if (tmpDay > 1) tmpDay--;
          else if (tmpDay == 1) tmpDay = dMax;
        }
      }
      else if(blinkDisplay == 1) {
        if(!dateSet) {
          if (tmpMinute > 0) tmpMinute--;
          else if (tmpMinute == 0) tmpMinute = 59;
        }
        else{
          if(rBit(clockFlags, B_INTD)) goto daysetMinus;
          monthsetMinus:
          if (tmpMonth > 1) tmpMonth--;
          else if (tmpMonth == 1) tmpMonth = 12;
        }
      }
      else if(blinkDisplay == 2) {
        if(!dateSet) {
          if (tmpSecond > 0) tmpSecond--;
          else if (tmpSecond == 0) tmpSecond = 59;
        }
        else{
          if (tmpYear > 0) tmpYear--;
          else if (tmpYear == 0) tmpYear = 30;
        }
      }
    }

    if(cF4 == SHORTPRESS) {
      clearInterface();
      // parameter++
      if(blinkDisplay == 0) { // Set hour or day
        if(!dateSet) { // Set hour
          if (tmpHour < 23) tmpHour++;
          else if (tmpHour == 23) tmpHour = 0;
        }
        else{ // Set day or month (B_INTD)
          if(rBit(clockFlags, B_INTD)) goto monthsetPlus;
          daysetPlus:
          uint8_t dMax = 31;
          // Leap year detection
          if(tmpMonth == 2) dMax = ((tmpYear % 400 == 0) || ((tmpYear % 4 == 0) && (tmpYear % 100 != 0))) ? 29 : 28;
          // Short month detection
          else if((tmpMonth == 4) || (tmpMonth == 6) || (tmpMonth == 9) || (tmpMonth == 11)) dMax = 30;
          if (tmpDay < dMax) tmpDay++;
          else if (tmpDay == dMax) tmpDay = 1;
        }
      }
      else if(blinkDisplay == 1) {
        if(!dateSet) {
          if (tmpMinute < 59) tmpMinute++;
          else if (tmpMinute == 59) tmpMinute = 0;
        }
        else{
          if(rBit(clockFlags, B_INTD)) goto daysetPlus;
          monthsetPlus:
          if (tmpMonth < 12) tmpMonth++;
          else if (tmpMonth == 12) tmpMonth = 1;
        }
      }
      else if(blinkDisplay == 2) {
        if(!dateSet) {
          if (tmpSecond < 59) tmpSecond++;
          else if (tmpSecond == 59) tmpSecond = 0;
        }
        else{
          if (tmpYear < 30) tmpYear++;
          else if (tmpYear == 30) tmpYear = 0;
        }
      }
    }

    // Transfer to RTC
    Wire.beginTransmission(0x68);
    Wire.write(byte(0));
    Wire.write(decToBcd(tmpSecond));
    Wire.write(decToBcd(tmpMinute));
    Wire.write(decToBcd(tmpHour));
    Wire.write(0x06);
    Wire.write(decToBcd(tmpDay));
    Wire.write(decToBcd(tmpMonth));
    Wire.write(decToBcd(tmpYear));
    Wire.write(byte(0));
    Wire.endTransmission();

    setOnceFlag = FALSE;  // Reset static flag
  }
}

// Button check routine
void cButtonRoutine() {
  cF1 = checkOption(B_F1_PIN);  // Short press: main interface switch
  cF2 = checkOption(B_F2_PIN);  // Short press: color switch
  cF3 = checkOption(B_F3_PIN);
  cF4 = checkOption(B_F4_PIN);  // Short press: display mode switch
}

// This is the LED loop routine
void ledRoutine() {
  m.LED_Manager_Routine(&m);

  // Check if night shift time set is enabled. If yes, check for trigger signal
  // if(rBit(clockFlags, B_NSHIFT)) if(updateIntervalEvent(&nShiftUpdater)) nightShiftRoutine(global_h, global_m, global_s);
}

// This routine is to check for incoming serial data
void serialRoutine() {/*
  // This condition makes the beginning of everything serial.
  int sRead = Serial.available();

  if(sRead > 0) {
    // If the communication pattern of 16 bytes is detected, write a message
    // Serial.print(sRead);

    uint8_t inputBuffer[24];
    Serial.readBytes(inputBuffer, 24);

    serialCommandDecode(inputBuffer, serialCommandEncodeUSB);

    // Discard the rest
    uint8_t flushBuffer[Serial.available()];
    Serial.readBytes(flushBuffer, Serial.available());
    Serial.flush();
  }

  int bRead = BTSerial.available();

  if(bRead > 0) {
    uint8_t inputBuffer[24];
    BTSerial.readBytes(inputBuffer, 24);

    serialCommandDecode(inputBuffer, serialCommandEncodeBluetooth);

    // Discard buffer
    uint8_t flushBuffer[BTSerial.available()];
    BTSerial.readBytes(flushBuffer, BTSerial.available());
    BTSerial.flush();
  }*/
}

// This routine is to check whether night shift should be turned on/off or not
inline void nightShiftRoutine(uint8_t h, uint8_t m, uint8_t s) {
  /*// If night shift is on, look for disable trigger, otherwise look for enable trigger

  if(LED21_st == LED21_SHIFTSLEEP) {
    // Night shift is on
    // Look for disable trigger
    if(h == LED21_hDS) { // if statement reduction
      if(m == LED21_mDS) {
        if(s < 3) {                               // Safe switching time span. Bug (which is ok): Waking when s < 3!
          led = LED21_shslpst;                    // Restore previous LED mode, 'Implicit wake'
          LED21_st = LED21_SILENT;
          displayWrite = &displayWrite_REG;       // And turn on normal mode silently
          clearInterface();
        }
      }
    }
  }

  else {
    // Night shift is off
    // Look for enable trigger
    if(h == LED21_hEN) { // if statement reduction
      if(m == LED21_mEN) {
        uint8_t silent_target[NUM_BYTES] = {0};
        delay(50);
        ledDirectWrite(silent_target);

        char k[NUM_DIGITS_V];
        for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_CSILENT + i);
        displayWrite(3, 0x00, 1000, k);

        LED21_shslpst = led;                    // Backup previous LED mode
        if(LED21_shslpst == LED_SERIAL_0 || LED21_shslpst == LED_SERIAL_1) LED21_shslpst = LED_STATIC; // won't happen that often #dontcare
        led = LED_SILENT;                       // Go into silent mode
        LED21_st = LED21_SHIFTSLEEP;            // 'Implicit Sleep'
        LED21_DF = LED21_DFMAX;                 // Highest dim factor
        clearInterface();
      }
    }
  }*/
}

// Serial command decode function. Takes input buffer from USB Serial or Software Serial
// Thus, if response is required, the 2nd argument calls the correct response function
void serialCommandDecode(uint8_t *inputBuffer, void (*encoderInstance)(uint8_t *)) {
  /*// If aligned protocol is detected
  if((inputBuffer[0] == 0x23) && (inputBuffer[23] == 0x24)) {
    uint8_t cmdByte = inputBuffer[1];

    // If LED set is detected
    if(cmdByte == 0x01) {
      displayWrite = &displayWrite_REG; // Reset display function
      // Set LED mode to 2 (Serial custom mode)
      led = LED_SERIAL_0;
      // And write LED information to target
      for(uint8_t i = 2; i < 20; i++) rgb_arr[i - 2] = inputBuffer[i];
      render();
    }

    // If LED smooth set is detected
    else if(cmdByte == 0x02) {
      displayWrite = &displayWrite_REG; // Reset display function
      // Set LED mode to 3 (Serial smooth write)
      led = LED_SERIAL_1;
      // And write LED information to target
      for(uint8_t i = 2; i < 20; i++) target_arr[i - 2] = inputBuffer[i];
    }

    // If time set command is detected
    else if(cmdByte == 0x10) {
      // Transfer to RTC
      Wire.beginTransmission(0x68);
      Wire.write(byte(0));
      Wire.write(decToBcd(inputBuffer[2]));
      Wire.write(decToBcd(inputBuffer[3]));
      Wire.write(decToBcd(inputBuffer[4]));
      Wire.write(0x06);
      Wire.write(decToBcd(inputBuffer[5]));
      Wire.write(decToBcd(inputBuffer[6]));
      Wire.write(decToBcd(inputBuffer[7]));
      Wire.write(byte(0));
      Wire.endTransmission();

      // A make sure flag
      if(inputBuffer[8] == 0x23) {
        // Say that time and date is synced now.
        char k[NUM_DIGITS_V];
        for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_TIMESYNC1 + i);
        displayWrite(3, 0x00, 750, k);
        for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_TIMESYNC2 + i);
        displayWrite(3, 0x00, 750, k);
      }

      // Answer with a beginning of a message. If it's all good, the PC controller will complete the message :p
      uint8_t transferBuffer[10] = {0x23, 0x10, 'T', 'i', 'm', 'e', ' ', 'S', 'y', 0x24};
      encoderInstance(transferBuffer);
    }

    // If night shift is set (silent mode)
    else if(cmdByte == 0x11) {
      if((inputBuffer[2] == 0x23) && (inputBuffer[9] == 0x23)) { // Further format verify
        // If time set night shift is enabled
        char k[NUM_DIGITS_V];

        if(inputBuffer[7] == 1) { // Set enable minute, hour; disable minute, hour
          LED21_mEN = inputBuffer[3];
          LED21_hEN = inputBuffer[4];
          LED21_mDS = inputBuffer[5];
          LED21_hDS = inputBuffer[6];
          sBit(&clockFlags, B_NSHIFT);       // Set night shift time enable bit
          saveConfig(1);                     // Silently save settings

          for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_SILENT2 + i);
          displayWrite(3, 0x00, 750, k);
          for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_SILENT3 + i);
          displayWrite(3, 0x00, 750, k);
          for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_ON + i);
          displayWrite(3, 0x00, 750, k);
        }
        else if(inputBuffer[7] == 0) { // Reset enable minute, hour; disable minute, hour
          LED21_mEN = 0;
          LED21_hEN = 22;
          LED21_mDS = 0;
          LED21_hDS = 8;
          cBit(&clockFlags, B_NSHIFT);       // Clear night shift time enable bit
          saveConfig(1);                     // Silently save settings

          for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_SILENT2 + i);
          displayWrite(3, 0x00, 750, k);
          for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_SILENT3 + i);
          displayWrite(3, 0x00, 750, k);
          for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_OFF + i);
          displayWrite(3, 0x00, 750, k);
        }
        // else do nothing

        if(inputBuffer[8] < 2) {
          // Simple night shift is enabled
          led = LED_SILENT;                                           // To silent mode
          LED21_DF = (inputBuffer[8]) ? LED21_DFMAX : 1;              // And dim down if requested
          LED21_st = (inputBuffer[8]) ? LED21_SLEEP : LED21_SILENT;   // Set FSM to silent/sleep
          for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_CSILENT + i);
          displayWrite(3, 0x00, 1000, k);

          uint8_t silent_target[NUM_BYTES];
          for(uint8_t offset = 0; offset < NUM_BYTES; offset += 3) {
            silent_target[offset] = led_scPresets[0][0];
            silent_target[offset + 1] = led_scPresets[0][1];
            silent_target[offset + 2] = led_scPresets[0][2];
          }
          ledDirectWrite(silent_target);
          clearInterface();
        }
      }
    }

    // If clock configuration set is detected
    else if(cmdByte == 0x12) {
      // If 12h/24h set
      if(inputBuffer[2] == B_12H) {
        if(inputBuffer[3]) sBit(&clockFlags, B_12H);     // Set 12h mode
        else cBit(&clockFlags, B_12H);                   // clear 12h mode

        // And output
        if(rBit(clockFlags, B_12H)) {
          char k[NUM_DIGITS_V] = {1, 2, 'H'};
          for(uint8_t i = 3; i < NUM_DIGITS_V; i++) k[i] = ' ';
          displayWrite(3, 0x00, 1000, k);
        }else{
          char k[NUM_DIGITS_V] = {2, 4, 'H'};
          for(uint8_t i = 3; i < NUM_DIGITS_V; i++) k[i] = ' ';
          displayWrite(3, 0x00, 1000, k);
        }
      }
      else if(inputBuffer[2] == B_INTD) {

        if(inputBuffer[3]) sBit(&clockFlags, B_INTD);    // Set intl' date
        else cBit(&clockFlags, B_INTD);                  // clear intl' date

        // And output
        if(rBit(clockFlags, B_INTD)) {
          char k[NUM_DIGITS_V];
          for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_INTLDATESET + i);
          displayWrite(3, 0x00, 1000, k);
        }else{
          char k[NUM_DIGITS_V];
          for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_INTLDATERESET + i);
          displayWrite(3, 0x00, 1000, k);
        }
      }
      else if(inputBuffer[2] == B_LZERO) {
        if(inputBuffer[3]) sBit(&clockFlags, B_LZERO);    // enable leading zero
        else cBit(&clockFlags, B_LZERO);                  // disable leading zero

        // And output
        if(rBit(clockFlags, B_LZERO)) {
          char k[NUM_DIGITS_V];
          for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LEADINGZERO_ON + i);
          displayWrite(3, 0x00, 1000, k);
        }else{
          char k[NUM_DIGITS_V];
          for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LEADINGZERO_OFF + i);
          displayWrite(3, 0x00, 1000, k);
        }
      }
    }

    // If message display is detected
    else if(cmdByte == 0x1F) {
      // Get message delay time. It's the incoming value in seconds
      uint16_t msgDelay = 1000;
      if(inputBuffer[20] < 10) msgDelay *= (uint16_t)inputBuffer[20];

      char msg[NUM_DIGITS_V] = {' ', ' ', ' ', ' ', ' ', ' '};

      // Write first message
      uint8_t offset = 2;
      for(uint8_t i = offset; i < (offset + 6); i++) msg[i - offset] = (char)inputBuffer[i];
      displayWrite(3, 0x00, msgDelay, msg);

      // If more is available, do more!
      // Input buffer idx 21 is the long flag 0 (12 characters), idx 22 is the long flag 1 (18 characters)
      if(inputBuffer[21] == 1) {
        offset += 6;
        for(uint8_t i = offset; i < (offset + 6); i++) msg[i - offset] = (char)inputBuffer[i];
        displayWrite(3, 0x00, msgDelay, msg);

        // If even more is available, do more!
        if(inputBuffer[22] == 1) {
          offset += 6;
          for(uint8_t i = offset; i < (offset + 6); i++) msg[i - offset] = (char)inputBuffer[i];
          displayWrite(3, 0x00, msgDelay, msg);
        }
      }
    }

    // If LED preset mode is detected
    else if(cmdByte == 0x20) {
      displayWrite = &displayWrite_REG; // Reset display function
      // Get input LED mode
      uint8_t cmdMode = inputBuffer[2];

      // If static color preset change is detected
      if(cmdMode == 0x01) {
        led = LED_STATIC;             // Set LED mode to 0
        LED0P = inputBuffer[3];       // Set color to param 0 (inputBuffer[3])

        // Communicate
        char LED0PMC[NUM_DIGITS_V];
        LED0PMC[0] = 'C';
        LED0PMC[1] = ' ';
        for(uint8_t i = 2; i < NUM_RGB; i++) LED0PMC[i] = LED0PM[inputBuffer[3]][i - 2];
        displayWrite(3, 0x00, 500, LED0PMC);
      }

      // If regular fade preset is detected
      else if(cmdMode == 0x02) {
        led = LED_FADE;
        char k[NUM_DIGITS_V];
        for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_FADE + i);
        displayWrite(3, 0x00, 1000, k);
      }

      // If cross fade is detected
      else if(cmdMode == 0x03) {
        LED7_delta = inputBuffer[3];  // Apply param 0 (inputBuffer[3]) to LED 7 delta value

        if(led == LED_CROSSFADE) {     // If it is already in CF mode, just display the message
          // Higher delta: wider rainbow
          if(LED7_delta == 42) displayWrite(3, 0x00, 1000, LED7PM[2]);
          else if(LED7_delta == 10) displayWrite(3, 0x00, 1000, LED7PM[0]);
          else if(LED7_delta == 21) displayWrite(3, 0x00, 1000, LED7PM[1]);
        }
        else{                         // Otherwise
          led = LED_CROSSFADE;        // Switch to cross fade and message
          char k[NUM_DIGITS_V];
          for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_CROSSFADE + i);
          displayWrite(3, 0x00, 1000, k);
        }
      }

      // If chase fade is detected
      else if(cmdMode == 0x04) {
        LED8_dp = inputBuffer[3];     // Apply param 0 (inputBuffer[3]) to LED 8 direction state
        // And do all the stuff as if it is a regular button triggered change
        if(inputBuffer[3] == 0) LED8_dr = FALSE;
        else if(inputBuffer[3] == 1) LED8_dr = TRUE;
        else if(inputBuffer[3] == 2) LED8_dr = !INTF0_dr;

        if(led == LED_CHASEFADE) {     // If it is already in CH mode, just display message
          char LED8PMC[NUM_DIGITS_V];
          for(uint8_t i = 0; i < 2; i++) LED8PMC[i] = ' ';
          for(uint8_t i = 2; i < NUM_RGB; i++) LED8PMC[i] = LED8PM[inputBuffer[3]][i - 2];
          displayWrite(3, 0x00, 1000, LED8PMC);
        }
        else{                         // Otherwise
          led = LED_CHASEFADE;        // Switch to chase fade (LED 8)
          char k[NUM_DIGITS_V];
          for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_CHASEFADE + i);
          displayWrite(3, 0x00, 1000, k);
        }
      }

      // If resistor color mode is detected
      else if(cmdMode == 0x05) {
        led = LED_RESISTOR;           // To resistor mode (LED 10)
        char k[NUM_DIGITS_V];
        for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_CRCODE + i);
        displayWrite(3, 0x00, 1000, k);
      }

      // If microphone mode is detected
      else if(cmdMode == 0x06) {
        LED7_delta = inputBuffer[3];  // Apply param 0 (inputBuffer[3]) to LED 20 threshold value

        if(led == LED_MUSIC) {                // If it is already in mic mode, just display message
          // Higher delta: wider rainbow
          if(LED7_delta == 42) displayWrite(3, 0x00, 1000, LED7PM[2]);
          else if(LED7_delta == 10) displayWrite(3, 0x00, 1000, LED7PM[0]);
          else if(LED7_delta == 21) displayWrite(3, 0x00, 1000, LED7PM[1]);
        }
        else{                         // Otherwise
          led = LED_MUSIC;            // Switch to microphone mode (LED 20)
          char k[NUM_DIGITS_V];
          for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_CSOUND + i);
          displayWrite(3, 0x00, 1000, k);
        }
      }

      // If police lights mode is detected
      else if(cmdMode == 0x07) {
        led = LED_COP;                 // Switch to police light mode!
        char k[NUM_DIGITS_V];
        for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_CCOP + i);
        displayWrite(3, 0x00, 1000, k);
      }
    }

    // If Welcome Message set is detected
    else if(cmdByte == 0x21) {
      uint8_t offset = 2;
      for(uint8_t i = offset; i < (offset + 6); i++) welcomeText[i - offset] = (char)inputBuffer[i];
      saveConfig(1);
      displayWrite(3, 0x00, 1000, welcomeText);
    }

    // If FW version request
    else if(cmdByte == 0x22) {
      uint8_t transferBuffer[10];
      for(uint8_t i = 0; i < 10; i++) transferBuffer[i] = 0;
      transferBuffer[0] = 0x23;     // Start byte
      transferBuffer[1] = 0x22;     // FW output byte
      for(uint8_t i = 2; i < 9; i++) transferBuffer[i] = (uint8_t)fwString[i - 2];
      transferBuffer[9] = 0x24;     // Stop byte
      encoderInstance(transferBuffer);
    }

    // Configuration save request
    else if(cmdByte == 0x33) {
      // Call save config procedure
      saveConfig(0);
    }

    // Configuration reset request
    else if(cmdByte == 0x34) {
      // Call save config procedure
      firstConfig();
    }

    // Bad command or bit error :( Some random return otherwise. You should never ever get to this point.
    else{
      char k[NUM_DIGITS_V];
      for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_ERROR + i);
      // for(uint8_t i = 1; i < 7; i++) k[i - 1] = (char)inputBuffer[i];
      displayWrite(3, 0x00, 1000, k);
    }
  }*/
}

// Serial command encode function for USB Serial
inline void serialCommandEncodeUSB(uint8_t *transferBuffer) {
  // Write to USB Serial instance (constant 10 byte response)
  Serial.write(transferBuffer, 10);
}

inline void serialCommandEncodeBluetooth(uint8_t *transferBuffer) {
  // Write to Bluetooth Serial instance (constant 10 byte response)
  BTSerial.write(transferBuffer, 10);
}

// Reset config, load initial values
void firstConfig() {
  struct LED_SavedParam_Serialization k;
  interface = INTERFACE_TIME;// Interface default: 0
  k.led = LED_STATIC;          // LED default: static (0)
  INTF0_DM = 0;              // Dot mode default: Blink
  isFahrenheit = 0;          // Celsius
  k.LED0P = 0;                 // Default: off
  k.LED7_delta = 21;           // Default xFade delta
  k.LED8_dp = 0;               // Default: right to left
  k.LED11_pt = 0;              // Default: standard cop
  LED21_DF = 1;              // Default: Full brightness
  LED21_hEN = 22;
  LED21_mEN = 0;
  LED21_hDS = 8;
  LED21_mDS = 0;
  clockFlags = 0;            // Default: 10
  // Default welcome message (HELLO)
  char w[NUM_DIGITS_V] = {'H', 'E', 'L', 'L', 'O', ' '};
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) welcomeText[i] = w[i];

  char msg[NUM_DIGITS_V];
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) msg[i] = pgm_read_byte_near(MSG_DEFAULT1 + i);
  displayWrite(3, 0x00, 750, msg);
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) msg[i] = pgm_read_byte_near(MSG_DEFAULT2 + i);
  displayWrite(3, 0x00, 750, msg);
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) msg[i] = pgm_read_byte_near(MSG_DEFAULT3 + i);
  displayWrite(3, 0x00, 750, msg);

  m.EEPReadIn(&m, k);
}

// Global variables load procedure
struct LED_SavedParam_Serialization loadConfig() {
  // Address "pointer"
  struct LED_SavedParam_Serialization k;

  uint16_t addr = 0;
  // Global savings
  // Interface read
  interface = (INTERFACE_MODE_t)EEPROM.read(addr);
  addr++;
  // LED save
  k.led = (LED_MODE_t)EEPROM.read(addr);
  addr++;

  // Call to save all settings
  // Interface 0: Read dot mode
  INTF0_DM = EEPROM.read(addr);
  // if((INTF0_DM == 2) && (led == LED_SILENT)) INTF0_DM == 1;   // Remove dot mode 2 flicker
  addr++;

  // Interface 1: Nothin to read
  // Interface 2: Read fahrenheit flag
  isFahrenheit = EEPROM.read(addr);
  addr++;

  // Interface end

  // LED 0 static presets: Read color configuration
  k.LED0P = EEPROM.read(addr);
  addr++;

  // LED 2 serial command colors: Read array configuration if serial LED mode is enabled
  // if(led == LED_SERIAL_0) {
  uint8_t *ser0 = (uint8_t *) malloc(NUM_BYTES_V * sizeof(uint8_t));
  for(uint8_t i = 0; i < NUM_BYTES_V; i++) ser0[i] = EEPROM.read(addr + i);
  k.SER0 = ser0;
  // render();
  // }
  addr += NUM_BYTES_V;

  // LED 3 smooth write colors: Read array configuration if monitor mode is enabled
  // if(led == LED_SERIAL_1) {
  uint8_t *ser1 = (uint8_t *) malloc(NUM_BYTES_V * sizeof(uint8_t));
  for(uint8_t i = 0; i < NUM_BYTES_V; i++) ser1[i] = EEPROM.read(addr + i);
  k.SER1 = ser1;
  // ledDirectWrite(target_arr);
  // }
  addr += NUM_BYTES_V;

  // LED 6 spectrum fade: Nothing to read
  // LED 7 cross spectrum fade: Read delta
  k.LED7_delta = EEPROM.read(addr);
  if(k.LED7_delta == 0) k.LED7_delta = 21;
  addr++;

  // LED 8 chase fade: Read chase fade direction
  k.LED8_dp = EEPROM.read(addr);
  addr++;

  // LED 10 resistor: Nothing to read
  // LED 11 cop mode: Read pattern
  k.LED11_pt = EEPROM.read(addr);
  addr++;

  // Read setting bit
  clockFlags = EEPROM.read(addr);
  addr++;

  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) welcomeText[i] = EEPROM.read(addr + i);
  addr += 6;

  k.LED21_DF = EEPROM.read(addr);
  addr++;

  k.LED21_hEN = EEPROM.read(addr);
  addr++;
  k.LED21_mEN = EEPROM.read(addr);
  addr++;
  k.LED21_hDS = EEPROM.read(addr);
  addr++;
  k.LED21_mDS = EEPROM.read(addr);

  return k;
}

// Global variables save procedure
void saveConfig(uint8_t mute, struct LED_SavedParam_Serialization k) {
  // Address "pointer"

  uint16_t addr = 0;
  // Global savings
  // Interface save
  EEPROM.update(addr, (uint8_t)interface);
  addr++;
  // LED save
  EEPROM.update(addr, (uint8_t)k.led);
  addr++;

  // Call to save all settings
  // Interface 0: Save dot mode
  EEPROM.update(addr, INTF0_DM);
  addr++;

  // Interface 1: Nothin to save
  // Interface 2: Save fahrenheit flag
  EEPROM.update(addr, isFahrenheit);
  addr++;

  // Interface end

  // LED 0 static presets: Save color configuration
  EEPROM.update(addr, k.LED0P);
  addr++;

  // LED 2 serial command colors: Save array configuration if serial LED mode is enabled
  if(k.led == LED_SERIAL_0) for(uint8_t i = 0; i < NUM_BYTES_V; i++) EEPROM.update(addr + i, LED_Hardware.rgb_arr[i]);
  addr += NUM_BYTES_V;

  // LED 3 serial command colors: Save array configuration if serial LED smooth mode is enabled
  if(k.led == LED_SERIAL_1) for(uint8_t i = 0; i < NUM_BYTES_V; i++) EEPROM.update(addr + i, LED_Hardware.target_arr[i]);
  addr += NUM_BYTES_V;

  // LED 6 spectrum fade: Nothing to save
  // LED 7 cross spectrum fade: Save delta
  EEPROM.update(addr, k.LED7_delta);
  addr++;

  // LED 8 chase fade: Save chase fade direction
  EEPROM.update(addr, k.LED8_dp);
  addr++;

  // LED 10 resistor: Nothing to save
  EEPROM.update(addr, k.LED11_pt);
  addr++;

  // Save clock flags
  EEPROM.update(addr, clockFlags);
  addr++;

  // Save welcome message
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) EEPROM.update(addr + i, welcomeText[i]);
  addr += 6;

  // Save dim factor
  EEPROM.update(addr, LED21_DF);
  addr++;

  // Save night shift parameters
  EEPROM.update(addr, LED21_hEN);
  addr++;
  EEPROM.update(addr, LED21_mEN);
  addr++;
  EEPROM.update(addr, LED21_hDS);
  addr++;
  EEPROM.update(addr, LED21_mDS);

  if(!mute) {
    char k[NUM_DIGITS_V];
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_SAVESETTINGS1 + i);
    displayWrite(3, 0x00, 750, k);
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_SAVESETTINGS2 + i);
    displayWrite(3, 0x00, 750, k);
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_SAVESETTINGS3 + i);
    displayWrite(3, 0x00, 750, k);
  }
}

// Check button for activity. If active, set return SHORTPRESS or LONGPRESS
#ifdef HW_VERSION_22
  uint8_t checkOption(uint8_t buttonPin) {
    // Button check function
    uint8_t num = getNum(buttonPin);
    uint8_t rV = 0;                // State return variable
    if(digitalRead(buttonPin) == LOW) { // v2.2 hw: input pullup
      if(p[num] == FALSE) {     // If button not pressed before
        p[num] = TRUE;          // Set pressed flag
        p_t[num] = millis();    // Set timer as millis
      }

      if ((millis() - p_t[num] > lp_t) && (lp[num] == FALSE)) {
        lp[num] = TRUE;         // Long press detected
        rV = LONGPRESS;                 // Set alternative number
      }
    }else{                      // If digitalRead returns FALSE
      if(p[num] == TRUE) {     // If pressed flag set
        if(lp[num] == TRUE) {  // If long press flag set
          lp[num] = FALSE;      // Reset long press flag
        }else{
          rV = SHORTPRESS;
        }
        p[num] = FALSE;
      }
    }
    return rV;
  }
#else
  uint8_t checkOption(uint8_t buttonPin) {
    // Button check function
    uint8_t num = getNum(buttonPin);
    uint8_t rV = 0;                // State return variable
    if(digitalRead(buttonPin) == HIGH) { // v2.1 or earlier: pulldown
      if(p[num] == FALSE) {     // If button not pressed before
        p[num] = TRUE;          // Set pressed flag
        p_t[num] = millis();    // Set timer as millis
      }

      if ((millis() - p_t[num] > lp_t) && (lp[num] == FALSE)) {
        lp[num] = TRUE;         // Long press detected
        rV = LONGPRESS;                 // Set alternative number
      }
    }else{                      // If digitalRead returns FALSE
      if(p[num] == TRUE) {     // If pressed flag set
        if(lp[num] == TRUE) {  // If long press flag set
          lp[num] = FALSE;      // Reset long press flag
        }else{
          rV = SHORTPRESS;
        }
        p[num] = FALSE;
      }
    }
    return rV;
  }
#endif

// Clear check routine variables when entering a new interface.
// Always use this function to safely switch interfaces
inline void switchInterface(INTERFACE_MODE_t input) {
  clearInterface();
  interface = input;
}

// Safely clear button states on transition
inline void clearInterface() {
  cF1 = 0;
  cF2 = 0;
  cF3 = 0;
  cF4 = 0;
}

inline int getNum(int num) {
  if(num == B_F1_PIN) return 0;
  if(num == B_F2_PIN) return 1;
  if(num == B_F3_PIN) return 2;
  if(num == B_F4_PIN) return 3;
  return -1;
}

// Wire, RTC Initializer, RTC Active Status Checker
void wrInit() {
  Wire.begin();
  rtc.begin();

  Wire.beginTransmission(0x68);
  Wire.write(0x07);
  Wire.write(0x10);

  Wire.endTransmission();

  if(! rtc.isrunning()) rtc.adjust(DateTime(__DATE__, __TIME__));
}

// Fancy welcome message slide in function. Wasted waaay to much time on this :p
void welcome(char *message) {
  uint8_t spaces = 0;                                             // Empty spaces
  for(int i = 0; i < NUM_DIGITS_V; i++) if(message[i] == ' ') spaces++;      // Count all spaces

  uint8_t delayMatrix[][NUM_DIGITS_V] = {{30, 15, 15, 15, 15, 255},
                              {30, 15, 15, 15, 255, 0},
                              {30, 15, 15, 255, 0, 0},
                              {30, 15, 255, 0, 0, 0},
                              {30, 255, 0, 0, 0, 0},
                              {255, 0, 0, 0, 0, 0}};

  for(int k = 0; k < (NUM_DIGITS_V - spaces); k++) {                          // k-th letter of message
    for(int i = 0; i < (NUM_DIGITS_V - k); i++) {                             // Let the letter slide in from the right to the next available position
      char dPattern[NUM_DIGITS_V];                                           // Define empty pattern
      for(int j = 0; j < NUM_DIGITS_V; j++) {
        if(j >= k) dPattern[j] = ' ';                             // All j's larger than current k will be filled with empty spaces
        else dPattern[j] = message[j];                            // If k has increased, fill letters already slided in in advance
      }
      dPattern[5 - i] = message[k];                               // Manipulate i-th filled empty pattern element with k-th letter of message
      displayWrite(3, 0x00, delayMatrix[k][i], dPattern);         // Render the message with delay information
    }
  }

  char empty[] = {' ', ' ', ' ', ' ', ' ', ' '};
  displayWrite(3, 0x00, 400, empty);
  displayWrite(3, 0x00, 1000, message);
}

#ifdef TEMPERATURE_ENABLE
  void updateTemperature() {
    uint8_t data[12];
    uint8_t addr[8];

    // Device detection & checking
    if(!ds.search(addr)) {
      ds.reset_search();
      ts = 0;
    }

    // if(OneWire::crc8(addr, 7) != addr[7]) ts = 0;         // CRC Error Check
    // if((addr[0] != 0x10) && (addr[0] != 0x28)) ts = 0;    // Sensor ID Check

    ds.reset();
    ds.select(addr);
    ds.write(0x44, 0);                                      // Disable parasitic power

    uint8_t present = ds.reset();
    ds.select(addr);
    ds.write(0xBE);                                         // Read scratchpad

    for(uint8_t i = 0; i < 9; i++) data[i] = ds.read();     // Fill in data
    ds.reset_search();

    uint16_t tReading = (data[1] << 8) | data[0];           // Get temperature reading

    // VFD display data conversion
    float fReading = (float)tReading * 6.25;                // Get float value. Display multiplication (* 100; * 1/16 for fixed to FP conversion)
    if(isFahrenheit) fReading = (1.8 * fReading) + 3200.0;  // Fahrenheit option handling

    ts = (uint32_t)fReading;                                // Act as global variable return
  }
#endif


void displayWrite_DIM(uint8_t renderOption, uint8_t ODDR, uint32_t delayOption, const char *message) {
  if(renderOption == 3) {
    // Messaging only with full brightness!
    displayWrite_REG(renderOption, ODDR, delayOption, message);
    return;
  }

  LED21_DC += 1;
  if(LED21_DC == LED21_DF) {
    displayWrite_REG(renderOption, ODDR, delayOption, message);
    LED21_DC = 0;
  }else{
    displayWrite_REG(3, 0x00, 0x00, offs);
  }
}

// Render message to the tubes. See inside the function for a detailed how to use
void displayWrite_REG(uint8_t renderOption, uint8_t ODDR, uint32_t delayOption, const char *message) {
  // uint8_t renderOption      // 0: Time, 1: Date, 2: Temperature, 3: Message

  // uint8_t ODDR = 0;         // Output Dot Overlay Register:
                               // [ reserved | reserved | dot5. | dot4. | dot3. | dot2. | dot1. | dot0. ]
                               // 7                                                                     0

  // int delayOption           // Message delay using delay function (freezing everything else) in ms.

  uint8_t codedOutput[NUM_DIGITS_V];         // Output Coded Pointer: {Sec, SecD, Min, MinD, Hr, HrD}

  if(renderOption == 0) {
    // If getDisplayData is requested to retrieve time information
    DateTime now = rtc.now();
    global_s = now.second(); // Reduce s/m/h getter overhead
    global_m = now.minute();
    global_h = now.hour();

    codedOutput[0] = charConvert(global_s % 10);
    codedOutput[1] = charConvert(global_s / 10);
    codedOutput[2] = charConvert(global_m % 10);
    codedOutput[3] = charConvert(global_m / 10);
    codedOutput[4] = charConvert(global_h % 10);
    codedOutput[5] = charConvert(global_h / 10);
    if(rBit(clockFlags, B_12H)) { // Alternative rendering with 12h format
      uint8_t global_h_alt = global_h ? global_h : 12;                                             // Special case fix: 00:00 -> 12:00 PM
      codedOutput[4] = (global_h_alt > 12) ? charConvert((global_h_alt - 12) % 10) : charConvert(global_h_alt % 10);
      if(!rBit(clockFlags, B_LZERO)) {
        if(global_h_alt > 12) {                                                                     // Let's not use ternary
          if(!((global_h_alt - 12) / 10)) codedOutput[5] = charConvert(' ');                       // Leading zero removal only from 1 PM to 9 PM
          else codedOutput[5] = charConvert((global_h_alt - 12) / 10);                             // Otherwise render normal 12h
        }
        else codedOutput[5] = charConvert((global_h_alt) / 10);                                    // Lower hour 12h render
      }
      else codedOutput[5] = (global_h_alt > 12) ? charConvert((global_h_alt - 12) / 10) : charConvert(global_h_alt / 10);
    }
  }
  else if(renderOption == 1) {
    // If getDisplayData is requested to retrieve date information
    DateTime now = rtc.now();
    global_s = now.second(); // Reduce s/m/h getter overhead
    global_m = now.minute();
    global_h = now.hour();
    codedOutput[0] = charConvert((now.year() % 10));
    codedOutput[1] = charConvert((now.year() % 100) / 10);
    if(rBit(clockFlags, B_INTD)) { // Alternative rendering with MM/DD/YY format
      codedOutput[4] = charConvert((now.month() % 10));
      codedOutput[5] = charConvert((now.month() / 10));
      codedOutput[2] = charConvert((now.day() % 10));
      codedOutput[3] = charConvert((now.day() / 10));
    }else{
      codedOutput[2] = charConvert((now.month() % 10));
      codedOutput[3] = charConvert((now.month() / 10));
      codedOutput[4] = charConvert((now.day() % 10));
      codedOutput[5] = charConvert((now.day() / 10));
    }
  }
  else if(renderOption == 2) {
    #ifdef TEMPERATURE_ENABLE
      // Output ts (Temperature sensor final value)
      codedOutput[0] = charConvert('C');
      codedOutput[1] = charConvert('.');
      codedOutput[2] = charConvert((char)((ts % 10)));
      codedOutput[3] = charConvert((char)((ts % 100) / 10));
      codedOutput[4] = charConvert((char)(((ts % 1000) / 100)));
      codedOutput[5] = charConvert((char)(ts / 1000));
    #endif
  }
  else if(renderOption == 3) {
    for(int i = 0; i < NUM_DIGITS_V; i++) codedOutput[i] = charConvert(message[5 - i]);
  }
  else if(renderOption == 4) {
    #ifdef TEMPERATURE_ENABLE
      // Farenheit render
      codedOutput[0] = charConvert('F');
      codedOutput[1] = charConvert('.');
      codedOutput[2] = charConvert((char)((ts % 10)));
      codedOutput[3] = charConvert((char)((ts % 100) / 10));
      codedOutput[4] = charConvert((char)(((ts % 1000) / 100)));
      codedOutput[5] = charConvert((char)(ts / 1000));
    #endif
  }

  // Check if ODDR bit is set for the i-th position of currentOutputCoded.
  // If yes, OR the dot position bit with 1
  for(int i = 0; i < NUM_DIGITS_V; i++) if(ODDR & (1 << i)) codedOutput[i] |= (1 << 0);

  // Undelayed VFD Output Render
  digitalWriteFast(LATCH_PIN, LOW);
  for(int i = 0; i < NUM_DIGITS_V; i++) shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, codedOutput[i]);
  digitalWriteFast(LATCH_PIN, HIGH);

  // Optional Display Delay Parameter
  if(delayOption != 0) delay(delayOption);
}

// Takes a regular char input and returns the corresponding 7 segment uint8_t. Byte mapping: 0b |a|b|c|d|e|f|g|.|
uint8_t charConvert(char input) {
  // Takes char value (0 to 255) and converts to VFD clock display pattern
  uint8_t output = 0;

  // I/O Logic
  switch(input) {

    // Decimal numbers
    case 0:
      output = 0b11111100;
      break;
    case 1:
      output = 0b01100000;
      break;
    case 2:
      output = 0b11011010;
      break;
    case 3:
      output = 0b11110010;
      break;
    case 4:
      output = 0b01100110;
      break;
    case 5:
      output = 0b10110110;
      break;
    case 6:
      output = 0b10111110;
      break;
    case 7:
      output = 0b11100000;
      break;
    case 8:
      output = 0b11111110;
      break;
    case 9:
      output = 0b11110110;
      break;

    // Letters
    case 'A':
      output = 0b11101110;
      break;
    case 'B':
      output = 0b00111110;
      break;
    case 'C':
      output = 0b10011100;
      break;
    case 'D':
      output = 0b01111010;
      break;
    case 'E':
      output = 0b10011110;
      break;
    case 'F':
      output = 0b10001110;
      break;
    case 'G':
      output = 0b11110110;
      break;
    case 'H':
      output = 0b01101110;
      break;
    case 'I':
      output = 0b00001100;
      break;
    case 'J':
      output = 0b01110000;
      break;
    case 'L':
      output = 0b00011100;
      break;
    case 'N':
      output = 0b00101010;
      break;
    case 'O':
      output = 0b11111100;
      break;
    case 'P':
      output = 0b11001110;
      break;
    case 'Q':
      output = 0b11100110;
      break;
    case 'R':
      output = 0b00001010;
      break;
    case 'S':
      output = 0b10110110;
      break;
    case 'T':
      output = 0b00011110;
      break;
    case 'U':
      output = 0b00111000;
      break;
    case 'V':
      output = 0b01111100;
      break;
    case 'Y':
      output = 0b01110110;
      break;

    // Special characters
    case ' ': // Empty Output
      output = 0b00000000;
      break;
    case '.': // Temperature Dot
      output = 0b11000110;
      break;
    case '-':
      output = 0b00000010;
      break;
    case '0':
      output = 0b11111100;
      break;
  }
  return output;
}

uint8_t decToBcd(uint8_t input) {
  return (((input / 10) << 4) + (input % 10));
}

// Bit manipulation functions
inline void sBit(uint8_t *x, const uint8_t pos) {
  *x |= (1 << pos);
}

inline void cBit(uint8_t *x, const uint8_t pos) {
  *x &= ~(1 << pos);
}

inline void tBit(uint8_t *x, const uint8_t pos) {
  *x ^= (1 << pos);
}

inline uint8_t rBit(uint8_t x, const uint8_t pos) {
  return (x >> pos) & 0x01;
}
