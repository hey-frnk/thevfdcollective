/**
  ******************************************************************************
  * @file     vfdco_fakeled.c
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     20-November-2019
  * @brief    This is the dummy driver to test the functionality of the LED libs
  ******************************************************************************
  * @toc      Table of contents, enter to navigate:
  ******************************************************************************
 **/
#include <stdio.h>
#include "../vfdco_config.h"
#include "../vfdco_led.h"

uint8_t rgb_arr[CONFIG_NUM_BYTES] = {0};

void vfdco_clr_init() {
	// Allocate color array and DMA buffer
  printf("SK6812 tester: Init with %hhu pixels, %hhu bpp, %hhu bytes.\n", CONFIG_NUM_PIXELS, CONFIG_NUM_BPP, CONFIG_NUM_BYTES);
}

void vfdco_clr_deInit(void) {
  printf("SK6812 tester: DeInit success.\n");
}

void vfdco_clr_set_RGB(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
	rgb_arr[4 * index] = g;
	rgb_arr[4 * index + 1] = r;
	rgb_arr[4 * index + 2] = b;
	rgb_arr[4 * index + 3] = 0;
}
void vfdco_clr_set_RGBW(uint8_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
	vfdco_clr_set_RGB(index, r, g, b);
	rgb_arr[4 * index + 3] = w;
}
void vfdco_clr_set_all_RGB(uint8_t r, uint8_t g, uint8_t b) {
	for(uint8_t i = 0; i < CONFIG_NUM_PIXELS; ++i) vfdco_clr_set_RGB(i, r, g, b);
}
void vfdco_clr_set_all_RGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
	for(uint8_t i = 0; i < CONFIG_NUM_PIXELS; ++i) vfdco_clr_set_RGBW(i, r, g, b, w);
}

void vfdco_clr_render() {
  // printf("SK6812 tester: Render function started with %hhu pixels, %hhu bpp, %hhu bytes.\n", n um_rgb, n um_bpp, n um_bytes);
  printf("                                   rgbw_arr: ");
  for (uint8_t i = 0; i < CONFIG_NUM_PIXELS; ++i) {
		// Skip Zeros
		if(rgb_arr[4*i]) printf("(%03hhu  ", rgb_arr[4*i]);
		else printf("(     ");

		if(rgb_arr[4*i + 1]) printf("%03hhu  ", rgb_arr[4*i + 1]);
		else printf("     ");

		if(rgb_arr[4*i + 2]) printf("%03hhu  ", rgb_arr[4*i + 2]);
		else printf("     ");

		if(rgb_arr[4*i + 3]) printf("%03hhu)", rgb_arr[4*i + 3]);
		else printf("   )");
  }
  printf("\n");
}








// #GoVegan
