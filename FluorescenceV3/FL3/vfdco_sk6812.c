/*
 * vfdco_sk6812.c
 *
 *  Created on: Nov 18, 2019
 *      Author: HARDWARECOP
 */

#ifdef _LED_IMPLEMENTATION
#error "An implementation of the LED driver already exists!"
#endif
#define _LED_IMPLEMENTATION

#include <stdlib.h>
#include <string.h>
#include "stm32f0xx_hal.h"
#include "../Commons/vfdco_config.h"
#include "../Commons/vfdco_led.h"

#if CONFIG_USE_RGBW == 0
#error "Wrong driver linked. This driver only works for CONFIG_USE_RGBW == 1 (SK6812). Fix: Set CONFIG_USE_RGBW to 1 or use WS2812B driver."
#endif

#define SK6812_PWM_DUTY_HI 38
#define SK6812_PWM_DUTY_LO 19
#define RESET_SLOT 82 //41
#define LED_NUMBER 6

#define WRITE_BUF_LENGTH (CONFIG_NUM_BPP * 8)

volatile uint8_t  write_buf[WRITE_BUF_LENGTH] = {0};
volatile uint8_t	 rgb_arr[CONFIG_NUM_BYTES] = {0};
// Write buffer with two
volatile uint_fast8_t write_buf_pos;

// Dimming
volatile uint8_t _led_dim_factor = 0;

extern TIM_HandleTypeDef htim2;

#if CONFIG_ENABLE_GAMMACORRECTION == 1
static const uint8_t gamma8[] = { // Cheap gamma correction https://learn.adafruit.com/led-tricks-gamma-correction/the-quick-fix
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255};
#endif

inline void vfdco_clr_init(uint8_t initial_dim_factor) {
  write_buf_pos = 0;
  _led_dim_factor = initial_dim_factor;
  HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t *)write_buf, WRITE_BUF_LENGTH);
}

void vfdco_clr_set_dim_factor(uint8_t dim_factor) {
  _led_dim_factor = dim_factor;
}

#if CONFIG_ENABLE_COLORCORRECTION == 1
static inline uint8_t _vfdco_clr_scale8(uint8_t x, uint8_t scale) {
  return ((uint16_t)x * scale) >> 8;
}
#endif

void vfdco_clr_set_RGB(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
  #if CONFIG_ENABLE_GAMMACORRECTION == 1
  rgb_arr[4 * index] = gamma8[g];
  rgb_arr[4 * index + 1] = gamma8[r];
  rgb_arr[4 * index + 2] = gamma8[b];
  #else
    #if CONFIG_ENABLE_COLORCORRECTION == 1
    // TypicalLEDStrip = 0xFFB0F0: 255(R), 176(G), 240(B)
    rgb_arr[4 * index] = _vfdco_clr_scale8(g, 0xB0);
    rgb_arr[4 * index + 1] = r; // no scale
    rgb_arr[4 * index + 2] = _vfdco_clr_scale8(b, 0xF0);
    #else
    rgb_arr[4 * index] = g;
    rgb_arr[4 * index + 1] = r;
    rgb_arr[4 * index + 2] = b;
    #endif
  #endif

  rgb_arr[4 * index + 3] = 0;
}
void vfdco_clr_set_RGBW(uint8_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
  vfdco_clr_set_RGB(index, r, g, b);
  rgb_arr[4 * index + 3] = w;
}
void vfdco_clr_set_all_RGB(uint8_t r, uint8_t g, uint8_t b) {
  for(uint_fast8_t i = 0; i < CONFIG_NUM_PIXELS; ++i) vfdco_clr_set_RGB(i, r, g, b);
}
void vfdco_clr_set_all_RGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
  for(uint_fast8_t i = 0; i < CONFIG_NUM_PIXELS; ++i) vfdco_clr_set_RGBW(i, r, g, b, w);
}

void vfdco_clr_target_RGB(uint8_t *tp, uint8_t r, uint8_t g, uint8_t b) {
  tp[0] = g;
  tp[1] = r;
  tp[2] = b;
  tp[3] = 0;
}
void vfdco_clr_target_RGBW(uint8_t *tp, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
  tp[0] = g;
  tp[1] = r;
  tp[2] = b;
  tp[3] = w;
}
void vfdco_clr_target_all_RGB(uint8_t *tp, uint8_t r, uint8_t g, uint8_t b) {
  for(uint8_t i = 0; i < CONFIG_NUM_BYTES; i += 4) vfdco_clr_target_RGB(tp + i, r, g, b);
}
void vfdco_clr_target_all_RGBW(uint8_t *tp, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
  for(uint8_t i = 0; i < CONFIG_NUM_BYTES; i += 4) vfdco_clr_target_RGBW(tp + i, r, g, b, w);
}

/**
 * @brief Smooth fading between intermediate target array and LED buffer
 * @param target_arr base address of intermediate target array
 */
void vfdco_clr_minimize_difference(uint8_t *target_arr) {
  uint8_t dt = 0;
  for(uint8_t i = 0; i < CONFIG_NUM_BYTES; i++) {
    if(rgb_arr[i] < target_arr[i]) rgb_arr[i]++;
    else if(rgb_arr[i] > target_arr[i]) rgb_arr[i]--;
    else ++dt;
  }
}

volatile void vfdco_clr_render() {
  // Ooh boi the first data buffer!!
  // __disable_irq();
  write_buf_pos = 0;
  for(uint_fast8_t i = 0; i < 8; ++i) {
    write_buf[i     ] = SK6812_PWM_DUTY_LO << ((((rgb_arr[0] >> _led_dim_factor) << i) & 0x80) > 0);
    write_buf[i +  8] = SK6812_PWM_DUTY_LO << ((((rgb_arr[1] >> _led_dim_factor) << i) & 0x80) > 0);
    write_buf[i + 16] = SK6812_PWM_DUTY_LO << ((((rgb_arr[2] >> _led_dim_factor) << i) & 0x80) > 0);
    write_buf[i + 24] = SK6812_PWM_DUTY_LO << ((((rgb_arr[3] >> _led_dim_factor) << i) & 0x80) > 0);
  }

  write_buf_pos++; // Since we're ready for the next buffer
  HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t *)write_buf, WRITE_BUF_LENGTH);
  // __enable_irq();
}

void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim) {
	// if(htim->Instance == TIM2) {
		// DMA buffer set from LED(write_buf_pos) to LED(write_buf_pos + 1)
    // __disable_irq();
		if(write_buf_pos < CONFIG_NUM_PIXELS) {
			// We're in. Let's fill the mem
			for(uint_fast8_t i = 0; i < 8; ++i) {
				write_buf[i     ] = SK6812_PWM_DUTY_LO << ((((rgb_arr[write_buf_pos * 4    ] >> _led_dim_factor) << i) & 0x80) > 0);
				write_buf[i +  8] = SK6812_PWM_DUTY_LO << ((((rgb_arr[write_buf_pos * 4 + 1] >> _led_dim_factor) << i) & 0x80) > 0);
				write_buf[i + 16] = SK6812_PWM_DUTY_LO << ((((rgb_arr[write_buf_pos * 4 + 2] >> _led_dim_factor) << i) & 0x80) > 0);
				write_buf[i + 24] = SK6812_PWM_DUTY_LO << ((((rgb_arr[write_buf_pos * 4 + 3] >> _led_dim_factor) << i) & 0x80) > 0);
			}
			write_buf_pos++;
		} else if (write_buf_pos >= CONFIG_NUM_PIXELS + 1) {
			// Last two transfers are resets. 64 * 1.25 us = 80 us = good enough reset
			// for(uint8_t i = 0; i < WRITE_BUF_LENGTH; ++i) write_buf[i] = 0;
      memset(write_buf, 0x00, WRITE_BUF_LENGTH);
			write_buf_pos++;

			if(write_buf_pos >= CONFIG_NUM_PIXELS + 2) {
				// Stop transfer, we're done for now until someone needs us again
				write_buf_pos = 0;
				HAL_TIM_PWM_Stop_DMA(&htim2, TIM_CHANNEL_1);
			}
		} else { // heart clap, we skip a beat
			write_buf_pos++;
		}
    // __enable_irq();
	// }
}
