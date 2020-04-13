/**
  ******************************************************************************
  * @file     vfdco_led.h
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     20-November-2019
  * @brief    This is the driver for an array of digital or analog RGB or RGBW LEDs 
  ******************************************************************************
 **/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _VFDCO_LED_DRIVER
#define _VFDCO_LED_DRIVER

#include <stdint.h>
#include "vfdco_config.h"

/**
 * @brief Initialize LED driver with an initial brightness dimming factor
 * @param dim_factor used to divide brightness by 2^(dim_factor). Hence, e.g. dim_factor == 2 divides brightness by 4, and the LEDs are 1/4 bright
 */
void vfdco_clr_init(uint8_t initial_dim_factor);

/**
 * @brief Set a dimming factor to the LEDs, in order to reduce brightness
 * @param dim_factor used to divide brightness by 2^(dim_factor). Hence, e.g. dim_factor == 2 divides brightness by 4, and the LEDs are 1/4 bright
 */
void vfdco_clr_set_dim_factor(uint8_t dim_factor);

/**
 * @brief Set LED buffer at index to red value of r, green value of g, blue value of b
 * @param index Which LED
 * @param r Red color value
 * @param g Green color value
 * @param b Blue color value
 */
void vfdco_clr_set_RGB(uint8_t index, uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Set LED buffer at index to red value of r, green value of g, blue value of b, white value of w
 * By using LEDs without white component, the w value is ignored
 * @param index Which LED
 * @param r Red color value
 * @param g Green color value
 * @param b Blue color value
 * @param w White color value
 */
void vfdco_clr_set_RGBW(uint8_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t w);

/**
 * @brief Set all CONFIG_NUM_PIXELS LEDs of the buffer to red value of r, green value of g, blue value of b
 * @param r Red color value for all LEDs
 * @param g Green color value for all LEDs
 * @param b Blue color value for all LEDs
 */
void vfdco_clr_set_all_RGB(uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Set all CONFIG_NUM_PIXELS LEDs of the buffer to red value of r, green value of g, blue value of b, white value of w
 * By using LEDs without white component, the w value is ignored
 * @param r Red color value for all LEDs
 * @param g Green color value for all LEDs
 * @param b Blue color value for all LEDs
 * @param w White color value for all LEDs
 */
void vfdco_clr_set_all_RGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w);

/**
 * @brief Write LED buffer to phsical LEDs
 */
void vfdco_clr_render();

/**
 * @brief Set target LED buffer at index to red value of r, green value of g, blue value of b
 * When a smooth transistion is required, use this to set a target buffer, and use minimize_difference to transistion
 * between the current buffer value in rgb_arr and the target buffer value in tp
 * Per definition set to 4 (and always 4!!!) * CONFIG_NUM_PIXELS -> NOT dependent on drivers CONFIG_NUM_BPP
 * @param tp Base address of target buffer
 * @param r Red color value
 * @param g Green color value
 * @param b Blue color value
 */
void vfdco_clr_target_RGB(uint8_t *tp, uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Set target LED buffer at index to red value of r, green value of g, blue value of b, white value of w
 * When a smooth transistion is required, use this to set a target buffer, and use minimize_difference to transistion
 * between the current buffer value in rgb_arr and the target buffer value in tp
 * Per definition set to 4 (and always 4!!!) * CONFIG_NUM_PIXELS -> NOT dependent on drivers CONFIG_NUM_BPP
 * @param tp Base address of target buffer
 * @param r Red color value
 * @param g Green color value
 * @param b Blue color value
 * @param w White color value
 */
void vfdco_clr_target_RGBW(uint8_t *tp, uint8_t r, uint8_t g, uint8_t b, uint8_t w);

/**
 * @brief Set all CONFIG_NUM_PIXELS LEDs of the target buffer to red value of r, green value of g, blue value of b
 * When a smooth transistion is required, use this to set a target buffer, and use minimize_difference to transistion
 * between the current buffer value in rgb_arr and the target buffer value in tp
 * Per definition set to 4 (and always 4!!!) * CONFIG_NUM_PIXELS -> NOT dependent on drivers CONFIG_NUM_BPP
 * @param tp Base address of target buffer
 * @param r Red color value for all LEDs
 * @param g Green color value for all LEDs
 * @param b Blue color value for all LEDs
 */
void vfdco_clr_target_all_RGB(uint8_t *tp, uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Set all CONFIG_NUM_PIXELS LEDs of the target buffer to red value of r, green value of g, blue value of b, white value of w
 * When a smooth transistion is required, use this to set a target buffer, and use minimize_difference to transistion
 * between the current buffer value in rgb_arr and the target buffer value in tp
 * Per definition set to 4 (and always 4!!!) * CONFIG_NUM_PIXELS -> NOT dependent on drivers CONFIG_NUM_BPP
 * @param tp Base address of target buffer
 * @param r Red color value for all LEDs
 * @param g Green color value for all LEDs
 * @param b Blue color value for all LEDs
 * @param w White color value for all LEDs
 */
void vfdco_clr_target_all_RGBW(uint8_t *tp, uint8_t r, uint8_t g, uint8_t b, uint8_t w);

/**
 * @brief Transition between the initial buffer values in rgb_arr to the target buffer values in target_arr
 * When minimize_difference is called, one single transition step is performed
 * @param target_arr Base address of target buffer
 */
void vfdco_clr_minimize_difference(uint8_t *target_arr);

#endif

#ifdef __cplusplus
}
#endif
