/**
  ******************************************************************************
  * @file     fl_app_lights.h
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     23-December-2019
  * @brief    This file contains declarations for light patterns
  *           Designed to be used with the Fluorescence App
  ******************************************************************************
  * @tableofcontents Table of contents, enter to navigate:
  * SECTION_HSL
  * SECTION_COLOR_FADER
  * SECTION_LIGHT_PATTERN
  * SECTION_LIGHT_PATTERN_STATIC
  * SECTION_LIGHT_PATTERN_SPECTRUM
  * SECTION_LIGHT_PATTERN_RAINBOW
  * SECTION_LIGHT_PATTERN_CHASE
  * SECTION_LIGHT_PATTERN_TIME_CODE
  * SECTION_LIGHT_PATTERN_COP
  * SECTION_LIGHT_PATTERN_MOMENTSOFBLISS
  * SECTION_CONTAINER_LIGHT_PATTERN
  ******************************************************************************
 **/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _VFDCO_LIGHTS_H
#define _VFDCO_LIGHTS_H

#include "fl_app_time.h"
#include "fl_app_colors.h"
#include <stdint.h>

/** Begin of:
  * @tableofcontents SECTION_SUPPORTING_FUNCTIONS
  * @brief Prototypes. Implementation see very end
 **/
// HSL to RGB fast calculation
uint32_t _led_color_hsl2rgb(uint8_t h, uint8_t s, uint8_t l);
// Generate random number
uint8_t led_color_simple_randomizer(uint8_t bits);


/** Begin of:
  * @tableofcontents SECTION_HSL
  * @details
  *         Definition of the HSL class. An HSL color is fully defined by:
  *         H: Hue, circular color value between 0 (red) ... 85 (green) ... finally 255 (red again)
  *         S: Saturation, between 0 (grayscale) ... 255 (fully saturated)
  *         L: Lightness, between 0 (essentially black/off), 127 (full), 255 (essentially white)
 **/
/**
  * @brief  Definition of HSL class as hsl_t
 **/
typedef struct hsl_t { uint8_t h, s, l; } hsl_t; // Hue Saturation Lightness

/**
 * @brief Initializes an hsl_t item
 * @param h Hue: uint8_t
 * @param s Saturation: uint8_t
 * @param l Lightness: uint8_t
 * @return hsl_t
 */
hsl_t HSL_Init( uint8_t h, uint8_t s, uint8_t l);

/** Begin of:
 * @tableofcontents SECTION_COLOR_FADER
 * @brief The classes Spectrum, Rainbow and Bliss use the Color Fader
**/
typedef enum {
  FADER_STATE_ACTIVE            =   1,  // Stationary state: Regular operation
  FADER_STATE_CYCLIC_RECOVERY   =   0,  // Special case: Cyclic rollover
} LED_COLOR_STATE_t;

struct LED_Color_Fader {
  LED_COLOR_STATE_t state;            // Color Fader FSM
  time_event_t      timer;
  hsl_t             color_1, color_2;
  int8_t            chain_hue_diff;   // Constant difference of hue between chained pixels
  uint8_t           fade_pos;         // Fade position (substate)
};
/**
 * @brief Constructor of LED_Color_Fader class
 */
void LED_Color_Fader_Init(
  struct LED_Color_Fader    *instance,      // Instance
  uint_fast32_t             timer_interval, // Timer interval
  hsl_t                     color_1,        // Color 1
  hsl_t                     color_2,        // Color 2. Set all components to 0 for single color fade
  int8_t                    chain_hue_diff  // Hue difference between chained pixels
);
extern LED_COLOR_STATE_t (*LED_Color_Fader_Next)(struct LED_Color_Fader *self);

/** Begin of:
 * @tableofcontents SECTION_LIGHT_PATTERN
 * @details Light_Pattern is the virtual base class. Its children perform operations
 * to LED lights to generate patterns. Each child class must overwrite the following virtual methods:
 * - F3: This method is triggered upon an HID event, typically sets a configuration. Can be set to NULL
 * - F3Var: This method is triggered upon an HID event, typically sets a configuration. Can be set to NULL
 * - Update: This method is called periodically in object lifetime. Typically used to update FSMs and render. Must not be NULL
 * - Hello: This method is called upon initialization and typically displays a message. Can be set to NULL
 * - Save: This method is called before destruction and saves variables onto the same memory block used in the initializer
**/
typedef union Light_Pattern Light_Pattern;
extern void (*Light_Pattern_Update) (Light_Pattern *unsafe_self);

/** Begin of:
  * @tableofcontents SECTION_LIGHT_PATTERN_STATIC
 **/
/**
  * @brief  Definition of Light_Pattern_Static class
 **/
struct Light_Pattern_Static {
  uint8_t               *settings;
  time_event_t          t;
  uint8_t               position;   // Color lookup array index
  uint8_t               target_arr[CONFIG_NUM_BYTES];
};

/**
  * @brief  Constructor of Light_Pattern_Static class
 **/
void Light_Pattern_Static_Init(struct Light_Pattern_Static *self, uint8_t *settings);

struct Light_Pattern_Serial0 {
  uint8_t               *settings;
  time_event_t          t;
};
void Light_Pattern_Serial0_Init(struct Light_Pattern_Serial0 *self, uint8_t *settings);

struct Light_Pattern_Serial1 {
  uint8_t               *settings;
  time_event_t          t;
  uint8_t               target_arr[CONFIG_NUM_BYTES];
};
void Light_Pattern_Serial1_Init(struct Light_Pattern_Serial1 *self, uint8_t *settings);


/** Begin of:
  * @tableofcontents SECTION_LIGHT_PATTERN_SPECTRUM
 **/
/**
  * @brief  Definition of Light_Pattern_Spectrum class
 **/
struct Light_Pattern_Spectrum {
  uint8_t                *settings;
  struct LED_Color_Fader spectrum_fader;
};

/**
  * @brief  Constructor of Light_Pattern_Spectrum class
 **/
void Light_Pattern_Spectrum_Init(struct Light_Pattern_Spectrum *self, uint8_t *settings);


/** Begin of:
  * @tableofcontents SECTION_LIGHT_PATTERN_RAINBOW
 **/
/**
  * @brief  Definition of Light_Pattern_Rainbow class
 **/
struct Light_Pattern_Rainbow {
  uint8_t                *settings;
  struct LED_Color_Fader rainbow_fader;
};

/**
  * @brief  Constructor of Light_Pattern_Rainbow class
 **/
void Light_Pattern_Rainbow_Init(struct Light_Pattern_Rainbow *self, uint8_t *settings);


/** Begin of:
  * @tableofcontents SECTION_LIGHT_PATTERN_CHASE
 **/
/**
  * @brief  Definition of Light_Pattern_Chase class
 **/
struct Light_Pattern_Chase {
  uint8_t         *settings;
  uint8_t         chase_mode;
  uint8_t         color_pos;
  uint8_t         color_peak_diff;
  uint8_t         state;
  uint8_t         flip_timer_previous_second;
  vfdco_time_t    *flip_timer;
  time_event_t    update_timer;
};

/**
  * @brief  Constructor of Light_Pattern_Chase class
 **/
void Light_Pattern_Chase_Init(struct Light_Pattern_Chase *self, vfdco_time_t *time, uint8_t *settings);


/** Begin of:
  * @tableofcontents SECTION_LIGHT_PATTERN_TIME_CODE
 **/
/**
  * @brief  Definition of Light_Pattern_Time_Code class
 **/
struct Light_Pattern_Time_Code {
  time_event_t          clock;

  vfdco_time_t          *time;
  uint8_t               target_arr[CONFIG_NUM_BYTES];
};

/**
  * @brief  Constructor of Light_Pattern_Time_Code class
 **/
void Light_Pattern_Time_Code_Init(struct Light_Pattern_Time_Code *self, vfdco_time_t *time_instance);


/** Begin of:
  * @tableofcontents SECTION_LIGHT_PATTERN_COP
**/
/**
  * @brief  Definition of Light_Pattern_Cop class
**/
struct Light_Pattern_Cop {
  time_event_t          clock;
  uint8_t               state;
};

/**
  * @brief  Constructor of Light_Pattern_Cop class
**/
void Light_Pattern_Cop_Init(struct Light_Pattern_Cop *self);


/** Begin of:
  * @tableofcontents SECTION_LIGHT_PATTERN_MOMENTSOFBLISS
**/
/**
  * @brief  Definition of Light_Pattern_MomentsOfBliss class
**/
struct Light_Pattern_MomentsOfBliss {
  uint8_t               *settings;
  uint_fast8_t          moment;
  uint_fast8_t          undrift_counter;
  uint_fast8_t          undrift_max;
  int_fast8_t           undrift_huediff_max;

  struct LED_Color_Fader base_fader;
};

/**
  * @brief  Constructor of Light_Pattern_MomentsOfBliss class
**/
void Light_Pattern_MomentsOfBliss_Init(struct Light_Pattern_MomentsOfBliss *self, uint8_t *settings);

/** Begin of:
  * @tableofcontents SECTION_CONTAINER_LIGHT_PATTERN
  * @details The Light Pattern container. C Unions are awesome, the German Union party not so.
  * The container is a single element memory pool consisting of a base member and all used childs
  * with the size of the largest child member. By type-punning them into child types,
  * static polymorphism is achieved. Use the Clear() function to zero out the Light Pattern Container
**/
typedef union Light_Pattern {
  struct Light_Pattern_Static           _lp_static;
  struct Light_Pattern_Spectrum         _lp_spectrum;
  struct Light_Pattern_Rainbow          _lp_rainbow;
  struct Light_Pattern_Chase            _lp_chase;
  struct Light_Pattern_Time_Code        _lp_timecode;
  struct Light_Pattern_Cop              _lp_cop;
  struct Light_Pattern_MomentsOfBliss   _lp_bliss;
  struct Light_Pattern_Serial0          _lp_ser0;
  struct Light_Pattern_Serial1          _lp_ser1;
} Light_Pattern;

/**
  * @brief Zero out the Light Pattern Container
**/
void Container_Light_Pattern_Clear(Light_Pattern *self);

#endif

#ifdef __cplusplus
}
#endif



// Go Vegan
