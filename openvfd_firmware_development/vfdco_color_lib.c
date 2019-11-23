/**
  ******************************************************************************
  * @file     vfdco_color_lib.c
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     13-November-2019
  * @brief    This file contains definitions of a color library written for WS2812B/SK6812
  *           Designed to be used with Fluorescence by The VFD Collective
  ******************************************************************************
  * @toc      Table of contents, enter to navigate:
  *           SECTION_DRIVER_INTERFACE
  *           SECTION_DRIVER_INITIALIZE
  *           SECTION_COLOR_DEFINITIONS
  *             -SUBSECTION_HSL
  *             -SUBSECTION_COLOR_PEAK
  *             -SUBSECTION_COLOR_FADER
  *           SECTION_LED_COLOR_MODE
  ******************************************************************************
 **/

#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "vfdco_color_lib.h"

// Fast hsl2rgb algorithm: https://stackoverflow.com/questions/13105185/fast-algorithm-for-rgb-hsl-conversion
/*static */uint32_t _led_color_hsl2rgb(uint8_t h, uint8_t s, uint8_t l) {
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

    return ((r + m) << 8) | ((g + m) << 16) | (b + m);
}

#define GAUSSIAN_MAGIC_NUMBER 0.424660900144f

static float box_muller_transform(float mu, float sigma) {
  static float t = 0.0f;
  double x, w1, w2, r;

  if(t == 0.0f) {
    do {
      w1 = 2.0f * rand() * (1.0f / RAND_MAX) - 1.0f;
      w2 = 2.0f * rand() * (1.0f / RAND_MAX) - 1.0f;
      r = w1 * w1 + w2 * w2;
    } while(r >= 1.0f);
    r = sqrt(-2.0f * logf(r) / r);
    t = w2 * r;
    return(mu + w1 * r * sigma);
  } else {
    x = t;
    t = .0f;
    return (mu + x * sigma);
  }
}

/** Begin of:
  * @toc SUBSECTION_HSL
 **/
hsl_t *HSL_Init(uint8_t h, uint8_t s, uint8_t l) {
  hsl_t *hsl = (hsl_t *)malloc(sizeof(hsl_t));
  hsl->h = h;
  hsl->s = s;
  hsl->l = l;
  return hsl;
}

hsl_t *HSL_Init_Range(uint8_t h, uint8_t s, uint8_t l, float dh, float ds, float dl) {
  hsl_t *hsl = (hsl_t *)malloc(sizeof(hsl_t));
  // Get randomized color
  hsl->h = fmodf(box_muller_transform(h, GAUSSIAN_MAGIC_NUMBER * dh), 255.0f);
  hsl->s = fmodf(box_muller_transform(h, GAUSSIAN_MAGIC_NUMBER * ds), 255.0f);
  hsl->l = fmodf(box_muller_transform(h, GAUSSIAN_MAGIC_NUMBER * dl), 255.0f);
  return hsl;
}

static inline LED_COLOR_STATE_t _LED_Color_Next(const struct LED_Color *unsafe_self) {
  return unsafe_self->VTable.Next(unsafe_self);
}
static inline void _LED_Color_Delete(const struct LED_Color *unsafe_self) {
  unsafe_self->VTable.Delete(unsafe_self);
}

void LED_Color_Init(struct LED_Color *f) {
  f->Next = _LED_Color_Next;
  f->Delete = _LED_Color_Delete;
}

/** Begin of:
  * @toc SUBSECTION_COLOR_FADER
 **/
static inline void _blend_normal(uint8_t i, uint8_t br, uint8_t bg, uint8_t bb) {
  rgb_arr[num_bpp * i    ] = bg;
  rgb_arr[num_bpp * i + 1] = br;
  rgb_arr[num_bpp * i + 2] = bb;
}
static inline void _blend_mup(uint8_t i, uint8_t br, uint8_t bg, uint8_t bb) {
  rgb_arr[num_bpp * i    ] = ((uint16_t)bg * rgb_arr[num_bpp * i    ]) >> 8;
  rgb_arr[num_bpp * i + 1] = ((uint16_t)br * rgb_arr[num_bpp * i + 1]) >> 8;
  rgb_arr[num_bpp * i + 2] = ((uint16_t)bb * rgb_arr[num_bpp * i + 2]) >> 8;
}
static inline void _blend_scr(uint8_t i, uint8_t br, uint8_t bg, uint8_t bb) {
  rgb_arr[num_bpp * i    ] = (uint16_t)((255 - bg) * (uint16_t)(255 - rgb_arr[num_bpp * i    ])) >> 8;
  rgb_arr[num_bpp * i + 1] = (uint16_t)((255 - bg) * (uint16_t)(255 - rgb_arr[num_bpp * i + 1])) >> 8;
  rgb_arr[num_bpp * i + 2] = (uint16_t)((255 - bg) * (uint16_t)(255 - rgb_arr[num_bpp * i + 2])) >> 8;
}
static inline void _blend_overlay(uint8_t i, uint8_t br, uint8_t bg, uint8_t bb) {
  rgb_arr[num_bpp * i    ] = rgb_arr[num_bpp * i    ] < 128 ?
                                (2 * (uint16_t)bg * rgb_arr[num_bpp * i    ]) >> 8 :
                                (uint16_t)(2 * (255 - bg) * (uint16_t)(255 - rgb_arr[num_bpp * i    ])) >> 8;
  rgb_arr[num_bpp * i + 1] = rgb_arr[num_bpp * i + 1] < 128 ?
                                (2 * (uint16_t)bg * rgb_arr[num_bpp * i + 1]) >> 8 :
                                (uint16_t)(2 * (255 - bg) * (uint16_t)(255 - rgb_arr[num_bpp * i + 1])) >> 8;
  rgb_arr[num_bpp * i + 2] = rgb_arr[num_bpp * i + 2] < 128 ?
                                (2 * (uint16_t)bg * rgb_arr[num_bpp * i + 2]) >> 8 :
                                (uint16_t)(2 * (255 - bg) * (uint16_t)(255 - rgb_arr[num_bpp * i + 2])) >> 8;
}
static inline void _blend_s_li(uint8_t i, uint8_t br, uint8_t bg, uint8_t bb) {

}

/**
 * @brief  Implementation of methods LED_Color_Fader::NextColorLin/Spline for single and multiple peaks
**/
static inline LED_COLOR_STATE_t _LED_Color_Fader_NextColorLinSingle(const struct LED_Color *unsafe_self) {
  struct LED_Color_Fader *self = (struct LED_Color_Fader *)unsafe_self;

  hsl_t *target = self->pks[0]; // Just get the one pk
  uint8_t end_pos = self->start_pos + self->num_chain;

  uint8_t time_bits = self->time_bits;      // Overhead reduce
  uint8_t time_period = 1 << time_bits;     // Time period, resuolution by: 2^(time_bits)

  uint8_t i_s = target->s, i_l;
  // State FSM output
  if(self->state == LED_COLOR_STATE_FADE_IN) {
    i_l = ((int32_t)target->l * self->fade_pos) >> time_bits;

    ++self->fade_pos;
    // State transistion if time_period has elapsed. Next state: Active
    if(!(self->fade_pos < time_period)) {
      self->fade_pos = 0;
      self->state = LED_COLOR_STATE_ACTIVE;
    }
  }
  else if(self->state == LED_COLOR_STATE_FADE_OUT) {
    // Let the light fade out by decreasing lightness linearly until 0 is reached
    i_l = (int32_t)target->l - (((int32_t)target->l * (int32_t)self->fade_pos) >> time_bits);

    ++self->fade_pos;
    // State transistion if time_period has elapsed. Next state: We're done!
    if(!(self->fade_pos < time_period)) {
      self->fade_pos = 0;
      self->state = LED_COLOR_STATE_COMPLETE;
    }
  }
  else if(self->state == LED_COLOR_STATE_ACTIVE) {
    i_l = target->l;

    ++self->fade_pos;
    // Obtain next hue value, forever and overwrite starting point color
    ++(self->pks[0]->h);
    // State transistion if time_period has elapsed.
    // Next state: Cyclic rollover if --cyclic is >= 0, else fade out
    if(!(self->fade_pos < time_period)) {
      self->fade_pos = 0;
      if(--self->cyclic < 0)   self->state = LED_COLOR_STATE_FADE_OUT;
    }
  }

  // Write to next color
  for(uint8_t i = self->start_pos; i < end_pos; ++i) {
    uint8_t i_h = i * self->chain_huediff;
    uint32_t target_color = _led_color_hsl2rgb(target->h + i_h, i_s, i_l);
    self->_blend(i, (target_color >> 8) & 0xFF, (target_color >> 16) & 0xFF, target_color & 0xFF);
  }

  vfdco_clr_render();
  return self->state;
}

static inline LED_COLOR_STATE_t _LED_Color_Fader_NextColorLin(const struct LED_Color *unsafe_self) {
  struct LED_Color_Fader *self = (struct LED_Color_Fader *)unsafe_self;
  uint8_t time_bits = self->time_bits;      // Overhead reduce
  uint8_t time_period = 1 << time_bits;     // Time period, resuolution by: 2^(time_bits)

  uint8_t i_h, i_s, i_l;
  // State FSM output
  if(self->state == LED_COLOR_STATE_FADE_IN) {
    // Pick up first pix as target
    hsl_t *target = self->pks[0];
    // Let the light fade in by increasing lightness linearly until target->l is reached
    i_h = target->h;
    i_s = target->s;
    i_l = ((int32_t)target->l * self->fade_pos) >> time_bits;

    ++self->fade_pos;
    // State transistion if time_period has elapsed. Next state: Active
    if(!(self->fade_pos < time_period)) {
      self->fade_pos = 0;
      self->state = LED_COLOR_STATE_ACTIVE;
    }
  } else if(self->state == LED_COLOR_STATE_FADE_OUT) {
    // Pick up last pix as current
    hsl_t *current = self->pks[self->num_pks - 1];
    // And let the light fade out by decreasing lightness linearly until 0 is reached
    i_h = current->h;
    i_s = current->s;
    i_l = (int32_t)current->l - (((int32_t)current->l * (int32_t)self->fade_pos) >> time_bits);

    ++self->fade_pos;
    // State transistion if time_period has elapsed. Next state: We're done!
    if(!(self->fade_pos < time_period)) {
      self->fade_pos = 0;
      self->state = LED_COLOR_STATE_COMPLETE;
    }
  } else if(self->state == LED_COLOR_STATE_ACTIVE) {
    // Pick up current and target pix depending on position and
    uint16_t t = self->fade_pos & (time_period - 1);      // Current time = fade_pos % time_period, always a value between [0 ... time_period - 1]
    uint8_t access_index = self->fade_pos >> time_bits;   // Current access index = fade_pos / time_period, always between [0 ... num_pks - 1]
    hsl_t *current  = self->pks[access_index];
    hsl_t *target   = self->pks[access_index + 1];
    // Linearly transition HSL from curr -> target
    i_h = (((target->h - (int16_t)current->h) * (int32_t)t) >> time_bits) + current->h;
    i_s = (((target->s - (int16_t)current->s) * (int32_t)t) >> time_bits) + current->s;
    i_l = (((target->l - (int16_t)current->l) * (int32_t)t) >> time_bits) + current->l;

    ++self->fade_pos;
    // State transistion if time_period has elapsed.
    // Next state: Cyclic rollover if --cyclic is >= 0, else fade out
    if(!(self->fade_pos < time_period * (self->num_pks - 1))) {
      self->fade_pos = 0;
      if(--self->cyclic >= 0)   self->state = LED_COLOR_STATE_CYCLIC_RECOVERY;
      else                      self->state = LED_COLOR_STATE_FADE_OUT;
    }
  } else if(self->state == LED_COLOR_STATE_CYCLIC_RECOVERY) {
    // Special case where we need to fade from last pix to first pix
    hsl_t *current = self->pks[self->num_pks - 1];
    hsl_t *target = self->pks[0];
    // Linearly transition HSL from curr -> target
    i_h = (((target->h - (int16_t)current->h) * (int32_t)self->fade_pos) >> time_bits) + current->h;
    i_s = (((target->s - (int16_t)current->s) * (int32_t)self->fade_pos) >> time_bits) + current->s;
    i_l = (((target->l - (int16_t)current->l) * (int32_t)self->fade_pos) >> time_bits) + current->l;

    ++self->fade_pos;
    // State transistion if time_period has elapsed. Next state: Active
    if(!(self->fade_pos < time_period)) {
      self->fade_pos = 0;
      self->state = LED_COLOR_STATE_ACTIVE;
    }
  }

  // Write to array
  uint8_t end_pos = self->start_pos + self->num_chain;
  if(end_pos > num_rgb) end_pos = num_rgb;

  for(uint8_t i = self->start_pos; i < end_pos; ++i) {
    i_h += i * self->chain_huediff;                             // i-th hue difference (delta), intended angle overflow
    uint32_t target_color = _led_color_hsl2rgb(i_h, i_s, i_l);  // Get target RGB
    self->_blend(i, (target_color >> 8) & 0xFF, (target_color >> 16) & 0xFF, target_color & 0xFF);
  }
  // Write to LEDs, physically
  vfdco_clr_render();
  return self->state;
}

/**
 * @brief  Constructor of LED_Color_Fader class
**/
struct LED_Color_Fader *LED_Color_Fader_Init(
  uint8_t                   num_pks,
  hsl_t                     **pks,
  uint8_t                   num_chain,
  uint8_t                   chain_hue_diff,
  int8_t                    cyclic,
  uint8_t                   time_resolution_bits,
  uint8_t                   start_pos,
  LED_COLOR_BLEND_MODE_t    blend_mode
) {
  struct LED_Color_Fader *f = (struct LED_Color_Fader *)malloc(sizeof(struct LED_Color_Fader));
  LED_Color_Init(&f->super);

  f->num_pks = num_pks;
  f->pks = pks;
  f->num_chain = num_chain;
  f->chain_huediff = chain_hue_diff;
  f->cyclic = cyclic;
  f->time_bits = time_resolution_bits;
  f->start_pos = start_pos;

  struct LED_Color_VTable _fader_vtable = {
    .Next = (num_pks > 1) ?
        _LED_Color_Fader_NextColorLin :
        _LED_Color_Fader_NextColorLinSingle,
    .Delete = _LED_Color_Fader_Delete
  };

  f->super.VTable = _fader_vtable;
  switch(blend_mode) {
    case LED_COLOR_BLEND_MODE_NORMAL:     f->_blend = _blend_normal;   break;
    case LED_COLOR_BLEND_MODE_MULTIPLY:   f->_blend = _blend_mup;      break;
    case LED_COLOR_BLEND_MODE_SCREEN:     f->_blend = _blend_scr;      break;
    case LED_COLOR_BLEND_MODE_OVERLAY:    f->_blend = _blend_overlay;  break;
    case LED_COLOR_BLEND_MODE_SOFT_LIGHT: f->_blend = _blend_s_li;     break;
    default:                              f->_blend = _blend_normal;   break;
  }

  f->fade_pos = 0;
  f->state = LED_COLOR_STATE_FADE_IN;
  return f;
}

void _LED_Color_Fader_Delete(const struct LED_Color *unsafe_self) {
  free((struct LED_Color_Fader *)unsafe_self);
}

/**
 * @brief  Implementation of virtual functions LED_Color_Mode::VTable (static void _LED_Color_Mode_F3)
**/
static inline void _LED_Color_Mode_F3(const struct LED_Color_Mode *unsafe_self) {
 if(!unsafe_self->VTable.F3) return;
 unsafe_self->VTable.F3(unsafe_self);
}
static inline void _LED_Color_Mode_F3Var(const struct LED_Color_Mode *unsafe_self) {
 if(!unsafe_self->VTable.F3Var) return;
 unsafe_self->VTable.F3Var(unsafe_self);
}
static inline void _LED_Color_Mode_Update(const struct LED_Color_Mode *unsafe_self) {
 // if(!self->VTable.Update) return; Will make sure this never happens. Optimize for loop performance
 unsafe_self->VTable.Update(unsafe_self);
}
static inline void _LED_Color_Mode_Hello(const struct LED_Color_Mode *unsafe_self) {
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







// Go vegan
