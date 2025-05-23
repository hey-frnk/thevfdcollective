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
const uint8_t NUM_DIGITS_V = NUM_RGB;

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

// ---- LED Control variables
uint8_t *rgb_arr = NULL;          // LED color storage array
uint8_t *target_arr = NULL;       // Smooth fade target array
uint32_t t_f;                     // LED time check

// ---- LED Preset configuration store. ATF1TION: Different ordering!
//                                 |  G|   R|   B|
//                                 ---------------

#define LED0_cOffset 11                               // # Single color presets
#define LED0_mcOffset (LED0_cOffset - 1)              // # Single color presets - 1

const uint8_t led_scPresets[][3] =  {{  0,   0,   0}, // Off! ("Off")
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

const uint8_t led_Presets[][NUM_BYTES] = {
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
const uint8_t led_Resistor[][3] =  {{0, 0, 0}, {128, 255, 64}, {0, 255, 0}, {30, 255, 0}, {125, 255, 0}, {255, 0, 0}, {0, 0, 255}, {0, 200, 255}, {40, 40, 60}, {255, 255, 255}};

uint8_t LED0P = 0;                // LED preset mode index
// ---- LED Preset configuration set message
const char LED0PM[][4] = {         {' ', 'O', 'F', 'F'},
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

uint8_t LED6_st = 0;              // LED regular fade position

// Lightness & Saturation presets
typedef enum {LED_L_HIGH = 127,   LED_L_REG = 90,   LED_L_LOW = 50    } LED_L_t;
typedef enum {LED_S_HIGH = 255,   LED_S_REG = 196,  LED_S_LOW = 127   } LED_S_t;
LED_L_t LED6_l = LED_L_HIGH;             // LED regular fade lightness
LED_S_t LED6_s = LED_S_HIGH;            // LED regular fade saturation

// LED cross fade starting position
uint8_t LED7_dp = 0;              // LED cross fade position
uint8_t LED7_delta = 21;          // LED cross fade delta
const char LED7PM[][NUM_DIGITS_V] = {        {'C', 'H', 'I', 'L', 'L', ' '},
                                  {'R', 'E', 'G', 'U', 'L', 'A'},
                                  {'T', 'I', 'G', 'H', 'T', ' '}};

uint8_t LED8_ds = 0;              // LED chase fade second flip
uint8_t LED8_dp = 0;              // LED chase fade direction state
uint8_t LED8_dr = 0;              // LED chase fade direction flag
uint8_t LED8_st = 0;              // LED chase fade FSM position
uint8_t LED8_ph = 0;              // LED chase fade rainbow position
const char LED8PM[][4] = {        {' ', 'R', '-', 'L'},
                                  {' ', 'L', '-', 'R'},
                                  {'F', 'L', 'I', 'P'},
                                  {'C', 'L', 'A', 'P'}};

uint8_t LED11_pt = 0;             // LED cop mode pattern
uint8_t LED11_st = 0;             // LED cop mode FSM position
const uint8_t LED11_colors[][3] = {{  0, 255,  10},       // Cop red
                                   {  0,  15, 255}};      // Cop blue

uint8_t LED20_st = 0;                                     // LED microphone mode off fader state
uint8_t LED20_dst = FALSE;                                // LED microphone mode blink delay state
uint8_t LED20_dp = 0;                                     // LED cross fade position
const uint8_t LED20_samplesCount = 8;                     // Empirical sample acquisition array length
uint16_t LED20_samples[LED20_samplesCount] = {0};         // Empirical sample acquisition array allocation
uint8_t LED20_sampleIndex = 0;                            // Buffer push sample index
const uint8_t LED20_analogSamples = 128;                  // How many samples per reading
const uint8_t LED20_sense = 2;                            // Noise threshold
const float LED20_linFactor = -0.50;                      // VU meter log mapping adjust: linFactor in [-1 ... inf], -1 <= lF < 0: log, 0: lin, > 0: exp

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

// Progmem messages (less used, optimized for RAM availability)
const char MSG_COLOR[NUM_DIGITS_V] PROGMEM = {' ', 'C', 'O', 'L', 'O', 'R'};
const char MSG_FADE[NUM_DIGITS_V] PROGMEM = {'C', ' ', 'F', 'A', 'D', 'E'};
const char MSG_CROSSFADE[NUM_DIGITS_V] PROGMEM = {'C', ' ', 'C', 'R', 'F', 'D'};
const char MSG_CHASEFADE[NUM_DIGITS_V] PROGMEM = {'C', ' ', 'C', 'H', 'F', 'D'};
const char MSG_CRCODE[NUM_DIGITS_V] PROGMEM = {'C', 'R', 'C', 'O', 'D', 'E'};
const char MSG_CSOUND[NUM_DIGITS_V] PROGMEM = {'C', 'S', 'O', 'U', 'N', 'D'};
const char MSG_CCOP[NUM_DIGITS_V] PROGMEM = {'C', ' ', ' ', 'C', 'O', 'P'};
const char MSG_CSILENT[NUM_DIGITS_V] PROGMEM = {'C', ' ', 'S', 'L', 'N', 'T'};
const char MSG_LED6_L0[NUM_DIGITS_V] PROGMEM = {' ', ' ', 'H', 'I', 'G', 'H'};
const char MSG_LED6_L1[NUM_DIGITS_V] PROGMEM = {' ', ' ', ' ', 'R', 'E', 'G'};
const char MSG_LED6_L2[NUM_DIGITS_V] PROGMEM = {' ', ' ', ' ', ' ', 'L', 'O'};
const char MSG_INTLDATESET[NUM_DIGITS_V] PROGMEM = {'O', 'O', 'D', 'D', 'Y', 'Y'};
const char MSG_INTLDATERESET[NUM_DIGITS_V] PROGMEM = {'D', 'D', 'O', 'O', 'Y', 'Y'};
const char MSG_LEADINGZERO_ON[NUM_DIGITS_V] PROGMEM = {0, ' ', ' ', ' ', 'O', 'N'};
const char MSG_LEADINGZERO_OFF[NUM_DIGITS_V] PROGMEM = {0, ' ', ' ', 'O', 'F', 'F'};
const char MSG_DEFAULT1[NUM_DIGITS_V] PROGMEM= {'D', 'E', 'F', 'A', 'U', 'L'};
const char MSG_DEFAULT2[NUM_DIGITS_V] PROGMEM = {'S', 'E', 'T', 'I', 'N', 'G'};
const char MSG_DEFAULT3[NUM_DIGITS_V] PROGMEM = {'R', 'E', 'T', 'O', 'R', 'D'};
const char MSG_TIMESYNC1[NUM_DIGITS_V] PROGMEM = {'T', '-', 'D', ' ', ' ', ' '};
const char MSG_TIMESYNC2[NUM_DIGITS_V] PROGMEM = {'S', 'Y', 'N', 'C', 'E', 'D'};
const char MSG_SAVESETTINGS1[NUM_DIGITS_V] PROGMEM = {'A', 'L', 'L', ' ', ' ', ' '};
const char MSG_SAVESETTINGS2[NUM_DIGITS_V] PROGMEM = {'S', 'E', 'T', 'I', 'N', 'G'};
const char MSG_SAVESETTINGS3[NUM_DIGITS_V] PROGMEM = {'S', 'A', 'V', 'E', 'D', ' '};
const char MSG_SILENT1[NUM_DIGITS_V] PROGMEM = {'F', 'U', 'L', 'L', ' ', ' '};
const char MSG_SILENT2[NUM_DIGITS_V] PROGMEM = {'N', 'I', 'G', 'H', 'T', ' '};
const char MSG_SILENT3[NUM_DIGITS_V] PROGMEM = {'S', 'H', 'I', 'F', 'T', ' '};
const char MSG_ON[NUM_DIGITS_V] PROGMEM = {' ', ' ', ' ', ' ', 'O', 'N'};
const char MSG_OFF[NUM_DIGITS_V] PROGMEM = {' ', ' ', ' ', 'O', 'F', 'F'};
const char MSG_ERROR[NUM_DIGITS_V] PROGMEM = {'E', 'R', 'R', 'O', 'R', ' '};

// ------------------------------------------------------------------------------------------
//              Time interval updating event class: Clocked FSM

typedef struct intervalEvent{
  unsigned long interval;
  unsigned long previousMillis;
} intervalEvent;

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
  if((rgb_arr = (uint8_t *) malloc(NUM_BYTES))) memset(rgb_arr, 0, NUM_BYTES);
  if((target_arr = (uint8_t *) malloc(NUM_BYTES))) memset(target_arr, 0, NUM_BYTES);
  render();

  // Wire, RTC Initializer
  wrInit();

  #ifdef FIRSTCONFIG  // If first time configuring, load default and save all settings
    firstConfig();
    saveConfig(1);
  #endif
  // Initialize global saved values
  loadConfig();

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
      saveConfig(0);
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

  // LED 0: Color preset
  if(led == LED_STATIC) {

    // If not single Color
    if(LED0P > LED0_mcOffset) {
      for(uint8_t i = 0; i < NUM_BYTES; i++) target_arr[i] = led_Presets[LED0P - LED0_cOffset][i];
      ledSmoothWrite();
    }
    else{ // Save some RAM
      for(uint8_t offset = 0; offset < NUM_BYTES; offset += 3) {
        target_arr[offset] = led_scPresets[LED0P][0];
        target_arr[offset + 1] = led_scPresets[LED0P][1];
        target_arr[offset + 2] = led_scPresets[LED0P][2];
      }
      ledSmoothWrite();
    }

    if(cF2 == SHORTPRESS) {
      led = LED_FADE;                    // Switch to regular fade
      char k[NUM_DIGITS_V];
      for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_FADE + i);
      displayWrite(3, 0x00, 1000, k);
      clearInterface();
    }

    if(cF3 == SHORTPRESS) {
      LED0P++;
      if(LED0P == 16) LED0P = 0;

      // Dynamic memory saving
      char LED0PMC[NUM_DIGITS_V];
      LED0PMC[0] = 'C';
      LED0PMC[1] = ' ';
      for(uint8_t i = 2; i < NUM_RGB; i++) LED0PMC[i] = LED0PM[LED0P][i - 2];

      displayWrite(3, 0x00, 500, LED0PMC);    // Write change message
      clearInterface();
    }
  }

  // LED 2: Serial accessible color mode
  else if(led == LED_SERIAL_0) {
    // ledDirectWrite(scustom_arr);

    if(cF2 == SHORTPRESS) {
      led = LED_FADE;                    // Switch to regular fade
      char k[NUM_DIGITS_V];
      for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_FADE + i);
      displayWrite(3, 0x00, 1000, k);
      clearInterface();
    }
  }

  // LED 3: Serial smooth write color mode
  else if(led == LED_SERIAL_1) {
    ledSmoothWrite();

    if(cF2 == SHORTPRESS) {
      led = LED_FADE;                    // Switch to regular fade
      char k[NUM_DIGITS_V];
      for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_FADE + i);
      displayWrite(3, 0x00, 1000, k);
      clearInterface();
    }
  }

  // LED 6: Regular fade
  else if(led == LED_FADE) {
    if(updateIntervalEvent(&chUpdater)) LED6_st++;

    uint32_t phase = ledPhase(LED6_st, LED6_s, LED6_l);

    for(uint8_t offset = 0; offset < NUM_BYTES; offset += 3) {
      target_arr[offset] = (uint8_t)((phase >> 16) & 0xFF);     // G
      target_arr[offset + 1] = (uint8_t)((phase >> 8) & 0xFF);  // R
      target_arr[offset + 2] = (uint8_t)(phase & 0xFF);         // B
    }

    ledSmoothWrite();

    if(cF2 == SHORTPRESS) {
      led = LED_CROSSFADE;                    // Switch to cross fade
      char k[NUM_DIGITS_V];
      for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_CROSSFADE + i);
      displayWrite(3, 0x00, 1000, k);
      clearInterface();
    }

    if(cF3 == SHORTPRESS) { // HSL Lightness Change
      char k[NUM_DIGITS_V];
      if(LED6_l == LED_L_REG) {
        LED6_l = LED_L_HIGH; // Change to bright
        for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L0 + i);
        k[0] = 'L';
        displayWrite(3, 0x00, 1000, k);
      }
      else if(LED6_l == LED_L_HIGH) {
        LED6_l = LED_L_LOW; // Change to dim
        for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L2 + i);
        k[0] = 'L';
        displayWrite(3, 0x00, 1000, k);
      }
      else{
        LED6_l = LED_L_REG; // Back to normal
        for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L1 + i);
        k[0] = 'L';
        displayWrite(3, 0x00, 1000, k);
      }
      clearInterface();
    }

    if(cF3 == LONGPRESS) { // HSL Saturation Change
      char k[NUM_DIGITS_V];
      if(LED6_s == LED_S_HIGH) {
        LED6_s = LED_S_LOW; // Change to super low sat
        for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L2 + i);
        k[0] = 'S';
        displayWrite(3, 0x00, 1000, k);
      }
      else if(LED6_s == LED_S_LOW) {
        LED6_s = LED_S_REG; // Change to low sat
        for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L1 + i);
        k[0] = 'S';
        displayWrite(3, 0x00, 1000, k);
      }
      else{
        LED6_s = LED_S_HIGH; // Change to normal (high) sat
        for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L0 + i);
        k[0] = 'S';
        displayWrite(3, 0x00, 1000, k);
      }
      clearInterface();
    }
  }

  // LED 7: Cross fade!
  else if(led == LED_CROSSFADE) {
    if(updateIntervalEvent(&cfUpdater)) LED7_dp++; // Just let it overflow and begin from 0 :p
    uint8_t offset = 0;

    // Cycle position
    for(uint8_t i = 0; i < NUM_RGB; i++) {
      uint32_t phase = ledPhase(LED7_dp + (i * LED7_delta), 255, LED6_l);
      rgb_arr[offset] = (uint8_t)((phase >> 16) & 0xFF);     // G
      rgb_arr[offset + 1] = (uint8_t)((phase >> 8) & 0xFF);  // R
      rgb_arr[offset + 2] = (uint8_t)(phase & 0xFF);         // B
        offset += 3;
    }
    render();


    if(cF2 == SHORTPRESS) {
      led = LED_CHASEFADE;                    // To chase fade (LED 8)
      char k[NUM_DIGITS_V];
      for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_CHASEFADE + i);
      displayWrite(3, 0x00, 1000, k);
      clearInterface();
    }

    if(cF3 == SHORTPRESS) {
      // Higher delta: wider rainbow
      if(LED7_delta == 42) {
        LED7_delta = 10;
        displayWrite(3, 0x00, 1000, LED7PM[0]);
      }
      else if(LED7_delta == 10) {
        LED7_delta = 21;
        displayWrite(3, 0x00, 1000, LED7PM[1]);
      }
      else if(LED7_delta == 21) {
        LED7_delta = 42;
        displayWrite(3, 0x00, 1000, LED7PM[2]);
      }
      else{
        LED7_delta = 10;
        displayWrite(3, 0x00, 1000, LED7PM[0]);
      }
      clearInterface();
    }

    if(cF3 == LONGPRESS) { // HSL Lightness Change
      char k[NUM_DIGITS_V];
      if(LED6_l == LED_L_REG) {
        LED6_l = LED_L_HIGH; // Change to bright
        for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L0 + i);
        k[0] = 'L';
        displayWrite(3, 0x00, 1000, k);
      }
      else if(LED6_l == LED_L_HIGH) {
        LED6_l = LED_L_LOW; // Change to dim
        for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L2 + i);
        k[0] = 'L';
        displayWrite(3, 0x00, 1000, k);
      }
      else{
        LED6_l = LED_L_REG; // Back to normal
        for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L1 + i);
        k[0] = 'L';
        displayWrite(3, 0x00, 1000, k);
      }
      clearInterface();
    }
  }

  // LED 8: Chase fade!
  else if(led == LED_CHASEFADE) {
    if(LED8_dp < 3) {                                         // If reactive to second flip
      // DateTime now = rtc.now();                              // Get time
      if(LED8_ds != global_s) {                           // If the second has changed
        if(LED8_dp == 2) LED8_dr = !LED8_dr;                 // Change chase fade direction
        LED8_st = 0;                                         // Reset state machine
        LED8_ds = global_s;                              // Overwrite old second with new second
        LED8_ph += 22;                                       // Let it overflow and get different values.
      }
    }
    else{                                                    // If reactive to sound
      if(getMicData() > 196) {                                // If the inF1sity of the audio samples are higher than 196 - threshold
        if(updateIntervalEvent(&vu2Updater)) {                // And some time has elapsed
          LED8_dr = !LED8_dr;                                // Flip direction
          LED8_st = 0;                                       // Reset state machine
          LED8_ph += 29;                                     // And get some different color values!
        }
      }
    }

    if(LED8_st < 6) {                                         // Only run this code fragment if state is in range (< 6)
      uint32_t phase = ledPhase(LED8_ph, 255, LED6_l);       // Get new phase
      uint8_t offset = 0;
      if(!LED8_dr) offset = LED8_st * 3;                     // Get manipulating position
      else offset = NUM_BYTES - ((LED8_st * 3) + 3);         // If direction backward, then backward!
      rgb_arr[offset] = (uint8_t)((phase >> 16) & 0xFF);     // Manipulate G
      rgb_arr[offset + 1] = (uint8_t)((phase >> 8) & 0xFF);  // Manipulate R
      rgb_arr[offset + 2] = (uint8_t)(phase & 0xFF);         // Manipulate B
    }
    render();

    if(updateIntervalEvent(&chUpdater)) LED8_st++;

    if(cF2 == SHORTPRESS) {
      led = LED_RESISTOR;        // To resistor mode (LED 10)
      char k[NUM_DIGITS_V];
      for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_CRCODE + i);
      displayWrite(3, 0x00, 1000, k);
      clearInterface();
    }

    if(cF3 == SHORTPRESS) {       // Short press results change in direction
      // LED8_dp = 0: From right to left (default)
      // LED8_dp = 1: From left to right
      // LED8_dp = 2: Direction flip
      // LED8_dp = 3: Flip on clap or any significant change in microphone input value
      LED8_dp++;
      if(LED8_dp == 0) LED8_dr = FALSE;
      else if(LED8_dp == 1) LED8_dr = TRUE;
      else if(LED8_dp == 2) LED8_dr = !INTF0_dr;
      else if(LED8_dp == 4) LED8_dp = 0;
      char LED8PMC[NUM_DIGITS_V];
      for(uint8_t i = 0; i < 2; i++) LED8PMC[i] = ' ';
      for(uint8_t i = 2; i < NUM_RGB; i++) LED8PMC[i] = LED8PM[LED8_dp][i - 2];
      displayWrite(3, 0x00, 1000, LED8PMC);
      clearInterface();
    }

    if(cF3 == LONGPRESS) { // HSL Lightness Change
      char k[NUM_DIGITS_V];
      if(LED6_l == LED_L_REG) {
        LED6_l = LED_L_HIGH; // Change to bright
        for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L0 + i);
        k[0] = 'L';
        displayWrite(3, 0x00, 1000, k);
      }
      else if(LED6_l == LED_L_HIGH) {
        LED6_l = LED_L_LOW; // Change to dim
        for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L2 + i);
        k[0] = 'L';
        displayWrite(3, 0x00, 1000, k);
      }
      else{
        LED6_l = LED_L_REG; // Back to normal
        for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_LED6_L1 + i);
        k[0] = 'L';
        displayWrite(3, 0x00, 1000, k);
      }
      clearInterface();
    }
  }

  // LED 10: Resisor color code!
  else if(led == LED_RESISTOR) {
    uint8_t clockData[NUM_RGB];

    // Get the time once again
    // DateTime now = rtc.now();
    clockData[0] = global_s % 10;
    clockData[1] = global_s / 10;
    clockData[2] = global_m % 10;
    clockData[3] = global_m / 10;
    clockData[4] = global_h % 10;
    clockData[5] = global_h / 10;

    uint8_t offset = 0;
    for(uint8_t i = 0; i < NUM_DIGITS_V; i++) {
      target_arr[offset] = led_Resistor[clockData[i]][0];        // G
      target_arr[offset + 1] = led_Resistor[clockData[i]][1];    // R
      target_arr[offset + 2] = led_Resistor[clockData[i]][2];    // B
      offset += 3;
    }

    ledSmoothWrite();

    if(cF2 == SHORTPRESS) {
      led = LED_COP;                    // Switch to police light mode!
      char k[NUM_DIGITS_V];
      for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_CCOP + i);
      displayWrite(3, 0x00, 1000, k);
      clearInterface();
    }
  }

  // LED 11: Cop lights!
  else if(led == LED_COP) {
    // cfUpdater has update time of 25 ms (ideal for cop mode)
    if(updateIntervalEvent(&cfUpdater)) {
      if(LED11_st < 13) LED11_st++;
      else if(LED11_st == 13) LED11_st = 0;
    }

    if(LED11_pt == 0) {
      if(LED11_st == 0) copHalfRender(0, 1);                      // b | r fill

      else if(LED11_st == 5) {
        for(uint8_t i = 0; i < NUM_BYTES; i += 3) for(uint8_t j = 0; j < 3; j++) rgb_arr[i + j] = led_scPresets[0][j];
        render();                                                 // off fill
      }

      else if(LED11_st == 6) copHalfRender(0, 1);                 // b | r fill
      else if(LED11_st == 7) copHalfRender(1, 0);                 // r | b fill

      else if(LED11_st == 12) {
        for(uint8_t i = 0; i < NUM_BYTES; i += 3) for(uint8_t j = 0; j < 3; j++) rgb_arr[i + j] = led_scPresets[1][j];
        render();                                                 // white fill
      }

      else if(LED11_st == 13) copHalfRender(1, 0);                // r | b fill
    }

    if(cF2 == SHORTPRESS) {
      led = LED_MUSIC;                    // Switch to microphone mode!
      char k[NUM_DIGITS_V];
      for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_CSOUND + i);
      displayWrite(3, 0x00, 1000, k);
      clearInterface();
    }
  }

  // LED 20: Microphone mode!
  else if(led == LED_MUSIC) {
    // If time interval passed, decrease turned on LEDs by one (regular state update)
    if(updateIntervalEvent(&vuUpdater)) if(LED20_st < 7) LED20_st++;

    // Cross fade LED color position update
    if(updateIntervalEvent(&cfUpdater)) LED20_dp++; // Just let it overflow and begin from 0 :p

    // Read microphone information, convert value to updateable state
    // Get mic data (log), divide by 36
    uint8_t rLevel = 6 - (uint8_t)(roundf(getMicData() / 42.5));
    // Write the less valued LEDs only when the sound is actively changed

    if(LED20_st >= rLevel) {
      LED20_st = rLevel;                                        // If the new state is lower than the previous state: Overwrite current state with new rLevel (interrupt state)
      LED20_dst = FALSE;                                        // Delay state = 0 (reset)
    }
    else{
      if(updateIntervalEvent(&vu2Updater)) LED20_dst = TRUE;    // If the time has elapsed, write empty
    }

    if(LED20_st < 7) {                                           // If new information
      uint8_t offset = 0;

      for(uint8_t i = 0; i < (6 - LED20_st); i++) {
        uint32_t phase = ledPhase(LED20_dp + (i * LED7_delta), 255, 127);
        rgb_arr[offset] = (uint8_t)((phase >> 16) & 0xFF);      // G
        rgb_arr[offset + 1] = (uint8_t)((phase >> 8) & 0xFF);   // R
        rgb_arr[offset + 2] = (uint8_t)(phase & 0xFF);          // B
        offset += 3;
      }

      // And set all the others zero
      for(uint8_t lOffset = offset; lOffset < NUM_BYTES; lOffset++) rgb_arr[lOffset] = 0;

      // Black out the inactives
      if(LED20_dst) {
        if(LED20_st < 6) {
          uint8_t tOffset = 0;                                  // Temporary offset variable
          for(uint8_t i = 0; i < (5 - LED20_st); i++) {
            rgb_arr[tOffset] = 0;
            rgb_arr[tOffset + 1] = 0;
            rgb_arr[tOffset + 2] = 0;
            tOffset += 3;
          }
        }
      }

      render();
    }

    if(cF2 == SHORTPRESS) {
      led = LED_SILENT;                    // To silent mode
      LED21_st = LED21_SILENT;             // Standard silent
      char k[NUM_DIGITS_V];
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

    if(cF3 == SHORTPRESS) {
      // Higher delta: wider rainbow
      if(LED7_delta == 42) {
        LED7_delta = 10;
        displayWrite(3, 0x00, 1000, LED7PM[0]);
      }
      else if(LED7_delta == 10) {
        LED7_delta = 21;
        displayWrite(3, 0x00, 1000, LED7PM[1]);
      }
      else if(LED7_delta == 21) {
        LED7_delta = 42;
        displayWrite(3, 0x00, 1000, LED7PM[2]);
      }
      else{
        LED7_delta = 10;
        displayWrite(3, 0x00, 1000, LED7PM[0]);
      }
      clearInterface();
    }
  }

  // LED 21: Special silent mode!
  else if(led == LED_SILENT) {

    displayWrite = &displayWrite_DIM;
    if(LED21_st == LED21_SILENT) {
      // State: standard silent mode
      // 'Leave Silent': A short press on F2 leaves silent mode and returns to LED0
      if(cF2 == SHORTPRESS) {
        led = LED_STATIC;                    // Back to LED 0
        LED21_st = LED21_SILENT;             // Just make sure
        displayWrite = &displayWrite_REG;
        char k[NUM_DIGITS_V];
        for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_COLOR + i);
        displayWrite = &displayWrite_REG;
        displayWrite(3, 0x00, 1000, k);
        clearInterface();
      }
      // 'Dim' Short press on F3 changes state to sleep mode
      if(cF3 == SHORTPRESS) {
        LED21_st = LED21_SLEEP;
        LED21_DF = LED21_DFMAX;
        // Message
        displayWrite = &displayWrite_REG;
        char k[NUM_DIGITS_V];
        for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_SILENT2 + i);
        displayWrite(3, 0x00, 750, k);
        for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_SILENT3 + i);
        displayWrite(3, 0x00, 750, k);
        clearInterface();
      }
    }

    else if(LED21_st == LED21_SLEEP) {
      // State: dimmed silence
      // 'Break Dim': A short press on F2 leaves silent mode and returns to LED0
      if(cF2 == SHORTPRESS) {
        led = LED_STATIC;                    // Back to LED 0
        LED21_st = LED21_SLEEP;              // Just make sure
        displayWrite = &displayWrite_REG;
        char k[NUM_DIGITS_V];
        for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_COLOR + i);
        displayWrite(3, 0x00, 1000, k);
        clearInterface();
      }
      // 'Undim': Short press on F3 changes state to silent mode
      if(cF3 == SHORTPRESS) {
        LED21_st = LED21_SILENT;
        LED21_DF = 1;
        // Message
        displayWrite = &displayWrite_REG;
        char k[NUM_DIGITS_V];
        for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_SILENT1 + i);
        displayWrite(3, 0x00, 750, k);
        clearInterface();
      }
    }

    else if(LED21_st == LED21_SHIFTSLEEP) {
      // State: dimmed silence trigered by night shift

      // 'Explicit restore': F2 restores to saved LED state, breaking night shift
      if(cF2 == SHORTPRESS) {
        led = LED21_shslpst;                    // Restore previous LED mode
        LED21_st = LED21_SILENT;                // Just make sure
        displayWrite = &displayWrite_REG;
        char k[NUM_DIGITS_V];                   // Say: Colors restor'd!
        for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_COLOR + i);
        displayWrite(3, 0x00, 750, k);
        for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_DEFAULT3 + i);
        displayWrite(3, 0x00, 750, k);
        clearInterface();
      }

      // 'Explicit wake': F3 changes to silent, breaking night shift
      if(cF3 == SHORTPRESS) {
        LED21_st = LED21_SILENT;
        LED21_DF = 1;
        // Message
        char k[NUM_DIGITS_V];
        for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_SILENT1 + i);
        displayWrite(3, 0x00, 750, k);
        clearInterface();
      }
    }


  }

  // Check if night shift time set is enabled. If yes, check for trigger signal
  if(rBit(clockFlags, B_NSHIFT)) if(updateIntervalEvent(&nShiftUpdater)) nightShiftRoutine(global_h, global_m, global_s);
}

// This routine is to check for incoming serial data
void serialRoutine() {
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

    /*for(uint8_t i = 0; i < 24; i++) {
      Serial.print((int)inputBuffer[i]);
      Serial.print(" ");
    }
    Serial.print("Start \n");*/

    serialCommandDecode(inputBuffer, serialCommandEncodeBluetooth);

    // Discard buffer
    uint8_t flushBuffer[BTSerial.available()];
    BTSerial.readBytes(flushBuffer, BTSerial.available());
    BTSerial.flush();

    /*for(uint8_t i = 0; i < 24; i++) {
      Serial.print((int)inputBuffer[i]);
      Serial.print(" ");
    }
    Serial.print("Stop \n");*/
  }
}

// This routine is to check whether night shift should be turned on/off or not
inline void nightShiftRoutine(uint8_t h, uint8_t m, uint8_t s) {
  // If night shift is on, look for disable trigger, otherwise look for enable trigger

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
  }
}

// Serial command decode function. Takes input buffer from USB Serial or Software Serial
// Thus, if response is required, the 2nd argument calls the correct response function
void serialCommandDecode(uint8_t *inputBuffer, void (*encoderInstance)(uint8_t *)) {
  // If aligned protocol is detected
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
  }
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
  interface = INTERFACE_TIME;// Interface default: 0
  led = LED_STATIC;          // LED default: static (0)
  INTF0_DM = 0;              // Dot mode default: Blink
  isFahrenheit = 0;          // Celsius
  LED0P = 0;                 // Default: off
  LED7_delta = 21;           // Default xFade delta
  LED8_dp = 0;               // Default: right to left
  LED11_pt = 0;              // Default: standard cop
  LED21_DF = 1;              // Default: Full brightness
  LED21_hEN = 22;
  LED21_mEN = 0;
  LED21_hDS = 8;
  LED21_mDS = 0;
  clockFlags = 0;            // Default: 10
  // Default welcome message (HELLO)
  char w[NUM_DIGITS_V] = {'H', 'E', 'L', 'L', 'O', ' '};
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) welcomeText[i] = w[i];

  char k[NUM_DIGITS_V];
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_DEFAULT1 + i);
  displayWrite(3, 0x00, 750, k);
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_DEFAULT2 + i);
  displayWrite(3, 0x00, 750, k);
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_DEFAULT3 + i);
  displayWrite(3, 0x00, 750, k);
}

// Global variables load procedure
void loadConfig() {
  // Address "pointer"

  uint16_t addr = 0;
  // Global savings
  // Interface read
  interface = (INTERFACE_MODE_t)EEPROM.read(addr);
  addr++;
  // LED save
  led = (LED_MODE_t)EEPROM.read(addr);
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
  LED0P = EEPROM.read(addr);
  addr++;

  // LED 2 serial command colors: Read array configuration if serial LED mode is enabled
  if(led == LED_SERIAL_0) {
    for(uint8_t i = 0; i < NUM_BYTES; i++) rgb_arr[i] = EEPROM.read(addr + i);
    render();
  }
  addr += NUM_BYTES;

  // LED 3 smooth write colors: Read array configuration if monitor mode is enabled
  if(led == LED_SERIAL_1) {
    for(uint8_t i = 0; i < NUM_BYTES; i++) target_arr[i] = EEPROM.read(addr + i);
    ledDirectWrite(target_arr);
  }
  addr += NUM_BYTES;

  // LED 6 spectrum fade: Nothing to read
  // LED 7 cross spectrum fade: Read delta
  LED7_delta = EEPROM.read(addr);
  if(LED7_delta == 0) LED7_delta = 21;
  addr++;

  // LED 8 chase fade: Read chase fade direction
  LED8_dp = EEPROM.read(addr);
  addr++;

  // LED 10 resistor: Nothing to read
  // LED 11 cop mode: Read pattern
  LED11_pt = EEPROM.read(addr);
  addr++;

  // Read setting bit
  clockFlags = EEPROM.read(addr);
  addr++;

  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) welcomeText[i] = EEPROM.read(addr + i);
  addr += 6;

  LED21_DF = EEPROM.read(addr);
  addr++;

  LED21_hEN = EEPROM.read(addr);
  addr++;
  LED21_mEN = EEPROM.read(addr);
  addr++;
  LED21_hDS = EEPROM.read(addr);
  addr++;
  LED21_mDS = EEPROM.read(addr);
}

// Global variables save procedure
void saveConfig(uint8_t mute) {
  // Address "pointer"

  uint16_t addr = 0;
  // Global savings
  // Interface save
  EEPROM.update(addr, (uint8_t)interface);
  addr++;
  // LED save
  EEPROM.update(addr, (uint8_t)led);
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
  EEPROM.update(addr, LED0P);
  addr++;

  // LED 2 serial command colors: Save array configuration if serial LED mode is enabled
  if(led == LED_SERIAL_0) for(uint8_t i = 0; i < NUM_BYTES; i++) EEPROM.update(addr + i, rgb_arr[i]);
  addr += NUM_BYTES;

  // LED 3 serial command colors: Save array configuration if serial LED smooth mode is enabled
  if(led == LED_SERIAL_1) for(uint8_t i = 0; i < NUM_BYTES; i++) EEPROM.update(addr + i, target_arr[i]);
  addr += NUM_BYTES;

  // LED 6 spectrum fade: Nothing to save
  // LED 7 cross spectrum fade: Save delta
  EEPROM.update(addr, LED7_delta);
  addr++;

  // LED 8 chase fade: Save chase fade direction
  EEPROM.update(addr, LED8_dp);
  addr++;

  // LED 10 resistor: Nothing to save
  EEPROM.update(addr, LED11_pt);
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

inline uint16_t LED20_mapUnsigned16(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
  if(in_max - in_min == 0) return 0;
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Dynamically adjust sensitivity according to amplitude (sort-of-AGC)
uint16_t LED20_lLevelArr_vPush(uint16_t amplitudeIn) {
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

const float beta = 0.25;

// This function reads the microphone input and returns a value between 0 and 255
// Threshold sets the minimum value the mic is sensitive to. Must not be larger than 49 (not checked in the function, results division by zero otherwise)
uint8_t getMicData(void) {
  uint16_t dMicRead = 512;
  uint16_t dPrevSample = 512;

  // Obtain amplitude
  uint16_t dMicMax = 0;
  uint16_t dMicMin = 1023;
  for(uint8_t i = 0; i < LED20_analogSamples; i++) {
    dPrevSample = dMicRead;
    dMicRead = analogRead(MIC_PIN);
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

// Fill LED array left half with one and the right half with other color
void copHalfRender(uint8_t right, uint8_t left) {
  for(uint8_t i = 0; i < (NUM_BYTES >> 1); i+= 3) for(uint8_t j = 0; j < 3; j++) rgb_arr[i + j] = LED11_colors[right][j];
  for(uint8_t i = (NUM_BYTES >> 1); i < NUM_BYTES; i+= 3) for(uint8_t j = 0; j < 3; j++) rgb_arr[i + j] = LED11_colors[left][j];
  render();
}

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

// Smooth transistion LED render
void ledSmoothWrite() {
  // Obtain equality
  for(uint8_t i = 0; i < NUM_BYTES; i++) {
    if(rgb_arr[i] < target_arr[i]) rgb_arr[i]++;
    else if(rgb_arr[i] > target_arr[i]) rgb_arr[i]--;
  }
  render();
}

// Direct LED render
void ledDirectWrite(uint8_t *ledTarget) {
  memcpy(rgb_arr, ledTarget, NUM_BYTES);
  memcpy(target_arr, ledTarget, NUM_BYTES);
  render();
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

// This section is writF1 by:
// Acrobotic - 01/10/2013
// Author: x1sc0

/*License:
  Beerware License; if you find the code useful, and we happen to cross
  paths, you're encouraged to buy us a beer. The code is distributed hoping
  that you in fact find it useful, but  without warranty of any kind.*/

void render(void) {
  if(!rgb_arr) return;

  while((micros() - t_f) < 50L);  // wait for 50us (data latch)

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
