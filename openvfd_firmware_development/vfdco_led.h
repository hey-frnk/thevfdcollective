/**
  ******************************************************************************
  * @file     vfdco_led.h
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     20-November-2019
  * @brief    This is the dummy driver to test the functionality of the LED libs
  ******************************************************************************
  * @toc      Table of contents, enter to navigate:
  ******************************************************************************
 **/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _VFDCO_LED_DRIVER
#define _VFDCO_LED_DRIVER

#include <stdint.h>

// uint8_t  // n um_rgb,                  // Number of physical LEDs (pixels)
         // n um_bpp,                  // Number of bytes per pixel bpp
         // n um_bytes;                // Number of bytes per pixel bpp (3: RGB, 4: RGBW) * n um_rgb
         // Array of color values of size n um_bytes to be written in the next write cycle
         // to the physical WS2812B/SK6812 LEDs
uint8_t  *rgb_arr;

// Initialize SW/HW of num_pixels * SK6812 LEDs
void vfdco_clr_init(uint8_t num_pixels);
// You might never ever make use of it but hey... in case
void vfdco_clr_deInit(void);

// Some simple access functions. Add some more if u feel creative
void vfdco_clr_set_RGB(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
void vfdco_clr_set_RGBW(uint8_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
void vfdco_clr_set_all_RGB(uint8_t r, uint8_t g, uint8_t b);
void vfdco_clr_set_all_RGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w);

// Colors all written? Shuttle the data bit by bit into the LEDs
void vfdco_clr_render();



#endif

#ifdef __cplusplus
}
#endif
