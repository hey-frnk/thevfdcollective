/**
  ******************************************************************************
  * @file     vfdco_color_lib.h
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     13-November-2019
  * @brief    This file contains declarations of a color library written for WS2812B/SK6812
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

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _VFDCO_COLOR_LIB
#define _VFDCO_COLOR_LIB

#include <stdint.h>

/** Begin of:
  * @toc SECTION_DRIVER_INTERFACE
  * @brief  This is the interface between the physical driver and the color lib
            the color lib will work, when every declaration here is implemented in the driver
 **/
// #include <vfdco_ws2812b.h>
// #include <vfdco_sk6812.h>

// #define HARDWARE_OPTION_WS2812B
#define HARDWARE_OPTION_SK6812

extern uint8_t  num_rgb;                  // Number of physical LEDs (pixels)
extern uint8_t  num_bpp;                  // Number of bytes per pixel bpp
extern uint8_t  num_bytes;                // Number of bytes per pixel bpp (3: RGB, 4: RGBW) * num_rgb
                // Array of color values of size num_bytes to be written in the next write cycle
                // to the physical WS2812B/SK6812 LEDs
extern uint8_t  *rgb_arr;

// Initialize SW/HW of num_pixels * SK6812 LEDs
extern void vfdco_clr_init(uint8_t num_pixels);
// You might never ever make use of it but hey... in case
extern void vfdco_clr_deInit(void);

// Some simple access functions. Add some more if u feel creative
extern void vfdco_clr_set_RGB(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
extern void vfdco_clr_set_RGBW(uint8_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
extern void vfdco_clr_set_all_RGB(uint8_t r, uint8_t g, uint8_t b);
extern void vfdco_clr_set_all_RGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w);

uint32_t _led_color_hsl2rgb(uint8_t h, uint8_t s, uint8_t l);

// Colors all written? Shuttle the data bit by bit into the LEDs
extern void vfdco_clr_render();

/** Begin of:
  * @toc SECTION_DRIVER_INITIALIZE
  * @brief  Driver initialize function
 **/
/*void VFDCO_LED_Driver_Init(   uint8_t num_pixel,        // Number of physical LEDs (pixels)
                              uint8_t bytes_per_pixel,  // Number of bytes per pixel
                              uint8_t *rgb_arr          // Reference to allocated rgb_arr
);*/


/** Begin of:
  * @toc SECTION_COLOR_DEFINITIONS
 **/
/** Begin of:
 * @toc SUBSECTION_HSL
**/
typedef struct hsl_t { uint8_t h, s, l; } hsl_t; // Hue Saturation Lightness

hsl_t *HSL_Init(  uint8_t h, // Hue value
                  uint8_t s, // Saturation value
                  uint8_t l  // Lightness value
);

hsl_t *HSL_Init_Range(    uint8_t h, uint8_t s, uint8_t l, // Like above
                          // FWHW: Transforms tolerance into gaussian distributed color value using box muller transform (3x)
                          float dh, float ds, float dl
);

void HSL_Delete(hsl_t *self);

/** Begin of:
 * @toc SUBSECTION_COLOR_PEAK
**/

typedef enum {
  // Blend mode implementation according to https://en.wikipedia.org/wiki/Blend_modes
  // Assume R, G, B in [0 ... 1], f(a, b) = blended with a = existing value, b = blend layer
   LED_COLOR_BLEND_MODE_NORMAL      =   0,  // f(a, b) = b
   LED_COLOR_BLEND_MODE_MULTIPLY    =   1,  // f(a, b) = ab
   LED_COLOR_BLEND_MODE_SCREEN      =   2,  // f(a, b) = 1 - (1-a)*(1-b)
   LED_COLOR_BLEND_MODE_OVERLAY     =   3,  // f(a, b) = {2ab, a < 0.5} {1 - 2(1-a)(1-b), else}
   LED_COLOR_BLEND_MODE_SOFT_LIGHT  =   4,  // f(a, b) = (1-2b) * a^2 + 2ba
} LED_COLOR_BLEND_MODE_t;

typedef enum {
  LED_COLOR_STATE_COMPLETE          =   0,  // Fade stopped. All colors done
  LED_COLOR_STATE_ACTIVE            =   1,  // Stationary state: Regular operation
  LED_COLOR_STATE_FADE_IN           =  10,  // Transient state: Fading in
  LED_COLOR_STATE_FADE_OUT          =  11,  // Transient state: Fading out
  LED_COLOR_STATE_CYCLIC_RECOVERY   =  20   // Special case: Cyclic rollover
} LED_COLOR_STATE_t;

/** Begin of:
 * @toc SUBSECTION_COLOR_FADER
**/
struct LED_Color_Fader {
  // Option: Peaks
  uint8_t       num_pks;        // Number of peaks
  hsl_t         **pks;        // Peaks array
  // Option: Chaining
  uint8_t       num_chain,      // Number of chained pixels. 0 = singular
                chain_huediff;  // Constant difference of hue between chained pixels
  // Option: Time
  int8_t        cyclic,         // If == k: pk[n] -> pk[0] for k cycles, else fade in & out once
                time_bits;      // Time resolution in bits, max 7 bit (don't use the eighth or bad things might happen)
  // Option: StartPos
  uint8_t       start_pos;      // LED start index < end index

  // State Variables
  LED_COLOR_STATE_t state;      // Color Fader FSM
  uint16_t      fade_pos;       // Fade position (substate)

  // Functions
  void          (*Next)           (struct LED_Color_Fader *self);
  void          (*_blend)         (uint8_t, uint8_t, uint8_t, uint8_t);
};

/**
  * @brief  Constructor of LED_Color_Fader class
 **/
struct LED_Color_Fader *LED_Color_Fader_Init(
  uint8_t                   num_pks,                // Number of HSL colors
  hsl_t                     **pks,                  // Array of HSL colors
  uint8_t                   num_chain,              // Number of chained pixels
  uint8_t                   chain_hue_diff,         // Hue difference between chained pixels
  int8_t                    cyclic,                 // Fade N cycles
  uint8_t                   time_resolution_bits,   // Time resolution in bit
  uint8_t                   start_pos,              // Pixel index to start
  LED_COLOR_BLEND_MODE_t    blend_mode              // Pixel blend setting.
);

void LED_Color_Fader_Delete(struct LED_Color_Fader *self);


/** Begin of:
  * @toc SECTION_LED_COLOR_MODE
 **/
/**
  * @brief  Virtual table for LED_Color_Mode
 **/
struct LED_Color_Mode;
struct LED_Color_Mode_VTable {
  void                  (*F3)               (const struct LED_Color_Mode *unsafe_self);
  void                  (*F3Var)            (const struct LED_Color_Mode *unsafe_self);
  void                  (*Update)           (const struct LED_Color_Mode *unsafe_self);
  void                  (*Hello)            (void);
};

/**
  * @brief  Definition of LED_Color_Mode class
 **/
struct LED_Color_Mode {
  // VTable (virtual) functions
  void                  (*F3)               (const struct LED_Color_Mode *unsafe_self);
  void                  (*F3Var)            (const struct LED_Color_Mode *unsafe_self);
  void                  (*Update)           (const struct LED_Color_Mode *unsafe_self);
  void                  (*Hello)            (const struct LED_Color_Mode *unsafe_self);

  struct LED_Color_Mode_VTable VTable;
};

/**
  * @brief  Constructor of LED_Color_Mode class
 **/
void LED_Color_Mode_Init(struct LED_Color_Mode *self);



#endif

#ifdef __cplusplus
}
#endif
