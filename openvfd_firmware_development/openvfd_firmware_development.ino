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

#include "LED_Shared.hpp"
#include "LED_Color.hpp"
#include "LED_Color_Static.hpp"
#include "LED_Color_Spectrum.hpp"
#include "LED_Color_Cross.hpp"
#include "LED_Color_Chase.hpp"
#include "LED_Color_R.hpp"
#include "LED_Color_Cop.hpp"
#include "LED_Color_Music.hpp"
#include "LED_Color_Serial.hpp"
#include "LED_Mode_Manager.hpp"

// Remove comment to enable temperature sensor reading at STEM pin (DS18B20)
// #define    TEMPERATURE_ENABLE

#ifdef TEMPERATURE_ENABLE
  #include <OneWire.h>          // One Wire DS18B20 Temperature Sensor Access - FLUORESCENCE: No TEMP
#endif

// Running firmware for the first time? Remove comment to configure OpenVFD for the first time (EEPROM reset)!
// #define FIRSTCONFIG

// Hardware Version Control
// #define HW_VERSION_22

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

#define    NUM_RGB     6    // 6 LEDs for OpenVFD
#define    NUM_BYTES   (NUM_RGB * 3)    // 3 * 6 = 18 bytes
// const uint8_t NUM_DIGITS_V = NUM_RGB;

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
LED_MODE_t          led         = LED_STATIC;

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

LED_Color LED_Hardware = LED_Color(NUM_RGB, 3);
LED_Mode_Manager *m = NULL;

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
  // if((rgb_arr = (uint8_t *) malloc(NUM_BYTES))) memset(rgb_arr, 0, NUM_BYTES);
  // if((target_arr = (uint8_t *) malloc(NUM_BYTES))) memset(target_arr, 0, NUM_BYTES);
  // render();

  // Wire, RTC Initializer
  wrInit();

  #ifdef FIRSTCONFIG  // If first time configuring, load default and save all settings
    firstConfig();
    saveConfig(1);
  #endif
  // Initialize global saved values
  struct LED_SavedParam_Serialization k = loadConfig();
  m = new LED_Mode_Manager(&LED_Hardware, k);

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
      saveConfig(0, m->EEPGenerate());
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
  m->LED_Manager_Routine();

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

  m->EEPReadIn(k);
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
  uint8_t *ser0 = (uint8_t *) malloc(NUM_BYTES * sizeof(uint8_t));
  for(uint8_t i = 0; i < NUM_BYTES; i++) ser0[i] = EEPROM.read(addr + i);
  k.SER0 = ser0;
  // render();
  // }
  addr += NUM_BYTES;

  // LED 3 smooth write colors: Read array configuration if monitor mode is enabled
  // if(led == LED_SERIAL_1) {
  uint8_t *ser1 = (uint8_t *) malloc(NUM_BYTES * sizeof(uint8_t));
  for(uint8_t i = 0; i < NUM_BYTES; i++) ser1[i] = EEPROM.read(addr + i);
  k.SER1 = ser1;
  // ledDirectWrite(target_arr);
  // }
  addr += NUM_BYTES;

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
  if(k.led == LED_SERIAL_0) for(uint8_t i = 0; i < NUM_BYTES; i++) EEPROM.update(addr + i, LED_Hardware.rgb_arr[i]);
  addr += NUM_BYTES;

  // LED 3 serial command colors: Save array configuration if serial LED smooth mode is enabled
  if(k.led == LED_SERIAL_1) for(uint8_t i = 0; i < NUM_BYTES; i++) EEPROM.update(addr + i, LED_Hardware.target_arr[i]);
  addr += NUM_BYTES;

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
