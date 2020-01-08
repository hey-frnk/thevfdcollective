/*
 * vfdco_sk6812.c
 *
 *  Created on: Nov 18, 2019
 *      Author: HARDWARECOP
 */

#include "../../vfdco_led.h"
#include <stdlib.h>
#include <string.h>
#include "stm32f0xx_hal.h"

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

void vfdco_clr_init(uint8_t num_pixels) {
	num_rgb = num_pixels;									// Number of physical LEDs
	num_bpp = 4;													// SK6812 Bytes per LED. 4: G(8), R(8), B(8), W(8)
	num_bytes = num_bpp * num_pixels;			// Bytes of static color array

	write_buf_length = num_bpp * 8; 			// For every bit, there's a new PWM byte -> 8 * bpp

	// Allocate color array and DMA buffer
	rgb_arr = (uint8_t *)malloc(num_bytes * sizeof(uint8_t));
	write_buf = (uint8_t *)malloc(write_buf_length * sizeof(uint8_t));

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
	for(uint_fast8_t i = 0; i < num_rgb; ++i) vfdco_clr_set_RGB(i, r, g, b);
}
inline void vfdco_clr_set_all_RGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
	for(uint_fast8_t i = 0; i < num_rgb; ++i) vfdco_clr_set_RGBW(i, r, g, b, w);
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
	if(write_buf_pos < num_rgb) {
		// We're in. Let's fill the mem
		for(uint_fast8_t i = 0; i < 8; ++i) {
			write_buf[i     ] = SK6812_PWM_DUTY_LO << (((rgb_arr[write_buf_pos * 4    ] << i) & 0x80) > 0);
			write_buf[i +  8] = SK6812_PWM_DUTY_LO << (((rgb_arr[write_buf_pos * 4 + 1] << i) & 0x80) > 0);
			write_buf[i + 16] = SK6812_PWM_DUTY_LO << (((rgb_arr[write_buf_pos * 4 + 2] << i) & 0x80) > 0);
			write_buf[i + 24] = SK6812_PWM_DUTY_LO << (((rgb_arr[write_buf_pos * 4 + 3] << i) & 0x80) > 0);
		}
		write_buf_pos++;
	} else if (write_buf_pos >= num_rgb + 1) {
		// Last two transfers are resets. 64 * 1.25 us = 80 us = good enough reset
		memset(write_buf, 0x00, write_buf_length);
		write_buf_pos++;

		if(write_buf_pos >= num_rgb + 2) {
			// Stop transfer, we're done for now until someone needs us again
			write_buf_pos = 0;
			HAL_TIM_PWM_Stop_DMA(&htim2, TIM_CHANNEL_1);
		}
	} else { // heart clap, we skip a beat
		write_buf_pos++;
	}
}
