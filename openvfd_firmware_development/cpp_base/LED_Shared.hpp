#ifndef LED_SHARED_HPP
#define LED_SHARED_HPP

#ifdef DEBUG
#include "ArduinoSIM.h"
#else
#include <Arduino.h>

#define NUM_DIGITS_V 6

#define    FALSE       0
#define    TRUE        1

#define    SHORTPRESS  1
#define    LONGPRESS   2

extern void (*displayWrite)(uint8_t, uint8_t, uint32_t, const char *);

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

const char LED7PM[][NUM_DIGITS_V] = {{'C', 'H', 'I', 'L', 'L', ' '},
                                  {'R', 'E', 'G', 'U', 'L', 'A'},
                                  {'T', 'I', 'G', 'H', 'T', ' '}};

const char LED8PM[][4] = {        {' ', 'R', '-', 'L'},
                                  {' ', 'L', '-', 'R'},
                                  {'F', 'L', 'I', 'P'},
                                  {'C', 'L', 'A', 'P'}};

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

const uint8_t led_Presets[][18] = {
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

const uint8_t LED11_colors[][3] = {{  0, 255,  10},       // Cop red
                                   {  0,  15, 255}};      // Cop blue

typedef enum {LED_L_HIGH = 127,   LED_L_REG = 90,   LED_L_LOW = 50    } LED_L_t;
typedef enum {LED_S_HIGH = 255,   LED_S_REG = 196,  LED_S_LOW = 127   } LED_S_t;

uint32_t ledPhase(uint8_t h, uint8_t s, uint8_t l);
uint8_t getMicData(uint8_t MicPin);

typedef struct intervalEvent{
  unsigned long interval;
  unsigned long previousMillis;
} intervalEvent;

extern struct intervalEvent newiE(long p1);
extern void resetiE(intervalEvent *input);
extern uint8_t updateIntervalEvent(intervalEvent *input);

#endif
