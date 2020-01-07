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

#define   SINGLE_COLOR_FADE_SPEED     2

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
  vfdco_hid_action_status_t                  (*F3)               (struct Light_Pattern *unsafe_self);
  vfdco_hid_action_status_t                  (*F3Var)            (struct Light_Pattern *unsafe_self);
  void                  (*Update)           (struct Light_Pattern *unsafe_self);
  void                  (*Hello)            (struct Light_Pattern *unsafe_self);
  void                  (*Delete)           (struct Light_Pattern *unsafe_self);

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


#endif



// Go Vegan
