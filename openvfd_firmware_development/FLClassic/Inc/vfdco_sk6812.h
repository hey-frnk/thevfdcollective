/*
 * vfdco_sk6812.h
 *
 *  Created on: Nov 18, 2019
 *      Author: HARDWARECOP
 */

#ifndef INC_VFDCO_SK6812_H_
#define INC_VFDCO_SK6812_H_

#include <stdint.h>
#include "stm32f0xx_hal.h"

#define SK6812_PWM_DUTY_HI 38
#define SK6812_PWM_DUTY_LO 19
#define RESET_SLOT 82 //41
#define LED_NUMBER 6

uint8_t  num_rgb,                  // Number of physical LEDs (pixels)
         num_bpp,                  // Number of bytes per pixel bpp
         num_bytes;                // Number of bytes per pixel bpp (3: RGB, 4: RGBW) * num_rgb
         // Array of color values of size num_bytes to be written in the next write cycle
         // to the physical WS2812B/SK6812 LEDs
uint8_t  *rgb_arr;
uint8_t  *write_buf;

// Write buffer with two
uint16_t write_buf_length, write_buf_pos;

extern TIM_HandleTypeDef htim2;
extern DMA_HandleTypeDef hdma_tim2_ch1;

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


#endif /* INC_VFDCO_SK6812_H_ */
