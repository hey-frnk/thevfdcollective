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
#include "vfdco_hid.h"
#include <stdint.h>

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
  void                  (*Delete)           (struct Light_Pattern *unsafe_self);
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
  void                        (*Delete)           (struct Light_Pattern *unsafe_self);

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


/** Begin of:
  * @toc
 **/
/**
  * @brief  Definition of Light_Pattern_Spectrum class
 **/
struct Light_Pattern_Spectrum {
  struct Light_Pattern super;

  hsl_t                **color;
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

  hsl_t                **color;
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

  hsl_t                *color;
  hsl_d_t              diff_color;
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
  uint8_t               *target_arr;
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

  hsl_t                 **colors;
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


#endif



// Go Vegan
