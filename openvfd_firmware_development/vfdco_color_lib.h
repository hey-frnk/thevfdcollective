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

#ifndef _LED_H
#define _LED_H

#include <stdint.h>

/** Begin of:
  * @toc SECTION_DRIVER_INTERFACE
  * @brief  This is the interface between the physical driver and the color lib
            the color lib will work, when every declaration here is implemented in the driver
 **/
// #include <vfdco_ws2812b.h>
// #include <vfdco_sk6812.h>

// #define HARDWARE_OPTION_WS2812B
// #define HARDWARE_OPTION_SK6812

extern uint8_t  num_rgb,                  // Number of physical LEDs (pixels)
                num_bpp,                  // Number of bytes per pixel bpp
                num_bytes,                // Number of bytes per pixel bpp (3: RGB, 4: RGBW) * num_rgb
                // Array of color values of size num_bytes to be written in the next write cycle
                // to the physical WS2812B/SK6812 LEDs
                *rgb_arr;

extern void     vfdco_clr_render(void);   // Transmits num_bytes of color data to LEDs upon call


/** Begin of:
  * @toc SECTION_DRIVER_INITIALIZE
  * @brief  Driver initialize function
 **/
void VFDCO_LED_Driver_Init(   uint8_t num_pixel,        // Number of physical LEDs (pixels)
                              uint8_t bytes_per_pixel,  // Number of bytes per pixel
                              uint8_t *rgb_arr          // Reference to allocated rgb_arr
);


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
/*struct color_peak_t {
  hsl_t         *pk,   // Peak HSL
                *ps;   // Peak partial slope Cps = FWHW / 2 = sigma * sqrt(2 ln2)
}; super unnecessary*/

/** Begin of:
 * @toc SUBSECTION_COLOR_FADER
**/
struct LED_Color_Fader {
  // Option: Peaks
  uint8_t       num_pks;        // Number of peaks
  hsl_t         **pks_h;        // Peaks array
  // Option: Chaining
  uint8_t       num_chain,      // Number of chained pixels. 0 = singular
                chain_huediff;  // Constant difference of hue between chained pixels
  // Option: Time
  int8_t        cyclic,         // If == k: pk[n] -> pk[0] for k cycles, else fade in & out once
                time_bits;      // Time resolution in bits, max 7 bit (don't use the eighth or bad things might happen)
  // Option: StartPos
  uint8_t       start_pos;      // LED start index < end index

  // State Variables
  uint16_t      fade_pos;       // Fade position [0 ... (num_pks + 1) * 256]

  // Functions
  void          (*NextColorLin)           (const struct color_fade *self);
  void          (*NextColorSpline)        (const struct color_fade *self);
};

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
)


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
