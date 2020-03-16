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
#include "fl_app_colors.h"
#include "fluorescenceapp.h"
#include "ui_fluorescenceapp.h"
#include "QtColorWidgets/colorwidgets_global.hpp"

#ifdef _LED_IMPLEMENTATION
#error "An implementation of the LED driver already exists!"
#endif
#define _LED_IMPLEMENTATION

uint8_t rgb_arr[CONFIG_NUM_BYTES] = {0};
extern QWidget *preset_dynamic_colors[NUM_PRESET_DYNAMIC_COLORS];

void vfdco_clr_init(uint8_t initial_dim_factor) {
	// Allocate color array and DMA buffer
  printf("SK6812 tester: Init with %d pixels, %d bpp, %d bytes.\n", CONFIG_NUM_PIXELS, CONFIG_NUM_BPP, CONFIG_NUM_BYTES);
  printf("SK6812 tester: Init with dim factor %hhu.\n", initial_dim_factor);
}

void vfdco_clr_deInit(void) {
  printf("SK6812 tester: DeInit success.\n");
}

void vfdco_clr_set_dim_factor(uint8_t dim_factor) {
  printf("SK6812 tester: Set with dim factor %hhu.\n", dim_factor);
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
  /*uint8_t dt = 0;
  for(uint8_t i = 0; i < CONFIG_NUM_BYTES; i++) {
    if(rgb_arr[i] < target_arr[i]) rgb_arr[i]++;
    else if(rgb_arr[i] > target_arr[i]) rgb_arr[i]--;
    else ++dt;
  }*/
  memcpy(rgb_arr, target_arr, CONFIG_NUM_BYTES);
}

void vfdco_clr_render() {
    for(uint_fast8_t i = 0; i < NUM_PRESET_DYNAMIC_COLORS; ++i) {
        dynamic_cast<color_widgets::ColorPreview *>(preset_dynamic_colors[CONFIG_NUM_PIXELS - i - 1])->setColor(
            QColor::fromRgb(rgb_arr[4 * i + 1], rgb_arr[4 * i], rgb_arr[4 * i + 2])
        );
    }
}








// #GoVegan
