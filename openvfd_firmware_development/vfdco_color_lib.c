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

#include "vfdco_color_lib.h"

// Fast hsl2rgb algorithm: https://stackoverflow.com/questions/13105185/fast-algorithm-for-rgb-hsl-conversion
static uint32_t _led_color_hsl2rgb(uint8_t h, uint8_t s, uint8_t l) {
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
    return(u + w1 * r * d);
  } else {
    x = t;
    t = .0f;
    return (u + x * d);
  }
}

/** Begin of:
  * @toc SECTION_DRIVER_INITIALIZE
  * @brief  Driver initialize function
  * @random This is sooo useless lol but u know just to be formal
 **/
void VFDCO_LED_Driver_Init(   uint8_t num_pixel,        // Number of physical LEDs (pixels)
                              uint8_t bytes_per_pixel,  // Number of bytes per pixel
                              uint8_t *rgb              // Reference to allocated rgb_arr
) {
  num_rgb = num_pixel;
  num_bpp = bytes_per_pixel;
  num_bytes = num_pixel * bytes_per_pixel;
  rgb_arr = rgb;
}

/** Begin of:
  * @toc SECTION_LED_COLOR_MODE
 **/

/** Begin of:
  * @toc SECTION_COLOR_DEFINITIONS
 **/

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


/** Begin of:
  * @toc SUBSECTION_COLOR_FADER
 **/
/**
 * @brief  Implementation of methods LED_Color_Fader::NextColorLin/Spline for single and multiple peaks
**/
static inline void _LED_Color_Fader_NextColorLinSingle(const struct LED_Color_Fader *self) {
  hsl_t *pk1 = self->pks[0]; // Just get the one pk

  uint8_t end_pos = self->start_pos + self->num_chain;

  // Write to next color
  for(uint8_t i = start_pos; i < end_pos; ++i) {
    i_h = i * self->chain_huediff;
    uint32_t target_color = _led_color_hsl2rgb(i_h, pk1->s, pk1->l);

    #ifdef HARDWARE_OPTION_SK6812 // Optimize for speed
    // Eventually we need to replace rgb_arr[G] <-> rgb_arr[R]
    *(((uint32_t *)rgb_arr) + i) = target_color; // Evil, filthy type punning
    #else
    uint8_t color_pos = i * num_bpp; // Insert GRB values
    rgb_arr[color_pos] =     (uint8_t)((target_color >> 16) & 0xFF);     // G
    rgb_arr[color_pos + 1] = (uint8_t)((target_color >> 8) & 0xFF);      // R
    rgb_arr[color_pos + 2] = (uint8_t)(target_color & 0xFF);             // B
    #endif

  }

  vfdco_clr_render();

  // Obtain next hue value, forever and overwrite starting point color
  ++self->pks[0]->h;
}

static inline void _LED_Color_Fader_NextColorLin(const struct LED_Color_Fader *self) {
  uint8_t time_bits = self->time_bits;
  uint8_t time_period = 1 << time_bits;
  uint8_t accidx = (uint8_t)(self->fade_pos >> time_bits),
          accT = (uint8_t)(self->fade_pos & (time_period - 1));

  // Check if fade is in range and not fading in or out
  uint8_t max_time = time_period * num_pks;
  uint8_t isFadingIn = self->fade_pos < time_period; // If smaller than one time period, then it's fading in!
  uint8_t isInRange = self->fade_pos < (max_time + time_period); // If max_time + 1 period, then its still in range
  hsl_t *pk1 = isInRange ? self->pks[accidxH + 1] : (isFadingIn ? self->pks[0] : self->pks[num_pks - 1]);
  hsl_t *pk2 = isInRange ? self->pks[accidx] : NULL;

  uint8_t end_pos = self->start_pos + self->num_chain;
  if(end_pos > num_rgb) end_pos = num_rgb;

  // Cheap ass linear interpolation
  uint8_t i_h, i_s, i_l;
  if(isInRange) {
    // linear fade from pk to pk for H, S and L
    i_h = (((int32_t)((int16_t)(pk1->h) - (int16_t)(pk2->h)) * (int32_t)accT) >> time_bits) + pk1->h;
    i_s = (((int32_t)((int16_t)(pk1->s) - (int16_t)(pk2->s)) * (int32_t)accT) >> time_bits) + pk1->s;
    i_l = (((int32_t)((int16_t)(pk1->l) - (int16_t)(pk2->l)) * (int32_t)accT) >> time_bits) + pk1->l;
  } else if(isFadingIn) {
    // linear from 0(L) to 1st pk if inside fade
    i_h = pk1->h;
    i_s = pk1->s;
    i_l = (((int32_t)pk1->l * (int32_t)accT) >> time_bits) + pk1->l;
  } else {
    // linear from last pk to 0(L) if falling out of fade
    i_h = pk1->h;
    i_s = pk1->s;
    i_l = (int32_t)pk1->l - (((int32_t)pk1->l * (int32_t)accT) >> time_bits);
  }

  // Write to array
  for(uint8_t i = self->start_pos; i < end_pos; ++i) {
    // i-th hue difference (delta), intended angle overflow
    i_h += i * self->chain_huediff;
    // Get target RGB
    uint32_t target_color = _led_color_hsl2rgb(i_h, i_s, i_l);

    #ifdef HARDWARE_OPTION_SK6812 // Optimize for speed
    *(((uint32_t *)rgb_arr) + i) = target_color; // Evil, filthy type punning
    #else
    uint8_t color_pos = i * num_bpp; // Insert GRB values
    rgb_arr[color_pos] =     (uint8_t)((target_color >> 16) & 0xFF);     // G
    rgb_arr[color_pos + 1] = (uint8_t)((target_color >> 8) & 0xFF);      // R
    rgb_arr[color_pos + 2] = (uint8_t)(target_color & 0xFF);             // B
    #endif
  }
  vfdco_clr_render();

  // Go to next color
  ++self->fade_pos;
  // If we've been through one cycle
  if(self->fade_pos == max_time + time_period) {
    if(--self->cyclic >= 0) self->fade_pos = time_period;  // start over again
    else                    self->fade_pos++; // go one over
  }
  // If we've made it
  if(self->fade_pos == max_time + (time_period << 1)) {
    // Call destructor, we're done
  }
}

static inline void _LED_Color_Fader_NextColorSpline(const struct LED_Color_Fader *self) {

}

/**
 * @brief  Constructor of LED_Color_Fader class
**/
void LED_Color_Fader_Init(  struct LED_Color_Fader    *self,
                            uint8_t                   num_pks,
                            hsl_t                     **pks,
                            uint8_t                   num_chain,
                            uint8_t                   chain_huediff,
                            int8_t                    cyclic,
                            uint8_t                   time_bits,
                            uint8_t                   start_pos
) {
  self->num_pks = num_pks;
  self->pks = pks;
  self->num_chain = num_chain;
  self->chain_huediff = chain_huediff;
  self->cyclic = cyclic;
  self->fade_pos = fade_pos;

  self->NextColorLin = (num_pks > 1) ? _LED_Color_Fader_NextColorLin : _LED_Color_Fader_NextColorLinSingle;
  self->NextColorSpline = _LED_Color_Fader_NextColorSpline;

  self->fade_pos = 0;
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
