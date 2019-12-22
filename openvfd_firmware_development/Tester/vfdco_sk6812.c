/**
  ******************************************************************************
  * @file     vfdco_sk6812.h
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     20-November-2019
  * @brief    This is the dummy driver to test the functionality of the LED libs
  ******************************************************************************
  * @toc      Table of contents, enter to navigate:
  ******************************************************************************
 **/
#include "../vfdco_sk6812.h"
#include <stdlib.h>
#include <stdio.h>

void vfdco_clr_init(uint8_t num_pixels) {
	num_rgb = num_pixels;									// Number of physical LEDs
	num_bpp = 4;													// SK6812 Bytes per LED. 4: G(8), R(8), B(8), W(8)
	num_bytes = num_bpp * num_pixels;			// Bytes of static color array

	// Allocate color array and DMA buffer
	rgb_arr = (uint8_t *)calloc(num_bytes, sizeof(uint8_t));

  printf("SK6812 tester: Init with %hhu pixels, %hhu bpp, %hhu bytes.\n", num_rgb, num_bpp, num_bytes);
}

void vfdco_clr_deInit(void) {
	free(rgb_arr);
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
	for(uint8_t i = 0; i < num_rgb; ++i) vfdco_clr_set_RGB(i, r, g, b);
}
void vfdco_clr_set_all_RGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
	for(uint8_t i = 0; i < num_rgb; ++i) vfdco_clr_set_RGBW(i, r, g, b, w);
}

void vfdco_clr_render() {
  // printf("SK6812 tester: Render function started with %hhu pixels, %hhu bpp, %hhu bytes.\n", num_rgb, num_bpp, num_bytes);
  printf("                                   rgbw_arr: ");
  for (uint8_t i = 0; i < num_rgb; ++i) {
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
