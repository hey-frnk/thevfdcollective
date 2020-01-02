/**
  ******************************************************************************
  * @file     vfdco_color_mode.h
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     23-December-2019
  * @brief    This file contains implementations for color modes
  *           Designed to be used with Fluorescence by The VFD Collective
  ******************************************************************************
  * @toc      Table of contents, enter to navigate:
  ******************************************************************************
 **/

#include "../vfdco_color_lib.h"
#include "../vfdco_lights.h"
#include <stdlib.h>
#ifdef DEBUG
#include <stdio.h>
#endif

/**
  * @brief  Implementation of virtual functions Light_Pattern::VTable (static void _Light_Pattern_F3)
**/
static inline void _Light_Pattern_F3(struct Light_Pattern *unsafe_self) {
 if(!unsafe_self->VTable.F3) return;
 unsafe_self->VTable.F3(unsafe_self);
}
static inline void _Light_Pattern_F3Var(struct Light_Pattern *unsafe_self) {
 if(!unsafe_self->VTable.F3Var) return;
 unsafe_self->VTable.F3Var(unsafe_self);
}
static inline void _Light_Pattern_Update(struct Light_Pattern *unsafe_self) {
 // if(!self->VTable.Update) return; Will make sure this never happens. Optimize for loop performance
 unsafe_self->VTable.Update(unsafe_self);
}
static inline void _Light_Pattern_Hello(struct Light_Pattern *unsafe_self) {
 if(!unsafe_self->VTable.Hello) return;
 unsafe_self->VTable.Hello();
}

/**
  * @brief  Constructor of Light_Pattern class
**/
void Light_Pattern_Init(struct Light_Pattern *self) {
  self->F3 = _Light_Pattern_F3;
  self->F3Var = _Light_Pattern_F3Var;
  self->Update = _Light_Pattern_Update;
  self->Hello = _Light_Pattern_Hello;
}

/** Begin of:
  * @toc SECTION_LED_COLOR_STATIC
 **/
/**
  * @brief  Implementation of virtual function Light_Pattern_Static::Update (static void _Light_Pattern_Static_Update)
 **/
static void _Light_Pattern_Static_Update(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Static *self = (struct Light_Pattern_Static *)unsafe_self;

  hsl_t *tc = self->target_color;
  hsl_t *cc = self->current_color;
  if(Time_Event_Update(&self->t)) {
    uint8_t ndt = 0;
    if(       cc->h < tc->h) cc->h++;
    else if(  cc->h > tc->h) cc->h--;
    else ndt++;
    if(       cc->s < tc->s) cc->s++;
    else if(  cc->s > tc->s) cc->s--;
    else ndt++;
    if(       cc->l < tc->l) cc->l++;
    else if(  cc->l > tc->l) cc->l--;
    else ndt++;

    if(ndt != 3) { // If change has occured, write to LEDs
      for(uint8_t i = 0; i < num_rgb; ++i) {
        uint8_t i_h = cc->h;
        i_h += i * (int16_t)self->hue_diff;                             // i-th hue difference (delta), intended angle overflow
        uint32_t target_color = _led_color_hsl2rgb(i_h, cc->s, cc->l);  // Get target RGB
        vfdco_clr_set_RGB(i, (target_color >> 8) & 0xFF, (target_color >> 16) & 0xFF, target_color & 0xFF);
      }
      vfdco_clr_render();
    }
  }
}

/**
  * @brief  Implementation of virtual function Light_Pattern_Static::F3 (static void _Light_Pattern_Static_F3)
 **/
static void _Light_Pattern_Static_F3(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Static *self = (struct Light_Pattern_Static *)unsafe_self;
  self->position++;
  printf("%d ", self->position);
  if(self->position >= NUM_STATIC_T4) self->position = 0;

  // Color transition by introducing unequal colors & putting into recovery

  if(self->position < NUM_STATIC_T1) {
    // Single Color Special
    self->target_color->h = Static_Colors_Special[self->position].h;
    self->target_color->s = Static_Colors_Special[self->position].s;
    self->target_color->l = Static_Colors_Special[self->position].l;
    self->hue_diff = 0;
  } else if(self->position < NUM_STATIC_T2) {
    // Single Color
    self->target_color->h = Static_Color_Hues[self->position - NUM_STATIC_T1];
    self->target_color->s = 255;
    self->target_color->l = 127;
    self->hue_diff = 0;
  } else if (self->position < NUM_STATIC_T3) {
    // Rainbows
    uint8_t t_pos = self->position - NUM_STATIC_T2;
    self->target_color->h = Static_Color_Hues[0]; // Red to start
    self->target_color->s = Static_Color_Rainbow_Saturation[t_pos];
    self->target_color->l = Static_Color_Rainbow_Lightness[t_pos];
    self->hue_diff = 51; // Rainbow equidistance
  } else { // < T4
    // Multicolor
    uint8_t t_pos = self->position - NUM_STATIC_T3;
    self->target_color->h = Static_Color_Hues[t_pos];
    self->target_color->s = 255;
    self->target_color->l = 127;
    self->hue_diff = Static_Color_Presets[t_pos];
  }

  /*// Dynamic memory saving
  char LED0PMC[NUM_DIGITS_V];
  LED0PMC[0] = 'C';
  LED0PMC[1] = ' ';
  for(uint8_t i = 2; i < num_rgb; i++) LED0PMC[i] = pgm_read_byte_near(&LED0PM[self->position][i - 2]); // LED0PM[self->position][i - 2];

  displayWrite(3, 0x00, 500, LED0PMC);    // Write change message*/
}

/**
  * @brief  Implementation of virtual function Light_Pattern_Static::Hello (static void _Light_Pattern_Static_Hello)
 **/
static inline void _Light_Pattern_Static_Hello(void) {
  /*char k[NUM_DIGITS_V];
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_COLOR + i);
  displayWrite(3, 0x00, 1000, k);*/
}

/**
  * @brief  Constructor of Light_Pattern_Static class
 **/
void Light_Pattern_Static_Init(struct Light_Pattern_Static *self) {
  Light_Pattern_Init(&self->super);

  /*self->color_instance = malloc(2 * sizeof(hsl_t *));
  self->color_instance[0] = HSL_Init(Static_Colors_Special[0].h, Static_Colors_Special[0].s, Static_Colors_Special[0].l);
  self->color_instance[1] = HSL_Init(Static_Colors_Special[0].h, Static_Colors_Special[0].s, Static_Colors_Special[0].l);

  self->fader_instance = LED_Color_Fader_Init(20, LED_COLOR_BLEND_MODE_NORMAL, 0, LED_COLOR_REPEAT_ONCE, 2, (hsl_t **)self->color_instance, 6, 0);*/

  self->t = Time_Event_Init(SINGLE_COLOR_FADE_SPEED);

  self->current_color = (hsl_t *)calloc(1, sizeof(hsl_t));
  self->target_color = HSL_Init(Static_Colors_Special[0].h, Static_Colors_Special[0].s, Static_Colors_Special[0].l);

  self->position = 0;
  self->hue_diff = 0;

  struct Light_Pattern_VTable _static_vtable = {
    .F3 = _Light_Pattern_Static_F3,
    .F3Var = NULL,
    .Update = _Light_Pattern_Static_Update,
    .Hello = _Light_Pattern_Static_Hello
  };
  self->super.VTable = _static_vtable;
}
