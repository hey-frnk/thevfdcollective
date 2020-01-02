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
#include "../vfdco_color_mode.h"
#include <stdlib.h>
#ifdef DEBUG
#include <stdio.h>
#endif

/**
  * @brief  Implementation of virtual functions LED_Color_Mode::VTable (static void _LED_Color_Mode_F3)
**/
static inline void _LED_Color_Mode_F3(struct LED_Color_Mode *unsafe_self) {
 if(!unsafe_self->VTable.F3) return;
 unsafe_self->VTable.F3(unsafe_self);
}
static inline void _LED_Color_Mode_F3Var(struct LED_Color_Mode *unsafe_self) {
 if(!unsafe_self->VTable.F3Var) return;
 unsafe_self->VTable.F3Var(unsafe_self);
}
static inline void _LED_Color_Mode_Update(struct LED_Color_Mode *unsafe_self) {
 // if(!self->VTable.Update) return; Will make sure this never happens. Optimize for loop performance
 unsafe_self->VTable.Update(unsafe_self);
}
static inline void _LED_Color_Mode_Hello(struct LED_Color_Mode *unsafe_self) {
 if(!unsafe_self->VTable.Hello) return;
 unsafe_self->VTable.Hello();
}

/**
  * @brief  Constructor of LED_Color_Mode class
**/
void LED_Color_Mode_Init(struct LED_Color_Mode *self) {
  self->F3 = _LED_Color_Mode_F3;
  self->F3Var = _LED_Color_Mode_F3Var;
  self->Update = _LED_Color_Mode_Update;
  self->Hello = _LED_Color_Mode_Hello;
}

/** Begin of:
  * @toc SECTION_LED_COLOR_STATIC
 **/
/**
  * @brief  Implementation of virtual function LED_Color_Static::Update (static void _LED_Color_Static_Update)
 **/
static void _LED_Color_Static_Update(struct LED_Color_Mode *unsafe_self) {
  struct LED_Color_Static *self = (struct LED_Color_Static *)unsafe_self;

  LED_COLOR_STATE_t prev_state = self->fader_instance->state;
  LED_COLOR_STATE_t curr_state = self->fader_instance->super.Next(&self->fader_instance->super);
  // On rising edge, set to regular active by fading between two same colors
  if((prev_state == LED_COLOR_STATE_FADE_IN || prev_state == LED_COLOR_STATE_CYCLIC_RECOVERY)
      && curr_state == LED_COLOR_STATE_ACTIVE) {
    self->color_instance[1]->h = self->color_instance[0]->h;
    self->color_instance[1]->s = self->color_instance[0]->s;
    self->color_instance[1]->l = self->color_instance[0]->l;
    int8_t bckup_colordiff = self->fader_instance->chain_huediff;
    self->fader_instance->super.Delete((struct LED_Color *)self->fader_instance);
    printf("c1hsl: %d %d %d, c2hsl: %d %d %d\n",  self->color_instance[0]->h, self->color_instance[0]->s, self->color_instance[0]->l, self->color_instance[1]->h, self->color_instance[1]->s, self->color_instance[1]->l);
    self->fader_instance = LED_Color_Fader_Init(
      20, LED_COLOR_BLEND_MODE_NORMAL, 0, LED_COLOR_REPEAT_FOREVER,
      2, self->color_instance, 6, bckup_colordiff
    );
    self->fader_instance->state = LED_COLOR_STATE_ACTIVE;
  }
}

/**
  * @brief  Implementation of virtual function LED_Color_Static::F3 (static void _LED_Color_Static_F3)
 **/
static void _LED_Color_Static_F3(struct LED_Color_Mode *unsafe_self) {
  struct LED_Color_Static *self = (struct LED_Color_Static *)unsafe_self;
  self->position++;
  printf("%d ", self->position);
  if(self->position >= NUM_STATIC_T4) self->position = 0;

  // Color transition by introducing unequal colors & putting into recovery
  int8_t hue_diff = 0;

  if(self->position < NUM_STATIC_T1) {
    // Single Color Special
    self->color_instance[0]->h = Static_Colors_Special[self->position].h;
    self->color_instance[0]->s = Static_Colors_Special[self->position].s;
    self->color_instance[0]->l = Static_Colors_Special[self->position].l;
  } else if(self->position < NUM_STATIC_T2) {
    // Single Color
    self->color_instance[0]->h = Static_Color_Hues[self->position - NUM_STATIC_T1];
    self->color_instance[0]->s = 255;
    self->color_instance[0]->l = 127;
  } else if (self->position < NUM_STATIC_T3) {
    // Rainbows
    uint8_t t_pos = self->position - NUM_STATIC_T2;
    self->color_instance[0]->h = Static_Color_Hues[0]; // Red to start
    self->color_instance[0]->s = Static_Color_Rainbow_Saturation[t_pos];
    self->color_instance[0]->l = Static_Color_Rainbow_Lightness[t_pos];
    hue_diff = 51; // Rainbow equidistance
  } else { // < T4
    // Multicolor
    uint8_t t_pos = self->position - NUM_STATIC_T3;
    self->color_instance[0]->h = Static_Color_Hues[t_pos];
    self->color_instance[0]->s = 255;
    self->color_instance[0]->l = 127;
    hue_diff = Static_Color_Presets[t_pos];
  }

  self->fader_instance->super.Delete((struct LED_Color *)self->fader_instance);
  self->fader_instance = LED_Color_Fader_Init(
    20, LED_COLOR_BLEND_MODE_NORMAL, 0, LED_COLOR_REPEAT_ONCE,
    2, self->color_instance, 6, hue_diff
  );
  self->fader_instance->state = LED_COLOR_STATE_CYCLIC_RECOVERY; // Put into recovery

  /*// Dynamic memory saving
  char LED0PMC[NUM_DIGITS_V];
  LED0PMC[0] = 'C';
  LED0PMC[1] = ' ';
  for(uint8_t i = 2; i < num_rgb; i++) LED0PMC[i] = pgm_read_byte_near(&LED0PM[self->position][i - 2]); // LED0PM[self->position][i - 2];

  displayWrite(3, 0x00, 500, LED0PMC);    // Write change message*/
}

/**
  * @brief  Implementation of virtual function LED_Color_Static::Hello (static void _LED_Color_Static_Hello)
 **/
static inline void _LED_Color_Static_Hello(void) {
  /*char k[NUM_DIGITS_V];
  for(uint8_t i = 0; i < NUM_DIGITS_V; i++) k[i] = pgm_read_byte_near(MSG_COLOR + i);
  displayWrite(3, 0x00, 1000, k);*/
}

/**
  * @brief  Constructor of LED_Color_Static class
 **/
void LED_Color_Static_Init(struct LED_Color_Static *self) {
  LED_Color_Mode_Init(&self->super);

  self->color_instance = malloc(2 * sizeof(hsl_t *));
  self->color_instance[0] = HSL_Init(Static_Colors_Special[0].h, Static_Colors_Special[0].s, Static_Colors_Special[0].l);
  self->color_instance[1] = HSL_Init(Static_Colors_Special[0].h, Static_Colors_Special[0].s, Static_Colors_Special[0].l);

  self->fader_instance = LED_Color_Fader_Init(20, LED_COLOR_BLEND_MODE_NORMAL, 0, LED_COLOR_REPEAT_ONCE, 2, (hsl_t **)self->color_instance, 6, 0);
  self->position = 0;

  struct LED_Color_Mode_VTable _static_vtable = {
    .F3 = _LED_Color_Static_F3,
    .F3Var = NULL,
    .Update = _LED_Color_Static_Update,
    .Hello = _LED_Color_Static_Hello
  };
  self->super.VTable = _static_vtable;
}
