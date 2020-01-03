/**
  ******************************************************************************
  * @file     vfdco_color_mode.h
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     23-December-2019
  * @brief    This file contains declarations for color modes
  *           Designed to be used with Fluorescence by The VFD Collective
  ******************************************************************************
  * @toc      Table of contents, enter to navigate:
  ******************************************************************************
 **/

#ifndef _VFDCO_LIGHTS_H
#define _VFDCO_LIGHTS_H

#include "vfdco_color_lib.h"
#include "vfdco_time.h"
#include <stdint.h>

#define   NUM_STATIC_COLOR_SPECIAL    3
#define   NUM_STATIC_COLOR_HUES       8
#define   NUM_STATIC_COLOR_RAINBOWS   2
#define   NUM_STATIC_COLOR_PRESETS    3

#define   NUM_STATIC_T1               (NUM_STATIC_COLOR_SPECIAL)
#define   NUM_STATIC_T2               (NUM_STATIC_T1 + NUM_STATIC_COLOR_HUES)
#define   NUM_STATIC_T3               (NUM_STATIC_T2 + NUM_STATIC_COLOR_RAINBOWS)
#define   NUM_STATIC_T4               (NUM_STATIC_T3 + NUM_STATIC_COLOR_PRESETS)

#define   SINGLE_COLOR_FADE_SPEED     2

/*static const hsl_t Static_Colors_Special[NUM_STATIC_COLOR_SPECIAL] = {
  {.h =   0,   .s =   0,   .l =   0}, // Off (Black)
  {.h =   0,   .s =   0,   .l = 255}  // White
};*/

static const uint8_t Static_Color_Hues[NUM_STATIC_COLOR_HUES] = {
  // As always the sat is 255, lightness 127, only memorize hues
    0, // Red      , also Red --> Blue
   85, // Green    , also Red --> Green
  160, // Blue,    , also Blue --> Green
   25, // Yellow
    5, // Orange
  128, // Cyan
  240, // Magenta
  205  // Purple
};

/* static const int8_t Static_Color_Presets[NUM_STATIC_COLOR_PRESETS] = {
   -17, // Difference to get from green to blue with num_rgb == 6
   -17, // Difference to get from red to green
   -17  // Difference to get from green to blue
}; */

static const uint8_t Static_Color_Rainbow_Lightness[NUM_STATIC_COLOR_RAINBOWS] = {
  127,
  170
};
static const uint8_t Static_Color_Rainbow_Saturation[NUM_STATIC_COLOR_RAINBOWS] = {
  255,
  196
};




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
  void                  (*F3)               (struct Light_Pattern *unsafe_self);
  void                  (*F3Var)            (struct Light_Pattern *unsafe_self);
  void                  (*Update)           (struct Light_Pattern *unsafe_self);
  void                  (*Hello)            (struct Light_Pattern *unsafe_self);

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
  uint8_t               *target_arr;
};

/**
  * @brief  Constructor of LED_Color_Static class
 **/
void Light_Pattern_Static_Init(struct Light_Pattern_Static *self);



#endif
