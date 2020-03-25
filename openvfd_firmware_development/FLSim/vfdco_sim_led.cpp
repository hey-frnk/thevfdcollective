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

#include <QLabel>

#ifdef _LED_IMPLEMENTATION
#error "An implementation of the LED driver already exists!"
#endif
#define _LED_IMPLEMENTATION

// From fluorescencesimulator.cpp import:
extern QLabel *led[CONFIG_NUM_PIXELS];
extern QLabel *led_rgbw[CONFIG_NUM_PIXELS];
extern QLabel *led_hexw[CONFIG_NUM_PIXELS];
extern QLabel *led_hsl[CONFIG_NUM_PIXELS];
extern bool visualize_dimming;

uint8_t rgb_arr[CONFIG_NUM_BYTES] = {0};
uint8_t _led_dim_factor = 0;

void vfdco_clr_init(uint8_t initial_dim_factor) {
	// Allocate color array and DMA buffer
  printf("SK6812 tester: Init with %hhu pixels, %hhu bpp, %hhu bytes.\n", CONFIG_NUM_PIXELS, CONFIG_NUM_BPP, CONFIG_NUM_BYTES);
  printf("SK6812 tester: Init with dim factor %hhu.\n", initial_dim_factor);
  vfdco_clr_set_dim_factor(initial_dim_factor);
}

void vfdco_clr_deInit(void) {
  printf("SK6812 tester: DeInit success.\n");
}

void vfdco_clr_set_dim_factor(uint8_t dim_factor) {
  printf("SK6812 tester: Set with dim factor %hhu.\n", dim_factor);
  _led_dim_factor = dim_factor;
}

#if (CONFIG_USE_RGBW == 1)
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
#else
void vfdco_clr_set_RGB(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
    rgb_arr[3 * index] = g;
    rgb_arr[3 * index + 1] = r;
    rgb_arr[3 * index + 2] = b;
}
void vfdco_clr_set_RGBW(uint8_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    vfdco_clr_set_RGB(index, r, g, b);
    w = 0;
}
void vfdco_clr_set_all_RGB(uint8_t r, uint8_t g, uint8_t b) {
    for(uint8_t i = 0; i < CONFIG_NUM_PIXELS; ++i) vfdco_clr_set_RGB(i, r, g, b);
}
void vfdco_clr_set_all_RGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    for(uint8_t i = 0; i < CONFIG_NUM_PIXELS; ++i) vfdco_clr_set_RGBW(i, r, g, b, w);
}
#endif

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
  for(uint8_t i = 0; i < 4 * CONFIG_NUM_PIXELS; i += 4) vfdco_clr_target_RGB(tp + i, r, g, b);
}
void vfdco_clr_target_all_RGBW(uint8_t *tp, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
  for(uint8_t i = 0; i < 4 * CONFIG_NUM_PIXELS; i += 4) vfdco_clr_target_RGBW(tp + i, r, g, b, w);
}

/**
 * @brief Smooth fading between intermediate target array and LED buffer
 * @param target_arr base address of intermediate target array
 */
void vfdco_clr_minimize_difference(uint8_t *target_arr) {
  uint8_t j = 0;
  for(uint8_t i = 0; i < 4 * CONFIG_NUM_PIXELS; i++) {
    #if (CONFIG_USE_RGBW == 0)
    if(i % 4 == 3) continue;
    #endif
    if(rgb_arr[j] < target_arr[i]) rgb_arr[j]++;
    else if(rgb_arr[j] > target_arr[i]) rgb_arr[j]--;
    ++j;
  }
  // if(dt != CONFIG_NUM_BYTES) vfdco_clr_render();
}

void vfdco_clr_render() {
    for(uint_fast8_t i = 0; i < CONFIG_NUM_PIXELS; ++i) {
        #if (CONFIG_USE_RGBW == 1)
        uint8_t r = rgb_arr[4 * i + 1], g = rgb_arr[4 * i], b = rgb_arr[4 * i + 2], w = rgb_arr[4 * i + 3];
        #else
        uint8_t r = rgb_arr[3 * i + 1], g = rgb_arr[3 * i], b = rgb_arr[3 * i + 2], w = 0;
        #endif
        if(visualize_dimming) {
            r >>= _led_dim_factor;
            g >>= _led_dim_factor;
            b >>= _led_dim_factor;
            w >>= _led_dim_factor;
        }
        QColor _clr(r, g, b);
        led[CONFIG_NUM_PIXELS - i - 1]->setStyleSheet("background-color:" + _clr.name());
        led_hexw[CONFIG_NUM_PIXELS - i - 1]->setText(_clr.name().toUpper() + QString(", %1").arg(w, 3, 16, QChar('0')).toUpper());
        led_rgbw[CONFIG_NUM_PIXELS - i - 1]->setText(QString("%1 %2 %3 %4").arg(r, 3, 10, QChar('0')).arg(g, 3, 10, QChar('0')).arg(b, 3, 10, QChar('0')).arg(w, 3, 10, QChar('0')));
        led_hsl[CONFIG_NUM_PIXELS - i - 1]->setText(QString("%1 %2 %3").arg(_clr.hue(), 3, 10, QChar('0')).arg(_clr.hslSaturation(), 3, 10, QChar('0')).arg(_clr.lightness(), 3, 10, QChar('0')));
    }
}








// #GoVegan
