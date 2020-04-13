/**
  ******************************************************************************
  * @file     vfdco_mic.h
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     20-November-2019
  * @brief    This is the microphone driver
  ******************************************************************************
 **/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _VFDCO_MIC_DRIVER
#define _VFDCO_MIC_DRIVER

#include <stdint.h>
#include "vfdco_config.h"

/**
 * @brief Initialize microphone driver
 * By using Arduino, configure GPIO settings by pinMode(...)
 * By using STM32 HAL, this function can stay unused as initialization is done by STM32 HAL in void main()
 */
void vfdco_mic_init();

/**
 * @brief Read in sound intensity level captured by the microphone
 * @return uint8_t Level, number between and including 0 and 6
 */
uint8_t vfdco_mic_read_level(void);

#endif

#ifdef __cplusplus
}
#endif