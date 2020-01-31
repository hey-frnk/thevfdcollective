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
  *           SECTION_SUPPORTING_FUNCTIONS
  *           SECTION_HSL
  *           SECTION_BLENDING_FUNCTIONS
  *           SECTION_LED_COLOR
  *             -SUBSECTION_COLOR_FADER
  *             -SUBSECTION_COLOR_FLASHER
  *             -SUBSECTION_COLOR_CHASER
  *           SECTION_LED_MANAGER
  ******************************************************************************
 **/

#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "../vfdco_config.h"
#include "../vfdco_color_lib.h"

/** Begin of:
  * @toc SECTION_SUPPORTING_FUNCTIONS
 **/
/**
 * @brief Fast hsl2rgb algorithm: https://stackoverflow.com/questions/13105185/fast-algorithm-for-rgb-hsl-conversion
**/
uint32_t _led_color_hsl2rgb(uint8_t h, uint8_t s, uint8_t l) {
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

  return ((r + m) << 8) | ((g + m) << 16) | (b + m);
}


volatile uint8_t _rrx = 0;
volatile uint8_t _rry = 0;
volatile uint8_t _rrz = 0;
volatile uint8_t _rra = 1;
// Xorshift Randomizer
uint8_t led_color_simple_randomizer(uint8_t bits) {
  if(!bits) return 0;
  uint8_t _rrt = _rrx ^ (_rrx >> 1);
  _rrx = _rry;
  _rry = _rrz;
  _rrz = _rra;
  _rra = _rrz ^ _rrt ^ (_rrz >> 3) ^ (_rrt << 1);
  return _rra & ((1 << bits) - 1);
}

/** Begin of:
  * @toc SECTION_HSL
 **/
/**
  * @brief  Implementation of constructor HSL class, HSL::HSL(h, s, l)
 **/
hsl_t *HSL_Init(uint8_t h, uint8_t s, uint8_t l) {
  hsl_t *hsl = (hsl_t *)malloc(sizeof(hsl_t));
  hsl->h = h;
  hsl->s = s;
  hsl->l = l;
  return hsl;
}
rgb_t *RGB_Init(uint8_t r, uint8_t g, uint8_t b) {
  rgb_t *_rgb = (rgb_t *)malloc(sizeof(rgb_t));
  _rgb->r = r;
  _rgb->g = g;
  _rgb->b = b;
  return _rgb;
}

/**
 * @brief  Declaration of destructor HSL class, HSL::~HSL
**/
inline void HSL_Delete(hsl_t *self) {
  free(self);
}
inline void RGB_Delete(rgb_t *self) {
  free(self);
}


/** Begin of:
  * @toc SECTION_BLENDING_FUNCTIONS
 **/
/**
 * @brief  Implementation of blend mode normal f(a, b) = b
**/
/*static void _blend(uint8_t i, uint8_t br, uint8_t bg, uint8_t bb) {
  rgb_arr[CONFIG_NUM_BPP * i    ] = bg;
  rgb_arr[CONFIG_NUM_BPP * i + 1] = br;
  rgb_arr[CONFIG_NUM_BPP * i + 2] = bb;
}*/
/**
 * @brief  Implementation of blend mode synchronous normal f(a, b) = b
**/
/*static inline void _blend_snormal(uint8_t i, uint8_t br, uint8_t bg, uint8_t bb) {
  rgb_arr[CONFIG_NUM_BPP * i    ] = bg;
  rgb_arr[CONFIG_NUM_BPP * i + 1] = br;
  rgb_arr[CONFIG_NUM_BPP * i + 2] = bb;
}*/
/**
 * @brief  Implementation of blend mode multiply f(a, b) = ab
**/
/*static inline void _blend_mup(uint8_t i, uint8_t br, uint8_t bg, uint8_t bb) {
  rgb_arr[CONFIG_NUM_BPP * i    ] = ((uint16_t)bg * rgb_arr[CONFIG_NUM_BPP * i    ]) >> 8;
  rgb_arr[CONFIG_NUM_BPP * i + 1] = ((uint16_t)br * rgb_arr[CONFIG_NUM_BPP * i + 1]) >> 8;
  rgb_arr[CONFIG_NUM_BPP * i + 2] = ((uint16_t)bb * rgb_arr[CONFIG_NUM_BPP * i + 2]) >> 8;
}*/
/**
 * @brief  Implementation of blend mode screen f(a, b) = 1 - (1-a)*(1-b)
**/
/*static inline void _blend_scr(uint8_t i, uint8_t br, uint8_t bg, uint8_t bb) {
  rgb_arr[CONFIG_NUM_BPP * i    ] = (uint16_t)((255 - bg) * (uint16_t)(255 - rgb_arr[CONFIG_NUM_BPP * i    ])) >> 8;
  rgb_arr[CONFIG_NUM_BPP * i + 1] = (uint16_t)((255 - bg) * (uint16_t)(255 - rgb_arr[CONFIG_NUM_BPP * i + 1])) >> 8;
  rgb_arr[CONFIG_NUM_BPP * i + 2] = (uint16_t)((255 - bg) * (uint16_t)(255 - rgb_arr[CONFIG_NUM_BPP * i + 2])) >> 8;
}*/
/**
 * @brief  Implementation of blend mode overlay f(a, b) = {2ab, a < 0.5} {1 - 2(1-a)(1-b), else}
**/
/*static inline void _blend_overlay(uint8_t i, uint8_t br, uint8_t bg, uint8_t bb) {
  rgb_arr[CONFIG_NUM_BPP * i    ] = rgb_arr[CONFIG_NUM_BPP * i    ] < 128 ?
                                (2 * (uint16_t)bg * rgb_arr[CONFIG_NUM_BPP * i    ]) >> 8 :
                                (uint16_t)(2 * (255 - bg) * (uint16_t)(255 - rgb_arr[CONFIG_NUM_BPP * i    ])) >> 8;
  rgb_arr[CONFIG_NUM_BPP * i + 1] = rgb_arr[CONFIG_NUM_BPP * i + 1] < 128 ?
                                (2 * (uint16_t)bg * rgb_arr[CONFIG_NUM_BPP * i + 1]) >> 8 :
                                (uint16_t)(2 * (255 - bg) * (uint16_t)(255 - rgb_arr[CONFIG_NUM_BPP * i + 1])) >> 8;
  rgb_arr[CONFIG_NUM_BPP * i + 2] = rgb_arr[CONFIG_NUM_BPP * i + 2] < 128 ?
                                (2 * (uint16_t)bg * rgb_arr[CONFIG_NUM_BPP * i + 2]) >> 8 :
                                (uint16_t)(2 * (255 - bg) * (uint16_t)(255 - rgb_arr[CONFIG_NUM_BPP * i + 2])) >> 8;
}*/
/**
 * @brief  Implementation of blend mode soft light f(a, b) = (1-2b) * a^2 + 2ba
**/
/*static inline void _blend_s_li(uint8_t i, uint8_t br, uint8_t bg, uint8_t bb) {

}*/

/**
 * @brief Map given LED_COLOR_BLEND_MODE_t to a function. Oh dear hope you don't have to debug this...
**/
/*static void (*_blend_init(LED_COLOR_BLEND_MODE_t m)) (uint8_t, uint8_t, uint8_t, uint8_t) {
  switch(m) {
    case LED_COLOR_BLEND_MODE_NORMAL:     return _blend_normal;   break;
    case LED_COLOR_BLEND_MODE_SYNC_NORMAL:return _blend_snormal;  break;
    case LED_COLOR_BLEND_MODE_MULTIPLY:   return _blend_mup;      break;
    case LED_COLOR_BLEND_MODE_SCREEN:     return _blend_scr;      break;
    case LED_COLOR_BLEND_MODE_OVERLAY:    return _blend_overlay;  break;
    case LED_COLOR_BLEND_MODE_SOFT_LIGHT: return _blend_s_li;     break;
    default:                              return _blend_normal;   break;
  }
}*/




/** Begin of:
  * @toc SECTION_LED_COLOR
 **/
/**
 * @brief  Implementation of virtual method LED_Color::Next
**/
static inline LED_COLOR_STATE_t _LED_Color_Next(struct LED_Color *unsafe_self) {
  return unsafe_self->VTable.Next(unsafe_self);
}
/**
 * @brief  Implementation of virtual destructor LED_Color::~LED_Color
**/
static inline void _LED_Color_Delete(struct LED_Color *unsafe_self) {
  unsafe_self->VTable.Delete(unsafe_self);
}
/**
 * @brief  Implementation of constructor LED_Color::LED_Color
**/
void LED_Color_Init(struct LED_Color *f, uint_fast32_t timer1_interval) {
  f->Next = _LED_Color_Next;
  f->Delete = _LED_Color_Delete;

  f->timer = Time_Event_Init(timer1_interval);
}

/** Begin of:
  * @toc SUBSECTION_COLOR_FADER
 **/
/**
 * @brief  Implementation of method LED_Color_Fader::Next for single peak
**/
static inline LED_COLOR_STATE_t _LED_Color_Fader_NextColorLinSingle(struct LED_Color *unsafe_self) {
  struct LED_Color_Fader *self = (struct LED_Color_Fader *)unsafe_self;

  hsl_t *target = self->pks[0]; // Just get the one pk
  uint8_t end_pos = self->start_pos + self->num_chain;

  uint_fast16_t time_period = 1 << LED_COLOR_FADER_TIME_BITS;     // Time period, resuolution by: 2^(time_bits)
  uint8_t render_enable = Time_Event_Update(&unsafe_self->timer);

  uint8_t i_s = target->s, i_l = 0;
  // State FSM output
  if(self->state == LED_COLOR_STATE_FADE_IN) {
    i_l = ((int32_t)target->l * self->fade_pos) >> LED_COLOR_FADER_TIME_BITS;

    if(render_enable) self->fade_pos += 2;
    // State tranistion if time_period has elapsed. Next state: Active
    if(!(self->fade_pos < time_period)) {
      self->fade_pos = 0;
      self->state = LED_COLOR_STATE_ACTIVE;
    }
  } else if(self->state == LED_COLOR_STATE_FADE_OUT) {
    // Let the light fade out by decreasing lightness linearly until 0 is reached
    i_l = (int32_t)target->l - (((int32_t)target->l * (int32_t)self->fade_pos) >> LED_COLOR_FADER_TIME_BITS);

    if(render_enable) self->fade_pos += 2;
    // State transition if time_period has elapsed. Next state: We're done!
    if(!(self->fade_pos < time_period)) {
      self->fade_pos = 0;
      self->state = LED_COLOR_STATE_COMPLETE;
      // for(uint8_t i = self->start_pos; i < end_pos; ++i) self->_blend(i, 0, 0, 0); // clr
      // vfdco_clr_render();
    }
  } else if(self->state == LED_COLOR_STATE_ACTIVE) {
    i_l = target->l;

    if(render_enable) {
    	++self->fade_pos;

    	// Obtain next hue value, forever and overwrite starting point color
    	++(self->pks[0]->h);
    	// State transition if time_period has elapsed.
    	// Next state: Cyclic rollover if --repeat is >= 0, else fade out
    	if(!(self->fade_pos < time_period)) {
    		self->fade_pos = 0;
        // If not repeat forever
        if(self->repeat != LED_COLOR_REPEAT_FOREVER) if(--self->repeat < 0) self->state = LED_COLOR_STATE_FADE_OUT;
    	}
    }
  }

  if(render_enable) {
  	// Write to next color
  	for(uint_fast8_t i = self->start_pos; i < end_pos; ++i) {
  		int16_t i_h = i * self->chain_huediff;
  		uint32_t target_color = _led_color_hsl2rgb(target->h + i_h, i_s, i_l);
  		vfdco_clr_set_RGB(i, (target_color >> 8) & 0xFF, (target_color >> 16) & 0xFF, target_color & 0xFF);
  	}
  	vfdco_clr_render();
  }


  return self->state;
}

/**
 * @brief  Implementation of method LED_Color_Fader::Next for multiple peaks
**/
static inline LED_COLOR_STATE_t _LED_Color_Fader_NextColorLin(struct LED_Color *unsafe_self) {
  struct LED_Color_Fader *self = (struct LED_Color_Fader *)unsafe_self;
  uint_fast8_t time_bits = LED_COLOR_FADER_TIME_BITS;      // Overhead reduce
  uint_fast16_t time_period = 1 << time_bits;     // Time period, resuolution by: 2^(time_bits)

  uint8_t render_enable = Time_Event_Update(&unsafe_self->timer);

  uint8_t i_h = 0, i_s = 0, i_l = 0;
  // State FSM output
  if(self->state == LED_COLOR_STATE_FADE_IN) {
    // Pick up first pix as target
    hsl_t *target = self->pks[0];
    // Let the light fade in by increasing lightness linearly until target->l is reached
    i_h = target->h;
    i_s = target->s;
    i_l = ((int_fast32_t)target->l * self->fade_pos) >> time_bits;

    if(render_enable) self->fade_pos += 2;
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

    if(render_enable) self->fade_pos += 2;
    // State transistion if time_period has elapsed. Next state: We're done!
    if(!(self->fade_pos < time_period)) {
      self->fade_pos = 0;
      self->state = LED_COLOR_STATE_COMPLETE;

      uint_fast8_t end_pos = self->start_pos + self->num_chain;
      if(end_pos > CONFIG_NUM_PIXELS) end_pos = CONFIG_NUM_PIXELS;
      for(uint8_t i = self->start_pos; i < end_pos; ++i) vfdco_clr_set_RGB(i, 0, 0, 0); // clr
      vfdco_clr_render();
    }
  } else if(self->state == LED_COLOR_STATE_ACTIVE) {
    // Pick up current and target pix depending on position and
    uint_fast16_t t = self->fade_pos & (time_period - 1);      // Current time = fade_pos % time_period, always a value between [0 ... time_period - 1]
    uint_fast8_t fast_time_bits = 1 << time_bits;
    uint_fast8_t access_index = self->fade_pos >> time_bits;   // Current access index = fade_pos / time_period, always between [0 ... num_pks - 1]
    hsl_t *current  = self->pks[access_index];
    hsl_t *target   = self->pks[access_index + 1];
    // Linearly transition HSL from curr -> target
    /*
      i_h = (((target->h - (int_fast16_t)current->h) * (int_fast32_t)t) >> time_bits) + current->h;
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
    i_h = (((((int16_t)(fast_time_bits + (fast_time_bits >> 1) + target->h - current->h) % fast_time_bits) - (fast_time_bits >> 1)) * t) >> 8) + current->h;  // what the actual fuck?!
    i_s = (((target->s - (int_fast16_t)current->s) * (int_fast32_t)t) >> time_bits) + current->s;
    i_l = (((target->l - (int_fast16_t)current->l) * (int_fast32_t)t) >> time_bits) + current->l;

    if(render_enable) ++self->fade_pos;
    // State transistion if time_period has elapsed.
    // Next state: Cyclic rollover if --repeat is >= 0, else fade out
    if(!(self->fade_pos < time_period * (self->num_pks - 1))) {
      self->fade_pos = 0;
      if(self->repeat != LED_COLOR_REPEAT_FOREVER) {
        if(--self->repeat < 0)  self->state = LED_COLOR_STATE_FADE_OUT;
        else                    self->state = LED_COLOR_STATE_CYCLIC_RECOVERY;
      }
      else                      self->state = LED_COLOR_STATE_CYCLIC_RECOVERY;
    }
  } else if(self->state == LED_COLOR_STATE_CYCLIC_RECOVERY) {
    // Special case where we need to fade from last pix to first pix
    hsl_t *current = self->pks[self->num_pks - 1];
    hsl_t *target = self->pks[0];
    // Linearly transition HSL from curr -> target
    i_h = (((target->h - (int_fast16_t)current->h) * (int_fast32_t)self->fade_pos) >> time_bits) + current->h;
    i_s = (((target->s - (int_fast16_t)current->s) * (int_fast32_t)self->fade_pos) >> time_bits) + current->s;
    i_l = (((target->l - (int_fast16_t)current->l) * (int_fast32_t)self->fade_pos) >> time_bits) + current->l;

    if(render_enable) ++self->fade_pos;
    // State transistion if time_period has elapsed. Next state: Active
    if(!(self->fade_pos < time_period)) {
      self->fade_pos = 0;
      self->state = LED_COLOR_STATE_ACTIVE;
    }
  }

  if(render_enable) {
  	// Write to array
  	uint_fast8_t end_pos = self->start_pos + self->num_chain;
  	if(end_pos > CONFIG_NUM_PIXELS) end_pos = CONFIG_NUM_PIXELS;

  	for(uint_fast8_t i = self->start_pos; i < end_pos; ++i) {
  		i_h += i * (int16_t)self->chain_huediff;                    // i-th hue difference (delta), intended angle overflow
  		uint32_t target_color = _led_color_hsl2rgb(i_h, i_s, i_l);  // Get target RGB
  		vfdco_clr_set_RGB(i, (target_color >> 8) & 0xFF, (target_color >> 16) & 0xFF, target_color & 0xFF);
  	}
  	// Write to LEDs, physically
  	vfdco_clr_render();
  }
  return self->state;
}

/**
 * @brief  Implementation of destructor LED_Color_Fader::~LED_Color_Fader
**/
void _LED_Color_Fader_Delete(struct LED_Color *unsafe_self) {
  free((struct LED_Color_Fader *)unsafe_self);
}

/**
 * @brief  Implementation of constructor LED_Color_Fader::LED_Color_Fader
**/
struct LED_Color_Fader *LED_Color_Fader_Init(
  uint_fast32_t             timer1_interval,
  /*LED_COLOR_BLEND_MODE_t    blend_mode,*/
  uint8_t                   start_pos,
  int8_t                    repeat,
  uint8_t                   num_pks,
  hsl_t                     **pks,
  uint8_t                   num_chain,
  int8_t                    chain_hue_diff
) {
  struct LED_Color_Fader *f = (struct LED_Color_Fader *)malloc(sizeof(struct LED_Color_Fader));
  LED_Color_Init(&f->super, timer1_interval);

  // LED Fader Attributes
  f->start_pos = start_pos;
  f->num_pks = num_pks;
  f->pks = pks;
  f->num_chain = num_chain;
  f->chain_huediff = chain_hue_diff;
  f->repeat = repeat;

  // Method mapping
  struct LED_Color_VTable _fader_vtable = {
    .Next = (num_pks > 1) ?
        _LED_Color_Fader_NextColorLin :
        _LED_Color_Fader_NextColorLinSingle,
    .Delete = _LED_Color_Fader_Delete
  };

  f->super.VTable = _fader_vtable;
  /*f->_blend = _blend_init(blend_mode);*/

  f->fade_pos = 0;
  f->state = LED_COLOR_STATE_FADE_IN;
  return f;
}


/** Begin of:
  * @toc SUBSECTION_COLOR_FLASHER
 **/
/**
 * @brief  Implementation of method LED_Color_Flasher::Next
**/
static inline LED_COLOR_STATE_t _LED_Color_Flasher_Next(struct LED_Color *unsafe_self) {
  struct LED_Color_Flasher *self = (struct LED_Color_Flasher *)unsafe_self;

  if(self->state == LED_COLOR_STATE_ACTIVE) {
    if(Time_Event_Update(&unsafe_self->timer)) {
    	++self->flash_pos;

    	vfdco_clr_set_RGB(self->start_pos, self->pk->r, self->pk->g, self->pk->b);

    	if(!(self->flash_pos < self->flash_duration)) {
    		self->flash_pos = 0;
    		self->state = LED_COLOR_STATE_CYCLIC_RECOVERY;
    		vfdco_clr_set_RGB(self->start_pos, 0, 0, 0);           // Zero out
    	}
    	// Write to LEDs, physically
    	vfdco_clr_render();
    }
  } else if(self->state == LED_COLOR_STATE_CYCLIC_RECOVERY) {
    if(Time_Event_Update(&unsafe_self->timer)) ++self->flash_pos;
    #ifdef DEBUG
    printf("Ghost Write\n");
    #endif

    if(!(self->flash_pos < self->flash_offtime)) {
      self->flash_pos = 0;
      if(self->flash_repeat != LED_COLOR_REPEAT_FOREVER) {
        if(--self->flash_repeat < 0) self->state = LED_COLOR_STATE_COMPLETE;
        else                         self->state = LED_COLOR_STATE_ACTIVE;
      } else {
        self->state = LED_COLOR_STATE_ACTIVE;
      }
    }
  }

  return self->state;
}

/**
 * @brief  Destructor of LED_Color_Flasher class
**/
void _LED_Color_Flasher_Delete(struct LED_Color *unsafe_self) {
  free((struct LED_Color_Flasher *)unsafe_self);
}

/**
 * @brief  Implementation of constructor LED_Color_Flasher::LED_Color_Flasher
**/
struct LED_Color_Flasher *LED_Color_Flasher_Init(
  uint_fast32_t             timer1_interval,
  /*LED_COLOR_BLEND_MODE_t    blend_mode,*/
  uint8_t                   start_pos,
  int8_t                    repeat,
  rgb_t                     *pk,
  uint8_t                   duration,
  uint8_t                   offtime
) {
  struct LED_Color_Flasher *f = (struct LED_Color_Flasher *)malloc(sizeof(struct LED_Color_Flasher));
  LED_Color_Init(&f->super, timer1_interval);

  // LED Flasher Attributes
  f->start_pos = start_pos;
  f->pk = pk;
  f->flash_duration = duration;
  f->flash_offtime = duration << offtime;
  f->flash_repeat = repeat;

  // Method mapping
  struct LED_Color_VTable _flasher_vtable = {
    .Next = _LED_Color_Flasher_Next,
    .Delete = _LED_Color_Flasher_Delete
  };

  f->super.VTable = _flasher_vtable;
  /*f->_blend = _blend_init(blend_mode);*/

  f->flash_pos = 0;
  f->state = LED_COLOR_STATE_ACTIVE;
  return f;
}


/** Begin of:
  * @toc SUBSECTION_COLOR_CHASER
 **/
/**
 * @brief  Implementation of method LED_Color_Chaser::Next
**/
static inline LED_COLOR_STATE_t _LED_Color_Chaser_Next(struct LED_Color *unsafe_self) {
  struct LED_Color_Chaser *self = (struct LED_Color_Chaser *)unsafe_self;

  uint_fast16_t chase_duration = self->chase_duration;
  uint_fast16_t chase_cpreserving = self->chase_cpreserving;

  uint8_t render_enable = Time_Event_Update(&unsafe_self->timer);

  if(self->state == LED_COLOR_STATE_ACTIVE || self->state == LED_COLOR_STATE_CYCLIC_RECOVERY) {
    if(render_enable) ++self->chase_pos;

    // If the chase duration has elapsed, proceed onto the next LED and chase pos
    if(self->state == LED_COLOR_STATE_ACTIVE) {
      if(!(self->chase_pos < self->chase_duration)) {
        self->chase_pos = 0;
        ++self->pk_state;

        // Modify chase direction by speeding up (half duration time) or slowing down (double duration time)
        if        (self->chase_mode & 0x01) self->chase_duration /= 2;
        else if   (self->chase_mode & 0x02) self->chase_duration *= 2;

        // If the last LED has reached, start over!
        if(!(self->pk_state < self->chase_length)) {
          if(!chase_cpreserving)  {
            // If NONPRESERVING, we're done
            self->pk_state = 0;
            // And if repeated enough, quit
            if(self->chase_repeat != LED_COLOR_REPEAT_FOREVER) if(--self->chase_repeat < 0) self->state = LED_COLOR_STATE_COMPLETE;
          } else {
            // If preserving, revert state and chase pos, enter fade out "recovery"
            --self->pk_state;
            if        (self->chase_mode & 0x01) self->chase_duration *= 2;
            else if   (self->chase_mode & 0x02) self->chase_duration /= 2;
            self->chase_pos = self->chase_duration;

            self->state = LED_COLOR_STATE_CYCLIC_RECOVERY;
          }
        }
      }
    } else {
      // Full fade out cycle
      if(!(self->chase_pos < self->chase_duration * (chase_cpreserving + 1))) {
        self->chase_pos = 0;

        // And if repeated enough, quit
        if(self->chase_repeat != LED_COLOR_REPEAT_FOREVER) {
          if(--self->chase_repeat < 0) {
            self->state = LED_COLOR_STATE_COMPLETE;
            return self->state;
          }
          else goto repeat_restore; // https://xkcd.com/292/
        } else {
          repeat_restore:
          if(chase_cpreserving) {
            for(uint_fast8_t i = 0; i < self->pk_state + 1; ++i) {
              if((self->chase_mode <= LED_COLOR_CHASER_MODE_SPLITDEC) && (self->start_pos + i < CONFIG_NUM_PIXELS)) vfdco_clr_set_RGB(self->start_pos + i, 0, 0, 0);
              if((self->chase_mode >= LED_COLOR_CHASER_MODE_SPLITLIN) && (self->start_pos - i >= 0))                vfdco_clr_set_RGB(self->start_pos - i, 0, 0, 0);
            }
          } else {
            if((self->chase_mode <= LED_COLOR_CHASER_MODE_SPLITDEC) && (self->start_pos + self->pk_state < CONFIG_NUM_PIXELS)) vfdco_clr_set_RGB(self->start_pos + self->pk_state, 0, 0, 0);
            if((self->chase_mode >= LED_COLOR_CHASER_MODE_SPLITLIN) && (self->start_pos - self->pk_state >= 0))                vfdco_clr_set_RGB(self->start_pos - self->pk_state, 0, 0, 0);
          }
          vfdco_clr_render();

          self->pk_state = 0; // Reset position
          self->state = LED_COLOR_STATE_ACTIVE;

          if(self->chase_mode | 0x03) self->chase_duration = self->_chase_duration_restore;

          return self->state;
        }
      }
    }

    if(chase_cpreserving) {
      // pk_state +1 determines the amount of single sided LEDs to be written
      for(uint_fast8_t i = 0; i < self->pk_state + 1; ++i) {
        // Right sided write. Only write if pixel is in range and LR or Split is active
        if((self->chase_mode <= LED_COLOR_CHASER_MODE_SPLITDEC) && (self->start_pos + i < CONFIG_NUM_PIXELS)) {
          int16_t lightness = self->pk->l + i * self->pk_diff->l;
          // Ugly AF, sorry
          if((self->state == LED_COLOR_STATE_CYCLIC_RECOVERY || self->pk_state != i) && chase_cpreserving != LED_COLOR_CHASER_PRESERVING) {
            uint32_t attenuation = ((uint32_t)lightness * (chase_duration * (self->pk_state - i - 1) + (uint32_t)self->chase_pos))
                                / (uint32_t)(chase_cpreserving * (uint32_t)chase_duration);
            lightness -= (attenuation > lightness) ? lightness : attenuation;
          }
          uint32_t target_right = _led_color_hsl2rgb(self->pk->h + i * self->pk_diff->h, self->pk->s + i * self->pk_diff->s, lightness);
          vfdco_clr_set_RGB(self->start_pos + i, (target_right >> 8) & 0xFF, (target_right >> 16) & 0xFF, target_right & 0xFF);
        }
        // Left sided write
        if((self->chase_mode >= LED_COLOR_CHASER_MODE_SPLITLIN) /*&& (self->pk_state > 0)*/ && (self->start_pos - i >= 0)) {
          uint8_t lightness = self->pk->l - i * self->pk_diff->l;
          // Ugly AF, sorry
          if((self->state == LED_COLOR_STATE_CYCLIC_RECOVERY || self->pk_state != i) && chase_cpreserving != LED_COLOR_CHASER_PRESERVING) {
            uint32_t attenuation = ((uint32_t)lightness * (chase_duration * (self->pk_state - i - 1) + (uint32_t)self->chase_pos))
                                / (uint32_t)(chase_cpreserving * (uint32_t)chase_duration);
            lightness -= (attenuation > lightness) ? lightness : attenuation;
          }
          uint32_t target_left = _led_color_hsl2rgb(self->pk->h - i * self->pk_diff->h, self->pk->s - i * self->pk_diff->s, lightness);
          vfdco_clr_set_RGB(self->start_pos - i, (target_left >> 8) & 0xFF, (target_left >> 16) & 0xFF, target_left & 0xFF);
        }
      }
    } else {
      // Right sided write. Only write if pixel is in range and LR or Split is active
      if((self->chase_mode <= LED_COLOR_CHASER_MODE_SPLITDEC) && (self->start_pos + self->pk_state < CONFIG_NUM_PIXELS)) {
        uint8_t lightness = self->pk->l + self->pk_state * self->pk_diff->l;
        uint32_t target_right = _led_color_hsl2rgb(self->pk->h + self->pk_state * self->pk_diff->h, self->pk->s + self->pk_state * self->pk_diff->s, lightness);
        vfdco_clr_set_RGB(self->start_pos + self->pk_state, (target_right >> 8) & 0xFF, (target_right >> 16) & 0xFF, target_right & 0xFF);
      }
      // Left sided write
      if((self->chase_mode >= LED_COLOR_CHASER_MODE_SPLITLIN) && (self->start_pos - self->pk_state >= 0)) {
        uint8_t lightness = self->pk->l - self->pk_state * self->pk_diff->l;
        uint32_t target_left = _led_color_hsl2rgb(self->pk->h - self->pk_state * self->pk_diff->h, self->pk->s - self->pk_state * self->pk_diff->s, lightness);
        vfdco_clr_set_RGB(self->start_pos - self->pk_state, (target_left >> 8) & 0xFF, (target_left >> 16) & 0xFF, target_left & 0xFF);
      }
    }
  }


  // Write to LEDs, physically
  if(render_enable) vfdco_clr_render();
  return self->state;
}

/**
 * @brief  Destructor of LED_Color_Chaser class LED_Color_Chaser::~LED_Color_Chaser
**/
void _LED_Color_Chaser_Delete(struct LED_Color *unsafe_self) {
  free((struct LED_Color_Chaser *)unsafe_self);
}

/**
  * @brief Implementation of constructor LED_Color_Chaser::LED_Color_Chaser
 **/
struct LED_Color_Chaser *LED_Color_Chaser_Init(
  uint_fast32_t             timer1_interval,
  /*LED_COLOR_BLEND_MODE_t    blend_mode,*/
  uint8_t                   start_pos,
  int8_t                    repeat,
  uint8_t                   length,
  hsl_t                     *pk,
  hsl_d_t                   *pk_diff,
  uint16_t                  duration,
  uint8_t                   cpreserving,
  uint8_t                   mode
) {
  struct LED_Color_Chaser *f = (struct LED_Color_Chaser *)malloc(sizeof(struct LED_Color_Chaser));
  LED_Color_Init(&f->super, timer1_interval);

  // LED Chaser Attributes
  f->pk = pk;
  f->pk_diff = pk_diff;
  f->chase_cpreserving = cpreserving;
  f->chase_mode = mode;
  f->chase_repeat = repeat;
  f->chase_duration = duration;
  f->_chase_duration_restore = duration;
  f->start_pos = start_pos;

  // Chase length init clipper
  if(mode <= LED_COLOR_CHASER_MODE_SPLITDEC) {
    f->chase_length = (length + start_pos > CONFIG_NUM_PIXELS) ? length - start_pos + 1 : length;
  } else {
    f->chase_length = (length > start_pos) ? start_pos + 1 : length;
  }

  // Method mapping
  struct LED_Color_VTable _chaser_vtable = {
    .Next = _LED_Color_Chaser_Next,
    .Delete = _LED_Color_Chaser_Delete
  };

  f->super.VTable = _chaser_vtable;
  /*f->_blend = _blend_init(blend_mode);*/

  f->chase_pos = 0;
  f->pk_state = 0;
  f->state = LED_COLOR_STATE_ACTIVE;
  return f;
}



/*void _LED_Color_Manager_Run_All(struct LED_Color_Manager *self) {
  for(uint_fast8_t i = 0; i < self->process_active; ++i) {
    if(self->process_list[i]->Next(self->process_list[i])) {
      ++self->render_state;
    } else {
      self->_Process_Finished_Callback(self, i);
    }
  }
  if(Time_Event_Update(&self->synchronous_updater)) {
    self->render_state = 0;
    vfdco_clr_render();
  }

  // Random preempting higher order processes

}

void _LED_Color_Manager_Create_Process(struct LED_Color_Manager *self, struct LED_Color *process) {
  // Enqueue to active directly
  if(self->process_active < self->process_active_max) {
    ++self->process_active;
    self->process_active_list[self->process_active - 1] = process;
  } else {
    if(self->process_preempted < self->process_preempted_max) {
      ++self->process_preempted;
      self->process_preempted_list[self->process_preempted - 1] = process;
    } else {

    }
  }
}


void _LED_Color_Manager_Remove_Process(struct LED_Color_Manager *self, uint_fast8_t pid) {
  if(pid < process_active) { // Kill active process
    self->process_active_list[pid]->Delete(self->process_active_list[pid]);
    // Shrink process list
    for(uint_fast8_t i = pid; i < self->process_active; ++i) {
      self->process_active_list[i] = self->process_active_list[i + 1];
      --self->process_active;
    }
  } else { // Kill preempted process
    uint_fast8_t ipid = pid - process_active;
  }
}

void _LED_Color_Manager_Process_Finished_Callback(struct LED_Color_Manager *self, uint_fast8_t pid) {

}

void LED_Color_Manager_Init(struct LED_Color_Manager *self, uint_fast8_t process_active_max, uint_fast8_t process_preempted_max) {
  self->process_active_list = (LED_Color **)calloc(process_active_max, sizeof(LED_Color *)); // NULL-Array
  self->process_preempted_list = (LED_Color **)calloc(process_preempted_max, sizeof(LED_Color *)); // NULL-Array
}*/


// Go vegan
