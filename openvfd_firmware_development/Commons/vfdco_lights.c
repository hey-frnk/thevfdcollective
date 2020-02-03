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

#include "../vfdco_config.h"
#include "../vfdco_color_lib.h"
#include "../vfdco_lights.h"
#include "../vfdco_display.h"
#include <stdlib.h>
#include <string.h>

enum {LIGHTNESS_H  = CONFIG_LIGHTNESS_HIGH,   LIGHTNESS_M  = CONFIG_LIGHTNESS_MEDIUM,   LIGHTNESS_L  = CONFIG_LIGHTNESS_LOW    };
enum {SATURATION_H = CONFIG_SATURATION_HIGH,   SATURATION_M = CONFIG_SATURATION_MEDIUM,  SATURATION_L = CONFIG_SATURATION_LOW   };

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

// Caution: G-R-B ordering (due to legacy copy & paste, sorry)
static const uint8_t Static_Color_Rainbows[][6 * 3] =
{
  {  0, 200, 255,  // Rainbow colors!
     0,   0, 255,
   255,   0,   0,
   128, 255,   0,
    30, 255,   0,
     0, 255,   0
  },
  {128, 255, 255,  // Pastel rainbow!
   128,  50, 255,
   255,   0, 128,
   255, 128, 128,
   255, 255, 128,
   100, 255, 128
  },
  {255,   0,   0,  // Green to blue!
   240,   0,  64,
   216,   0, 128,
   128,   0, 216,
    64,   0, 240,
     0,   0, 255
  },
  {  0, 255,   3,  // Red to blue!
     0, 255,  10,
     0, 240,  25,
     0, 200,  80,
     0, 100, 150,
     0,  50, 255
  },
  {  3, 255,   0,  // Red to green!
    30, 255,   0,
    60, 240,   0,
   100, 180,   0,
   180, 180,   0,
   255,  20,   0
 }
};

// ---- LED Resistor preset GRB     0: Off     1: Brown        2: Red       3: Orange     4: Yellow      5: Green     6: Blue      7: Purple      8: Gray       9: White
static const uint8_t Time_Code_Colors[][3] =  {{0, 0, 0}, {128, 255, 64}, {0, 255, 0}, {30, 255, 0}, {125, 255, 0}, {255, 0, 0}, {0, 0, 255}, {0, 200, 255}, {40, 40, 60}, {255, 255, 255}};

#define LIGHTS_BLISS_MAXMOMENTS 6

static const uint8_t MomentsOfBliss_Colors[LIGHTS_BLISS_MAXMOMENTS][7] = { // (C) The VFD Collective
  // Dev: SECTION_LIGHT_PATTERN_MOMENTSOFBLISS
  // D+: Hue difference between LEDs. Random number between 0..2^(D+), 4x MSB
  // T: Peak differene tolerance. Random bumber between 0..2^(T), 4x LSB
// C1H, C1S, C1L, C2H, C2S, C2L,  D+|T
  {105, 245, 100, 190, 245, 110, 0x42}, // Nordlicht.          Ink: fluorescent green to teal, touches of purple
  // {140, 150, 127, 150, 180, 127, 0x42}, // Schneesturm.        Ink: acrylic. dark midnight blue, shades of gray and cold white
  {120, 255, 220, 230, 255, 200, 0x53}, // Frühlingspastell.   Ink: pastel. cherry, pink, some light green and rarely drip of light blue
  { 55, 255, 127, 145, 255, 127, 0x42}, // Hummelhonig.        Ink: highly saturated green and blue gradients
  {140, 255, 127, 150, 255, 240, 0x52}, // Meeresgeflüster.    Ink: watercolor. light sky blue to turquoise, with white sparks and rarely some yellow
  {  0, 255, 120,  35, 255, 120, 0x32}, // Herbstlagerfeuer.   Ink: acrylic. lots of orange and strong yellow tones. rarely some green and brick red
  {235, 255, 127,  10, 255, 127, 0x42}, // Some sunset name.   Ink: strong red. every warm red tone, some orange, some magenta
};

// Messaging
static const char Messages_Color_Static[][4] = {
  {' ', 'O', 'F', 'F'}, // Off
  {' ', 'L', 'O', 'N'}, // Warm White
  {' ', ' ', 'O', 'N'}, // Cold White
  {' ', 'R', 'E', 'D'}, // Red
  {' ', 'G', 'R', 'N'}, // Green
  {'B', 'L', 'U', 'E'}, // Blue
  {'Y', 'E', 'L', 'O'}, // Yellow
  {'O', 'R', 'N', 'G'}, // Orange
  {'C', 'Y', 'A', 'N'}, // Cyan
  {'P', 'R', 'E', 'D'}, // Magenta
  {'P', 'R', 'P', 'L'}, // Purple
  {' ', 'R', 'N', 'B'}, // Rainbow
  {'P', 'R', 'N', 'B'}, // Pastel Rainbow
  {' ', 'G',   2, 'B'}, // Green to Blue
  {' ', 'R',   2, 'B'}, // Red to Blue
  {' ', 'R',   2, 'G'}  // Red to Green
};
static const char Messages_Color_Rainbow[][CONFIG_NUM_DIGITS] = {
  {'C', 'H', 'I', 'L', 'L', ' '},
  {'R', 'E', 'G', 'U', 'L', 'A'},
  {'T', 'I', 'G', 'H', 'T', ' '}
};
static const char Messages_Color_Chase[][4] = {
  {' ', 'R', '-', 'L'},
  {' ', 'L', '-', 'R'},
  {'F', 'L', 'I', 'P'},
  {' ', ' ', 'O', 'N'},
  {' ', 'O', 'F', 'F'}
};

static const char Messages_Color_Hello[][CONFIG_NUM_DIGITS] = {
  {' ', 'C', 'O', 'L', 'O', 'R'},
  {' ', 'B', 'L', 'I', 'S', 'S'},
  {' ', ' ', 'F', 'A', 'D', 'E'},
  {' ', 'R', 'N', 'B', 'F', 'D'},
  {' ', 'C', 'H', 'A', 'S', 'E'},
  {'T', 'I', 'C', 'O', 'D', 'E'},
  {'C', ' ', ' ', 'C', 'O', 'P'}
};

static const char Messages_Color_Bliss[][CONFIG_NUM_DIGITS] = {
  {'N', 'O', 'R', 'D', 'L', 'I'},
  // {'S', 'C', 'H', 'N', 'E', 'E'},
  {'P', 'A', 'S', 'T', 'E', 'L'},
  {' ', 'H', 'O', 'N', 'I', 'G'},
  {' ', 'O', 'C', 'E', 'A', 'N'},
  {' ', 'F', 'E', 'U', 'E', 'R'},
  {' ', 'A', 'B', 'E', 'N', 'D'}
};




static void _target_RGB(uint8_t *tp, uint8_t r, uint8_t g, uint8_t b) {
  tp[0] = g;
  tp[1] = r;
  tp[2] = b;
  tp[3] = 0;
}
static void _target_RGBW(uint8_t *tp, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
  tp[0] = g;
  tp[1] = r;
  tp[2] = b;
	tp[3] = w;
}
static void _target_all_RGB(uint8_t *tp, uint8_t r, uint8_t g, uint8_t b) {
	for(uint8_t i = 0; i < CONFIG_NUM_BYTES; i += 4) _target_RGB(tp + i, r, g, b);
}
static void _target_all_RGBW(uint8_t *tp, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
	for(uint8_t i = 0; i < CONFIG_NUM_BYTES; i += 4) _target_RGBW(tp + i, r, g, b, w);
}

static /*inline*/ void _minimize_difference(uint8_t *target_arr) {
	uint8_t dt = 0;
	for(uint8_t i = 0; i < CONFIG_NUM_BYTES; i++) { // Sorry for the ll-access
		if(rgb_arr[i] < target_arr[i]) rgb_arr[i]++;
		else if(rgb_arr[i] > target_arr[i]) rgb_arr[i]--;
		else ++dt;
	}
	if(dt != CONFIG_NUM_BYTES) vfdco_clr_render();
}

/**
  * @brief  Implementation of virtual functions Light_Pattern::VTable (static void _Light_Pattern_F3)
**/
static inline vfdco_hid_action_status_t _Light_Pattern_F3(struct Light_Pattern *unsafe_self) {
 if(!unsafe_self->VTable.F3) return BUTTON_ACTION_NOT_PERFORMED;
 unsafe_self->VTable.F3(unsafe_self);
 return BUTTON_ACTION_PERFORMED;
}
static inline vfdco_hid_action_status_t _Light_Pattern_F3Var(struct Light_Pattern *unsafe_self) {
 if(!unsafe_self->VTable.F3Var) return BUTTON_ACTION_NOT_PERFORMED;
 unsafe_self->VTable.F3Var(unsafe_self);
 return BUTTON_ACTION_PERFORMED;
}
static inline void _Light_Pattern_Update(struct Light_Pattern *unsafe_self) {
 // if(!self->VTable.Update) return; Will make sure this never happens. Optimize for loop performance
 unsafe_self->VTable.Update(unsafe_self);
}
/* static inline void _Light_Pattern_Delete(struct Light_Pattern *unsafe_self) {
 // if(!self->VTable.Update) return; Will make sure this never happens. Optimize for loop performance
 unsafe_self->VTable.Delete(unsafe_self);
}*/
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
  // self->Delete = _Light_Pattern_Delete;
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
    _minimize_difference(self->target_arr);
  }
}

/**
  * @brief  Implementation of virtual function Light_Pattern_Static::F3 (static void _Light_Pattern_Static_F3)
 **/
static void _Light_Pattern_Static_F3(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Static *self = (struct Light_Pattern_Static *)unsafe_self;

  self->position++;
  if(self->position >= NUM_STATIC_T4) self->position = 0;

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

  } else { // < T4
    // Multicolor, just use legacy colors, they are hand crafted and look better
    uint8_t t_pos = self->position - NUM_STATIC_T2;
    for(uint8_t i = 0; i < CONFIG_NUM_PIXELS; ++i) {
      _target_RGB(self->target_arr + 4 * i, Static_Color_Rainbows[t_pos][3 * i + 1], Static_Color_Rainbows[t_pos][3 * i], Static_Color_Rainbows[t_pos][3 * i + 2]);
    }
  }

  char k[CONFIG_NUM_DIGITS] = {'C', ' '};
  for(uint_fast8_t i = 0; i < CONFIG_NUM_DIGITS; ++i) k[i + 2] = Messages_Color_Static[self->position][i];
  vfdco_display_render_message(k, 0, CONFIG_MESSAGE_SHORT);
}

/**
  * @brief  Implementation of virtual function Light_Pattern_Static::Hello (static void _Light_Pattern_Static_Hello)
 **/
static inline void _Light_Pattern_Static_Hello(void) {
  vfdco_display_render_message(Messages_Color_Hello[0], 0, CONFIG_MESSAGE_LONG);
}

/* static inline void _Light_Pattern_Static_Delete(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Static *self = (struct Light_Pattern_Static *)unsafe_self;
  free(self);
} */

/**
  * @brief  Constructor of Light_Pattern_Static class
 **/
void Light_Pattern_Static_Init(struct Light_Pattern_Static *self) {
  Light_Pattern_Init(&self->super);
  self->t = Time_Event_Init(CONFIG_SINGLE_COLOR_FADE_SPEED);

  memset(self->target_arr, 0, CONFIG_NUM_BYTES);
  self->position = 0;

  struct Light_Pattern_VTable _static_vtable = {
    .F3 = _Light_Pattern_Static_F3,
    .F3Var = NULL,
    .Update = _Light_Pattern_Static_Update,
    .Hello = _Light_Pattern_Static_Hello
		// .Delete = _Light_Pattern_Static_Delete
  };
  self->super.VTable = _static_vtable;
}





/** Begin of:
  * @toc SECTION_LIGHT_PATTERN_SPECTRUM
 **/
/**
  * @brief  Implementation of virtual function Light_Pattern_Spectrum::Update (static void _Light_Pattern_Spectrum_Update)
 **/
static void _Light_Pattern_Spectrum_Update(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Spectrum *self = (struct Light_Pattern_Spectrum *)unsafe_self;

  #ifdef LED_COLOR_ENABLE_POLYMORPHIC_USE
  self->spectrum_fader.super.Next(&self->spectrum_fader.super);
  #else
  self->spectrum_fader.Next(&self->spectrum_fader);
  #endif
}

/**
* @brief  Implementation of virtual function Light_Pattern_Spectrum::F3 (static void _Light_Pattern_Spectrum_F3)
**/
static void _Light_Pattern_Spectrum_F3(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Spectrum *self = (struct Light_Pattern_Spectrum *)unsafe_self;
  hsl_t *_color = &self->spectrum_fader.pk_1;

  char k[CONFIG_NUM_DIGITS] = {'B', 'R', 'I', ' ', ' ', 1};
  if(_color->l == LIGHTNESS_M) {
    _color->l = LIGHTNESS_H;
    k[5] = 3;
  }
  else if(_color->l == LIGHTNESS_H) {
    _color->l = LIGHTNESS_L;
  }
  else{
    _color->l = LIGHTNESS_M;
    k[5] = 2;
  }
  vfdco_display_render_message(k, 0b00000100, CONFIG_MESSAGE_SHORT);
}

/**
* @brief  Implementation of virtual function Light_Pattern_Spectrum::F3Var (static void _Light_Pattern_Spectrum_F3Var)
**/
static void _Light_Pattern_Spectrum_F3Var(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Spectrum *self = (struct Light_Pattern_Spectrum *)unsafe_self;
  hsl_t *_color = &self->spectrum_fader.pk_1;

  char k[CONFIG_NUM_DIGITS] = {'S', 'A', 'T', ' ', ' ', 1};
  if(_color->s == SATURATION_H) {
    _color->s = SATURATION_L;
  }
  else if(_color->s == SATURATION_L) {
    _color->s = SATURATION_M;
    k[5] = 2;
  }
  else{
    _color->s = SATURATION_H;
    k[5] = 3;
  }
  vfdco_display_render_message(k, 0b00000100, CONFIG_MESSAGE_SHORT);
}

/**
* @brief  Implementation of virtual function Light_Pattern_Spectrum::Hello (static void _Light_Pattern_Spectrum_Hello)
**/
static inline void _Light_Pattern_Spectrum_Hello(void) {
  vfdco_display_render_message(Messages_Color_Hello[2], 0, CONFIG_MESSAGE_LONG);
}

/* static inline void _Light_Pattern_Spectrum_Delete(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Spectrum *self = (struct Light_Pattern_Spectrum *)unsafe_self;
  free(self);
}*/

/**
* @brief  Constructor of Light_Pattern_Spectrum class
**/
void Light_Pattern_Spectrum_Init(struct Light_Pattern_Spectrum *self) {
  Light_Pattern_Init(&self->super);

  // Oh this is like driving a truck out of its garage to pick up a pretzel from a backery 100 ft away
  LED_Color_Fader_Init(
    &self->spectrum_fader,
    CONFIG_SPECTRUM_FADE_SPEED,  // Timer interval
    #ifdef LED_COLOR_FADER_EXTENDED
    0,                           // Pixel index to start
    #endif
    LED_COLOR_REPEAT_FOREVER,    // Fade N cycles
    HSL_Init(0, SATURATION_H, LIGHTNESS_H),
    HSL_Init(0, 0, 0),
    #ifdef LED_COLOR_FADER_EXTENDED
    CONFIG_NUM_DIGITS,           // Number of chained pixels
    #endif
    0                            // Hue difference between chained pixels
  );

  // Skip fade in
  ((struct LED_Color_Fader *)&self->spectrum_fader)->state = LED_COLOR_STATE_ACTIVE;

  struct Light_Pattern_VTable _spectrum_vtable = {
    .F3 = _Light_Pattern_Spectrum_F3,
    .F3Var = _Light_Pattern_Spectrum_F3Var,
    .Update = _Light_Pattern_Spectrum_Update,
    .Hello = _Light_Pattern_Spectrum_Hello
    // .Delete = _Light_Pattern_Spectrum_Delete
  };
  self->super.VTable = _spectrum_vtable;
}




/** Begin of:
* @toc SECTION_LIGHT_PATTERN_rainbow
**/
/**
* @brief  Implementation of virtual function Light_Pattern_Rainbow::Update (static void _Light_Pattern_Rainbow_Update)
**/
static void _Light_Pattern_Rainbow_Update(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Rainbow *self = (struct Light_Pattern_Rainbow *)unsafe_self;

  #ifdef LED_COLOR_ENABLE_POLYMORPHIC_USE
  self->rainbow_fader.super.Next(&self->rainbow_fader.super);
  #else
  self->rainbow_fader.Next(&self->rainbow_fader);
  #endif
}

/**
* @brief  Implementation of virtual function Light_Pattern_Rainbow::F3 (static void _Light_Pattern_Rainbow_F3)
**/
static void _Light_Pattern_Rainbow_F3(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Rainbow *self = (struct Light_Pattern_Rainbow *)unsafe_self;

  struct LED_Color_Fader *_f = (struct LED_Color_Fader *)&self->rainbow_fader;

  // char message[MESSAGE_LENGTH];
  if(_f->chain_huediff == 10) {
    _f->chain_huediff = 21;
    vfdco_display_render_message(Messages_Color_Rainbow[1], 0, CONFIG_MESSAGE_SHORT);
  }
  else if(_f->chain_huediff == 21) {
    _f->chain_huediff = 42;
    vfdco_display_render_message(Messages_Color_Rainbow[2], 0, CONFIG_MESSAGE_SHORT);
  }
  else{
    _f->chain_huediff = 10;
    vfdco_display_render_message(Messages_Color_Rainbow[0], 0, CONFIG_MESSAGE_SHORT);
  }
}

/**
* @brief  Implementation of virtual function Light_Pattern_Rainbow::F3Var (static void _Light_Pattern_Rainbow_F3Var)
**/
static void _Light_Pattern_Rainbow_F3Var(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Rainbow *self = (struct Light_Pattern_Rainbow *)unsafe_self;
  hsl_t *_color = &self->rainbow_fader.pk_1;

  char k[CONFIG_NUM_DIGITS] = {'S', 'A', 'T', ' ', ' ', 1};
  if(_color->s == SATURATION_H) {
    _color->s = SATURATION_L;
  }
  else if(_color->s == SATURATION_L) {
    _color->s = SATURATION_M;
    k[5] = 2;
  }
  else{
    _color->s = SATURATION_H;
    k[5] = 3;
  }
  vfdco_display_render_message(k, 0b00000100, CONFIG_MESSAGE_SHORT);
}

/**
* @brief  Implementation of virtual function Light_Pattern_Rainbow::Hello (static void _Light_Pattern_Rainbow_Hello)
**/
static inline void _Light_Pattern_Rainbow_Hello(void) {
  vfdco_display_render_message(Messages_Color_Hello[3], 0, CONFIG_MESSAGE_LONG);
}

/* static inline void _Light_Pattern_Rainbow_Delete(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Rainbow *self = (struct Light_Pattern_Rainbow *)unsafe_self;
  free(self);
} */

/**
* @brief  Constructor of Light_Pattern_Rainbow class
**/
void Light_Pattern_Rainbow_Init(struct Light_Pattern_Rainbow *self) {
  Light_Pattern_Init(&self->super);

  // Oh this is like driving a truck out of its garage to pick up a pretzel from a backery 100 ft away
  LED_Color_Fader_Init(
    &self->rainbow_fader,
    CONFIG_SPECTRUM_FADE_SPEED,  // Timer interval
    /*LED_COLOR_BLEND_MODE_NORMAL,*/ // Pixel blend setting.
    #ifdef LED_COLOR_FADER_EXTENDED
    0,                           // Pixel index to start
    #endif
    LED_COLOR_REPEAT_FOREVER,    // Fade N cycles
    HSL_Init(0, SATURATION_H, LIGHTNESS_H),
    HSL_Init(0, 0, 0),
    #ifdef LED_COLOR_FADER_EXTENDED
    CONFIG_NUM_DIGITS,           // Number of chained pixels
    #endif
    10                           // Hue difference between chained pixels
  );

  // Skip fade in
  ((struct LED_Color_Fader *)&self->rainbow_fader)->state = LED_COLOR_STATE_ACTIVE;

  struct Light_Pattern_VTable _rainbow_vtable = {
    .F3 = _Light_Pattern_Rainbow_F3,
    .F3Var = _Light_Pattern_Rainbow_F3Var,
    .Update = _Light_Pattern_Rainbow_Update,
    .Hello = _Light_Pattern_Rainbow_Hello
    // .Delete = _Light_Pattern_Rainbow_Delete
  };
  self->super.VTable = _rainbow_vtable;
}







/** Begin of:
* @toc SECTION_LIGHT_PATTERN_CHASE
**/
/**
* @brief  Implementation of virtual function Light_Pattern_Chase::Update (static void _Light_Pattern_Chase_Update)
**/
static void _Light_Pattern_Chase_Update(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Chase *self = (struct Light_Pattern_Chase *)unsafe_self;

  struct LED_Color_Chaser *_chaser = (struct LED_Color_Chaser *)&self->chase_fader;

  if(_chaser->state != LED_COLOR_STATE_CYCLIC_RECOVERY) {
    #ifdef LED_COLOR_ENABLE_POLYMORPHIC_USE
    _chaser->super.Next(&_chaser->super);
    #else
    LED_Color_Chaser_Next(_chaser);
    #endif
  }

  // If second has changed
  if(self->flip_timer->s != self->flip_timer_previous_second) {
    self->flip_timer_previous_second = self->flip_timer->s;
    _chaser->pk.h += 27;

    // Reset state variables
    _chaser->pk_state = 0;
    _chaser->chase_pos = 0;

    // Flip direction at every end
    if(self->chase_mode == 2) {
      if(self->flip_timer->s & 0x01) {
        _chaser->start_pos = 0;
        _chaser->chase_mode = LED_COLOR_CHASER_MODE_LR_LINEAR;
      } else {
        _chaser->start_pos = CONFIG_NUM_PIXELS - 1;
        _chaser->chase_mode = LED_COLOR_CHASER_MODE_RL_LINEAR;
      }
    }
  }
 }

/**
* @brief  Implementation of virtual function Light_Pattern_Chase::F3 (static void _Light_Pattern_Chase_F3)
* @function  Switches between L-R, R-L and second flip mode
**/
static void _Light_Pattern_Chase_F3(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Chase *self = (struct Light_Pattern_Chase *)unsafe_self;


  if(self->chase_mode == 0) { // L->R to R->L
    self->chase_mode = 1;
    struct LED_Color_Chaser *_chaser = (struct LED_Color_Chaser *)&self->chase_fader;
    _chaser->start_pos = CONFIG_NUM_PIXELS - 1;
    _chaser->chase_mode = LED_COLOR_CHASER_MODE_RL_LINEAR;
  } else if(self->chase_mode == 1) { // To flipper
    self->chase_mode = 2;

  } else {
    self->chase_mode = 0;
    struct LED_Color_Chaser *_chaser = (struct LED_Color_Chaser *)&self->chase_fader;
    _chaser->start_pos = 0;
    _chaser->chase_mode = LED_COLOR_CHASER_MODE_LR_LINEAR;
  }
  char k[CONFIG_NUM_DIGITS] = {' ', ' '};
  for(uint_fast8_t i = 0; i < 4; ++i) k[i + 2] = Messages_Color_Chase[self->chase_mode][i];
  vfdco_display_render_message(k, 0, CONFIG_MESSAGE_SHORT);
}

/**
* @brief  Implementation of virtual function Light_Pattern_Chase::F3Var (static void _Light_Pattern_Chase_F3Var)
**/
static void _Light_Pattern_Chase_F3Var(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Chase *self = (struct Light_Pattern_Chase *)unsafe_self;

  char k[CONFIG_NUM_DIGITS] = {'G', 'R'};
  if(self->chase_fader.pk_diff.h == 0) {
    self->chase_fader.pk_diff.h = 6;
    for(uint_fast8_t i = 0; i < 4; ++i) k[i + 2] = Messages_Color_Chase[3][i];
  } else {
    self->chase_fader.pk_diff.h = 0;
    for(uint_fast8_t i = 0; i < 4; ++i) k[i + 2] = Messages_Color_Chase[4][i];
  }
  vfdco_display_render_message(k, 0, CONFIG_MESSAGE_SHORT);
}

/**
* @brief  Implementation of virtual function Light_Pattern_Chase::Hello (static void _Light_Pattern_Chase_Hello)
**/
static inline void _Light_Pattern_Chase_Hello(void) {
  vfdco_display_render_message(Messages_Color_Hello[4], 0, CONFIG_MESSAGE_LONG);
}

/* static inline void _Light_Pattern_Chase_Delete(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Chase *self = (struct Light_Pattern_Chase *)unsafe_self;
  free(self);
} */

/**
* @brief  Constructor of Light_Pattern_Chase class
**/
void Light_Pattern_Chase_Init(struct Light_Pattern_Chase *self, vfdco_time_t *time, uint_fast8_t chase_mode) {
  Light_Pattern_Init(&self->super);

  hsl_d_t _hsld = {.h = 0, .s = 0, .l = 0};

  self->flip_timer = time;
  self->chase_mode = chase_mode;

  // Oh this is like driving a truck out of its garage to pick up a pretzel from a backery 100 ft away
  LED_Color_Chaser_Init(
    &self->chase_fader, // viel sympathischer...
    2,
    /*LED_COLOR_BLEND_MODE_NORMAL,*/
    (chase_mode == 1) ? CONFIG_NUM_PIXELS - 1 : 0, // 1: start right, else start left (usual)
    LED_COLOR_REPEAT_FOREVER, // Repeat N times
    #ifdef LED_COLOR_CHASER_EXTENDED
    CONFIG_NUM_PIXELS,
    #endif
    HSL_Init(0, SATURATION_H, LIGHTNESS_M),
    _hsld,
    18,
    #ifdef LED_COLOR_CHASER_ENABLE_COLOR_PRESERVING
    LED_COLOR_CHASER_NON_PRESERVING,
    #endif
    (chase_mode == 1) ? LED_COLOR_CHASER_MODE_RL_LINEAR : LED_COLOR_CHASER_MODE_LR_LINEAR
  );

  struct Light_Pattern_VTable _chase_vtable = {
    .F3 = _Light_Pattern_Chase_F3,
    .F3Var = _Light_Pattern_Chase_F3Var,
    .Update = _Light_Pattern_Chase_Update,
    .Hello = _Light_Pattern_Chase_Hello
    // .Delete = _Light_Pattern_Chase_Delete
  };
  self->super.VTable = _chase_vtable;
}





/** Begin of:
  * @toc SECTION_LED_COLOR_RESISTOR
 **/
/**
  * @brief  Implementation of virtual function LED_Color_Resistor::Update (static void _LED_Color_Resistor_Update)
 **/
static void _Light_Pattern_Time_Code_Update(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Time_Code *self = (struct Light_Pattern_Time_Code *)unsafe_self;

	if(Time_Event_Update(&self->clock)) {
		// Dereference const (read only) variables
	  uint8_t h = self->time->h;
	  uint8_t m = self->time->m;
	  uint8_t s = self->time->s;
		uint8_t *target_arr = self->target_arr;

		uint8_t digit_values[CONFIG_NUM_PIXELS] = {
      s % 10, s / 10,
      m % 10, m / 10,
      h % 10, h / 10
    };

		for(uint8_t i = 0; i < CONFIG_NUM_PIXELS; i++)
			_target_RGB(target_arr + 4 * i, Time_Code_Colors[digit_values[i]][1], Time_Code_Colors[digit_values[i]][0], Time_Code_Colors[digit_values[i]][2]);

		_minimize_difference(target_arr);
  }
}

/**
  * @brief  Implementation of virtual function LED_Color_Resistor::Hello (static void _LED_Color_Resistor_Hello)
 **/
static inline void _Light_Pattern_Time_Code_Hello(void) {
	vfdco_display_render_message(Messages_Color_Hello[5], 0, CONFIG_MESSAGE_LONG);
}

/* static inline void _Light_Pattern_Time_Code_Delete(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Time_Code *self = (struct Light_Pattern_Time_Code *)unsafe_self;
  free(self);
} */

/**
  * @brief  Constructor of LED_Color_Resistor class
 **/
void Light_Pattern_Time_Code_Init(struct Light_Pattern_Time_Code *self, vfdco_time_t *time_instance) {
  Light_Pattern_Init(&self->super);
	self->clock = Time_Event_Init(1);

	memset(self->target_arr, 0, CONFIG_NUM_BYTES);
  self->time = time_instance;

  struct Light_Pattern_VTable _time_code_vtable = {
    .F3 = NULL,
    .F3Var = NULL,
    .Update = _Light_Pattern_Time_Code_Update,
    .Hello = _Light_Pattern_Time_Code_Hello
		// .Delete = _Light_Pattern_Time_Code_Delete
  };
  self->super.VTable = _time_code_vtable;
}


/** Begin of:
* @toc SECTION_LIGHT_PATTERN_COP
**/
/**
* @brief  Implementation of virtual function Light_Pattern_Cop::Update (static void _Light_Pattern_Cop_Update)
**/
static void _Light_Pattern_Cop_Update(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Cop *self = (struct Light_Pattern_Cop *)unsafe_self;

  if(Time_Event_Update(&self->clock)) {
    if(self->state < 13) self->state++;
    else if(self->state == 13) self->state = 0;

    uint8_t _hl = (self->state & 0x01) ? 0x00 : 0xFF;

    // b | r and r | b fill
    if(self->state == 0 || self->state == 6 || self->state == 7 || self->state == 13) {
      for(uint_fast8_t i = 0;                        i < (CONFIG_NUM_PIXELS >> 1); ++i) vfdco_clr_set_RGB(i,  _hl,   0, ~_hl);
      for(uint_fast8_t i = (CONFIG_NUM_PIXELS >> 1); i < CONFIG_NUM_PIXELS;        ++i) vfdco_clr_set_RGB(i, ~_hl,   0,  _hl);
    }
    // off fill
    else if(self->state == 5 || self->state == 12) {
      vfdco_clr_set_all_RGB(  _hl,   _hl,   _hl);
    }
    vfdco_clr_render();
  }
}

/**
* @brief  Implementation of virtual function Light_Pattern_Cop::Hello (static void _Light_Pattern_Cop_Hello)
**/
static inline void _Light_Pattern_Cop_Hello(void) {
  vfdco_display_render_message(Messages_Color_Hello[6], 0, CONFIG_MESSAGE_LONG);
}

/**
* @brief  Implementation of virtual destructor Light_Pattern_Cop::~Light_Pattern_Cop (static void _Light_Pattern_Cop_Delete)
**/
/* static inline void _Light_Pattern_Cop_Delete(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_Cop *self = (struct Light_Pattern_Cop *)unsafe_self;
  free(self);
} */

/**
* @brief  Constructor of Light_Pattern_Cop class Light_Pattern_Cop::Light_Pattern_Cop (static void Light_Pattern_Cop_Init)
**/
void Light_Pattern_Cop_Init(struct Light_Pattern_Cop *self) {
  Light_Pattern_Init(&self->super);

  self->clock = Time_Event_Init(CONFIG_COP_FADE_SPEED);
  self->state = 0;

  struct Light_Pattern_VTable _vtable = {
    .F3      = NULL,
    .F3Var   = NULL,
    .Update  = _Light_Pattern_Cop_Update,
    .Hello   = _Light_Pattern_Cop_Hello
    // .Delete  = _Light_Pattern_Cop_Delete
  };
  self->super.VTable = _vtable;
}






/** Begin of:
* @toc SECTION_LIGHT_PATTERN_MOMENTSOFBLISS
**/
/**
* @brief  Implementation of virtual function Light_Pattern_MomentsOfBliss::Update (static void _Light_Pattern_MomentsOfBliss_Update)
**/
static void _Light_Pattern_MomentsOfBliss_Update(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_MomentsOfBliss *self = (struct Light_Pattern_MomentsOfBliss *)unsafe_self;

  struct LED_Color_Fader *base = (struct LED_Color_Fader *)&self->base_fader;
  LED_COLOR_STATE_t prev_state = base->state;

  if(
    #ifdef LED_COLOR_ENABLE_POLYMORPHIC_USE
    self->base_fader.super.Next(&self->base_fader.super)
    #else
    self->base_fader.Next(&self->base_fader)
    #endif
    == LED_COLOR_STATE_CYCLIC_RECOVERY && prev_state == LED_COLOR_STATE_ACTIVE) {
    uint8_t bits = MomentsOfBliss_Colors[self->moment][6];
    // Randomize h
    ++self->undrift_counter;
    if(self->undrift_counter < self->undrift_max) {
      // Randomize new hue & hue diff by a pos or neg number biased around 0 by (1<<bits) / 2
      base->pk_1.h = base->pk_1.h -((1 << (bits & 0x0F)) >> 1) + led_color_simple_randomizer(bits & 0x0F);
      // ((struct LED_Color_Fader *)self->base_fader)->chain_huediff = led_color_simple_randomizer(bits >> 4) - ((1 << (bits >> 4)) >> 1);
    } else {
      // Restore hue
      base->pk_1.h = MomentsOfBliss_Colors[self->moment][0];
      self->undrift_counter = 0;
      self->undrift_max = led_color_simple_randomizer(2) + 2;
    }

    // Huediff variation, operates independently from color diff
    if(self->undrift_huediff_max > 0) {
      if(base->chain_huediff < self->undrift_huediff_max) {
        ++base->chain_huediff;
      } else {
        self->undrift_huediff_max = -self->undrift_huediff_max;
        --base->chain_huediff;
      }
    }
    else {
      if(base->chain_huediff > self->undrift_huediff_max) {
        --base->chain_huediff;
      } else {
        self->undrift_huediff_max = -self->undrift_huediff_max;
        ++base->chain_huediff;
      }
    }
    if(!base->chain_huediff) {
      uint8_t huediff_max = MomentsOfBliss_Colors[self->moment][6] >> 4;
      self->undrift_huediff_max = led_color_simple_randomizer(huediff_max) - ((1 << huediff_max) >> 1) + 1;
    }
  }
}

static inline void _Light_Pattern_MomentsOfBliss_Remoment(struct Light_Pattern_MomentsOfBliss *self) {
  LED_Color_Fader_Init(
    &self->base_fader,
    CONFIG_MOMENTSOFBLISS_FADE_SPEED,  // Timer interval
    #ifdef LED_COLOR_FADER_EXTENDED
    0,                           // Pixel index to start
    #endif
    LED_COLOR_REPEAT_FOREVER,    // Fade N cycles
    HSL_Init(MomentsOfBliss_Colors[self->moment][0], MomentsOfBliss_Colors[self->moment][1], MomentsOfBliss_Colors[self->moment][2]),
    HSL_Init(MomentsOfBliss_Colors[self->moment][3], MomentsOfBliss_Colors[self->moment][4], MomentsOfBliss_Colors[self->moment][5]),
    #ifdef LED_COLOR_FADER_EXTENDED
    CONFIG_NUM_DIGITS,           // Number of chained pixels
    #endif
    0
  );

  self->undrift_max = led_color_simple_randomizer(2) + 2; // some number between 2 and 5 lol
  uint8_t huediff_max = MomentsOfBliss_Colors[self->moment][6] >> 4;
  self->undrift_huediff_max = led_color_simple_randomizer(huediff_max) - ((1 << huediff_max) >> 1);
}

/**
* @brief  Implementation of virtual function Light_Pattern_MomentsOfBliss::F3 (static void _Light_Pattern_MomentsOfBliss_F3)
**/
static void _Light_Pattern_MomentsOfBliss_F3(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_MomentsOfBliss *self = (struct Light_Pattern_MomentsOfBliss *)unsafe_self;

  ++self->moment;
  if(!(self->moment < LIGHTS_BLISS_MAXMOMENTS)) self->moment = 0;
  _Light_Pattern_MomentsOfBliss_Remoment(self);

  vfdco_display_render_message(Messages_Color_Bliss[self->moment], 0, CONFIG_MESSAGE_SHORT);
}

/**
* @brief  Implementation of virtual function Light_Pattern_MomentsOfBliss::F3Var (static void _Light_Pattern_MomentsOfBliss_F3Var)
**/
/*static void _Light_Pattern_MomentsOfBliss_F3Var(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_MomentsOfBliss *self = (struct Light_Pattern_MomentsOfBliss *)unsafe_self;
}*/

/**
* @brief  Implementation of virtual function Light_Pattern_MomentsOfBliss::Hello (static void _Light_Pattern_MomentsOfBliss_Hello)
**/
static inline void _Light_Pattern_MomentsOfBliss_Hello(void) {
  vfdco_display_render_message(Messages_Color_Hello[1], 0, CONFIG_MESSAGE_LONG);
}

/**
* @brief  Implementation of virtual destructor Light_Pattern_MomentsOfBliss::~Light_Pattern_MomentsOfBliss (static void _Light_Pattern_MomentsOfBliss_Delete)
**/
/* static inline void _Light_Pattern_MomentsOfBliss_Delete(struct Light_Pattern *unsafe_self) {
  struct Light_Pattern_MomentsOfBliss *self = (struct Light_Pattern_MomentsOfBliss *)unsafe_self;
  free(self);
} */

/**
* @brief  Constructor of Light_Pattern_MomentsOfBliss class Light_Pattern_MomentsOfBliss::Light_Pattern_MomentsOfBliss (static void Light_Pattern_MomentsOfBliss_Init)
**/
void Light_Pattern_MomentsOfBliss_Init(struct Light_Pattern_MomentsOfBliss *self, uint_fast8_t moment) {
  Light_Pattern_Init(&self->super);

  self->moment = moment;
  _Light_Pattern_MomentsOfBliss_Remoment(self);

  struct Light_Pattern_VTable _vtable = {
    .F3      = _Light_Pattern_MomentsOfBliss_F3,
    .F3Var   = NULL,//_Light_Pattern_MomentsOfBliss_F3Var,
    .Update  = _Light_Pattern_MomentsOfBliss_Update,
    .Hello   = _Light_Pattern_MomentsOfBliss_Hello
    // .Delete  = _Light_Pattern_MomentsOfBliss_Delete
  };
  self->super.VTable = _vtable;
}







/**
  * @brief  Definition of Container_Light_Pattern_Clear to set all components to zero
**/
void Container_Light_Pattern_Clear(Container_Light_Pattern_t *self) {
  memset(self, 0, sizeof(Container_Light_Pattern_t));
}



/*
Waiting for your call and for the mood
to release me from the longest afternoon
I'm da 1000th line.
*/
