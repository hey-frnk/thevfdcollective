/**
  ******************************************************************************
  * @file     vfdco_color_lib.h
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     13-November-2019
  * @brief    This file contains declarations of a color library written for WS2812B/SK6812
  *           Designed to be used with Fluorescence by The VFD Collective
  ******************************************************************************
  * @toc      Table of contents, enter to navigate:
  *           SECTION_ADJUSTABLE_PARAMETERS
  *           SECTION_DRIVER_INTERFACE
  *           SECTION_TIMER_INTERFACE
  *           SECTION_HSL
  *           SECTION_BLENDING_FUNCTIONS
  *           SECTION_LED_COLOR
  *             -SUBSECTION_COLOR_FADER
  *             -SUBSECTION_COLOR_FLASHER
  *             -SUBSECTION_COLOR_CHASER
  *           SECTION_COLOR_MANAGER
  ******************************************************************************
 **/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _VFDCO_COLOR_LIB
#define _VFDCO_COLOR_LIB

#include <stdint.h>
#include "vfdco_time.h"

/** Begin of:
  * @toc SECTION_ADJUSTABLE_PARAMETERS
  * @brief  Here are some adjustable parameters to tune the library
 **/
// 7 bit time fading
#define     LED_COLOR_FADER_TIME_BITS       8

/** Begin of:
  * @toc SECTION_DRIVER_INTERFACE
  * @brief  This is the interface between the physical driver and the color lib
            the color lib will work, when every declaration here is implemented in the driver
 **/
// #include <vfdco_ws2812b.h>
// #include <vfdco_sk6812.h>

// #define HARDWARE_OPTION_WS2812B
#define HARDWARE_OPTION_SK6812

extern uint8_t  num_rgb;                  // Number of physical LEDs (pixels)
extern uint8_t  num_bpp;                  // Number of bytes per pixel bpp
extern uint8_t  num_bytes;                // Number of bytes per pixel bpp (3: RGB, 4: RGBW) * num_rgb
                // Array of color values of size num_bytes to be written in the next write cycle
                // to the physical WS2812B/SK6812 LEDs
extern uint8_t  *rgb_arr;

// Initialize SW/HW of num_pixels * SK6812 LEDs
extern void vfdco_clr_init(uint8_t num_pixels);
// You might never ever make use of it but hey... in case
extern void vfdco_clr_deInit(void);

// Some simple access functions. Add some more if u feel creative
extern void vfdco_clr_set_RGB(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
extern void vfdco_clr_set_RGBW(uint8_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
extern void vfdco_clr_set_all_RGB(uint8_t r, uint8_t g, uint8_t b);
extern void vfdco_clr_set_all_RGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w);


/** Begin of:
  * @toc SECTION_TIMER_INTERFACE
  * @brief  This is the interface between the time driver and the color lib
            the color lib will work, when every declaration here is implemented in the driver
 **/
/*  typedef struct time_event_t{
      unsigned long interval;
      unsigned long previousTime;
    } time_event_t; */

// Create new timer (updates every p1 milliseconds)
extern struct time_event_t Time_Event_Init(long interval);
// Reset timer milliseconds to 0
extern void Time_Event_Reset(struct time_event_t *input);
// Check if p1 has elapsed. If yes, return 1, else return 0
extern uint8_t Time_Event_Update(struct time_event_t *input);


// Colors all written? Shuttle the data bit by bit into the LEDs
extern void vfdco_clr_render();

/** Begin of:
  * @toc SECTION_HSL
  * @brief  Definition of the HSL class. An HSL color is fully defined by:
  *         H: Hue, circular color value between 0 (red) ... 85 (green) ... finally 255 (red again)
  *         S: Saturation, between 0 (grayscale) ... 255 (fully saturated)
  *         L: Lightness, between 0 (essentially black/off), 127 (full), 255 (essentially white)
 **/
/**
  * @brief  Definition of HSL class as hsl_t
 **/
typedef struct hsl_t { uint8_t h, s, l; } hsl_t; // Hue Saturation Lightness
typedef struct hsl_d_t { int8_t h, s, l; } hsl_d_t; // Hue Saturation Lightness Difference

/**
 * @brief  Declaration of constructor HSL class, HSL::HSL(h, s, l)
**/
hsl_t *HSL_Init(  uint8_t h, // Hue value
                  uint8_t s, // Saturation value
                  uint8_t l  // Lightness value
);

/**
 * @brief  Declaration of alternative constructor HSL class, HSL::HSL(h, s, l, dh, ds, dl)
**/
hsl_t *HSL_Init_Range(    uint8_t h, uint8_t s, uint8_t l, // Like above
                          // FWHW: Transforms tolerance into gaussian distributed color value using box muller transform (3x)
                          float dh, float ds, float dl
);

/**
 * @brief  Declaration of destructor HSL class, HSL::* HSL(h, s, l)
**/
void HSL_Delete(hsl_t *self);

/** Begin of:
  * @toc SECTION_BLENDING_FUNCTIONS
 **/
typedef enum {
  // Blend mode implementation according to https://en.wikipedia.org/wiki/Blend_modes
  // Assume R, G, B in [0 ... 1], f(a, b) = blended with a = existing value, b = blend layer
   LED_COLOR_BLEND_MODE_NORMAL      =   0,  // f(a, b) = b
   LED_COLOR_BLEND_MODE_MULTIPLY    =   1,  // f(a, b) = ab
   LED_COLOR_BLEND_MODE_SCREEN      =   2,  // f(a, b) = 1 - (1-a)*(1-b)
   LED_COLOR_BLEND_MODE_OVERLAY     =   3,  // f(a, b) = {2ab, a < 0.5} {1 - 2(1-a)(1-b), else}
   LED_COLOR_BLEND_MODE_SOFT_LIGHT  =   4,  // f(a, b) = (1-2b) * a^2 + 2ba
} LED_COLOR_BLEND_MODE_t;

/** Begin of:
  * @toc SECTION_LED_COLOR
  * @brief LED_COLOR is an abstract class with each child class implementing a type of action performed
  *        by LEDs. For example LED_Color_Flasher flashes an LED upon instantiation with a given LED color
  *        at a given position. By calling the next method (virt.), the next action is performed for all
  *        active instances of LED_Color
 **/
/*
 * @brief  Different states of an LED_Color object
 *         In for any state != COMPLETE, LED_Color::Next will be called, or else the destructor LED_Color::~LED_Color
**/
typedef enum {
  LED_COLOR_STATE_COMPLETE          =   0,  // Fade stopped. All colors done, color manager will call destructor
  LED_COLOR_STATE_ACTIVE            =   1,  // Stationary state: Regular operation
  LED_COLOR_STATE_FADE_IN           =  10,  // Transient state: Fading in
  LED_COLOR_STATE_FADE_OUT          =  11,  // Transient state: Fading out
  LED_COLOR_STATE_CYCLIC_RECOVERY   =  20,  // Special case: Cyclic rollover
  LED_COLOR_STATE_ALWAYS_ACTIVE     =  22   // Repeat forever until external change of state
} LED_COLOR_STATE_t;

/**
  * @brief  Definition of the abstract LED_Color action class.
  *         For an LED_Color class to become a valid action, the mapping of Next and Delete is required
**/
struct LED_Color;
struct LED_Color_VTable {
  LED_COLOR_STATE_t     (*Next)             (struct LED_Color *unsafe_self);
  void                  (*Delete)           (struct LED_Color *unsafe_self);
};
struct LED_Color {
  // VTable (virtual) functions
  LED_COLOR_STATE_t     (*Next)             (struct LED_Color *unsafe_self);
  void                  (*Delete)           (struct LED_Color *unsafe_self);

  // Actual linked VTable
  struct LED_Color_VTable VTable;

  // Software timer
  time_event_t          timer;
};

/**
 * @brief  Declaration of constructor LED_Color::LED_Color
**/
void LED_Color_Init(struct LED_Color *f);




/** Begin of:
 * @toc SUBSECTION_COLOR_FADER
**/
struct LED_Color_Fader {
  // Functions
  struct LED_Color super;

  void          (*_blend)         (uint8_t, uint8_t, uint8_t, uint8_t);

  // Option: Peaks
  uint8_t       num_pks;        // Number of peaks
  hsl_t         **pks;        // Peaks array
  // Option: Chaining
  uint8_t       num_chain,      // Number of chained pixels. 0 = singular
                chain_huediff;  // Constant difference of hue between chained pixels
  // Option: Time
  int8_t        repeat;         // If == k: pk[n] -> pk[0] for k cycles, else fade in & out once
  // Option: StartPos
  uint8_t       start_pos;      // LED start index < end index

  // State Variables
  LED_COLOR_STATE_t state;      // Color Fader FSM
  uint16_t      fade_pos;       // Fade position (substate)
};

/**
  * @brief  Constructor of LED_Color_Fader class
 **/
struct LED_Color_Fader *LED_Color_Fader_Init(
  LED_COLOR_BLEND_MODE_t    blend_mode,             // Pixel blend setting.
  uint8_t                   start_pos,              // Pixel index to start
  int8_t                    repeat,                 // Fade N cycles
  uint8_t                   num_pks,                // Number of HSL colors
  hsl_t                     **pks,                  // Array of HSL colors
  uint8_t                   num_chain,              // Number of chained pixels
  uint8_t                   chain_hue_diff          // Hue difference between chained pixels
);

/** Begin of:
 * @toc SUBSECTION_COLOR_FLASHER
**/
struct LED_Color_Flasher {
  // Functions
  struct LED_Color super;

  void          (*_blend)           (uint8_t, uint8_t, uint8_t, uint8_t);
  hsl_t         *pk;                // Peaks array

  uint8_t       flash_duration;     // Duration of each flash
  uint16_t      flash_offtime;      // Duration of off time, 0: as long as flash, 1: twice as long, 2: 4x as long, ...
  int8_t        flash_repeat;       // Repeat flash how many times? (0 == single flash)

  // Option: StartPos
  uint8_t       start_pos;          // LED start index < end index

  // State Variables
  LED_COLOR_STATE_t state;          // Color Fader FSM
  uint16_t      flash_pos;          // Fade position (substate)
};
/**
  * @brief  Constructor of LED_Color_Flasher class
 **/
struct LED_Color_Flasher *LED_Color_Flasher_Init(
  LED_COLOR_BLEND_MODE_t    blend_mode,             // Pixel blend setting.
  uint8_t                   start_pos,              // Pixel index to start
  int8_t                    repeat,                 // Repeat flash how many times?
  hsl_t                     *pk,                    // Array of HSL colors
  uint8_t                   duration,               // Duration of each flash
  uint8_t                   offtime                 // Duration of flash offtime (factor)
);

/** Begin of:
 * @toc SUBSECTION_COLOR_CHASER
**/
enum {
  LED_COLOR_CHASER_C_PRESERVING_ON,     // Only active pixel will be affected by LED chase
  LED_COLOR_CHASER_C_PRESERVING_OFF     // Active pixel and past pixels will be affected by LED chase
};
enum {
  LED_COLOR_CHASER_MODE_LINEAR        = 0,        // Chase in a linear way
  LED_COLOR_CHASER_MODE_ACCELERATING  = 1,        // Become faster with every chasing step (exponential)
  LED_COLOR_CHASER_MODE_DECELERATING  = 2,        // Become slower with every chasing step (exponential)
  LED_COLOR_CHASER_MODE_SPLITLIN      = 4,       // Split in the middle and chase in both directions
  LED_COLOR_CHASER_MODE_SPLITACC      = 5,       // -||- accelerating
  LED_COLOR_CHASER_MODE_SPLITDEC      = 6        // -||- decelerating
};

struct LED_Color_Chaser {
  // Functions
  struct LED_Color super;

  void          (*_blend)             (uint8_t, uint8_t, uint8_t, uint8_t);
  hsl_t         *pk;                  // Fade to peak
  hsl_d_t       *pk_diff;             // Difference of each new peak to initial peak (factorized)

  uint8_t        chase_cpreserving;   // Restore past pixels or replace
  uint8_t        chase_mode;          // Chase mode
  int8_t         chase_repeat;        // Repeat

  uint16_t       chase_duration;      // (Min (acc), Max (dec)) time diff between LED chase

  // Option: Start & Length
  uint8_t        start_pos;           // LED start index < end index
  uint8_t        chase_length;        // start_pos + chase_length = end pos

  // State Variables
  LED_COLOR_STATE_t state;            // Color Chaser FSM
  uint8_t        pk_state;            // Peak state FSM
  uint16_t       chase_pos;           // Fade position (substate)
};
/**
  * @brief  Constructor of LED_Color_Chaser class
 **/
struct LED_Color_Chaser *LED_Color_Chaser_Init(
  LED_COLOR_BLEND_MODE_t    blend_mode,             // Pixel blend setting.
  uint8_t                   start_pos,              // Pixel index to start
  int8_t                    repeat,                 // Repeat N times
  uint8_t                   length,                 // Pixel index to start
  hsl_t                     *pk,                    // Array of HSL colors
  hsl_d_t                   *pk_diff,               // Difference peaks
  uint16_t                  duration,               // Chase duration
  uint8_t                   cpreserving,            // Color preserving?
  uint8_t                   mode                    // Chase mode
);


/** Begin of:
  * @toc SECTION_COLOR_MANAGER
  * @brief COLOR_MANAGER is a class providing scheduling and execution of objects of LED_Color class.
 **/
struct LED_Color_Manager {
  struct LED_Color  **process_list;             // Queue of active LED_Color processes
  uint_fast8_t      process_count;              // Number of active LED_Color processes

  // Run every process that is not in LED_COLOR_STATE_COMPLETE once (internal)
  void              (*_Run_All)          (void);
  // Enqueue given process into process_list and mark as active/running
  void              (*Create_Process)    (struct LED_Color *process);
  // Dequeue given PID of process list
  void              (*Remove_Process)    (uint_fast8_t pid);
  void              (*_Process_Finished_Callback) (uint_fast8_t pid); // (Automatic call version)

  struct LED_Color_Fader *     (*Init_LED_Color_Fader_From_Bitstream)   (uint8_t *p, uint_fast16_t length);
  struct LED_Color_Flasher *   (*Init_LED_Color_Flasher_From_Bitstream) (uint8_t *p, uint_fast16_t length);
  struct LED_Color_Chaser *    (*Init_LED_Color_Chaser_From_Bitstream)  (uint8_t *p, uint_fast16_t length);
};





/** Begin of:
  * @toc SECTION_LED_COLOR_MODE
 **/
/**
  * @brief  Virtual table for LED_Color_Mode
 **/
struct LED_Color_Mode;
struct LED_Color_Mode_VTable {
  void                  (*F3)               (struct LED_Color_Mode *unsafe_self);
  void                  (*F3Var)            (struct LED_Color_Mode *unsafe_self);
  void                  (*Update)           (struct LED_Color_Mode *unsafe_self);
  void                  (*Hello)            (void);
};

/**
  * @brief  Definition of LED_Color_Mode class
 **/
struct LED_Color_Mode {
  // VTable (virtual) functions
  void                  (*F3)               (struct LED_Color_Mode *unsafe_self);
  void                  (*F3Var)            (struct LED_Color_Mode *unsafe_self);
  void                  (*Update)           (struct LED_Color_Mode *unsafe_self);
  void                  (*Hello)            (struct LED_Color_Mode *unsafe_self);

  struct LED_Color_Mode_VTable VTable;
};

/**
  * @brief  Constructor of LED_Color_Mode class
 **/
void LED_Color_Mode_Init(struct LED_Color_Mode *self);



#endif

#ifdef __cplusplus
}
#endif
