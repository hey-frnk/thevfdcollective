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
