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

static inline void _target_RGB(uint8_t *tp, uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
	tp[4 * index] = g;
	tp[4 * index + 1] = r;
	tp[4 * index + 2] = b;
	tp[4 * index + 3] = 0;
}
static inline void _target_RGBW(uint8_t *tp, uint8_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
	_target_RGB(tp, index, r, g, b);
	tp[4 * index + 3] = w;
}
static inline void _target_all_RGB(uint8_t *tp, uint8_t r, uint8_t g, uint8_t b) {
	for(uint8_t i = 0; i < num_rgb; ++i) _target_RGB(tp, i, r, g, b);
}
static inline void _target_all_RGBW(uint8_t *tp, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
	for(uint8_t i = 0; i < num_rgb; ++i) _target_RGBW(tp, i, r, g, b, w);
}

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
  if(Time_Event_Update(&self->t)) {
    uint8_t dt = 0;
    for(uint8_t i = 0; i < num_bytes; i++) { // Sorry for the ll-access
      if(rgb_arr[i] < self->target_arr[i]) rgb_arr[i]++;
      else if(rgb_arr[i] > self->target_arr[i]) rgb_arr[i]--;
      else ++dt;
    }
    if(dt != num_bytes) vfdco_clr_render();
  }
}

/**
  * @brief  Implementation of virtual function Light_Pattern_Static::F3 (static void _Light_Pattern_Static_F3)
 **/
static void _Light_Pattern_Static_F3(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Static *self = (struct Light_Pattern_Static *)unsafe_self;

  self->position++;
  if(self->position >= NUM_STATIC_T4) self->position = 0;

	#ifdef DEBUG
  printf("%d ", self->position);
	#endif

  if(self->position < NUM_STATIC_T1) {
    // Single Color Special
    switch(self->position) {
      case 0: _target_all_RGB   (self->target_arr,   0,   0,   0     ); break;
      case 1: _target_all_RGBW  (self->target_arr,   0,   0,  96, 255); break;
      case 2: _target_all_RGBW  (self->target_arr,   0,   0,   0, 255); break;
    }
  } else if(self->position < NUM_STATIC_T2) {
    // Single Color
    uint32_t target_color = _led_color_hsl2rgb(Static_Color_Hues[self->position - NUM_STATIC_T1], 255, 127);
    _target_all_RGB(self->target_arr, (target_color >> 8) & 0xFF, (target_color >> 16) & 0xFF, target_color & 0xFF);

  } else if (self->position < NUM_STATIC_T3) {
    // Rainbows
    uint8_t t_pos = self->position - NUM_STATIC_T2;
    for(uint8_t i = 0; i < num_rgb; ++i) {
      uint8_t i_h = Static_Color_Hues[0] + 2;
      i_h += i * 40; // i-th hue difference (delta), intended angle overflow
      uint32_t target_color = _led_color_hsl2rgb(i_h, Static_Color_Rainbow_Saturation[t_pos], Static_Color_Rainbow_Lightness[t_pos]);
      _target_RGB(self->target_arr, i, (target_color >> 8) & 0xFF, (target_color >> 16) & 0xFF, target_color & 0xFF);
    }

  } else { // < T4
    // Multicolor
    uint8_t t_pos = self->position - NUM_STATIC_T3;

    for(uint8_t i = 0; i < num_rgb; ++i) {
      uint8_t i_h = Static_Color_Hues[t_pos] + 2;
      i_h -= i * 15; // i-th hue difference (delta), intended angle overflow
      uint32_t target_color = _led_color_hsl2rgb(i_h, 255, 127);  // Get target RGB
      _target_RGB(self->target_arr, i, (target_color >> 8) & 0xFF, (target_color >> 16) & 0xFF, target_color & 0xFF);
    }
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

  self->target_arr = (uint8_t *)calloc(num_bytes, sizeof(uint8_t));
  self->position = 0;

  struct Light_Pattern_VTable _static_vtable = {
    .F3 = _Light_Pattern_Static_F3,
    .F3Var = NULL,
    .Update = _Light_Pattern_Static_Update,
    .Hello = _Light_Pattern_Static_Hello
  };
  self->super.VTable = _static_vtable;
}
