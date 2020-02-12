/**
  ******************************************************************************
  * @file     vfdco_lights.h
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     23-December-2019
  * @brief    This file contains declarations for light patterns
  *           Designed to be used with Fluorescence by The VFD Collective
  ******************************************************************************
  * @toc      Table of contents, enter to navigate:
  ******************************************************************************
 **/

#ifndef _VFDCO_LIGHTS_H
#define _VFDCO_LIGHTS_H

#include "vfdco_time.h"
#include "vfdco_hid.h"
#include <stdint.h>

/** Begin of:
  * @toc SECTION_SUPPORTING_FUNCTIONS
 **/
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
typedef struct rgb_t {uint8_t r, g, b; } rgb_t; // Red Green Blue

/**
 * @brief  Declaration of constructor HSL class, HSL::HSL(h, s, l)
**/
hsl_t HSL_Init( uint8_t h, // Hue value
                uint8_t s, // Saturation value
                uint8_t l  // Lightness value
);

uint8_t led_color_simple_randomizer(uint8_t bits);
uint32_t _led_color_hsl2rgb(uint8_t h, uint8_t s, uint8_t l);

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

enum {
  LED_COLOR_REPEAT_RUN_ONCE         =   0,  // Only one run
  LED_COLOR_REPEAT_ONCE             =   1,  // Repeat once
  LED_COLOR_REPEAT_TWICE            =   2,  // Repeat twice (lol)
  LED_COLOR_REPEAT_FOREVER          = -42,  // Repeat forever
};

/** Begin of:
 * @toc SUBSECTION_COLOR_FADER
**/
#define     LED_COLOR_FADER_TIME_BITS       8
#define     LED_COLOR_FADER_PERIOD        255

struct LED_Color_Fader {
  // Functions
  time_event_t  timer;

  hsl_t         pk_1;
  hsl_t         pk_2;
  // Option: Chaining
  int8_t        chain_huediff;  // Constant difference of hue between chained pixels
  // Option: Time
  int8_t        repeat;         // If == k: pk[n] -> pk[0] for k cycles, else fade in & out once
  // Option: StartPos

  // State Variables
  LED_COLOR_STATE_t state;      // Color Fader FSM
  uint8_t      fade_pos;        // Fade position (substate)
};

/**
  * @brief  Constructor of LED_Color_Fader class
 **/
void LED_Color_Fader_Init(
  struct LED_Color_Fader    *instance,              // Instance
  uint_fast32_t             timer1_interval,        // Timer interval
  int8_t                    repeat,                 // Fade N cycles
  hsl_t                     pk_1,
  hsl_t                     pk_2,
  int8_t                    chain_hue_diff          // Hue difference between chained pixels
);
LED_COLOR_STATE_t (*LED_Color_Fader_Next)(struct LED_Color_Fader *self);

enum {
  LED_COLOR_CHASER_MODE_LR_LINEAR         = 0,       // Chase in a linear way (from left to right)
  LED_COLOR_CHASER_MODE_LR_ACCELERATING   = 1,       // Become faster with every chasing step (exponential)
  LED_COLOR_CHASER_MODE_LR_DECELERATING   = 2,       // Become slower with every chasing step (exponential)

  LED_COLOR_CHASER_MODE_RL_LINEAR         = 8,       // Chase in a linear way (from right to left)
  LED_COLOR_CHASER_MODE_RL_ACCELERATING   = 9,       // Become faster with every chasing step (exponential)
  LED_COLOR_CHASER_MODE_RL_DECELERATING   = 10,      // Become slower with every chasing step (exponential)

  LED_COLOR_CHASER_MODE_SPLITLIN          = 4,       // Split in the middle and chase in both directions
  LED_COLOR_CHASER_MODE_SPLITACC          = 5,       // -||- accelerating
  LED_COLOR_CHASER_MODE_SPLITDEC          = 6        // -||- decelerating
}; // Trust me this sort of encoding actually makes sense

struct LED_Color_Chaser {
  // Functions
  time_event_t  timer;

  hsl_t          pk;                  // Fade to peak
  hsl_d_t        pk_diff;             // Difference of each new peak to initial peak (factorized)

  uint8_t        chase_mode;          // Chase mode
  int8_t         chase_repeat;        // Repeat

  uint16_t       chase_duration;      // (Min (acc), Max (dec)) time diff between LED chase
  uint8_t        _chase_duration_restore; // Backup variable

  // Option: Start & Length
  uint8_t        start_pos;           // LED start index < end index

  // State Variables
  LED_COLOR_STATE_t state;            // Color Chaser FSM
  uint8_t        pk_state;            // Peak state FSM
  uint16_t       chase_pos;           // Fade position (substate)
};
/**
  * @brief  Constructor of LED_Color_Chaser class
 **/
void LED_Color_Chaser_Init(
  struct LED_Color_Chaser   *instance,              // Instance
  uint_fast32_t             timer1_interval,        // Timer interval
  uint8_t                   start_pos,              // Pixel index to start
  int8_t                    repeat,                 // Repeat N times
  hsl_t                     pk,                     // Array of HSL colors
  hsl_d_t                   pk_diff,                // Difference peaks
  uint16_t                  duration,               // Chase duration
  uint8_t                   mode                    // Chase mode
);
LED_COLOR_STATE_t LED_Color_Chaser_Next(struct LED_Color_Chaser *self);





#define   NUM_STATIC_COLOR_SPECIAL    3
#define   NUM_STATIC_COLOR_HUES       8
#define   NUM_STATIC_COLOR_RAINBOWS   2
#define   NUM_STATIC_COLOR_PRESETS    3

#define   NUM_STATIC_T1               (NUM_STATIC_COLOR_SPECIAL)
#define   NUM_STATIC_T2               (NUM_STATIC_T1 + NUM_STATIC_COLOR_HUES)
#define   NUM_STATIC_T3               (NUM_STATIC_T2 + NUM_STATIC_COLOR_RAINBOWS)
#define   NUM_STATIC_T4               (NUM_STATIC_T3 + NUM_STATIC_COLOR_PRESETS)

/** Begin of:
 * @toc SECTION_LED_COLOR_MODE
**/
/**
  * @brief  Virtual table for LED_Color_Mode
**/
struct Light_Pattern;
struct Light_Pattern_VTable {
  void                  (*F3)               (struct Light_Pattern *unsafe_self);
  void                  (*F3Var)            (struct Light_Pattern *unsafe_self);
  void                  (*Update)           (struct Light_Pattern *unsafe_self);
  void                  (*Hello)            (void);
};

/**
  * @brief  Definition of LED_Color_Mode class
**/
struct Light_Pattern {
  // VTable (virtual) functions
  vfdco_hid_action_status_t   (*F3)               (struct Light_Pattern *unsafe_self);
  vfdco_hid_action_status_t   (*F3Var)            (struct Light_Pattern *unsafe_self);
  void                        (*Update)           (struct Light_Pattern *unsafe_self);
  void                        (*Hello)            (struct Light_Pattern *unsafe_self);

  struct Light_Pattern_VTable VTable;
};
/**
  * @brief  Constructor of LED_Color_Mode class
**/
void Light_Pattern_Init(struct Light_Pattern *self);





/** Begin of:
  * @toc SECTION_LED_COLOR_STATIC
 **/
/**
  * @brief  Definition of LED_Color_Static class
 **/
struct Light_Pattern_Static {
  struct Light_Pattern super;
  time_event_t          t;

  uint8_t               position;   // Color lookup array index
  uint8_t               target_arr[CONFIG_NUM_BYTES];
};

/**
  * @brief  Constructor of LED_Color_Static class
 **/
void Light_Pattern_Static_Init(struct Light_Pattern_Static *self);


/** Begin of:
  * @toc
 **/
/**
  * @brief  Definition of Light_Pattern_Spectrum class
 **/
struct Light_Pattern_Spectrum {
  struct Light_Pattern super;
  struct LED_Color_Fader spectrum_fader;
};

/**
  * @brief  Constructor of Light_Pattern_Spectrum class
 **/
void Light_Pattern_Spectrum_Init(struct Light_Pattern_Spectrum *self);


/** Begin of:
  * @toc
 **/
/**
  * @brief  Definition of Light_Pattern_Rainbow class
 **/
struct Light_Pattern_Rainbow {
  struct Light_Pattern super;
  struct LED_Color_Fader rainbow_fader;
};

/**
  * @brief  Constructor of Light_Pattern_Rainbow class
 **/
void Light_Pattern_Rainbow_Init(struct Light_Pattern_Rainbow *self);


/** Begin of:
  * @toc
 **/
/**
  * @brief  Definition of Light_Pattern_Chase class
 **/
struct Light_Pattern_Chase {
  struct Light_Pattern super;

  uint_fast8_t         chase_mode;
  uint8_t              flip_timer_previous_second;
  vfdco_time_t         *flip_timer;

  struct LED_Color_Chaser chase_fader;
};

/**
  * @brief  Constructor of Light_Pattern_Chase class
 **/
void Light_Pattern_Chase_Init(struct Light_Pattern_Chase *self, vfdco_time_t *time, uint_fast8_t chase_mode);


/** Begin of:
  * @toc SECTION_LED_COLOR_TIME_CODE
 **/
/**
  * @brief  Definition of LED_Color_Time_Code class
 **/
struct Light_Pattern_Time_Code {
  struct Light_Pattern super;
  time_event_t          clock;

  vfdco_time_t          *time;
  uint8_t               target_arr[CONFIG_NUM_BYTES];
};

/**
  * @brief  Constructor of LED_Color_Time_Code class
 **/
void Light_Pattern_Time_Code_Init(struct Light_Pattern_Time_Code *self, vfdco_time_t *time_instance);


/** Begin of:
  * @toc SECTION_LIGHT_PATTERN_COP
**/
/**
  * @brief  Definition of Light_Pattern_Cop class
**/
struct Light_Pattern_Cop {
  struct Light_Pattern super;

  time_event_t          clock;
  uint8_t               state;
};

/**
  * @brief  Constructor of Light_Pattern_Cop class
**/
void Light_Pattern_Cop_Init(struct Light_Pattern_Cop *self);


/** Begin of:
  * @toc SECTION_LIGHT_PATTERN_MOMENTSOFBLISS
**/
/**
  * @brief  Definition of Light_Pattern_MomentsOfBliss class
**/
struct Light_Pattern_MomentsOfBliss {
  struct Light_Pattern super;

  uint_fast8_t          moment;
  uint_fast8_t          undrift_counter;
  uint_fast8_t          undrift_max;
  int_fast8_t           undrift_huediff_max;

  struct LED_Color_Fader base_fader;
};

/**
  * @brief  Constructor of Light_Pattern_MomentsOfBliss class
**/
void Light_Pattern_MomentsOfBliss_Init(struct Light_Pattern_MomentsOfBliss *self, uint_fast8_t moment);


/** Begin of:
  * @toc SECTION_CONTAINER_LIGHT_PATTERN
  * Unions are awesome
**/
/**
  * @brief  Definition of The Light Pattern container
**/
typedef union Container_Light_Pattern_t {
  struct Light_Pattern                  base;
  struct Light_Pattern_Static           _lp_static;
  struct Light_Pattern_Spectrum         _lp_spectrum;
  struct Light_Pattern_Rainbow          _lp_rainbow;
  struct Light_Pattern_Chase            _lp_chase;
  struct Light_Pattern_Time_Code        _lp_timecode;
  struct Light_Pattern_Cop              _lp_cop;
  struct Light_Pattern_MomentsOfBliss   _lp_bliss;
} Container_Light_Pattern_t;

/**
  * @brief  Definition of Container_Light_Pattern_Clear to set all components to zero
**/
void Container_Light_Pattern_Clear(Container_Light_Pattern_t *self);

#endif



// Go Vegan
