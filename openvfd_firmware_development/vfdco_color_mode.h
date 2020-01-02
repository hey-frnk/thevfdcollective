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

#ifndef _VFDCO_COLOR_MODE_H
#define _VFDCO_COLOR_MODE_H

#include "vfdco_color_lib.h"
#include <stdint.h>

#define   NUM_STATIC_COLOR_SPECIAL    2
#define   NUM_STATIC_COLOR_HUES       8
#define   NUM_STATIC_COLOR_RAINBOWS   2
#define   NUM_STATIC_COLOR_PRESETS    3

#define   NUM_STATIC_T1               (NUM_STATIC_COLOR_SPECIAL)
#define   NUM_STATIC_T2               (NUM_STATIC_T1 + NUM_STATIC_COLOR_HUES)
#define   NUM_STATIC_T3               (NUM_STATIC_T2 + NUM_STATIC_COLOR_RAINBOWS)
#define   NUM_STATIC_T4               (NUM_STATIC_T3 + NUM_STATIC_COLOR_PRESETS)

#define   SINGLE_COLOR_FADE_SPEED    10

static const hsl_t Static_Colors_Special[NUM_STATIC_COLOR_SPECIAL] = {
  {.h =   0,   .s =   0,   .l =   0}, // Off (Black)
  {.h =   0,   .s =   0,   .l = 255}  // White
};

static const uint8_t Static_Color_Hues[NUM_STATIC_COLOR_HUES] = {
  // As always the sat is 255, lightness 127, only memorize hues
    0, // Red      , also Red --> Blue
   85, // Green    , also Red --> Green
  170, // Blue,    , also Blue --> Green
   43, // Yellow
   14, // Orange
  128, // Cyan
  212, // Magenta
  191  // Purple
};

static const int8_t Static_Color_Presets[NUM_STATIC_COLOR_PRESETS] = {
   -17, // Difference to get from green to blue with num_rgb == 6
    13, // Difference to get from red to green
    21  // Difference to get from green to blue
};

static const uint8_t Static_Color_Rainbow_Lightness[NUM_STATIC_COLOR_RAINBOWS] = {
  127,
  216
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



/** Begin of:
  * @toc SECTION_LED_COLOR_STATIC
 **/
/**
  * @brief  Definition of LED_Color_Static class
 **/
struct LED_Color_Static {
  struct LED_Color_Mode super;

  uint8_t               position;   // Color lookup array index
  hsl_t                 **color_instance;
  struct LED_Color_Fader *fader_instance;
};

/**
  * @brief  Constructor of LED_Color_Static class
 **/
void LED_Color_Static_Init(struct LED_Color_Static *self);



#endif
