/*MIT License

Copyright (c) The VFD Collective, Frank from The VFD Collective (Fu Zheng)
Date: 04/25/2020

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

/**
  ******************************************************************************
  * @file     vfdco_lights.h
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     23-December-2019
  * @brief    This file contains definitions for light patterns
  *           Designed to be used with Fluorescence by The VFD Collective
  ******************************************************************************
  * @tableofcontents Table of contents, enter to navigate:
  * SECTION_SUPPORTING_FUNCTIONS
  * SECTION_HSL
  * SECTION_COLOR_FADER
  * SECTION_LIGHT_PATTERN_CONSTANTS
  * SECTION_LIGHT_PATTERN
  * SECTION_LIGHT_PATTERN_STATIC
  * SECTION_LIGHT_PATTERN_SPECTRUM
  * SECTION_LIGHT_PATTERN_RAINBOW
  * SECTION_LIGHT_PATTERN_CHASE
  * SECTION_LIGHT_PATTERN_MUSIC
  * SECTION_LIGHT_PATTERN_TIME_CODE
  * SECTION_LIGHT_PATTERN_COP
  * SECTION_LIGHT_PATTERN_MOMENTSOFBLISS
  * SECTION_CONTAINER_LIGHT_PATTERN
  * SECTION_SUPPORTING_FUNCTIONS_IMPLEMENTATION
  ******************************************************************************
 **/

#include "vfdco_config.h"
#include "vfdco_led.h"
#include "vfdco_display.h"
#include "vfdco_mic.h"
#include "vfdco_lights.h"

// #include <stdlib.h>
#include <string.h>

/** Begin of:
  * @tableofcontents SECTION_SUPPORTING_FUNCTIONS
  * @brief Prototypes. Implementation see very end
 **/
// HSL to RGB fast calculation
static uint32_t _led_color_hsl2rgb(uint8_t h, uint8_t s, uint8_t l);

/** Begin of:
  * @tableofcontents SECTION_HSL
 **/

/**
 * @brief Initializes an hsl_t item
 * @param h Hue: uint8_t
 * @param s Saturation: uint8_t
 * @param l Lightness: uint8_t
 * @return hsl_t 
 */
hsl_t HSL_Init( uint8_t h, uint8_t s, uint8_t l) {
  hsl_t _hsl = { .h = h, .s = s, .l = l };
  return _hsl;
}

/** Begin of:
 * @tableofcontents SECTION_COLOR_FADER
 * @brief The classes Spectrum, Rainbow and Bliss use the Color Fader
**/
/**
 * @brief  Implementation of method LED_Color_Fader::Next for single peak
**/
static LED_COLOR_STATE_t _LED_Color_Fader_NextColorLinSingle(struct LED_Color_Fader *self) {
  uint8_t render_enable = Time_Event_Update(&self->timer);
  if(render_enable) {
    ++(self->color_1.h);
    // Write to next color
    for(uint_fast8_t i = 0; i < CONFIG_NUM_PIXELS; ++i) {
      int16_t i_h = i * self->chain_hue_diff;
      uint32_t target_color = _led_color_hsl2rgb(self->color_1.h + i_h, self->color_1.s, self->color_1.l);
      vfdco_clr_set_RGB(i, (target_color >> 8) & 0xFF, (target_color >> 16) & 0xFF, target_color & 0xFF);
    }
    vfdco_clr_render();
  }
  return self->state;
}

/**
 * @brief  Implementation of method LED_Color_Fader::Next for multiple peaks
**/
static LED_COLOR_STATE_t _LED_Color_Fader_NextColorLin(struct LED_Color_Fader *self) {
  uint8_t render_enable = Time_Event_Update(&self->timer);
  uint8_t i_h = 0, i_s = 0, i_l = 0;
  if(render_enable) {
    // Pick up current and target pix depending on position and
    // uint8_t access_index = self->fade_pos >> LED_COLOR_FADER_TIME_BITS;   // Current access index = fade_pos / time_period, always between [0 ... num_pks - 1]
    hsl_t *current;
    hsl_t *target;
    if(self->state) {
      current = &self->color_1;
      target =  &self->color_2;
    } else {
      current = &self->color_2;
      target =  &self->color_1;
    }
    // Linearly transition HSL from curr -> target
    /*
      i_h = (((target->h - (int_fast16_t)current->h) * (int_fast32_t)t) >> LED_COLOR_FADER_TIME_BITS) + current->h;
      is suboptimal, as e.g. going from 250 (magenta red) to 30 (orange) is traveled through the whole spectrum instead of just overflowing
      A workaround for this issue is proposed by the LerpHSL function.
      LerpHSL does a linear interpolation of the Hue component and chooses the shortest distance in a cyclic way
      The idea is visualized by Alan Zucconi https://www.alanzucconi.com/2016/01/06/colour-interpolation/
      The code is largely based on https://github.com/yuichiroharai/glsl-y-hsv/blob/master/lerpHSV.glsl, rewritten for fixed point arithmetics
      For an 8 bit interpolation state t [0 .. 255] and the 8 bit Hue values H1 and H2 of a color, the code simplifies to:
      H_NEW = (((((int16_t)(383 + H2 - H1) % 255) - 127) * t) >> 8) + H1;
      or any norm factor 1/k H_NEW = (((1.5k + H2 - H1) % k) - 0.5k) * t + H1 for t [0..1]
      The i_h below is adapted to
    */
    i_h = (((((uint16_t)(384 + target->h - current->h) % 255) - 127) * self->fade_pos) >> 8) + current->h;  // what the actual fuck?!
    i_s = (((target->s - (int16_t)current->s) * (int16_t)self->fade_pos) >> 8) + current->s;
    i_l = (((target->l - (int16_t)current->l) * (int16_t)self->fade_pos) >> 8) + current->l;

    ++self->fade_pos;
    // State transistion if time_period has elapsed. Next state: Active/Recovery
    if(!(self->fade_pos < 0xFF)) {
      self->fade_pos = 0;
      self->state = self->state ? FADER_STATE_CYCLIC_RECOVERY : FADER_STATE_ACTIVE;
    }
    // Write to array
    for(int8_t i = 0; i < CONFIG_NUM_PIXELS; ++i) {
      uint8_t _h = i_h + (i - (CONFIG_NUM_PIXELS >> 1)) * (int8_t)self->chain_hue_diff; // i-th hue difference (delta), intended angle overflow
      uint32_t target_color = _led_color_hsl2rgb(_h, i_s, i_l);  // Get target RGB
      vfdco_clr_set_RGB(i, (target_color >> 8) & 0xFF, (target_color >> 16) & 0xFF, target_color & 0xFF);
    }
    // Write to LEDs, physically
    vfdco_clr_render();
  }
  return self->state;
}

/**
 * @brief  Implementation of constructor LED_Color_Fader::LED_Color_Fader
**/
void LED_Color_Fader_Init(
  struct LED_Color_Fader    *f,             // Instance
  uint_fast32_t             timer_interval, // Timer interval
  hsl_t                     color_1,        // Color 1
  hsl_t                     color_2,        // Color 2. Set all components to 0 for single color fade
  int8_t                    chain_hue_diff  // Hue difference between chained pixels
) {
  f->timer = Time_Event_Init(timer_interval);

  // LED Fader Attributes
  f->color_1 = color_1;
  f->color_2 = color_2;
  f->chain_hue_diff = chain_hue_diff;

  // Method mapping
  LED_Color_Fader_Next = (color_2.h == 0 && color_2.s == 0 && color_2.l == 0) ? _LED_Color_Fader_NextColorLinSingle : _LED_Color_Fader_NextColorLin;

  f->fade_pos = 0;
  f->state = FADER_STATE_ACTIVE;
}

/** Begin of:
 * @tableofcontents SECTION_LIGHT_PATTERN_CONSTANTS
 * @brief Definition of all constants used in Light Patterns. Customizable constants are defined in vfdco_config.h
**/
// Hello Messages
// Preprocessor abuse. Generate synchronous arrays with static const char Messages_Hello_IDENTIFIER[6] = {_A, _B, _C, _D, _E, F};
#define GENERATE_HELLO_PAIR(ENTRY) \
  ENTRY(Static,     ' ', 'C', 'O', 'L', 'O', 'R') \
  ENTRY(Spectrum,   ' ', ' ', 'F', 'A', 'D', 'E') \
  ENTRY(Rainbow,    'R', 'A', 'I', 'N', 'B', 'O') \
  ENTRY(Chase,      ' ', 'C', 'H', 'A', 'S', 'E') \
  ENTRY(Music,      ' ', 'D', 'A', 'N', 'C', 'E') \
  ENTRY(Time_Code,  'T', 'I', 'C', 'O', 'D', 'E') \
  ENTRY(Cop,        'P', 'O', 'L', 'I', 'C', 'E') \
  ENTRY(Bliss,      ' ', 'B', 'L', 'I', 'S', 'S')

#define GENERATE_HELLO_STRING(_IDENTIFIER, _A, _B, _C, _D, _E, _F) static const char Messages_Hello_ ## _IDENTIFIER[6] = {_A, _B, _C, _D, _E, _F};
GENERATE_HELLO_PAIR(GENERATE_HELLO_STRING)

// Static Constants
#define   NUM_STATIC_COLOR_SPECIAL    3
#define   NUM_STATIC_COLOR_HUES       8
#define   NUM_STATIC_COLOR_RAINBOWS   2
#define   NUM_STATIC_COLOR_PRESETS    3
#define   NUM_STATIC_T1               (NUM_STATIC_COLOR_SPECIAL)
#define   NUM_STATIC_T2               (NUM_STATIC_T1 + NUM_STATIC_COLOR_HUES)
#define   NUM_STATIC_T3               (NUM_STATIC_T2 + NUM_STATIC_COLOR_RAINBOWS)
#define   NUM_STATIC_T4               (NUM_STATIC_T3 + NUM_STATIC_COLOR_PRESETS)
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

// Spectrum, Rainbow & Chase Constants
enum {LIGHTNESS_H  = CONFIG_LIGHTNESS_HIGH,   LIGHTNESS_M  = CONFIG_LIGHTNESS_MEDIUM,   LIGHTNESS_L  = CONFIG_LIGHTNESS_LOW    };
enum {SATURATION_H = CONFIG_SATURATION_HIGH,   SATURATION_M = CONFIG_SATURATION_MEDIUM,  SATURATION_L = CONFIG_SATURATION_LOW   };
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

// Time Code Constants
// ---- LED Resistor preset GRB     0: Off     1: Brown        2: Red       3: Orange     4: Yellow      5: Green     6: Blue      7: Purple      8: Gray       9: White
static const uint8_t Time_Code_Colors[][3] =  {{0, 0, 0}, {128, 255, 64}, {0, 255, 0}, {30, 255, 0}, {125, 255, 0}, {255, 0, 0}, {0, 0, 255}, {0, 200, 255}, {40, 40, 60}, {255, 255, 255}};

// Moments Of Bliss Constants
#define LIGHTS_BLISS_MAXMOMENTS 6
static const uint8_t MomentsOfBliss_Colors[LIGHTS_BLISS_MAXMOMENTS][7] = { // (C) The VFD Collective
  // Dev: SECTION_LIGHT_PATTERN_MOMENTSOFBLISS
  // D+: Hue difference between LEDs. Random number between 0..2^(D+), 4x MSB
  // T: Peak differene tolerance. Random bumber between 0..2^(T), 4x LSB
// C1H, C1S, C1L, C2H, C2S, C2L,  D+|T
  {105, 255, 100, 190, 255, 110, 0x42}, // Nordlicht.          Ink: fluorescent green to teal, touches of purple
  // {140, 150, 127, 150, 180, 127, 0x42}, // Schneesturm.        Ink: acrylic. dark midnight blue, shades of gray and cold white
  {160, 255, 132, 230, 255, 132, 0x53}, // Frühlingspastell.   Ink: pastel. cherry, pink, some light green and rarely drip of light blue
  { 50, 255, 127, 155, 255, 100, 0x52}, // Hummelhonig.        Ink: highly saturated green and blue gradients
  {160, 255, 100, 170, 255, 200, 0x52}, // Meeresgeflüster.    Ink: watercolor. light sky blue to turquoise, with white sparks and rarely some yellow
  {  0, 255, 120,  35, 255, 120, 0x32}, // Herbstlagerfeuer.   Ink: acrylic. lots of orange and strong yellow tones. rarely some green and brick red
  {235, 255, 127,   5, 255, 127, 0x42}, // Abendhimmel.        Ink: strong red. every warm red tone, some orange, some magenta
};
static const char Messages_Color_Bliss[][CONFIG_NUM_DIGITS] = {
  {'A', 'U', 'R', 'O', 'R', 'A'},
  // {'S', 'C', 'H', 'N', 'E', 'E'},
  {'P', 'A', 'S', 'T', 'E', 'L'},
  {' ', 'H', 'O', 'N', 'I', 'G'},
  {' ', 'O', 'C', 'E', 'A', 'N'},
  {' ', 'F', 'E', 'U', 'E', 'R'},
  {' ', 'A', 'B', 'E', 'N', 'D'}
};


/** Begin of:
  * @tableofcontents SECTION_LIGHT_PATTERN
  */
/** Begin of:
  * @tableofcontents SECTION_LIGHT_PATTERN_STATIC
 **/
/**
  * @brief  Implementation of virtual function Light_Pattern_Static::Update (static void _Light_Pattern_Static_Update)
 **/
static void _Light_Pattern_Static_Update(Light_Pattern *unsafe_self) {
  struct Light_Pattern_Static *self = (struct Light_Pattern_Static *)unsafe_self;
  if(Time_Event_Update(&self->t)) {
    vfdco_clr_minimize_difference(self->target_arr);
    vfdco_clr_render();
  }
}

static void _Light_Pattern_Static_Next_Color(struct Light_Pattern_Static *self, uint8_t reverse) {
  if(self->position >= NUM_STATIC_T4) self->position = 0;

  if(self->position < NUM_STATIC_T1) {
    // Single Color Special
    switch(self->position) {
      #if CONFIG_USE_RGBW == 1
      case 0: vfdco_clr_target_all_RGB   (self->target_arr,   0,   0,   0     ); break;
      case 1: vfdco_clr_target_all_RGBW  (self->target_arr,   0,   0,  96, 255); break;
      case 2: vfdco_clr_target_all_RGBW  (self->target_arr,   0,   0,   0, 255); break;
      #else
      case 0: vfdco_clr_target_all_RGB   (self->target_arr,   0,   0,   0     ); break;
      case 1: vfdco_clr_target_all_RGBW  (self->target_arr, 255, 255, 255,   0); break;
      case 2: vfdco_clr_target_all_RGBW  (self->target_arr, 255, 200,  32,   0); break;
      #endif
    }
  } else if(self->position < NUM_STATIC_T2) {
    // Single Color
    uint32_t target_color = _led_color_hsl2rgb(Static_Color_Hues[self->position - NUM_STATIC_T1], 255, 127);
    vfdco_clr_target_all_RGB(self->target_arr, (target_color >> 8) & 0xFF, (target_color >> 16) & 0xFF, target_color & 0xFF);

  } else { // < T4
    // Multicolor, just use legacy colors, they are hand crafted and look better
    uint8_t t_pos = self->position - NUM_STATIC_T2;
    for(uint8_t i = 0; i < CONFIG_NUM_PIXELS; ++i) {
      uint32_t access_index = reverse ? 4 * (CONFIG_NUM_PIXELS - i - 1) : 4 * i;
      vfdco_clr_target_RGB(
        self->target_arr + access_index/*4 * i*/, 
        Static_Color_Rainbows[t_pos][3 * i + 1], 
        Static_Color_Rainbows[t_pos][3 * i], 
        Static_Color_Rainbows[t_pos][3 * i + 2]
      );
    }
  }
}

/**
  * @brief  Implementation of virtual function Light_Pattern_Static::F3 (static void _Light_Pattern_Static_F3)
 **/
static void _Light_Pattern_Static_F3(Light_Pattern *unsafe_self) {
  struct Light_Pattern_Static *self = (struct Light_Pattern_Static *)unsafe_self;
  self->position++;
  _Light_Pattern_Static_Next_Color(self, 0);

  char k[CONFIG_NUM_DIGITS] = {'C', ' '};
  for(uint_fast8_t i = 0; i < 4; ++i) k[i + 2] = Messages_Color_Static[self->position][i];
  vfdco_display_render_message(k, 0, CONFIG_MESSAGE_SHORT);
}

/**
  * @brief  Implementation of virtual function Light_Pattern_Static::F3Var (static void _Light_Pattern_Static_F3Var)
 **/
static void _Light_Pattern_Static_F3Var(Light_Pattern *unsafe_self) {
  struct Light_Pattern_Static *self = (struct Light_Pattern_Static *)unsafe_self;
  if(self->position >= NUM_STATIC_T2) {
    _Light_Pattern_Static_Next_Color(self, 1);

    char k[CONFIG_NUM_DIGITS] = {'R', 'E', 'V', 'E', 'R', 'S'};
    vfdco_display_render_message(k, 0, CONFIG_MESSAGE_SHORT);
  }
}

/**
  * @brief  Implementation of virtual function Light_Pattern_Static::Hello (static void _Light_Pattern_Static_Hello)
 **/
static void _Light_Pattern_Static_Hello(void) {
  vfdco_display_render_message(Messages_Hello_Static, 0, CONFIG_MESSAGE_LONG);
}

/**
  * @brief  Implementation of virtual function Light_Pattern_Static::Save (static void _Light_Pattern_Static_Save)
 **/
static void _Light_Pattern_Static_Save(Light_Pattern *unsafe_self) {
  struct Light_Pattern_Static *self = (struct Light_Pattern_Static *)unsafe_self;
  self->settings[LIGHT_PATTERN_SETTING_STATIC_position] = self->position;
}

/**
  * @brief  Constructor of Light_Pattern_Static class
 **/
void Light_Pattern_Static_Init(struct Light_Pattern_Static *self, uint8_t *settings) {
  memset(self->target_arr, 0, 4 * CONFIG_NUM_PIXELS);

  // Default loading if saved value is rubbish, then load by assignment
  /* if(settings[LIGHT_PATTERN_SETTING_STATIC_position] >= NUM_STATIC_T4) Light_Pattern_Static_Default(settings); */
  self->position = settings[LIGHT_PATTERN_SETTING_STATIC_position];
  // Fill target array
  _Light_Pattern_Static_Next_Color(self, 0);

  self->t = Time_Event_Init(CONFIG_SINGLE_COLOR_FADE_SPEED);
  self->settings = settings;

  Light_Pattern_F3 = _Light_Pattern_Static_F3;
  Light_Pattern_F3Var = _Light_Pattern_Static_F3Var;
  Light_Pattern_Update = _Light_Pattern_Static_Update;
  Light_Pattern_Hello = _Light_Pattern_Static_Hello;
  Light_Pattern_Save = _Light_Pattern_Static_Save;
}




static void _Light_Pattern_Serial0_Update(Light_Pattern *unsafe_self) {
  struct Light_Pattern_Serial0 *self = (struct Light_Pattern_Serial0 *)unsafe_self;
  if(Time_Event_Update(&self->t)) {
    // Repeated write
    uint8_t *clr_arr = self->settings;
    for(uint8_t i = 0; i < CONFIG_NUM_PIXELS; ++i)
      vfdco_clr_set_RGBW(i, clr_arr[4 * i], clr_arr[4 * i + 1], clr_arr[4 * i + 2], clr_arr[4 * i + 3]);
    vfdco_clr_render();
  }
}
static void _Light_Pattern_Serial0_Save(Light_Pattern *unsafe_self) {
  (void)unsafe_self;
  return; // No save needed. Already done.
}
void Light_Pattern_Serial0_Init(struct Light_Pattern_Serial0 *self, uint8_t *settings) {
  self->t = Time_Event_Init(CONFIG_SINGLE_COLOR_FADE_SPEED);
  self->settings = settings; // Write into settings directly

  Light_Pattern_F3 = NULL;
  Light_Pattern_F3Var = NULL;
  Light_Pattern_Update = _Light_Pattern_Serial0_Update;
  Light_Pattern_Hello = NULL;
  Light_Pattern_Save = _Light_Pattern_Serial0_Save;
}

static void _Light_Pattern_Serial1_Update(Light_Pattern *unsafe_self) {
  struct Light_Pattern_Serial1 *self = (struct Light_Pattern_Serial1 *)unsafe_self;
  if(Time_Event_Update(&self->t)) {
    vfdco_clr_minimize_difference(self->target_arr);
    vfdco_clr_render();
  }
}
static void _Light_Pattern_Serial1_Save(Light_Pattern *unsafe_self) {
  (void)unsafe_self;
  return;
}
void Light_Pattern_Serial1_Init(struct Light_Pattern_Serial1 *self, uint8_t *settings) {
  for(uint8_t i = 0; i < CONFIG_NUM_PIXELS; ++i)
    vfdco_clr_target_RGBW(self->target_arr + 4 * i, settings[4 * i], settings[4 * i + 1], settings[4 * i + 2], settings[4 * i + 3]);
  
  self->t = Time_Event_Init(CONFIG_SINGLE_COLOR_FADE_SPEED);
  self->settings = settings;

  Light_Pattern_F3 = NULL;
  Light_Pattern_F3Var = NULL;
  Light_Pattern_Update = _Light_Pattern_Serial1_Update;
  Light_Pattern_Hello = NULL;
  Light_Pattern_Save = _Light_Pattern_Serial1_Save;
}




/** Begin of:
  * @tableofcontents SECTION_LIGHT_PATTERN_SPECTRUM
 **/
/**
  * @brief  Implementation of virtual function Light_Pattern_Spectrum::Update (static void _Light_Pattern_Spectrum_Update)
 **/
static void _Light_Pattern_Spectrum_Update(Light_Pattern *unsafe_self) {
  struct Light_Pattern_Spectrum *self = (struct Light_Pattern_Spectrum *)unsafe_self;
  LED_Color_Fader_Next(&self->spectrum_fader);
}

/**
* @brief  Implementation of virtual function Light_Pattern_Spectrum::F3 (static void _Light_Pattern_Spectrum_F3)
**/
static void _Light_Pattern_Spectrum_F3(Light_Pattern *unsafe_self) {
  struct Light_Pattern_Spectrum *self = (struct Light_Pattern_Spectrum *)unsafe_self;
  hsl_t *_color = &self->spectrum_fader.color_1;

  char k[CONFIG_NUM_DIGITS] = {'B', 'R', 'I', ' ', ' ', 1};
  if(_color->l == LIGHTNESS_M) {
    _color->l = LIGHTNESS_H;
    k[5] = 3;
  }
  else if(_color->l == LIGHTNESS_H) {
    _color->l = LIGHTNESS_L;
  }
  else {
    _color->l = LIGHTNESS_M;
    k[5] = 2;
  }
  vfdco_display_render_message(k, 0b00000100, CONFIG_MESSAGE_SHORT);
}

/**
* @brief  Implementation of virtual function Light_Pattern_Spectrum::F3Var (static void _Light_Pattern_Spectrum_F3Var)
**/
static void _Light_Pattern_Spectrum_F3Var(Light_Pattern *unsafe_self) {
  struct Light_Pattern_Spectrum *self = (struct Light_Pattern_Spectrum *)unsafe_self;
  hsl_t *_color = &self->spectrum_fader.color_1;

  char k[CONFIG_NUM_DIGITS] = {'S', 'A', 'T', ' ', ' ', 1};
  if(_color->s == SATURATION_H) {
    _color->s = SATURATION_L;
  }
  else if(_color->s == SATURATION_L) {
    _color->s = SATURATION_M;
    k[5] = 2;
  }
  else {
    _color->s = SATURATION_H;
    k[5] = 3;
  }
  vfdco_display_render_message(k, 0b00000100, CONFIG_MESSAGE_SHORT);
}

/**
* @brief  Implementation of virtual function Light_Pattern_Spectrum::Hello (static void _Light_Pattern_Spectrum_Hello)
**/
static void _Light_Pattern_Spectrum_Hello(void) {
  vfdco_display_render_message(Messages_Hello_Spectrum, 0, CONFIG_MESSAGE_LONG);
}

/**
  * @brief  Implementation of virtual function Light_Pattern_Spectrum::Save (static void _Light_Pattern_Spectrum_Save)
 **/
static void _Light_Pattern_Spectrum_Save(Light_Pattern *unsafe_self) {
  struct Light_Pattern_Spectrum *self = (struct Light_Pattern_Spectrum *)unsafe_self;
  self->settings[LIGHT_PATTERN_SETTING_SPECTRUM_saturation] = self->spectrum_fader.color_1.s;
  self->settings[LIGHT_PATTERN_SETTING_SPECTRUM_lightness]  = self->spectrum_fader.color_1.l;
}

/**
* @brief  Constructor of Light_Pattern_Spectrum class
**/
void Light_Pattern_Spectrum_Init(struct Light_Pattern_Spectrum *self, uint8_t *settings) {
  // Default loading if saved value is trash, then load by assignment
  /* if(settings[LIGHT_PATTERN_SETTING_SPECTRUM_saturation] == 0 || settings[LIGHT_PATTERN_SETTING_SPECTRUM_lightness] == 0)
   Light_Pattern_Spectrum_Default(settings); */
  // Oh this is like driving a truck out of its garage to pick up a pretzel from a backery 100 ft away
  LED_Color_Fader_Init(
    &self->spectrum_fader,
    CONFIG_SPECTRUM_FADE_SPEED,  // Timer interval
    HSL_Init(0, settings[LIGHT_PATTERN_SETTING_SPECTRUM_saturation], settings[LIGHT_PATTERN_SETTING_SPECTRUM_lightness]),
    HSL_Init(0, 0, 0),
    0                            // Hue difference between chained pixels
  );

  self->spectrum_fader.state = FADER_STATE_ACTIVE;
  self->settings = settings;

  Light_Pattern_F3 = _Light_Pattern_Spectrum_F3;
  Light_Pattern_F3Var = _Light_Pattern_Spectrum_F3Var;
  Light_Pattern_Update = _Light_Pattern_Spectrum_Update;
  Light_Pattern_Hello = _Light_Pattern_Spectrum_Hello;
  Light_Pattern_Save = _Light_Pattern_Spectrum_Save;
}


/** Begin of:
* @tableofcontents SECTION_LIGHT_PATTERN_RAINBOW
**/
/**
* @brief  Implementation of virtual function Light_Pattern_Rainbow::Update (static void _Light_Pattern_Rainbow_Update)
**/
static void _Light_Pattern_Rainbow_Update(Light_Pattern *unsafe_self) {
  struct Light_Pattern_Rainbow *self = (struct Light_Pattern_Rainbow *)unsafe_self;
  LED_Color_Fader_Next(&self->rainbow_fader);
}

/**
* @brief  Implementation of virtual function Light_Pattern_Rainbow::F3 (static void _Light_Pattern_Rainbow_F3)
**/
static void _Light_Pattern_Rainbow_F3(Light_Pattern *unsafe_self) {
  struct Light_Pattern_Rainbow *self = (struct Light_Pattern_Rainbow *)unsafe_self;
  struct LED_Color_Fader *_f = (struct LED_Color_Fader *)&self->rainbow_fader;
  // char message[MESSAGE_LENGTH];
  if(_f->chain_hue_diff == 10) {
    _f->chain_hue_diff = 21;
    vfdco_display_render_message(Messages_Color_Rainbow[1], 0, CONFIG_MESSAGE_SHORT);
  }
  else if(_f->chain_hue_diff == 21) {
    _f->chain_hue_diff = 42;
    vfdco_display_render_message(Messages_Color_Rainbow[2], 0, CONFIG_MESSAGE_SHORT);
  }
  else {
    _f->chain_hue_diff = 10;
    vfdco_display_render_message(Messages_Color_Rainbow[0], 0, CONFIG_MESSAGE_SHORT);
  }
}

/**
* @brief  Implementation of virtual function Light_Pattern_Rainbow::F3Var (static void _Light_Pattern_Rainbow_F3Var)
**/
static void _Light_Pattern_Rainbow_F3Var(Light_Pattern *unsafe_self) {
  struct Light_Pattern_Rainbow *self = (struct Light_Pattern_Rainbow *)unsafe_self;
  hsl_t *_color = &self->rainbow_fader.color_1;
  char k[CONFIG_NUM_DIGITS] = {'S', 'A', 'T', ' ', ' ', 1};
  if(_color->s == SATURATION_H) {
    _color->s = SATURATION_L;
  }
  else if(_color->s == SATURATION_L) {
    _color->s = SATURATION_M;
    k[5] = 2;
  }
  else {
    _color->s = SATURATION_H;
    k[5] = 3;
  }
  vfdco_display_render_message(k, 0b00000100, CONFIG_MESSAGE_SHORT);
}

/**
* @brief  Implementation of virtual function Light_Pattern_Rainbow::Hello (static void _Light_Pattern_Rainbow_Hello)
**/
static void _Light_Pattern_Rainbow_Hello(void) {
  vfdco_display_render_message(Messages_Hello_Rainbow, 0, CONFIG_MESSAGE_LONG);
}

/**
  * @brief  Implementation of virtual function Light_Pattern_Rainbow::Save (static void _Light_Pattern_Rainbow_Save)
 **/
static void _Light_Pattern_Rainbow_Save(Light_Pattern *unsafe_self) {
  struct Light_Pattern_Rainbow *self = (struct Light_Pattern_Rainbow *)unsafe_self;
  self->settings[LIGHT_PATTERN_SETTING_RAINBOW_chain_hue_diff]  = (uint8_t)self->rainbow_fader.chain_hue_diff;
  self->settings[LIGHT_PATTERN_SETTING_RAINBOW_saturation]      = self->rainbow_fader.color_1.s;
}

/**
* @brief  Constructor of Light_Pattern_Rainbow class
**/
void Light_Pattern_Rainbow_Init(struct Light_Pattern_Rainbow *self, uint8_t *settings) {
  // Default loading if saved value is garbage, then load by assignment
  /* if((settings[LIGHT_PATTERN_SETTING_RAINBOW_saturation] == 0) || settings[LIGHT_PATTERN_SETTING_RAINBOW_chain_hue_diff] == 0)
    Light_Pattern_Rainbow_Default(settings); */
  // Oh this is like driving a truck out of its garage to pick up a pretzel from a backery 100 ft away
  LED_Color_Fader_Init(
    &self->rainbow_fader,
    CONFIG_SPECTRUM_FADE_SPEED,  // Timer interval
    HSL_Init(0, settings[LIGHT_PATTERN_SETTING_RAINBOW_saturation], LIGHTNESS_H),
    HSL_Init(0, 0, 0),
    (int8_t)settings[LIGHT_PATTERN_SETTING_RAINBOW_chain_hue_diff] // Hue difference between chained pixels
  );

  self->rainbow_fader.state = FADER_STATE_ACTIVE;
  self->settings = settings;

  Light_Pattern_F3 = _Light_Pattern_Rainbow_F3;
  Light_Pattern_F3Var = _Light_Pattern_Rainbow_F3Var;
  Light_Pattern_Update = _Light_Pattern_Rainbow_Update;
  Light_Pattern_Hello = _Light_Pattern_Rainbow_Hello;
  Light_Pattern_Save = _Light_Pattern_Rainbow_Save;
}


/** Begin of:
* @tableofcontents SECTION_LIGHT_PATTERN_CHASE
**/
/**
* @brief  Implementation of virtual function Light_Pattern_Chase::Update (static void _Light_Pattern_Chase_Update)
**/
static void _Light_Pattern_Chase_Update(Light_Pattern *unsafe_self) {
  struct Light_Pattern_Chase *self = (struct Light_Pattern_Chase *)unsafe_self;
  if(Time_Event_Update(&self->update_timer)) {
    if(self->state < CONFIG_NUM_PIXELS) {
      // Write to next color
      for(uint_fast8_t i = 0; i <= self->state; ++i) {
        int16_t i_h = i * self->color_peak_diff;
        uint32_t target_color = _led_color_hsl2rgb(self->color_pos + i_h, SATURATION_H, LIGHTNESS_H);
        uint8_t access_idx = i;
        if(self->chase_mode == 1 || (self->chase_mode == 2 && !(self->flip_timer->s & 0x01))) access_idx = CONFIG_NUM_PIXELS - i - 1;
        vfdco_clr_set_RGB(access_idx, (target_color >> 8) & 0xFF, (target_color >> 16) & 0xFF, target_color & 0xFF);
      }
      vfdco_clr_render();
    }
    self->state++;

    if(self->flip_timer->s != self->flip_timer_previous_second) {
      self->flip_timer_previous_second = self->flip_timer->s;
      self->state = 0;                                        // Reset state machine
      self->color_pos += 22;                                  // Let it overflow and get different values.
    }
  }
 }

/**
* @brief  Implementation of virtual function Light_Pattern_Chase::F3 (static void _Light_Pattern_Chase_F3)
* @details  Switches between L-R, R-L and second flip mode
**/
static void _Light_Pattern_Chase_F3(Light_Pattern *unsafe_self) {
  struct Light_Pattern_Chase *self = (struct Light_Pattern_Chase *)unsafe_self;
  if(self->chase_mode == 0) { // L->R to R->L
    self->chase_mode = 1;
  } else if(self->chase_mode == 1) { // To flipper
    self->chase_mode = 2;
  } else {
    self->chase_mode = 0;
  }
  char k[CONFIG_NUM_DIGITS] = {' ', ' '};
  for(uint_fast8_t i = 0; i < 4; ++i) k[i + 2] = Messages_Color_Chase[self->chase_mode][i];
  vfdco_display_render_message(k, 0, CONFIG_MESSAGE_SHORT);
}

/**
* @brief  Implementation of virtual function Light_Pattern_Chase::F3Var (static void _Light_Pattern_Chase_F3Var)
**/
static void _Light_Pattern_Chase_F3Var(Light_Pattern *unsafe_self) {
  struct Light_Pattern_Chase *self = (struct Light_Pattern_Chase *)unsafe_self;

  char k[CONFIG_NUM_DIGITS] = {'G', 'R'};
  if(self->color_peak_diff == 0) {
    self->color_peak_diff = 6;
    for(uint_fast8_t i = 0; i < 4; ++i) k[i + 2] = Messages_Color_Chase[3][i];
  } else {
    self->color_peak_diff = 0;
    for(uint_fast8_t i = 0; i < 4; ++i) k[i + 2] = Messages_Color_Chase[4][i];
  }
  vfdco_display_render_message(k, 0, CONFIG_MESSAGE_SHORT);
}

/**
* @brief  Implementation of virtual function Light_Pattern_Chase::Hello (static void _Light_Pattern_Chase_Hello)
**/
static void _Light_Pattern_Chase_Hello(void) {
  vfdco_display_render_message(Messages_Hello_Chase, 0, CONFIG_MESSAGE_LONG);
}

/**
  * @brief  Implementation of virtual function Light_Pattern_Chase::Save (static void _Light_Pattern_Chase_Save)
 **/
static void _Light_Pattern_Chase_Save(Light_Pattern *unsafe_self) {
  struct Light_Pattern_Chase *self = (struct Light_Pattern_Chase *)unsafe_self;
  self->settings[LIGHT_PATTERN_SETTING_CHASE_chase_mode]      = self->chase_mode;
  self->settings[LIGHT_PATTERN_SETTING_CHASE_color_peak_diff] = self->color_peak_diff;
}

/**
* @brief  Constructor of Light_Pattern_Chase class
**/
void Light_Pattern_Chase_Init(struct Light_Pattern_Chase *self, vfdco_time_t *time, uint8_t *settings) {
  // Default loading if saved value is waste, then load by assignment
  /* if((settings[LIGHT_PATTERN_SETTING_CHASE_chase_mode] >= 3) || (settings[LIGHT_PATTERN_SETTING_CHASE_color_peak_diff] > 6)) 
    Light_Pattern_Chase_Default(settings); */

  self->update_timer = Time_Event_Init(CONFIG_CHASE_FADE_SPEED);
  self->flip_timer = time;
  self->chase_mode = settings[LIGHT_PATTERN_SETTING_CHASE_chase_mode];
  self->color_pos = 0;
  self->color_peak_diff = settings[LIGHT_PATTERN_SETTING_CHASE_color_peak_diff];
  self->state = 0;
  self->settings = settings;

  Light_Pattern_F3 = _Light_Pattern_Chase_F3;
  Light_Pattern_F3Var = _Light_Pattern_Chase_F3Var;
  Light_Pattern_Update = _Light_Pattern_Chase_Update;
  Light_Pattern_Hello = _Light_Pattern_Chase_Hello;
  Light_Pattern_Save = _Light_Pattern_Chase_Save;
}


/** Begin of:
* @tableofcontents SECTION_LIGHT_PATTERN_MUSIC
**/
/**
* @brief  Implementation of virtual function Light_Pattern_MUSIC::Update (static void _Light_Pattern_Music_Update)
**/
static void _Light_Pattern_Music_Update(Light_Pattern *unsafe_self) {
  struct Light_Pattern_Music *self = (struct Light_Pattern_Music *)unsafe_self;
  if(Time_Event_Update(&self->state_timer)) {
    // If interval passed, decrease turned on LEDs by one (regular state update)
    if(self->state < 7) ++self->state;
    
    // Rainbow fade position update
    ++self->color_pos_timer;
    if(self->color_pos_timer == 3) {
        self->color_pos_timer = 0;
        ++self->color_pos; // Intended overflow
    }

    uint8_t mic_read_in = 6 - vfdco_mic_read_level();

    if(self->state >= mic_read_in) {
      self->state = mic_read_in;
      self->delay_state = 0;
    } else {
      // Update delay timer
      ++self->delay_timer;
      if(self->delay_timer == 13) {
          self->delay_timer = 0;
          self->delay_state = 1;
      }
    }

    if(self->state < 7) {
      for(int8_t i = 0; i < CONFIG_NUM_PIXELS; ++i) {
        if(i < CONFIG_NUM_PIXELS - self->state) {
          uint8_t _h = self->color_pos + i * self->color_peak_diff; // i-th hue difference (delta), intended angle overflow
          uint32_t target_color = _led_color_hsl2rgb(_h, self->saturation, LIGHTNESS_H);  // Get target RGB
          vfdco_clr_set_RGB(i, (target_color >> 8) & 0xFF, (target_color >> 16) & 0xFF, target_color & 0xFF);
        } else {
          vfdco_clr_set_RGB(i, 0, 0, 0); // Black out
        }
      }

      if(self->delay_state) {
        if(self->state < CONFIG_NUM_PIXELS) {
          for(uint8_t i = 0; i < (CONFIG_NUM_PIXELS - 1 - self->state); ++i)
            vfdco_clr_set_RGB(i, 0, 0, 0);
        }
      }
    }
    
    vfdco_clr_render();
  }
 }

/**
* @brief  Implementation of virtual function Light_Pattern_Music::F3 (static void _Light_Pattern_Music_F3)
* @details  Switches between L-R, R-L and second flip mode
**/
static void _Light_Pattern_Music_F3(Light_Pattern *unsafe_self) {
  struct Light_Pattern_Music *self = (struct Light_Pattern_Music *)unsafe_self;
  if(self->color_peak_diff == 10) {
    self->color_peak_diff = 21;
    vfdco_display_render_message(Messages_Color_Rainbow[1], 0, CONFIG_MESSAGE_SHORT);
  }
  else if(self->color_peak_diff == 21) {
    self->color_peak_diff = 42;
    vfdco_display_render_message(Messages_Color_Rainbow[2], 0, CONFIG_MESSAGE_SHORT);
  }
  else {
    self->color_peak_diff = 10;
    vfdco_display_render_message(Messages_Color_Rainbow[0], 0, CONFIG_MESSAGE_SHORT);
  }
}

/**
* @brief  Implementation of virtual function Light_Pattern_Music::F3Var (static void _Light_Pattern_Music_F3Var)
**/
static void _Light_Pattern_Music_F3Var(Light_Pattern *unsafe_self) {
  struct Light_Pattern_Music *self = (struct Light_Pattern_Music *)unsafe_self;
  char k[CONFIG_NUM_DIGITS] = {'S', 'A', 'T', ' ', ' ', 1};
  if(self->saturation == SATURATION_H) {
    self->saturation = SATURATION_L;
  }
  else if(self->saturation == SATURATION_L) {
    self->saturation = SATURATION_M;
    k[5] = 2;
  }
  else {
    self->saturation = SATURATION_H;
    k[5] = 3;
  }
  vfdco_display_render_message(k, 0b00000100, CONFIG_MESSAGE_SHORT);
}

/**
* @brief  Implementation of virtual function Light_Pattern_Music::Hello (static void _Light_Pattern_Music_Hello)
**/
static void _Light_Pattern_Music_Hello(void) {
  vfdco_display_render_message(Messages_Hello_Music, 0, CONFIG_MESSAGE_LONG);
}

/**
  * @brief  Implementation of virtual function Light_Pattern_Music::Save (static void _Light_Pattern_Music_Save)
 **/
static void _Light_Pattern_Music_Save(Light_Pattern *unsafe_self) {
  struct Light_Pattern_Music *self = (struct Light_Pattern_Music *)unsafe_self;
  self->settings[LIGHT_PATTERN_SETTING_MUSIC_color_peak_diff] = self->color_peak_diff;
  self->settings[LIGHT_PATTERN_SETTING_MUSIC_saturation]      = self->saturation;
}

/**
* @brief  Constructor of Light_Pattern_Music class
**/
void Light_Pattern_Music_Init(struct Light_Pattern_Music *self, uint8_t *settings) {
  // Default loading if saved value is waste, then load by assignment
  /* if((settings[LIGHT_PATTERN_SETTING_MUSIC_saturation] == 0) || settings[LIGHT_PATTERN_SETTING_MUSIC_color_peak_diff] == 0)
    Light_Pattern_Music_Default(settings); */

  self->state_timer = Time_Event_Init(20);
  self->color_pos_timer = 0; // 3x multiplicator (60 ms)
  self->delay_timer = 0; // 12x multiplicator (240 ms)
  self->color_pos = 0;
  self->color_peak_diff = settings[LIGHT_PATTERN_SETTING_MUSIC_color_peak_diff];
  self->saturation = settings[LIGHT_PATTERN_SETTING_MUSIC_saturation];
  self->state = 0;
  self->delay_state = 0;

  self->settings = settings;

  Light_Pattern_F3 = _Light_Pattern_Music_F3;
  Light_Pattern_F3Var = _Light_Pattern_Music_F3Var;
  Light_Pattern_Update = _Light_Pattern_Music_Update;
  Light_Pattern_Hello = _Light_Pattern_Music_Hello;
  Light_Pattern_Save = _Light_Pattern_Music_Save;
}


/** Begin of:
  * @tableofcontents SECTION_LIGHT_PATTERN_TIME_CODE
 **/
/**
  * @brief  Implementation of virtual function Light_Pattern_Time_Code::Update (static void _Light_Pattern_Time_Code_Update)
 **/
static void _Light_Pattern_Time_Code_Update(Light_Pattern *unsafe_self) {
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
      vfdco_clr_target_RGB(target_arr + 4 * i, Time_Code_Colors[digit_values[i]][1], Time_Code_Colors[digit_values[i]][0], Time_Code_Colors[digit_values[i]][2]);
    vfdco_clr_minimize_difference(target_arr);
    vfdco_clr_render();
  }
}

/**
  * @brief  Implementation of virtual function Light_Pattern_Time_Code::Hello (static void _Light_Pattern_Time_Code_Hello)
 **/
static void _Light_Pattern_Time_Code_Hello(void) {
  vfdco_display_render_message(Messages_Hello_Time_Code, 0, CONFIG_MESSAGE_LONG);
}

/**
  * @brief  Constructor of Light_Pattern_Time_Code class
 **/
void Light_Pattern_Time_Code_Init(struct Light_Pattern_Time_Code *self, vfdco_time_t *time_instance) {
  memset(self->target_arr, 0, 4 * CONFIG_NUM_PIXELS);
  self->clock = Time_Event_Init(1);
  self->time = time_instance;

  Light_Pattern_F3 = NULL;
  Light_Pattern_F3Var = NULL;
  Light_Pattern_Update = _Light_Pattern_Time_Code_Update;
  Light_Pattern_Hello = _Light_Pattern_Time_Code_Hello;
  Light_Pattern_Save = NULL;
}

/** Begin of:
* @tableofcontents SECTION_LIGHT_PATTERN_COP
**/
/**
* @brief  Implementation of virtual function Light_Pattern_Cop::Update (static void _Light_Pattern_Cop_Update)
**/
static void _Light_Pattern_Cop_Update(Light_Pattern *unsafe_self) {
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
static void _Light_Pattern_Cop_Hello(void) {
  vfdco_display_render_message(Messages_Hello_Cop, 0, CONFIG_MESSAGE_LONG);
}

/**
* @brief  Constructor of Light_Pattern_Cop class Light_Pattern_Cop::Light_Pattern_Cop (static void Light_Pattern_Cop_Init)
**/
void Light_Pattern_Cop_Init(struct Light_Pattern_Cop *self) {
  self->clock = Time_Event_Init(CONFIG_COP_FADE_SPEED);
  self->state = 0;

  Light_Pattern_F3      = NULL;
  Light_Pattern_F3Var   = NULL;
  Light_Pattern_Update  = _Light_Pattern_Cop_Update;
  Light_Pattern_Hello   = _Light_Pattern_Cop_Hello;
  Light_Pattern_Save    = NULL;
}

/** Begin of:
* @tableofcontents SECTION_LIGHT_PATTERN_MOMENTSOFBLISS
**/
/**
* @brief  Implementation of virtual function Light_Pattern_MomentsOfBliss::Update (static void _Light_Pattern_MomentsOfBliss_Update)
**/
static void _Light_Pattern_MomentsOfBliss_Update(Light_Pattern *unsafe_self) {
  struct Light_Pattern_MomentsOfBliss *self = (struct Light_Pattern_MomentsOfBliss *)unsafe_self;

  struct LED_Color_Fader *base = (struct LED_Color_Fader *)&self->base_fader;
  LED_COLOR_STATE_t prev_state = base->state;

  if(LED_Color_Fader_Next(&self->base_fader) == FADER_STATE_CYCLIC_RECOVERY && prev_state == FADER_STATE_ACTIVE) {
    uint8_t bits = MomentsOfBliss_Colors[self->moment][6];
    // Randomize h
    ++self->undrift_counter;
    if(self->undrift_counter < self->undrift_max) {
      // Randomize new hue & hue diff by a pos or neg number biased around 0 by (1<<bits) / 2
      base->color_1.h = base->color_1.h -((1 << (bits & 0x0F)) >> 1) + vfdco_util_random(bits & 0x0F);
      // ((struct LED_Color_Fader *)self->base_fader)->chain_hue_diff = vfdco_util_random(bits >> 4) - ((1 << (bits >> 4)) >> 1);
    } else {
      // Restore hue
      base->color_1.h = MomentsOfBliss_Colors[self->moment][0];
      self->undrift_counter = 0;
      self->undrift_max = vfdco_util_random(2) + 2;
    }

    // Huediff variation, operates independently from color diff
    if(self->undrift_huediff_max > 0) {
      if(base->chain_hue_diff < self->undrift_huediff_max) {
        ++base->chain_hue_diff;
      } else {
        self->undrift_huediff_max = -self->undrift_huediff_max;
        --base->chain_hue_diff;
      }
    }
    else {
      if(base->chain_hue_diff > self->undrift_huediff_max) {
        --base->chain_hue_diff;
      } else {
        self->undrift_huediff_max = -self->undrift_huediff_max;
        ++base->chain_hue_diff;
      }
    }
    if(!base->chain_hue_diff) {
      uint8_t huediff_max = MomentsOfBliss_Colors[self->moment][6] >> 4;
      self->undrift_huediff_max = vfdco_util_random(huediff_max) - ((1 << huediff_max) >> 1) + 1;
    }
  }
}

static void _Light_Pattern_MomentsOfBliss_Remoment(struct Light_Pattern_MomentsOfBliss *self) {
  LED_Color_Fader_Init(
    &self->base_fader,
    CONFIG_MOMENTSOFBLISS_FADE_SPEED,  // Timer interval
    HSL_Init(MomentsOfBliss_Colors[self->moment][0], MomentsOfBliss_Colors[self->moment][1], MomentsOfBliss_Colors[self->moment][2]),
    HSL_Init(MomentsOfBliss_Colors[self->moment][3], MomentsOfBliss_Colors[self->moment][4], MomentsOfBliss_Colors[self->moment][5]),
    0
  );

  self->undrift_max = vfdco_util_random(2) + 2; // some number between 2 and 5 lol
  uint8_t huediff_max = MomentsOfBliss_Colors[self->moment][6] >> 4;
  self->undrift_huediff_max = vfdco_util_random(huediff_max) - ((1 << huediff_max) >> 1);
}

/**
* @brief  Implementation of virtual function Light_Pattern_MomentsOfBliss::F3 (static void _Light_Pattern_MomentsOfBliss_F3)
**/
static void _Light_Pattern_MomentsOfBliss_F3(Light_Pattern *unsafe_self) {
  struct Light_Pattern_MomentsOfBliss *self = (struct Light_Pattern_MomentsOfBliss *)unsafe_self;

  ++self->moment;
  if(!(self->moment < LIGHTS_BLISS_MAXMOMENTS)) self->moment = 0;
  _Light_Pattern_MomentsOfBliss_Remoment(self);

  vfdco_display_render_message(Messages_Color_Bliss[self->moment], 0, CONFIG_MESSAGE_SHORT);
}

/**
* @brief  Implementation of virtual function Light_Pattern_MomentsOfBliss::Hello (static void _Light_Pattern_MomentsOfBliss_Hello)
**/
static void _Light_Pattern_MomentsOfBliss_Hello(void) {
  vfdco_display_render_message(Messages_Hello_Bliss, 0, CONFIG_MESSAGE_LONG);
}

/**
  * @brief  Implementation of virtual function Light_Pattern_MomentsOfBliss::Save (static void _Light_Pattern_MomentsOfBliss_Save)
 **/
static void _Light_Pattern_MomentsOfBliss_Save(Light_Pattern *unsafe_self) {
  struct Light_Pattern_MomentsOfBliss *self = (struct Light_Pattern_MomentsOfBliss *)unsafe_self;
  self->settings[LIGHT_PATTERN_SETTING_BLISS_moment] = self->moment;
}

/**
* @brief  Constructor of Light_Pattern_MomentsOfBliss class Light_Pattern_MomentsOfBliss::Light_Pattern_MomentsOfBliss (static void Light_Pattern_MomentsOfBliss_Init)
**/
void Light_Pattern_MomentsOfBliss_Init(struct Light_Pattern_MomentsOfBliss *self, uint8_t *settings) {
  // Default loading if saved value is junk, then load by assignment
  // if(settings[LIGHT_PATTERN_SETTING_BLISS_moment] >= LIGHTS_BLISS_MAXMOMENTS) Light_Pattern_MomentsOfBliss_Default(settings);

  self->moment = settings[LIGHT_PATTERN_SETTING_BLISS_moment];
  _Light_Pattern_MomentsOfBliss_Remoment(self);
  self->settings = settings;

  Light_Pattern_F3      = _Light_Pattern_MomentsOfBliss_F3;
  Light_Pattern_F3Var   = NULL;
  Light_Pattern_Update  = _Light_Pattern_MomentsOfBliss_Update;
  Light_Pattern_Hello   = _Light_Pattern_MomentsOfBliss_Hello;
  Light_Pattern_Save    = _Light_Pattern_MomentsOfBliss_Save;
}


/** Begin of:
  * @tableofcontents SECTION_CONTAINER_LIGHT_PATTERN
**/
void Container_Light_Pattern_Clear(Light_Pattern *self) {
  memset(self, 0, sizeof(Light_Pattern));
}


/** Begin of:
 * @tableofcontents SECTION_SUPPORTING_FUNCTIONS_IMPLEMENTATION
 **/
/**
 * @brief Fast hsl2rgb algorithm: https://stackoverflow.com/questions/13105185/fast-algorithm-for-rgb-hsl-conversion
 * @param h 8 Bit Hue Value
 * @param s 8 Bit Saturation Value
 * @param l 8 Bit Lightness Value
 * @return uint32_t Output RGB value aligned as: |0x00|G|R|B|
 */
static uint32_t _led_color_hsl2rgb(uint8_t h, uint8_t s, uint8_t l) {
  if(l == 0) return 0;

  uint8_t  r, g, b, lo, c, x, m;
  uint16_t h1, l1, H;
  l1 = l + 1;
  if (l < 128)    c = ((l1 << 1) * s) >> 8;
  else            c = (512 - (l1 << 1)) * s >> 8;

  H = h * 6;              // 0 to 1535 (actually 1530)
  lo = H & 255;           // Low byte  = primary/secondary color mix
  h1 = lo + 1;

  if ((H & 256) == 0)   x = h1 * c >> 8;          // even sextant, like red to yellow
  else                  x = (256 - h1) * c >> 8;  // odd sextant, like yellow to green

  m = l - (c >> 1);
  switch(H >> 8) {       // High byte = sextant of colorwheel
    case 0 : r = c; g = x; b = 0; break; // R to Y
    case 1 : r = x; g = c; b = 0; break; // Y to G
    case 2 : r = 0; g = c; b = x; break; // G to C
    case 3 : r = 0; g = x; b = c; break; // C to B
    case 4 : r = x; g = 0; b = c; break; // B to M
    default: r = c; g = 0; b = x; break; // M to R
  }

  return (((uint32_t)r + m) << 8) | (((uint32_t)g + m) << 16) | ((uint32_t)b + m);
}

/*
Waiting for your call and for the mood
to release me from the longest afternoon
*/
