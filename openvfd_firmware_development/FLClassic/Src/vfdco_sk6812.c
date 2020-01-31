/*
 * vfdco_sk6812.c
 *
 *  Created on: Nov 18, 2019
 *      Author: HARDWARECOP
 */

#include <stdlib.h>
#include <string.h>
#include "stm32f0xx_hal.h"
#include "../../vfdco_config.h"
#include "../../vfdco_led.h"

#define SK6812_PWM_DUTY_HI 38
#define SK6812_PWM_DUTY_LO 19
#define RESET_SLOT 82 //41
#define LED_NUMBER 6

uint8_t  *write_buf;
uint8_t	 *rgb_arr;

// Write buffer with two
uint16_t write_buf_length;
uint_fast8_t write_buf_pos;

extern TIM_HandleTypeDef htim2;

/*static const uint8_t gamma8[] = { // Cheap ass gamma correction https://learn.adafruit.com/led-tricks-gamma-correction/the-quick-fix
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
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255};*/

void vfdco_clr_init(uint8_t num_pixels) {
	//n um_rgb = n um_pixels;									// Number of physical LEDs
	//n um_bpp = 4;													// SK6812 Bytes per LED. 4: G(8), R(8), B(8), W(8)
	//n um_bytes = n um_bpp * n um_pixels;			// Bytes of static color array

	write_buf_length = CONFIG_NUM_BPP * 8; 			// For every bit, there's a new PWM byte -> 8 * bpp

	// Allocate color array and DMA buffer
	rgb_arr = (uint8_t *)calloc(CONFIG_NUM_BYTES, sizeof(uint8_t));
	write_buf = (uint8_t *)calloc(write_buf_length, sizeof(uint8_t));

	write_buf_pos = 0;
	HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t *)write_buf, write_buf_length);
}

void vfdco_clr_deInit(void) {
	HAL_TIM_PWM_Stop_DMA(&htim2, TIM_CHANNEL_1);
	free(rgb_arr);
	free(write_buf);
}

inline void vfdco_clr_set_RGB(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
	rgb_arr[4 * index] = g;
	rgb_arr[4 * index + 1] = r;
	rgb_arr[4 * index + 2] = b;
	rgb_arr[4 * index + 3] = 0;
}
inline void vfdco_clr_set_RGBW(uint8_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
	vfdco_clr_set_RGB(index, r, g, b);
	rgb_arr[4 * index + 3] = w;
}
inline void vfdco_clr_set_all_RGB(uint8_t r, uint8_t g, uint8_t b) {
	for(uint_fast8_t i = 0; i < CONFIG_NUM_PIXELS; ++i) vfdco_clr_set_RGB(i, r, g, b);
}
inline void vfdco_clr_set_all_RGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
	for(uint_fast8_t i = 0; i < CONFIG_NUM_PIXELS; ++i) vfdco_clr_set_RGBW(i, r, g, b, w);
}

inline void vfdco_clr_render() {
	// Ooh boi the first data buffer!!
	write_buf_pos = 0;
	for(uint_fast8_t i = 0; i < 8; ++i) {
		write_buf[i     ] = SK6812_PWM_DUTY_LO << (((rgb_arr[0] << i) & 0x80) > 0);
		write_buf[i +  8] = SK6812_PWM_DUTY_LO << (((rgb_arr[1] << i) & 0x80) > 0);
		write_buf[i + 16] = SK6812_PWM_DUTY_LO << (((rgb_arr[2] << i) & 0x80) > 0);
		write_buf[i + 24] = SK6812_PWM_DUTY_LO << (((rgb_arr[3] << i) & 0x80) > 0);
	}

	write_buf_pos++; // Since we're ready for the next buffer
	HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t *)write_buf, write_buf_length);
}

void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim) {
	// DMA buffer set from LED(write_buf_pos) to LED(write_buf_pos + 1)
	if(write_buf_pos < CONFIG_NUM_PIXELS) {
		// We're in. Let's fill the mem
		for(uint_fast8_t i = 0; i < 8; ++i) {
			write_buf[i     ] = SK6812_PWM_DUTY_LO << (((rgb_arr[write_buf_pos * 4    ] << i) & 0x80) > 0);
			write_buf[i +  8] = SK6812_PWM_DUTY_LO << (((rgb_arr[write_buf_pos * 4 + 1] << i) & 0x80) > 0);
			write_buf[i + 16] = SK6812_PWM_DUTY_LO << (((rgb_arr[write_buf_pos * 4 + 2] << i) & 0x80) > 0);
			write_buf[i + 24] = SK6812_PWM_DUTY_LO << (((rgb_arr[write_buf_pos * 4 + 3] << i) & 0x80) > 0);
		}
		write_buf_pos++;
	} else if (write_buf_pos >= CONFIG_NUM_PIXELS + 1) {
		// Last two transfers are resets. 64 * 1.25 us = 80 us = good enough reset
		memset(write_buf, 0x00, write_buf_length);
		write_buf_pos++;

		if(write_buf_pos >= CONFIG_NUM_PIXELS + 2) {
			// Stop transfer, we're done for now until someone needs us again
			write_buf_pos = 0;
			HAL_TIM_PWM_Stop_DMA(&htim2, TIM_CHANNEL_1);
		}
	} else { // heart clap, we skip a beat
		write_buf_pos++;
	}
}
